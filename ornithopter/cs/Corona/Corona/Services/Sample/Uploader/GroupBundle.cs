using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte.Services.Sample.Uploader
{
	public class GroupBundle
	{
		public List<LiteGroup> LiteGroups = new List<LiteGroup>();

		public GroupBundle()
		{
			this.LiteGroups = ArrayTools.ToList(Directory.GetDirectories(Consts.GROUP_BUNDLE_DIR).Select(v => new LiteGroup(v)));

			this.LiteGroups.Sort((a, b) => StringTools.Comp(a.AccessKey, b.AccessKey));
		}

		public int GetTotalFileNum()
		{
			int ret = 0;

			foreach (LiteGroup liteGroup in this.LiteGroups)
				ret += liteGroup.GetGroup().GetFileBundle().GetFiles().Length;

			return ret;
		}

		public long GetTotalFileSize()
		{
			long ret = 0L;

			foreach (LiteGroup liteGroup in this.LiteGroups)
				ret += liteGroup.GetGroup().GetFileBundle().GetTotalFileSize();

			return ret;
		}
	}
}
