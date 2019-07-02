using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using Charlotte.Utils;

namespace Charlotte.Services.Sample.Uploader
{
	public class CodeDefinition
	{
		public class ToFair
		{
			public static string AccessKey(string accessKey)
			{
				accessKey = JString.ToJString(accessKey, true, false, false, true).Trim();

				Utilities.CheckLength(accessKey, 1, Consts.ACCESS_KEY_LEN_MAX, "アクセスキー");

				return accessKey;
			}

			public static string GroupLocalDir(string localDir)
			{
				localDir = DenebolaToolkit.GetFairLocalPath(localDir, Consts.GROUP_BUNDLE_DIR);

				Utilities.CheckLength(localDir, 1, Consts.GROUP_LOCAL_DIR_LEN_MAX, "グループ・ローカルディレクトリ名");

				return localDir;
			}

			public static string GroupName(string name)
			{
				name = JString.ToJString(name, true, false, false, true).Trim();

				Utilities.CheckLength(name, 1, Consts.GROUP_NAME_LEN_MAX, "グループ名");

				return name;
			}
		}
	}
}
