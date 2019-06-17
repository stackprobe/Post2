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
		public const string APP_IDENT = "{1a29010f-b6d5-436f-95f1-6254d0aacfe9}";
		public const string APP_TITLE = "Converter";

		static void Main(string[] args)
		{
			ProcMain.CUIMain(new Program().Main2, APP_IDENT, APP_TITLE);

#if DEBUG
			if (ProcMain.CUIError)
			{
				Console.WriteLine("Press ENTER.");
				Console.ReadLine();
			}
#endif
		}

		private void Main2(ArgsReader ar)
		{
			Directory.SetCurrentDirectory(ExtraTools.GetHomeDir());

			// ----

			HtmlConverter hc = new HtmlConverter();

			hc.InputHtmlFile = @"src\index.html";
			hc.OutputHtmlFile = @"out\index.html";

			hc.Perform();
		}
	}
}
