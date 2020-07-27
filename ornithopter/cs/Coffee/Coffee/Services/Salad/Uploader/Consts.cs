﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte.Services.Salad.Uploader
{
	public static class Consts
	{
		public const string GROUP_BUNDLE_DIR = @"C:\appdata\Post2.Coffee.Salad.Uploader";

		public const long TOTAL_FILE_SIZE_MAX = 138000000000L; // 138 GB

		public const string ACCESS_KEY_LOCAL_FILE = "access-key.txt";
		public const string GROUP_INFO_LOCAL_FILE = "info.txt";
		public const string FILE_BUNDLE_LOCAL_DIR = "oh";

		public const int GROUP_NUM_MAX = 18;
		public const int FILE_NUM_MAX = 108;

		public const int GROUP_LOCAL_DIR_LEN_MIN = 20;
		public const int GROUP_LOCAL_DIR_LEN_MAX = 50; // ローカルファイル名の分を侵食しないよう、短めに、
		public const int GROUP_NAME_LEN_MAX = 300;
		public const int ACCESS_KEY_LEN_MAX = 300;

		public const string TMP_FILE_SUFFIX = ".tmp";

		public const string SESSION_BUNDLE_FILE = @"C:\appdata\Post2.Coffee.Salad.Uploader.Session.txt";

		public const int SESSION_NUM_LMT = 100;
		public const int SESSION_TIMEOUT_SEC = 3600;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// $_git:secret

		// ----

		public const string S_TRUE = "true";
		public const string S_FALSE = "false";
	}
}
