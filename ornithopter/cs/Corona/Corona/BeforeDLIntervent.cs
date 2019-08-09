using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Charlotte.Tools;

namespace Charlotte
{
	public static class BeforeDLIntervent
	{
		public const string RECV_FILE = "Recv.httdat";
		public const string TARGET_FILE = "Target.httgetdat";
		public const string TARGET_CONTENT_TYPE_FILE = "TargetContentType.httgetdat";

		public static void Perform()
		{
			if (
				File.Exists("IP.httdat") == false ||
				File.Exists("Recv.httdat") == false ||
				File.Exists("Send.httdat") == false
				)
				throw new Exception("このプログラムは Get.exe から呼び出されます。");

			string targetPath = File.ReadAllLines(TARGET_FILE)[0];

			if (StringTools.EndsWithIgnoreCase(targetPath, ".alt.txt") == false)
				return;

			string intervateClassName = File.ReadAllLines(targetPath, Encoding.ASCII)[0];
			Type intervateClass = Type.GetType(intervateClassName);
			ReflectTools.MethodUnit intervateCtor = ReflectTools.GetConstructor(intervateClass);
			IIntervent intervate = (IIntervent)intervateCtor.Construct(new object[0]);

			intervate.BeforeDL();
		}
	}
}
