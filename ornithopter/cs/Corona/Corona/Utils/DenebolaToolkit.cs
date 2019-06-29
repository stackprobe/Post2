using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using System.IO;

// ^ sync @ DenebolaToolkit

namespace Charlotte.Utils
{
	// sync > @ DenebolaToolkit

	public class DenebolaToolkit
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

				ProcessTools.Start(@"C:\Factory\Bodewig\DenebolaTools\FairLocalPath.exe", "//R \"" + argsFile + "\"").WaitForExit();

				string[] outLines = File.ReadAllLines(outFile).Where(v => v.StartsWith(molp)).Select(v => v.Substring(molp.Length)).ToArray();

				if (outLines.Length != 1)
					throw null; // 想定外

				return outLines[0];
			}
		}
	}

	// < sync
}
