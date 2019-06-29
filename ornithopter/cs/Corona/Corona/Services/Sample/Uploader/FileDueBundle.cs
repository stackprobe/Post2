using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte.Services.Sample.Uploader
{
	public class FileDueBundle
	{
		public List<FileDue> FileDueList;

		public FileDueBundle(string file)
		{
			this.FileDueList = ArrayTools.ToList(File.ReadAllLines(file, StringTools.ENCODING_SJIS).Select(v =>
			{
				string[] tokens = v.Split('\t').ToArray();

				return new FileDue()
				{
					LocalFile = tokens[0],
					DeleteDateTime = long.Parse(tokens[1]),
				};
			}
			));

			this.FileDueList.Sort((a, b) => StringTools.CompIgnoreCase(a.LocalFile, b.LocalFile));
		}

		public void Save(string file)
		{
			File.WriteAllLines(
				file,
				this.FileDueList.Select(v =>
				{
					string[] tokens = new string[]
					{
						v.LocalFile,
						"" + v.DeleteDateTime,
					};

					return string.Join("\t", tokens);
				}),
				StringTools.ENCODING_SJIS
				);
		}
	}
}
