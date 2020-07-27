using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using Charlotte.Utils;
using System.IO;

namespace Charlotte.Services.Salad.Uploader
{
	public class Service : IService
	{
		private ObjectTree TPrm;
		private Session Session = null;
		private LiteGroup LiteGroup = null; // null == 未ログイン || Session.AccessKey に対応するグループが無い。

		public object Perform(object prm)
		{
			string command = HTTPRequest.I.Query["c"];
			object ret = null;

			this.TPrm = new ObjectTree(prm);

			if (command == "login")
			{
				ret = Login();
			}
			else if (command == "heartbeat")
			{
				ret = Heartbeat();
			}
			else if (command == "supervisor-dashboard")
			{
				ret = SupervisorDashboard();
			}
			else if (command == "supervisor-create-group")
			{
				ret = SupervisorCreateGroup();
			}
			else if (command == "supervisor-edit-group")
			{
				ret = SupervisorEditGroup();
			}
			else if (command == "supervisor-delete-group")
			{
				ret = SupervisorDeleteGroup();
			}
			else if (command == "dashboard")
			{
				ret = Dashboard();
			}
			else if (command == "get-link")
			{
				ret = GetDonwloadLink(false);
			}
			else if (command == "get-download-link")
			{
				ret = GetDonwloadLink(true);
			}
			else if (command == "delete-file")
			{
				ret = DeleteFile();
			}
			else if (command == "rename-file")
			{
				ret = RenameFile();
			}
			else if (command == "get-group")
			{
				ret = GetGroup();
			}
			else if (command == "edit-group")
			{
				ret = EditGroup();
			}
			else if (command == "create-file")
			{
				ret = CreateFile();
			}
			else if (command == "upload")
			{
				ret = Upload();
			}
			else
			{
				throw new Exception("不明なコマンド");
			}
			return ret;
		}

		private object Login()
		{
			string accessKey = this.TPrm[0].StringValue;

			if (new GroupBundle().LiteGroups.Any(v => v.AccessKey == accessKey))
			{
				// user
			}
			else if (accessKey == Consts.SUPERVISOR_ACCESS_KEY)
			{
				// supervisor
			}
			else
			{
				throw new Exception("不明なアクセスキー");
			}

			SessionBundle sessionBundle = new SessionBundle();
			Session session = null;

			if (Consts.SESSION_NUM_LMT < sessionBundle.Sessions.Count)
			{
				// XXX 不要セッションの管理が面倒なので、セッションが増えてきたら、同じグループについては同じセッションを使い回す。

				Session[] sessions = sessionBundle.Sessions.Where(v => v.AccessKey == accessKey).ToArray();

				if (1 <= sessions.Length)
				{
					session = sessions[SecurityTools.CRandom.GetInt(sessions.Length)];
					session.Accessed();

					sessionBundle.Save();
				}
			}
			if (session == null)
			{
				session = new Session()
				{
					SessionKey = SessionUtils.CreateSessionKey(),
					AccessKey = accessKey,
					SupervisorMode = accessKey == Consts.SUPERVISOR_ACCESS_KEY,
					LastAccessedDateTime = DateTimeToSec.Now.GetDateTime(),
				};

				sessionBundle.Sessions.Add(session);
				sessionBundle.Save();
			}

			return new object[]
			{
				session.SessionKey,
				session.SupervisorMode,
			};
		}

		private void LoggedIn(bool supervisorMode = false)
		{
			string sessionKey = this.TPrm[0].StringValue;

			this.TPrm = this.TPrm[1]; // 本来のパラメータに置き換え

			SessionBundle sessionBundle = new SessionBundle();
			Session session = sessionBundle.Sessions.First(v => v.SessionKey == sessionKey);

			if (supervisorMode && session.SupervisorMode == false)
				throw new Exception("管理者権限が必要です。");

			session.Accessed();

			this.Session = session;
			this.LiteGroup = new GroupBundle().LiteGroups.FirstOrDefault(v => v.AccessKey == session.AccessKey);
		}

