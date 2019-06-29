using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.IO;

namespace Charlotte
{
	public class JSModuleConverter
	{
		private string HeaderCode;
		private string FooterCode;

		public void LoadResourceDir(string dir)
		{
			this.HeaderCode = File.ReadAllText(Path.Combine(dir, "Header.js"), StringTools.ENCODING_SJIS);
			this.FooterCode = File.ReadAllText(Path.Combine(dir, "Footer.js"), StringTools.ENCODING_SJIS);
		}

		public void LoadSourceDir(string dir)
		{
			dir = FileTools.MakeFullPath(dir);

			foreach (string file in Directory.GetFiles(dir, "*.js", SearchOption.AllDirectories))
			{
				this.LoadSourceFile(file, dir);
			}
		}

		private List<string> Codes = new List<string>();

		public void LoadSourceFile(string file, string rootDir)
		{
			List<string> ptns = new List<string>();

			// @@, @@^, @@^^, @@^^^, ...
			{
				string coName = CommonUtils.PathToCoName(file, rootDir);
				string ptn = "@@";

				for (; ; )
				{
					ptns.Add(ptn);
					ptns.Add(coName);

					coName = CommonUtils.CoNameToParent(coName);

					if (coName == null)
						break;

					ptn += "^";
				}
			}

			Console.WriteLine("ptns: " + string.Join(", ", ptns));

			string code = File.ReadAllText(file, StringTools.ENCODING_SJIS);
			code = StringTools.MultiReplace(code, ptns.ToArray());
			this.Codes.Add(code);
		}

		public void WriteJSFile(string outJSFile)
		{
			using (StreamWriter writer = new StreamWriter(outJSFile, false, StringTools.ENCODING_SJIS))
			{
				writer.WriteLine(this.HeaderCode);

				foreach (string code in this.Codes)
					writer.WriteLine(code);

				writer.WriteLine(this.FooterCode);
			}
		}
	}
}
