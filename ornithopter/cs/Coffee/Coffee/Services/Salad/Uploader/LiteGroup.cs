using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.IO;

namespace Charlotte.Services.Salad.Uploader
{
	public class LiteGroup
	{
		public string Dir;
		public string AccessKey;

		public LiteGroup(string dir)
		{
			this.Dir = FileTools.MakeFullPath(dir);

			{
				string accessKeyFile = Path.Combine(this.Dir, Consts.ACCESS_KEY_LOCAL_FILE);

				this.AccessKey = File.ReadAllLines(accessKeyFile, StringTools.ENCODING_SJIS)[0];
			}
		}

		public Group GetGroup()
		{
			return new Group(this.Dir);
		}
	}
}
