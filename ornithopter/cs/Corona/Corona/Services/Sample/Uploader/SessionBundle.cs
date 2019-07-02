using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;
using Charlotte.Utils;

namespace Charlotte.Services.Sample.Uploader
{
	public class SessionBundle
	{
		public List<Session> Sessions;

		public SessionBundle()
		{
			if (File.Exists(Consts.SESSION_BUNDLE_FILE) == false)
				File.WriteAllBytes(Consts.SESSION_BUNDLE_FILE, BinTools.EMPTY);

			this.Sessions = ArrayTools.ToList(File.ReadAllLines(Consts.SESSION_BUNDLE_FILE, StringTools.ENCODING_SJIS).Select(v =>
			{
				string[] tokens = v.Split('\t').ToArray();

				return new Session()
				{
					SessionKey = tokens[0],
					AccessKey = tokens[1],
					SupervisorMode = Utilities.GetBoolean(tokens[2]),
					LastAccessedDateTime = long.Parse(tokens[3]),
				};
			}
			));

			this.Sessions.Sort((a, b) =>
			{
				int ret = StringTools.Comp(a.AccessKey, b.AccessKey);

				if (ret != 0)
					return ret;

				ret = StringTools.Comp(a.SessionKey, b.SessionKey);
				return ret;
			});

			this.Slim();
		}

		private void Slim()
		{
			long currDateTime = DateTimeToSec.Now.GetDateTime();
			long expireDateTime = DateTimeToSec.ToDateTime(DateTimeToSec.ToSec(currDateTime) - Consts.SESSION_TIMEOUT_SEC);

			ProcMain.WriteLog("SessionBundle.Slim()");
			ProcMain.WriteLog("現在日時 ⇒ " + currDateTime);
			ProcMain.WriteLog("期限切れ ⇒ " + expireDateTime);

			this.Sessions.RemoveAll(v => v.LastAccessedDateTime < expireDateTime);
		}

		public void Save()
		{
			File.WriteAllLines(
				Consts.SESSION_BUNDLE_FILE,
				this.Sessions.Select(v =>
				{
					string[] tokens = new string[]
					{
						v.SessionKey,
						v.AccessKey,
						Utilities.GetString(v.SupervisorMode),
						"" + v.LastAccessedDateTime,
					};

					return string.Join("\t", tokens);
				}),
				StringTools.ENCODING_SJIS
				);
		}
	}
}
