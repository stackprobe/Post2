﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

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
			public long WroteTime;
		}

		private static void DeleteDeadData()
		{
			FileData[] files = Directory.GetFiles(Consts.DATA_DIR).Select(v => new FileData()
			{
				File = v,
				WroteTime = new FileInfo(v).LastWriteTime.Ticks / 10000000L, // 100ns -> 秒
			})
			.ToArray();

			Array.Sort(files, (a, b) =>
			{
				int ret = LongTools.Comp(a.WroteTime, b.WroteTime);

				if (ret != 0)
					return ret;

				ret = StringTools.CompIgnoreCase(a.File, b.File);
				return ret;
			});

			long currTime = DateTime.Now.Ticks / 10000000L;
			long deadTime = currTime - (long)Consts.DATA_LIFESPAN_SEC;

			for (int index = 0; index < files.Length; index++)
			{
				bool dead = index + Consts.DATA_NUM_MAX < files.Length || files[index].WroteTime < deadTime;

				if (dead == false)
					break;

				FileTools.Delete(files[index].File);
			}
		}
	}
}
