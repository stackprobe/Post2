using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.IO;

namespace Charlotte
{
	public static class CommonUtils
	{
		public static string PathToCoName(string path, string root)
		{
			path = FileTools.ChangeRoot(path, root);
			path = Path.Combine(
				Path.GetDirectoryName(path),
				StringTools.GetIsland(Path.GetFileName(path), ".").Left
				);
			string coName = path.Replace('\\', '_');
			return coName;
		}

		public static string CoNameToParent(string name)
		{
			int index = name.LastIndexOf('_');

			if (index == -1)
				return null;

			return name.Substring(0, index);
		}
	}
}
