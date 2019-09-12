using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte.Services.Salad.Uploader
{
	public class Slimdown
	{
		public bool DiskYellowFlag = false;

		// <---- prm

		private class UploadedFile
		{
			public string FilePath;
			public long Size;
			public long WroteDateTime;

			public void Delete()
			{
				ProcMain.WriteLog("削除 ⇒ " + this.FilePath);

				FileTools.Delete(this.FilePath);
			}
		}

		private List<UploadedFile> UploadedFiles = new List<UploadedFile>();
		private long TotalSize = 0L;

		public void Perform()
		{
			Console.WriteLine("Slimdown_Start, DY: " + this.DiskYellowFlag);

			long currDateTime = DateTimeToSec.Now.GetDateTime();

			foreach (LiteGroup liteGroup in new GroupBundle().LiteGroups)
			{
				Group group = liteGroup.GetGroup();

				List<UploadedFile> upFiles = new List<UploadedFile>();
				long total = 0L;

				foreach (string file in group.GetFileBundle().Files)
				{
					FileInfo info = new FileInfo(file);

					UploadedFile upFile = new UploadedFile()
					{
						FilePath = file,
						Size = info.Length,
						WroteDateTime = DateTimeToSec.ToDateTime(info.LastWriteTime),
					};

					upFiles.Add(upFile);
					total += upFile.Size;
				}
				upFiles.Sort((a, b) =>
				{
					int ret = VariantTools.Comp(a, b, v =>
						StringTools.EndsWithIgnoreCase(v.FilePath, Consts.TMP_FILE_SUFFIX) &&
						DateTimeToSec.ToSec(v.WroteDateTime) + 3600 <= DateTimeToSec.ToSec(currDateTime) ? 0 : 1); // "１時間以上放置された作業ファイル" を先に

					if (ret != 0)
						return ret;

					ret = LongTools.Comp(a.WroteDateTime, b.WroteDateTime); // 古い順
					return ret;
				});

				while (1 <= upFiles.Count && (Consts.FILE_NUM_MAX < this.UploadedFiles.Count || group.GroupTotalFileSizeMax < total))
				{
					UploadedFile upFile = upFiles[0];

					upFiles.RemoveAt(0);

					upFile.Delete();
					total -= upFile.Size;
				}
				this.UploadedFiles.AddRange(upFiles);
				this.TotalSize += total;
			}
			this.UploadedFiles.Sort((a, b) => LongTools.Comp(a.WroteDateTime, b.WroteDateTime)); // 古い順

			while (1 <= this.UploadedFiles.Count && Consts.TOTAL_FILE_SIZE_MAX < this.TotalSize)
			{
				UploadedFile upFile = this.UploadedFiles[0];

				this.UploadedFiles.RemoveAt(0);

				FileTools.Delete(upFile.FilePath);
				this.TotalSize -= upFile.Size;
			}
			if (this.DiskYellowFlag)
			{
				int count = this.UploadedFiles.Count;

				// 10ファイル以上 -> 10%
				// 1～9ファイル -> 1
				// 0ファイル -> 0
				//
				if (10 <= count)
					count /= 10;
				else
					count = Math.Min(1, count);

				for (int index = 0; index < count; index++)
				{
					UploadedFile upFile = this.UploadedFiles[index];

					upFile.Delete();
				}
			}
			Console.WriteLine("Slimdown_End");
		}
	}
}
