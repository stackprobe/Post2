using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using Charlotte.Tools;

namespace Charlotte
{
	class Program
	{
		public const string APP_IDENT = "{5f88321b-fe1f-44f8-a2ea-96597b84aeae}";
		public const string APP_TITLE = "Converter";

		static void Main(string[] args)
		{
			ProcMain.CUIMain(new Program().Main2, APP_IDENT, APP_TITLE);

#if DEBUG
			//if (ProcMain.CUIError)
			{
				//Console.WriteLine("Press ENTER.");
				//Console.ReadLine();
			}
#endif
		}

		private void Main2(ArgsReader ar)
		{
			string resDir = ar.NextArg();
			string srcDir = ar.NextArg();
			string outJSFile = ar.NextArg();

			JSModuleConverter mc = new JSModuleConverter();

			mc.LoadResourceDir(resDir);
			mc.LoadSourceDir(srcDir);
			mc.WriteJSFile(outJSFile);
		}
	}
}
