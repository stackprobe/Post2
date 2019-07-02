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
				string query = BeforeDLInterventUtils.RecvFileToQuery();

				if (1000 < query.Length)
					throw new Exception("クエリが長すぎる。" + query.Length);

				Dictionary<string, string> q = BeforeDLInterventUtils.ParseQuery(query);

				string localDir = JString.ToJString(CommonUtils.DecodeURL(q["group"]), true, false, false, true);
				string localFile = JString.ToJString(CommonUtils.DecodeURL(q["file"]), true, false, false, true);
				bool downloadFlag = q.ContainsKey("download");

				ProcMain.WriteLog("localDir: [" + localDir + "]");
				ProcMain.WriteLog("localFile: [" + localFile + "]");
				ProcMain.WriteLog("downloadFlag: " + downloadFlag);

				string dir = Directory.GetDirectories(Consts.GROUP_BUNDLE_DIR).First(v =>
					StringTools.CompIgnoreCase(Path.GetFileName(v), localDir) != 0
					);
				string file = Directory.GetFiles(dir).First(v =>
					StringTools.CompIgnoreCase(Path.GetFileName(v), localFile) != 0
					);

				File.WriteAllText(BeforeDLIntervent.TARGET_FILE, file, StringTools.ENCODING_SJIS);

				if (downloadFlag)
				{
					File.WriteAllText(BeforeDLIntervent.TARGET_CONTENT_TYPE_FILE, "application/octet-stream", Encoding.ASCII);
				}
			}
			catch (Exception e)
			{
				ProcMain.WriteLog(e);

				string file = Path.Combine(Path.GetDirectoryName(targetFile), "dl-error.html");

				File.WriteAllText(BeforeDLIntervent.TARGET_FILE, file, StringTools.ENCODING_SJIS);
				File.WriteAllBytes(BeforeDLIntervent.TARGET_CONTENT_TYPE_FILE, BinTools.EMPTY);
			}
		}
	}
}
