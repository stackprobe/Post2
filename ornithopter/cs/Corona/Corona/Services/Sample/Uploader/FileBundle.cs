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

		public FileBundle(string dir)
		{
			this.Dir = dir;
		}

		public string[] GetFiles()
		{
			string[] files = Directory.GetFiles(this.Dir).ToArray();
			Array.Sort(files, StringTools.CompIgnoreCase);
			return files;
		}

		public long GetTotalFileSize()
		{
			long ret = 0L;

			foreach (string file in this.GetFiles())
				ret += new FileInfo(file).Length;

			return ret;
		}
	}
}
