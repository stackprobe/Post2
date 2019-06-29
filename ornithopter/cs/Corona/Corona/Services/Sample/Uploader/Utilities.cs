using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Charlotte.Services.Sample.Uploader
{
	public class Utilities
	{
		public static string GetString(bool flag)
		{
			return flag ? Consts.S_TRUE : Consts.S_FALSE;
		}

		public static bool GetBoolean(string str)
		{
			return str == Consts.S_TRUE;
		}
	}
}
