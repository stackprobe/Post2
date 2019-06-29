using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Charlotte.Services.Sample.Uploader
{
	public class Room
	{
		public string AccessKey;
		public string Title;
		public string Ident; // Roomフォルダ名 == ダウンロード時のパスの一部になる。
		public long TotalFileSizeMax;
	}
}
