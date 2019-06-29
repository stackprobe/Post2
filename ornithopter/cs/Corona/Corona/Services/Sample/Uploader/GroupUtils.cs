using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.IO;

namespace Charlotte.Services.Sample.Uploader
{
	public class GroupUtils
	{
		public static void CreateGroup(string name = "名無しのグループさん", long totalFileSizeMax = 1000000000L)
		{
			string dir = Path.Combine(Consts.GROUP_BUNDLE_DIR, CreateLocalDir());
			string fileBundleDir = Path.Combine(dir, Consts.FILE_BUNDLE_LOCAL_DIR);

			FileTools.CreateDir(dir);
			FileTools.CreateDir(fileBundleDir);

			File.WriteAllText(Path.Combine(dir, Consts.ACCESS_KEY_LOCAL_FILE), CreateAccessKey(), StringTools.ENCODING_SJIS);

			{
				string[] lines = new string[]
				{
					name,
					"" + totalFileSizeMax,
				};

				File.WriteAllLines(Path.Combine(dir, Consts.GROUP_INFO_LOCAL_FILE), lines, StringTools.ENCODING_SJIS);
			}

			File.WriteAllBytes(Path.Combine(dir, Consts.GROUP_INFO_LOCAL_FILE), BinTools.EMPTY);
		}

		private static string CreateLocalDir()
		{
			return SecurityTools.MakePassword_9();
		}

		private static string CreateAccessKey()
		{
			return SecurityTools.MakePassword_9();
		}
	}
}
