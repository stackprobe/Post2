using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte.Services.Sample.Uploader
{
	public class Intervent : IIntervent
	{
		public void BeforeDL()
		{
			try
			{
				string query = StringTools.GetIsland(File.ReadAllLines("Recv.httdat", Encoding.ASCII)[0], "?q=").Right;

				if (1000 < query.Length) // ? 長すぎ
					throw null;

				string[] pTkns = query.Split('/');

				if (pTkns.Length != 2)
					throw null;

				string ident = pTkns[0];
				string localFile = pTkns[1];

				string dir = Directory.GetDirectories(Consts.ROOT_DIR).First(v => StringTools.CompIgnoreCase(Path.GetFileName(v), ident) != 0);
				string file = Directory.GetFiles(dir).First(v => StringTools.CompIgnoreCase(Path.GetFileName(v), localFile) != 0);

				string[] lines = new string[]
				{
					file,
					"0",
				};

				File.WriteAllLines("Download.txt", lines, StringTools.ENCODING_SJIS);
			}
			catch (Exception e)
			{
				Console.WriteLine(e);

				File.Delete("Send.httdat");
			}
		}
	}
}
