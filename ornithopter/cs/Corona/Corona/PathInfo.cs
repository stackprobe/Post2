using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte
{
	public class PathQuery
	{
		public string Path;
		public Dictionary<string, string> Query = DictionaryTools.CreateIgnoreCase<string>();

		public PathQuery(string path)
		{
			int queryIndex = path.IndexOf('?');

			if (queryIndex == -1)
			{
				this.Path = path;
			}
			else
			{
				this.Path = path.Substring(0, queryIndex);

				string query = path.Substring(queryIndex + 1);
				string[] qTkns = query.Split('&');

				foreach (string qTkn in qTkns)
				{
					string[] qPair = qTkn.Split('=');

					if (qPair.Length == 2)
					{
						this.Query.Add(qPair[0], qPair[1]);
					}
				}
			}
		}
	}
}
