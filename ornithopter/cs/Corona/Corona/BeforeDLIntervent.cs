using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte
{
	public class BeforeDLIntervent
	{
		private class InterventInfo
		{
			public string Pattern;
			public IIntervent Intervent;
		}

		private List<InterventInfo> Infos = new List<InterventInfo>();

		public BeforeDLIntervent()
		{
			// ----

			this.Add("{c54bae94-712d-4065-b4be-ff6a7e518ac6}", new Services.Sample.Uploader.Intervent());

			// ----
		}

		private void Add(string targetPtn, IIntervent intervent)
		{
			this.Infos.Add(new InterventInfo()
			{
				Pattern = targetPtn,
				Intervent = intervent,
			});
		}

		public const string RECV_FILE = "Recv.httdat";
		public const string TARGET_FILE = "Target.httgetdat";
		public const string TARGET_CONTENT_TYPE_FILE = "TargetContentType.httgetdat";

		public void Perform()
		{
			if (
				File.Exists("IP.httdat") == false ||
				File.Exists("Recv.httdat") == false ||
				File.Exists("Send.httdat") == false
				)
				throw new Exception("恐らく Get.exe から呼ばれていません。");

			string targetFile = File.ReadAllLines(TARGET_FILE)[0];

			if (1000 < new FileInfo(targetFile).Length) // 小さいファイル以外は無視
				return;

			byte[] targetData = File.ReadAllBytes(targetFile);
			string target = JString.ToJString(File.ReadAllBytes(targetFile), false, false, false, false);

			foreach (InterventInfo info in this.Infos)
			{
				if (info.Pattern == target)
				{
					info.Intervent.BeforeDL();
					break;
				}
			}
		}
	}
}
