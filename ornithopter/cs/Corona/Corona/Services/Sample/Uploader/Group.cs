using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.IO;

namespace Charlotte.Services.Sample.Uploader
{
	public class Group
	{
		public string Dir;
		public string FileBundleDir;
		public string AccessKey; // 他のグループと被ってはならない。
		public string Name;
		public long TotalFileSizeMax;
		public FileDueBundle FileDueBundle;

		public Group(string dir)
		{
			this.Dir = FileTools.MakeFullPath(dir);
			this.FileBundleDir = Path.Combine(this.Dir, Consts.FILE_BUNDLE_LOCAL_DIR);
			this.AccessKey = File.ReadAllLines(Path.Combine(this.Dir, Consts.ACCESS_KEY_LOCAL_FILE), StringTools.ENCODING_SJIS)[0];

			{
				string[] lines = File.ReadAllLines(Path.Combine(this.Dir, Consts.GROUP_INFO_LOCAL_FILE), StringTools.ENCODING_SJIS);
				int c = 0;

				this.Name = lines[c++];
				this.TotalFileSizeMax = long.Parse(lines[c++]);
			}

			this.FileDueBundle = new FileDueBundle(Path.Combine(this.Dir, Consts.FILE_DUE_BUNDLE_LOCAL_FILE));
		}

		public void Save()
		{
			File.WriteAllText(Path.Combine(this.Dir, Consts.ACCESS_KEY_LOCAL_FILE), this.AccessKey, StringTools.ENCODING_SJIS);

			{
				string[] lines = new string[]
				{
					this.Name,
					"" + this.TotalFileSizeMax,
				};

				File.WriteAllLines(Path.Combine(this.Dir, Consts.GROUP_INFO_LOCAL_FILE), lines, StringTools.ENCODING_SJIS);
			}

			this.FileDueBundle.Save(Path.Combine(this.Dir, Consts.FILE_DUE_BUNDLE_LOCAL_FILE));
		}

		public FileBundle GetFileBundle()
		{
			return new FileBundle(this.FileBundleDir);
		}
	}
}
