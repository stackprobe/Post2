using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;
using Charlotte.Utils;

namespace Charlotte.Services.Sample.Uploader
{
	public class Session
	{
		public string SessionKey;
		public string AccessKey; // ログイン中に削除・変更されたり、supervisor のときなど、対応する Group が存在しないこともある。
		public bool SupervisorMode;
		public long LastAccessedDateTime;

		public void Accessed()
		{
			this.LastAccessedDateTime = DateTimeToSec.Now.GetDateTime();
		}
	}
}
