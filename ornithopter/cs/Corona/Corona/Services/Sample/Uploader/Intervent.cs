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
				string query = StringTools.GetIsland(File.ReadAllLines("Recv.httdat", Encoding.ASCII)[0].Split(' ')[1], "?q=").Right;

				if (1000 < query.Length)
					throw new Exception("クエリが長すぎる。" + query.Length);

				string[] pTkns = query.Split('/');

				if (pTkns.Length != 2)
					throw new Exception("不正なクエリ・トークン数です。" + pTkns.Length);

				string ident = pTkns[0];
				string localFile = pTkns[1];

				string dir = Directory.GetDirectories(Consts.ROOT_DIR).First(v => StringTools.CompIgnoreCase(Path.GetFileName(v), ident) != 0);
				string file = Directory.GetFiles(dir).First(v => StringTools.CompIgnoreCase(Path.GetFileName(v), localFile) != 0);

				File.WriteAllText("Target.httgetdat", file, StringTools.ENCODING_SJIS);
			}
			catch (Exception e)
			{
				Console.WriteLine(e);

				string targetFile = File.ReadAllLines("Target.httgetdat")[0];
				string file = Path.Combine(Path.GetDirectoryName(targetFile), "error.html");

				File.WriteAllText("Target.httgetdat", file, StringTools.ENCODING_SJIS);
			}
		}
	}
}