		private object Heartbeat()
		{
			this.LoggedIn();

			return "OK";
		}

		private object SupervisorDashboard()
		{
			this.LoggedIn(true);

			GroupBundle groupBundle = new GroupBundle();

			return new object[]
			{
				new object[] // zantei
				{
					groupBundle.GetTotalFileNum(),
					groupBundle.GetTotalFileSize(),
					groupBundle.LiteGroups.Count,
				},

				groupBundle.LiteGroups.Select(v =>
				{
					Group group = v.GetGroup();

					return new object[]
					{
						Path.GetFileName(group.Dir),
						group.AccessKey,
						group.Name,
						group.GroupTotalFileSizeMax,
					};
				}),
			};
		}

		private object SupervisorCreateGroup()
		{
			this.LoggedIn(true);

			string name = this.TPrm["Name"].StringValue;
			long groupTotalFileSizeMax = long.Parse(this.TPrm["GroupTotalFileSizeMax"].StringValue);

			// チェック・正規化
			{
				name = CodeDefinition.ToFair.GroupName(name);
				Utilities.CheckRange(groupTotalFileSizeMax, 1L, Consts.TOTAL_FILE_SIZE_MAX, "容量");
			}

			if (Consts.GROUP_NUM_MAX <= new GroupBundle().LiteGroups.Count)
				throw new Exception("これ以上グループを作成出来ません。");

			GroupUtils.CreateGroup(name, groupTotalFileSizeMax);

			return "OK";
		}

		private object SupervisorEditGroup()
		{
			this.LoggedIn(true);

			string currAccessKey = this.TPrm["CurrAccessKey"].StringValue;
			string localDir = this.TPrm["LocalDir"].StringValue;
			string accessKey = this.TPrm["AccessKey"].StringValue;
			string name = this.TPrm["Name"].StringValue;
			long groupTotalFileSizeMax = long.Parse(this.TPrm["GroupTotalFileSizeMax"].StringValue);

			// チェック・正規化
			{
				currAccessKey = CodeDefinition.ToFair.AccessKey(currAccessKey);
				localDir = CodeDefinition.ToFair.GroupLocalDir(localDir);
				accessKey = CodeDefinition.ToFair.AccessKey(accessKey);
				name = CodeDefinition.ToFair.GroupName(name);
				Utilities.CheckRange(groupTotalFileSizeMax, 1L, Consts.TOTAL_FILE_SIZE_MAX, "容量");
			}

			GroupBundle groupBundle = new GroupBundle();
			Group group = groupBundle.LiteGroups.First(v => v.AccessKey == currAccessKey).GetGroup();

			bool changeLocalDirFlag = StringTools.CompIgnoreCase(Path.GetFileName(group.Dir), localDir) != 0;
			bool changeAccessKeyFlag = group.AccessKey != accessKey;

			// チェック
			{
				if (changeLocalDirFlag)
				{
					string dirNew = Path.Combine(Consts.GROUP_BUNDLE_DIR, localDir);

					if (Directory.Exists(dirNew))
						throw new Exception("新しいローカルディレクトリは既に存在します。");

					if (File.Exists(dirNew))
						throw null; // 想定外
				}
				if (changeAccessKeyFlag)
				{
					if (
						groupBundle.LiteGroups.Any(v => v.AccessKey == accessKey) ||
						accessKey == Consts.SUPERVISOR_ACCESS_KEY
						)
						throw new Exception("新しいアクセスキーは既に存在します。");
				}
			}

			group.AccessKey = accessKey;
			group.Name = name;
			group.GroupTotalFileSizeMax = groupTotalFileSizeMax;
			group.Save();

			if (changeLocalDirFlag) // 最後に！
			{
				string dirNew = Path.Combine(Consts.GROUP_BUNDLE_DIR, localDir);

				Directory.Move(group.Dir, dirNew);
			}

			return "OK";
		}

		private object SupervisorDeleteGroup()
		{
			this.LoggedIn(true);

