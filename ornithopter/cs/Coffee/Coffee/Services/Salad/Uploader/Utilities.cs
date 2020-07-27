using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using Charlotte.Utils;
using System.IO;

namespace Charlotte.Services.Salad.Uploader
{
	public static class Utilities
	{
		public static string GetString(bool flag)
		{
			return flag ? Consts.S_TRUE : Consts.S_FALSE;
		}

		public static bool GetBoolean(string str)
		{
			return str == Consts.S_TRUE;
		}

		public static void CheckLength(string str, int minlen, int maxlen, string description)
		{
			if (str.Length < minlen || maxlen < str.Length)
			{
				throw new Exception("『" + description + "』は " + minlen + " 文字以上 " + maxlen + " 文字以下でなければなりません。");
			}
		}

		public static void CheckRange(long value, long minval, long maxval, string description)
		{
			if (value < minval || maxval < value)
			{
				throw new Exception("『" + description + "』は " + minval + " 以上 " + maxval + " 以下でなければなりません。");
			}
		}

		public static string ToCreatableFairFullPath(string path)
		{
			path = FileTools.MakeFullPath(path);

			string dir = Path.GetDirectoryName(path);
			string localPath = Path.GetFileName(path);

			localPath = DenebolaToolkit.GetFairLocalPath(localPath, dir + "0123456789"); // 重複回避のため追加される文字列のためにマージンを取る。

			path = Path.Combine(dir, localPath);
			path = ExtraTools.ToCreatablePath(path);

			return path;
		}
	}
}
