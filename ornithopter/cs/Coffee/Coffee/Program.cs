using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using Charlotte.Tools;
using System.Threading;
using Charlotte.Utils;

namespace Charlotte
{
	class Program
	{
		public const string APP_IDENT = "{445b354e-e5ff-4c78-8e6f-08bc7a253d07}";
		public const string APP_TITLE = "Coffee";

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
			if (ar.ArgIs("/BEFORE-DL"))
			{
				BeforeDLIntervent.Perform();
				return;
			}
			if (ar.ArgIs("/DISK-YELLOW"))
			{
#if true
				throw new Exception("廃止");
#else // del @ 2020.4.1
				foreach (IService service in new ServiceDistributor().GetAllService())
				{
					service.DiskYellow();
				}
				return;
#endif
			}
			if (ar.HasArgs())
				throw new Exception("不明なコマンド引数");

			HTTPRequest.I = new HTTPRequest();

			HTTPRequest.I.IP = File.ReadAllLines("IP.httdat", Encoding.ASCII)[0]; // 正規化済み @ Post2
			HTTPRequest.I.Method = File.ReadAllLines("Method.htt2dat", Encoding.ASCII)[0]; // 正規化済み @ Post2
			HTTPRequest.I.URLPath = File.ReadAllLines("Path.htt2dat", Encoding.ASCII)[0]; // 正規化済み @ Post2
			HTTPRequest.I.HTTP_Version = File.ReadAllLines("HTTP_Version.htt2dat", Encoding.ASCII)[0]; // 正規化済み @ Post2

			ProcMain.WriteLog("IP: " + HTTPRequest.I.IP);
			ProcMain.WriteLog("Method: " + HTTPRequest.I.Method);
			ProcMain.WriteLog("URLPath: " + HTTPRequest.I.URLPath);
			ProcMain.WriteLog("HTTP_Version: " + HTTPRequest.I.HTTP_Version);

			ParsePathQuery();

			{
				string[] keys = File.ReadAllLines("HeaderKeys.htt2dat", Encoding.ASCII); // 正規化済み @ Post2
				string[] values = File.ReadAllLines("HeaderValues.htt2dat", Encoding.ASCII); // 正規化済み @ Post2

				if (keys.Length != values.Length)
					throw null; // 想定外

				for (int index = 0; index < keys.Length; index++)
				{
					string key = keys[index];
					string value = values[index];

					ProcMain.WriteLog("Header: " + key + " ⇒ " + value);

					HTTPRequest.I.HeaderPairs.Add(key, value);
				}
			}

			{
				byte[] body = File.ReadAllBytes("Body.htt2dat");

				string serviceName = HTTPRequest.I.Query["sn"];
				IService service = new ServiceDistributor().GetService(serviceName);

				JsonTools.DecodeStringFilter = v => JString.ToJString(v, true, true, true, true);
				JsonTools.DecodeObjectCountMax = 1000; // 適当
				object prm = JsonTools.Decode(body);
				string sPrm = JsonTools.Encode(prm);

				if (600 < sPrm.Length)
					sPrm = sPrm.Substring(0, 500) + "...(省略)"; // 長すぎると表示に時間が掛かるので、

				ProcMain.WriteLog("prm: " + sPrm);

				try
				{
					object ret = service.Perform(prm);

					string sRet = JsonTools.Encode(ObjectTree.Conv(ret));
					ProcMain.WriteLog("ret: " + sRet);
					byte[] resBody = Encoding.UTF8.GetBytes(sRet);

					File.WriteAllBytes("ResBody.htt2dat", resBody);

					File.WriteAllLines("ResHeader.htt2dat", new string[]
					{
						"Content-Type: application/json",
					});
				}
				catch (Exception e)
				{
					ProcMain.WriteLog(e);

					File.WriteAllText("ResHeaderFirstLine.htt2dat", "HTTP/1.1 400 Or 500\r\n", Encoding.ASCII);
				}
			}
		}

		private void ParsePathQuery()
		{
			int queryIndex = HTTPRequest.I.URLPath.IndexOf('?');

			if (queryIndex == -1)
			{
				HTTPRequest.I.Path = HTTPRequest.I.URLPath;
			}
			else
			{
				HTTPRequest.I.Path = HTTPRequest.I.URLPath.Substring(0, queryIndex);

				string query = HTTPRequest.I.URLPath.Substring(queryIndex + 1);
				string[] qTkns = query.Split('&');

				foreach (string qTkn in qTkns)
				{
					string[] qPair = qTkn.Split('=');

					if (qPair.Length == 2)
					{
						HTTPRequest.I.Query.Add(qPair[0], qPair[1]);
					}
				}
			}
		}
	}
}
