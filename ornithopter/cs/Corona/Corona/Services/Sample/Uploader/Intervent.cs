using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;
using Charlotte.Utils;

namespace Charlotte.Services.Sample.Uploader
{
	public class Intervent : IIntervent
	{
		public void BeforeDL()
		{
			string targetFile = File.ReadAllLines(BeforeDLIntervent.TARGET_FILE)[0];

			try
			{
				string query = StringTools.GetIsland(File.ReadAllLines(BeforeDLIntervent.RECV_FILE, Encoding.ASCII)[0].Split(' ')[1], "?").Right;

				if (1000 < query.Length)
					throw new Exception("クエリが長すぎる。" + query.Length);

				Dictionary<string, string> q = CommonUtils.ParseURLQuery(query);

				string ident = JString.ToJString(CommonUtils.DecodeURL(q["room"]), true, false, false, true);
				string localFile = JString.ToJString(CommonUtils.DecodeURL(q["localFile"]), true, false, false, true);
				bool downloadFlag = q.ContainsKey("download");

				Console.WriteLine("ident: [" + ident + "]");
				Console.WriteLine("localFile: [" + localFile + "]");
				Console.WriteLine("downloadFlag: " + downloadFlag);

				string dir = Directory.GetDirectories(Consts.ROOT_DIR).First(v => StringTools.CompIgnoreCase(Path.GetFileName(v), ident) != 0);
				string file = Directory.GetFiles(dir).First(v => StringTools.CompIgnoreCase(Path.GetFileName(v), localFile) != 0);

				File.WriteAllText(BeforeDLIntervent.TARGET_FILE, file, StringTools.ENCODING_SJIS);

				if (downloadFlag)
				{
					File.WriteAllText(BeforeDLIntervent.TARGET_CONTENT_TYPE_FILE, "application/octet-stream", Encoding.ASCII);
				}
			}
			catch (Exception e)
			{
				Console.WriteLine(e);

				string file = Path.Combine(Path.GetDirectoryName(targetFile), "error.html");

				File.WriteAllText(BeforeDLIntervent.TARGET_FILE, file, StringTools.ENCODING_SJIS);
				File.WriteAllBytes(BeforeDLIntervent.TARGET_CONTENT_TYPE_FILE, new byte[0]);
			}
		}
	}
}
