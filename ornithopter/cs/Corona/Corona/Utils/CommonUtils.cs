using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte.Utils
{
	public static class CommonUtils
	{
		public static string DecodeURL(string url)
		{
			byte[] bUrl = Encoding.ASCII.GetBytes(url);

			using (MemoryStream dest = new MemoryStream())
			{
				for (int index = 0; index < bUrl.Length; index++)
				{
					if (bUrl[index] == 0x25) // ? '%'
					{
						dest.WriteByte((byte)Convert.ToInt32(Encoding.ASCII.GetString(BinTools.GetSubBytes(bUrl, index + 1, 2)), 16));
						index += 2;
					}
					else if (bUrl[index] == 0x2b) // ? '+'
					{
						dest.WriteByte(0x20); // ' '
					}
					else
					{
						dest.WriteByte(bUrl[index]);
					}
				}
				return Encoding.UTF8.GetString(dest.ToArray());
			}
		}

		public static string EncodeURL(string url)
		{
			byte[] bUrl = Encoding.UTF8.GetBytes(url);
			StringBuilder buff = new StringBuilder();

			foreach (byte chr in bUrl)
			{
				if (
					0x30 <= chr && chr <= 0x39 || // 半角数字
					0x41 <= chr && chr <= 0x5a || // 半角英大文字
					0x61 <= chr && chr <= 0x7a    // 半角英小文字
					)
				{
					buff.Append((char)chr);
				}
				else
				{
					buff.Append('%');
					buff.Append(StringTools.hexadecimal[(int)chr >> 4]);
					buff.Append(StringTools.hexadecimal[(int)chr & 15]);
				}
			}
			return buff.ToString();
		}
	}
}
