using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;
using Charlotte.Utils;

namespace Charlotte.Services.SaveLoad
{
	public class Ground
	{
		public static void InitDataDir()
		{
			if (Directory.Exists(Consts.DATA_DIR) == false)
				FileTools.CreateDir(Consts.DATA_DIR);

			DeleteDeadData();
		}

		private class FileData
		{
			public string File;
			public long WroteDateTime;
		}

		private static void DeleteDeadData()
		{
			FileData[] files = Directory.GetFiles(Consts.DATA_DIR).Select(v => new FileData()
			{
				File = v,
				WroteDateTime = DateTimeToSec.ToDateTime(new FileInfo(v).LastWriteTime),
			})
			.ToArray();

			Array.Sort(files, (a, b) =>
			{
				int ret = LongTools.Comp(a.WroteDateTime, b.WroteDateTime);

				if (ret != 0)
					return ret;

				ret = StringTools.CompIgnoreCase(a.File, b.File);
				return ret;
			});

			long currDateTime = DateTimeToSec.Now.GetDateTime();
			long expireDateTime = DateTimeToSec.ToDateTime(DateTimeToSec.ToSec(currDateTime) - Consts.DATA_LIFESPAN_SEC);

			for (int index = 0; index < files.Length; index++)
			{
				bool deleting = index + Consts.DATA_NUM_MAX < files.Length || files[index].WroteDateTime < expireDateTime;

				if (deleting == false)
					break;

				FileTools.Delete(files[index].File);
			}
		}
	}
}
