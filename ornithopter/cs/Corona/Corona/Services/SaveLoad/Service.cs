using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.IO;

namespace Charlotte.Services.SaveLoad
{
	public class Service
	{
		public class Save : IService
		{
			public object Perform(object prm)
			{
				Ground.InitDataDir();

				string key = SecurityTools.MakePassword_9();
				string file = Path.Combine(Consts.DATA_DIR, key);

				File.WriteAllBytes(file, Encoding.UTF8.GetBytes(JsonTools.Encode(prm)));

				return key;
			}

			public void DiskYellow()
			{
				Ground.InitDataDir();
			}
		}

		public class Load : IService
		{
			public object Perform(object prm)
			{
				Ground.InitDataDir();

				string key = (string)prm;

				if (StringTools.LiteValidate(key, StringTools.DECIMAL) == false)
					throw new Exception("不正なキーです。");

				string file = Path.Combine(Consts.DATA_DIR, key);

				if (File.Exists(file) == false)
					throw new Exception("不正なキー又はタイムアウトしました。");

				object ret = JsonTools.Decode(File.ReadAllBytes(file));
				FileTools.Delete(file);
				return ret;
			}

			public void DiskYellow()
			{
				Ground.InitDataDir();
			}
		}
	}
}
