using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte
{
	public class HTTPRequest
	{
		public static HTTPRequest I;

		public string IP;
		public string Method;
		public string URLPath;
		public string Path;
		public Dictionary<string, string> Query = DictionaryTools.CreateIgnoreCase<string>();
		public string HTTP_Version;
		public Dictionary<string, string> HeaderPairs = DictionaryTools.CreateIgnoreCase<string>();
	}
}
