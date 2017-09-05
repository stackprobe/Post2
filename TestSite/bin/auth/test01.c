#include "C:\Dev\Main\Post2\libs\Service.h"

void Post2_Main(void)
{
	char *auth = GetHeaderValue("Authorization");

	if(!*auth)
	{
		Res.StatusCode = 401;
		addElement(Res.HeaderLines, (uint)"WWW-Authenticate: Basic realm=\"Post2_Auth_1111:9999\"");
		Res.Body = ab_fromLine(
			"<html>"
			"<body style=\"font-family: メイリオ; font-size: large;\">"
			"Authorization Test!"
			"</body>"
			"</html>"
			);

		// Chrome, IE -- ユーザー・パスワードの入力ダイアログに realm= の文字列が表示される。
		// Chrome, IE -- ユーザー・パスワードの入力をキャンセルしたとき、Res.Body を表示する。
	}
	else if(strcmp(auth, "Basic MTExMTo5OTk5")) // ? ! 1111:9999
	{
		cout("auth: [%s]\n", auth);

		Res.StatusCode = 401;
		addElement(Res.HeaderLines, (uint)"WWW-Authenticate: Basic realm=\"Post2_Auth_Fault_1111:9999\"");
		Res.Body = ab_fromLine_x(xcout(
			"<html>"
			"<body style=\"font-family: メイリオ; font-size: large;\">"
			"Authorization: [%s]"
			"</body>"
			"</html>"
			,auth
			));
	}
	else
	{
		Res.Body = ab_fromLine_x(xcout(
			"<html>"
			"<body style=\"font-family: メイリオ; font-size: large;\">"
			"Authorization OK!"
			"</body>"
			"</html>"
			,auth
			));
	}
}
