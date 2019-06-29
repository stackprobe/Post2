using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Charlotte.Tools;

namespace Charlotte.Services.Sample.Uploader
{
	public class Consts
	{
		/// <summary>
		/// この直下にルーム(ディレクトリ)を配置する。
		/// ルームの直下にファイルを配置する。
		/// </summary>
		public const string ROOT_DIR = @"C:\appdata\Post2.Corona.Sample.Uploader";

		public const long TOTAL_FILE_SIZE_MAX = 138000000000L; // 138 GB
		public const int ROOM_NUM_MAX = 1000;
		public const int FILE_NUM_MAX = 1000;

		public const string ROOMS_FILE = @"C:\appdata\Post2.Corona.Sample.Uploader.Rooms.txt";

		public const int ACCESS_KEY_LEN_MAX = 300;
		public const int ROOM_NAME_LEN_MAX = 300;

		public const string SESSIONS_FILE = @"C:\appdata\Post2.Corona.Sample.Uploader.Sessions.txt";

		public const int SESSION_NUM_MAX = 1000;
		public const int SESSION_TIMEOUT_SEC = 86400;

		////// ///// ////// ///////////////////// / ///////////////////////////////////////////////////////////////////////////////// // $_git:secret
	}
}
