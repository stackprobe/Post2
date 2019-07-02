using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte.Services.Sample.Uploader
{
	public class FileBundle
	{
		public string Dir;
		public string[] Files;

		public FileBundle(string dir)
		{
			this.Dir = FileTools.MakeFullPath(dir);

			this.Files = Directory.GetFiles(this.Dir).ToArray();
			Array.Sort(this.Files, StringTools.CompIgnoreCase);
		}

		public long GetTotalFileSize()
		{
			long ret = 0L;

			foreach (string file in this.Files)
				ret += new FileInfo(file).Length;

			return ret;
		}
	}
}