			string accessKey = this.TPrm.StringValue;

			// チェック・正規化
			{
				accessKey = CodeDefinition.ToFair.AccessKey(accessKey);
			}

			string dir = new GroupBundle().LiteGroups.First(v => v.AccessKey == accessKey).GetGroup().Dir;

			FileTools.Delete(dir);

			return "OK";
		}

		private object Dashboard()
		{
			this.LoggedIn();

			Group group = this.LiteGroup.GetGroup();
			FileBundle fileBundle = group.GetFileBundle();

			return new object[]
			{
				new object[] // zantei
				{
					group.Name,
					group.GroupTotalFileSizeMax,
					fileBundle.GetTotalFileSize(),
					fileBundle.Files.Length,
					Path.GetFileName(group.Dir),
				},

				fileBundle.Files.Select(v =>
				{
					FileInfo info = new FileInfo(v);

					return new object[]
					{
						Path.GetFileName(v),
						info.Length,
						info.LastWriteTime.ToString(),
					};
				}),
			};
		}

		private object GetDonwloadLink(bool downloadFlag)
		{
			this.LoggedIn();

			Group group = this.LiteGroup.GetGroup();

			return new DownloadURL()
			{
				GroupLocalDir = Path.GetFileName(group.Dir),
				LocalFile = this.TPrm.StringValue,
				DownloadFlag = downloadFlag,
			}
			.GetString();
		}

		private object DeleteFile()
		{
			this.LoggedIn();

			Group group = this.LiteGroup.GetGroup();
			string dir = Path.Combine(group.Dir, Consts.FILE_BUNDLE_LOCAL_DIR);
			string localFile = DenebolaToolkit.GetFairLocalPath(this.TPrm.StringValue, dir);
			string file = Path.Combine(dir, localFile);

			ProcMain.WriteLog("ファイル削除 ⇒ " + file);

			FileTools.Delete(file);

			return "OK";
		}

		private object RenameFile()
		{
			this.LoggedIn();

			Group group = this.LiteGroup.GetGroup();
			string dir = Path.Combine(group.Dir, Consts.FILE_BUNDLE_LOCAL_DIR);
			string localFileOld = DenebolaToolkit.GetFairLocalPath(this.TPrm[0].StringValue, dir);
			string localFileNew = DenebolaToolkit.GetFairLocalPath(this.TPrm[1].StringValue, dir);
			string fileOld = Path.Combine(dir, localFileOld);
			string fileNew = Path.Combine(dir, localFileNew);
			bool forceMode = Utilities.GetBoolean(this.TPrm[2].StringValue);

			ProcMain.WriteLog("元ファイル.1 ⇒ " + fileOld);
			ProcMain.WriteLog("新ファイル.1 ⇒ " + fileNew);

			if (File.Exists(fileOld) == false)
				throw new Exception("元ファイルが見つかりません。");

			if (File.Exists(fileNew))
			{
				if (forceMode)
					fileNew = Utilities.ToCreatableFairFullPath(fileNew);
				else
					throw new Exception("新ファイルは既に存在します。");
			}
			ProcMain.WriteLog("新ファイル.2 ⇒ " + fileNew);

			File.Move(fileOld, fileNew);

			return "OK";
		}

		private object GetGroup()
		{
			this.LoggedIn();

			Group group = this.LiteGroup.GetGroup();

			return new object[]
			{
				Path.GetFileName( group.Dir),
				group.AccessKey,
				group.Name,
			};
		}

		private object EditGroup()
		{
			this.LoggedIn();

