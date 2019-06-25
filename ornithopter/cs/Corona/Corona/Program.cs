using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using Charlotte.Tools;

namespace Charlotte
{
	class Program
	{
		public const string APP_IDENT = "{445b354e-e5ff-4c78-8e6f-08bc7a253d07}";
		public const string APP_TITLE = "Corona";

		static void Main(string[] args)
		{
			ProcMain.CUIMain(new Program().Main2, APP_IDENT, APP_TITLE);

#if DEBUG
			//if (ProcMain.CUIError)
			{
				Console.WriteLine("Press ENTER.");
				Console.ReadLine();
			}
#endif
		}

		private void Main2(ArgsReader ar)
		{
			RootGround.I = new RootGround();

			if (ar.ArgIs("/DISK-YELLOW"))
			{
				foreach (IService service in RootGround.I.ServiceDistributor.GetAllService())
				{
					service.DiskYellow();
				}
				return;
			}
			if (ar.HasArgs())
				throw new Exception("不明なコマンド引数");

			RootGround.I.IP = File.ReadAllLines("IP.httdat", Encoding.ASCII)[0];
			RootGround.I.Method = File.ReadAllLines("Method.htt2dat", Encoding.ASCII)[0];
			RootGround.I.Path = File.ReadAllLines("Path.htt2dat", Encoding.ASCII)[0];
			RootGround.I.PathQuery = new PathQuery(RootGround.I.Path);
			RootGround.I.HTTP_Version = File.ReadAllLines("HTTP_Version.htt2dat", Encoding.ASCII)[0];

			{
				string[] keys = File.ReadAllLines("HeaderKeys.htt2dat", Encoding.ASCII);
				string[] values = File.ReadAllLines("HeaderValues.htt2dat", Encoding.ASCII);

				if (keys.Length != values.Length)
					throw null; // 想定外

				for (int index = 0; index < keys.Length; index++)
				{
					string key = keys[index];
					string value = values[index];

					RootGround.I.HeaderPairs.Add(key, value);
				}
			}

			RootGround.I.Body = File.ReadAllBytes("Body.htt2dat");

			{
				string command = RootGround.I.PathQuery.Query["q"];
				object prm = JsonTools.Decode(RootGround.I.Body);
				IService service = RootGround.I.ServiceDistributor.GetService(command);

				object ret = service.Perform(prm);

				RootGround.I.ResBody = Encoding.UTF8.GetBytes(JsonTools.Encode(ObjectTree.Conv(ret)));
			}

			RootGround.I.ResHeaderPairs.Add(new string[] { "Content-Type", "application/json" });

			File.WriteAllBytes("ResBody.htt2dat", RootGround.I.ResBody);
			File.WriteAllLines("ResHeader.htt2dat", RootGround.I.ResHeaderPairs.Select(v => v[0] + ": " + v[1]));
		}
	}
}
