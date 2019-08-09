using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte.Services.Sample.Uploader
{
	public static class SessionUtils
	{
		public static string CreateSessionKey()
		{
			return SecurityTools.MakePassword_9();
		}
	}
}
