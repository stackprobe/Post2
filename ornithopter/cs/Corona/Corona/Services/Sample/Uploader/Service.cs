using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using Charlotte.Utils;

namespace Charlotte.Services.Sample.Uploader
{
	public class Service : IService
	{
		private ObjectTree TPrm;
		private Session Session = null;
		private LiteGroup LiteGroup = null;

		public object Perform(object prm)
		{
			string command = HTTPRequest.I.Query["c"];
			object ret = null;

			this.TPrm = new ObjectTree(prm);

			if (command == "login")
			{
				ret = Login();
			}
			else if (command == "supervisor-dashboard")
			{
				ret = SupervisorDashboard();
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

			if (new GroupBundle().LiteGroups.Any(v => v.AccessKey == accessKey) == false)
			{
				throw new Exception("不明なアクセスキー");
			}

			Session session = new Session()
			{
				SessionKey = SessionUtils.CreateSessionKey(),
				AccessKey = accessKey,
				SupervisorMode = accessKey == Consts.SUPERVISOR_ACCESS_KEY,
				LastAccessedDateTime = DateTimeToSec.Now.GetDateTime(),
			};

			SessionBundle sessionBundle = new SessionBundle();

			sessionBundle.Sessions.Add(session);
			sessionBundle.Save();

			return new object[]
			{
				session.SessionKey,
				session.SupervisorMode,
			};
		}

		private void LoggedIn()
		{
			string sessionKey = this.TPrm[0].StringValue;

			this.TPrm = this.TPrm[1]; // 本来のパラメータに置き換え

			SessionBundle sessionBundle = new SessionBundle();
			Session session = sessionBundle.Sessions.First(v => v.SessionKey == sessionKey);

			session.Accessed();

			this.Session = session;
			this.LiteGroup = new GroupBundle().LiteGroups.FirstOrDefault(v => v.AccessKey == session.AccessKey);
		}

		private object SupervisorDashboard()
		{
			this.LoggedIn();

			return new object[]
			{
				new object[] // zantei
				{
					new GroupBundle().GetTotalFileNum(),  // XXX 時間掛かるかも？
					new GroupBundle().GetTotalFileSize(), // XXX 時間掛かるかも？
				},

				new GroupBundle().LiteGroups.Select(v =>
				{
					Group group = v.GetGroup();

					return new object[]
					{
						group.AccessKey,
						group.Name,
						group.TotalFileSizeMax,
					};
				}),
			};
		}

		public void DiskYellow()
		{
			// TODO ???
		}
	}
}
