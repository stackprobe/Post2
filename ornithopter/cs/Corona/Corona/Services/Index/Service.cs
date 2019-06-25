using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.IO;

namespace Charlotte.Services.Index
{
	public class Service : IService
	{
		public object Perform(object prm)
		{
			ObjectTree tPrm = new ObjectTree(prm);

			string command = tPrm[0].StringValue;

			if (command == "background-image-src-list")
			{
				return Directory.GetFiles(Path.Combine(ProcMain.SelfDir, @"..\..\..\..\..\home\corona\res\background-image"))
					.Select(v => "res/background-image/" + Path.GetFileName(v))
					.ToArray();
			}
			throw new Exception("不明なコマンド");
		}

		public void DiskYellow()
		{
			// noop
		}
	}
}
