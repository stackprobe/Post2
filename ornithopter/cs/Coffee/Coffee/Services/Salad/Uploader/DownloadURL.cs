using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Utils;

namespace Charlotte.Services.Salad.Uploader
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
				"ornithopter.ccsp.mydns.jp:58946/coffee/salad/uploader/download.alt.txt*/" +
				CommonUtils.EncodeURL(this.LocalFile) +
				"?group=" +
				CommonUtils.EncodeURL(this.GroupLocalDir) +
				"&file=" +
				CommonUtils.EncodeURL(this.LocalFile) +
				(this.DownloadFlag ? "&download=true" : "");
		}
	}
}
