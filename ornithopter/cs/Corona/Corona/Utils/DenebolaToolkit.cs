﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

// ^ sync @ DenebolaToolkit

namespace Charlotte.Utils
{
	// sync > @ DenebolaToolkit

	public static class DenebolaToolkit
	{
		public static string GetFairLocalPath(string localPath, string dir)
		{
			using (WorkingDir wd = new WorkingDir())
			{
				string argsFile = wd.MakePath();
				string outFile = wd.MakePath();
				string molp = SecurityTools.MakePassword_9();

				string[] args =
				{
					"//O",
					outFile,
					"//MOLP",
					molp,
					"//$",
					JString.AsLine(localPath),
					JString.AsLine(dir),
				};

				File.WriteAllLines(argsFile, args, StringTools.ENCODING_SJIS);

				ProcessTools.Start(@"C:\Factory\Bodewig\DenebolaToolkit\FairLocalPath.exe", "//R \"" + argsFile + "\"").WaitForExit();

				string[] outLines = File.ReadAllLines(outFile, StringTools.ENCODING_SJIS)
					.Where(v => v.StartsWith(molp))
					.Select(v => v.Substring(molp.Length)).ToArray();

				if (outLines.Length != 1)
					throw null; // 想定外

				return outLines[0];
			}
		}

		public static string GetFairFullPath(string path)
		{
			using (WorkingDir wd = new WorkingDir())
			{
				string argsFile = wd.MakePath();
				string outFile = wd.MakePath();
				string molp = SecurityTools.MakePassword_9();

				string[] args =
				{
					"//O",
					outFile,
					"//MOLP",
					molp,
					"//-E",
					"//$",
					JString.AsLine(path),
				};

				File.WriteAllLines(argsFile, args, StringTools.ENCODING_SJIS);

				ProcessTools.Start(@"C:\Factory\Bodewig\DenebolaToolkit\FairFullPath.exe", "//R \"" + argsFile + "\"").WaitForExit();

				string[] outLines = File.ReadAllLines(outFile, StringTools.ENCODING_SJIS)
					.Where(v => v.StartsWith(molp))
					.Select(v => v.Substring(molp.Length)).ToArray();

				if (outLines.Length != 1)
					throw new Exception("不正なパス文字列です。");

				return outLines[0];
			}
		}
	}

	// < sync
}
