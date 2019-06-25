using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte
{
	public class RootGround
	{
		public static RootGround I;

		public ServiceDistributor ServiceDistributor = new ServiceDistributor();

		public string IP;
		public string Method;
		public string Path;
		public PathQuery PathQuery;
		public string HTTP_Version;
		public Dictionary<string, string> HeaderPairs = DictionaryTools.CreateIgnoreCase<string>();
		public byte[] Body;

		public byte[] ResBody = new byte[0];
		public List<string[]> ResHeaderPairs = new List<string[]>();
	}
}
