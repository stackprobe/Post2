﻿using System;
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

			lines = ResolveErase(lines);
			lines = ResolveResource(lines);
			lines = WrapScript(lines);

			File.WriteAllLines(this.OutputHtmlFile, lines, StringTools.ENCODING_SJIS);
		}

		private string[] WrapScript(string[] lines)
		{
			List<string> before = new List<string>();
			List<string> scriptLines = new List<string>();
			List<string> after = new List<string>();
			List<string> dest = before;

			foreach (string line in lines)
			{
				if (line == "// @encodeBgn")
					dest = scriptLines;
				else if (line == "// @encodeEnd")
					dest = after;
				else
					dest.Add(line);
			}
			string script = string.Join("\r\n", scriptLines);

			if (script != "")
			{
				script = ScriptToBase64(script);

				char m = ArrayTools.Largest((StringTools.DECIMAL + StringTools.ALPHA + StringTools.alpha)
					.Where(v => script[0] != v && script[script.Length - 1] != v) // 2bs
					.Select(v => new { c = v, n = script.Where(w => w == v).Count() }), (a, b) => a.n - b.n).c;

				script = string.Join("" + m, script.Split(m).Reverse());
				script =
					"var s=\"" +
					script +
					"\",m=\"" +
					m +
					"\";" +
					ScriptEscape("eval(decodeURIComponent(escape(atob(s.split(m).reverse().join(m)))));");
			}
			dest = new List<string>();
			dest.AddRange(before);
			dest.Add(script);
			dest.AddRange(after);

			return dest.ToArray();
		}

		private string ScriptToBase64(string script)
		{
			for (; ; )
			{
				string ret = new Base64Unit().Encode(Encoding.UTF8.GetBytes(script));

				if (ret[ret.Length - 1] != '=')
					return ret;

				script += ' ';
			}
		}

		private string ScriptEscape(string script)
		{
			StringBuilder buff = new StringBuilder();

			foreach (char chr in script)
			{
				if ((StringTools.DECIMAL + StringTools.ALPHA + StringTools.alpha).Contains(chr))
					buff.Append(string.Format("\\u{0:x4}", (int)chr));
				else
					buff.Append(chr);
			}
			return buff.ToString();
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

			// 新しいタイプをここへ追加..

			throw new Exception("Unknown extension: " + ext);
		}

		private string[] ResolveErase(string[] lines)
		{
			List<string> dest = new List<string>();

			foreach (string line in lines)
				if (line.EndsWith("// @erase") == false)
					dest.Add(line);

			return dest.ToArray();
		}
	}
}