			try
			{
				string localDir = this.TPrm["LocalDir"].StringValue;
				string accessKey = this.TPrm["AccessKey"].StringValue;
				string name = this.TPrm["Name"].StringValue;

				// チェック・正規化
				{
					localDir = CodeDefinition.ToFair.GroupLocalDir(localDir);
					accessKey = CodeDefinition.ToFair.AccessKey(accessKey);
					name = CodeDefinition.ToFair.GroupName(name);
				}

				Group group = this.LiteGroup.GetGroup();

				bool changeLocalDirFlag = StringTools.CompIgnoreCase(Path.GetFileName(group.Dir), localDir) != 0;
				bool changeAccessKeyFlag = group.AccessKey != accessKey;

				// チェック
				{
					if (changeLocalDirFlag)
					{
						string dirNew = Path.Combine(Consts.GROUP_BUNDLE_DIR, localDir);

						if (Directory.Exists(dirNew))
							throw new Exception("新しいローカルディレクトリは既に存在します。");

						if (File.Exists(dirNew))
							throw null; // 想定外
					}
					if (changeAccessKeyFlag)
					{
						if (
							new GroupBundle().LiteGroups.Any(v => v.AccessKey == accessKey) ||
							accessKey == Consts.SUPERVISOR_ACCESS_KEY
							)
							//throw new Exception("新しいアクセスキーは既に存在します。");
							throw new Exception("アクセスキーに問題があります。"); // このアクセスキーが存在するってバレちゃうじゃん...
					}
				}

				group.AccessKey = accessKey;
				group.Name = name;
				group.Save();

				if (changeLocalDirFlag) // 最後に！
				{
					string dirNew = Path.Combine(Consts.GROUP_BUNDLE_DIR, localDir);

					Directory.Move(group.Dir, dirNew);
				}

				return new object[] { "OK", null };
			}
			catch (Exception e)
			{
				ProcMain.WriteLog(e);

				return new object[] { "NG", e.Message };
			}
		}

		private object CreateFile()
		{
			this.LoggedIn();

			TrySlimdown(10);

			Group group = this.LiteGroup.GetGroup();
			string dir = Path.Combine(group.Dir, Consts.FILE_BUNDLE_LOCAL_DIR);
			string localFile = DenebolaToolkit.GetFairLocalPath(this.TPrm.StringValue, dir);
			string file = Path.Combine(dir, localFile);

			ProcMain.WriteLog("ファイル作成.1 ⇒ " + file);

			file = Utilities.ToCreatableFairFullPath(file);

			ProcMain.WriteLog("ファイル作成.2 ⇒ " + file);

			File.WriteAllBytes(file, BinTools.EMPTY);

			return Path.GetFileName(file);
		}

		private object Upload()
		{
			this.LoggedIn();

			TrySlimdown(100);

			Group group = this.LiteGroup.GetGroup();
			string dir = Path.Combine(group.Dir, Consts.FILE_BUNDLE_LOCAL_DIR);
			string localFile = DenebolaToolkit.GetFairLocalPath(this.TPrm["LocalFile"].StringValue, dir);
			string file = Path.Combine(dir, localFile);

			if (File.Exists(file) == false)
				throw new Exception("指定されたファイルは存在しません。");

			long offset = long.Parse(this.TPrm["Offset"].StringValue);

			if (offset < 0L || LongTools.IMAX_64 < offset)
				throw new Exception("不正なオフセット値です。" + offset);

			byte[] data = new Base64Unit().Decode(this.TPrm["Data"].StringValue);
			long fileSize = new FileInfo(file).Length;

			if (fileSize < offset)
				throw new Exception("ファイルサイズより大きなオフセット値です。" + offset);

			// offset < fileSize の場合は、既に正常に書き込まれたと見なす。

			if (offset == fileSize)
			{
				using (FileStream writer = new FileStream(file, FileMode.Append, FileAccess.Write))
				{
					writer.Write(data, 0, data.Length);
				}
			}
			return "OK";
		}

		private void TrySlimdown(int rateDenom)
		{
			if (SecurityTools.CRandom.GetInt(rateDenom) == 0)
			{
				new Slimdown().Perform();
			}
		}

#if false // del @ 2020.4.1
		public void DiskYellow()
		{
			// memo: HTT を Bind した上で実行する必要がある。

			new Slimdown()
			{
				DiskYellowFlag = true,
			}
			.Perform();
		}
#endif
	}
}
