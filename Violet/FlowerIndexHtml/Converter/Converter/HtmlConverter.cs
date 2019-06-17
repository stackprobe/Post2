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
			List<string> dest = new List<string>();
			bool inScript = false;

			foreach (string fLine in lines)
			{
				string line = fLine;

				if (line == "</script>")
				{
					inScript = false;
				}
				if (inScript)
				{
					StringTools.Island resFilePfx = StringTools.GetIsland(line, "@res:");

					if (resFilePfx != null)
					{
						string resFile = resFilePfx.Right;

						// xxx ファイルタイプはpng固定

						line = "\"data:image/png;base64," + new Base64Unit().Encode(File.ReadAllBytes(resFile)) + "\",";
					}
					else
					{
						line = EncodeLiteral(line);
					}
					dest.Add(line);
				}
				else
				{
					dest.Add(line);
				}
				if (line == "<script>")
				{
					inScript = true;
				}
			}
			File.WriteAllLines(this.OutputHtmlFile, dest, StringTools.ENCODING_SJIS);
		}

		private static string EncodeLiteral(string line)
		{
			StringBuilder buff = new StringBuilder();
			bool inLiteral = false;

			foreach (char chr in line)
			{
				if (chr == '"') // xxx この辺適当...
				{
					buff.Append(chr);
					inLiteral = inLiteral == false;
				}
				else if (inLiteral)
				{
					buff.Append(string.Format("\\u{0:x4}", (int)chr));
				}
				else
				{
					buff.Append(chr);
				}
			}
			return buff.ToString();
		}
	}
}
