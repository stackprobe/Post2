using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Utils;

namespace Charlotte.Services.Sample.Uploader
{
	public class DownloadURL
	{
		public string GroupLocalDir;
		public string LocalFile;
		public bool DownloadFlag;

		// <---- prm

		public string GetString()
		{
			return
				"http://" +
				"ornithopter.myhome.cx:58946/corona/sample/uploader/download.alt.txt*/" +
				CommonUtils.EncodeURL(this.GroupLocalDir) +
				"?group=" +
				CommonUtils.EncodeURL(this.LocalFile) +
				"&file=" +
				CommonUtils.EncodeURL(this.GroupLocalDir) +
				(this.DownloadFlag ? "&download=true" : "");
		}
	}
}
