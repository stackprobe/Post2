using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte
{
	public class HtmlConverter
	{
		public string InputHtmlFile;
		public string OutputHtmlFile;

		// <---- prm

		public void Perform()
		{
			string[] lines = File.ReadAllLines(this.InputHtmlFile, StringTools.ENCODING_SJIS);

			lines = ResolveResource(lines);
			lines = WrapScript(lines); // 1
			lines = WrapScript(lines); // 2
			lines = WrapScript(lines); // 3

			File.WriteAllLines(this.OutputHtmlFile, lines, StringTools.ENCODING_SJIS);
		}

		private string[] WrapScript(string[] lines)
		{
			List<string> before = new List<string>();
			List<string> scriptLines = new List<string>();
			List<string> after = new List<string>();
			List<string> dest = before;

			for (int index = 0; index < lines.Length; index++)
			{
				if (IsEndScript(lines[index]))
					dest = after;

				dest.Add(lines[index]);

				if (IsStartScript(lines[index]))
					dest = scriptLines;
			}
			string script = string.Join("\r\n", scriptLines);

			script = new Base64Unit().Encode(Encoding.UTF8.GetBytes(script));
			script = "eval(decodeURIComponent(escape(atob(\"" + script + "\"))));";

			dest = new List<string>();
			dest.AddRange(before);
			dest.Add(script);
			dest.AddRange(after);

			return dest.ToArray();
		}

		private string[] ResolveResource(string[] lines)
		{
			List<string> dest = new List<string>();
			bool inScript = false;

			foreach (string fLine in lines)
			{
				string line = fLine;

				if (IsEndScript(line))
				{
					inScript = false;
				}
				if (inScript)
				{
					if (line.EndsWith("// @res"))
					{
						StringTools.Enclosed literal = StringTools.GetEnclosed(line, "\"", "\"");
						string resFile = literal.Inner;

						resFile = Path.Combine(Path.GetDirectoryName(this.InputHtmlFile), resFile);

						string dataUrl = "data:" + GetFileType(resFile) + ";base64," + new Base64Unit().Encode(File.ReadAllBytes(resFile));

						line = literal.Left + dataUrl + literal.Right;
					}
					dest.Add(line);
				}
				else
				{
					dest.Add(line);
				}
				if (IsStartScript(line))
				{
					inScript = true;
				}
			}
			return dest.ToArray();
		}

		private static bool IsStartScript(string line)
		{
			return line == "<script>";
		}

		private static bool IsEndScript(string line)
		{
			return line == "</script>";
		}

		private static string GetFileType(string file)
		{
			string ext = Path.GetExtension(file).ToLower();

			if (ext == ".png")
				return "image/png";

			// ここへ追加

			throw new Exception("Unknown extension: " + ext);
		}
	}
}
