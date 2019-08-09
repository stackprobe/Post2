using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte
{
	public static class BeforeDLInterventUtils
	{
		public static string RecvFileToQuery()
		{
			string firstLine;

			using (StreamReader reader = new StreamReader(BeforeDLIntervent.RECV_FILE, Encoding.ASCII))
			{
				firstLine = reader.ReadLine();
			}
			string[] firstLineTokens = firstLine.Split(' ');

			if (firstLineTokens.Length != 3)
				throw new Exception("Bad HTTP request first line");

			string url = firstLineTokens[1];
			StringTools.Island urlQuestion = StringTools.GetIsland(url, "?");

			if (urlQuestion == null)
				throw new Exception("クエリは指定されていません。");

			return urlQuestion.Right;
		}

		public static Dictionary<string, string> ParseQuery(string query)
		{
			Dictionary<string, string> dest = DictionaryTools.CreateIgnoreCase<string>();

			foreach (string qToken in query.Split('&'))
			{
				string[] qPair = qToken.Split('=');

				if (qPair.Length == 2)
				{
					dest.Add(qPair[0], qPair[1]);
				}
			}
			return dest;
		}
	}
}
