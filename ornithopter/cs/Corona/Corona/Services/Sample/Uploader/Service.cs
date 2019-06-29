using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte.Services.Sample.Uploader
{
	public class Service : IService
	{
		private ObjectTree TPrm;

		public object Perform(object prm)
		{
			string command = HTTPRequest.I.Query["c"];
			object ret = null;

			this.TPrm = new ObjectTree(prm);

			if (command == "login")
			{
				ret = Login();
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

			throw null; // TODO
		}

		public void DiskYellow()
		{
			// TODO ???
		}
	}
}
