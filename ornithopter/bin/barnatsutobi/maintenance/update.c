#include "C:\Dev\Main\Post2\libs\AuthorizationService.h"
#include "C:\Dev\Main\Post2\libs\Parser.h"

void Post2_Auth_Main(char *userPw)
{
	char *oshiraseTitle;
	char *oshiraseBody;
	char *oshiraseStamp;

	LOGPOS();

	oshiraseTitle = GetBodyValue("oshirase-title");
	oshiraseBody  = GetBodyValue("oshirase-body");
	oshiraseStamp = GetBodyValue("oshirase-stamp");

	LOGPOS();

	line2JToken(oshiraseTitle, 1, 1);
	line2JLine(oshiraseBody, 1, 1, 1, 1);
	line2JToken(oshiraseStamp, 1, 1);

	LOGPOS();

	SaveFileRec("C:\\appdata\\barnatsutobi\\oshirase\\title", oshiraseTitle);
	SaveFileRec("C:\\appdata\\barnatsutobi\\oshirase\\body",  oshiraseBody);
	SaveFileRec("C:\\appdata\\barnatsutobi\\oshirase\\stamp", oshiraseStamp);

	LOGPOS();
}
