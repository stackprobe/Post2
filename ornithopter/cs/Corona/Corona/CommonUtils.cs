using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte
{
	public class CommonUtils
	{
		public static void ToFairLine(ref string line)
		{
			line = JString.ToJString(line, true, false, true, true);
		}

		public static void ToFairText(ref string text)
		{
			text = JString.ToJString(text, true, true, true, true);
		}
	}
}
