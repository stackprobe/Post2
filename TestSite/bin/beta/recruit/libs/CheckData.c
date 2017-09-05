#include "all.h"

int IsFairMailAddress(char *mailaddress)
{
	errorCase(!mailaddress);

	return
		lineExp("<1,100,-.09AZaz>@<1,100,-.09AZaz>", mailaddress) &&
		!lineExp(".<>", mailaddress) &&
		!lineExp("<>.", mailaddress) &&
		!lineExp("<>..<>", mailaddress);
}
int IsFairPassword(char *password)
{
	errorCase(!password);

	return lineExp("<1,100,09AZaz>", password);
}
int IsFairUUID(char *uuid)
{
	errorCase(!uuid);

	return lineExp("{<8,09AFaf>-<4,09AFaf>-<4,09AFaf>-<4,09AFaf>-<12,09AFaf>}", uuid); // not_uuid
}
int To_IsFairProfileToken(char *token) // token: 変更する。
{
	errorCase(!token);

	line2JToken(token, 1, 1);

	return strlen(token) < 100;
}
int IsFairHost(char *host) // リクエストヘッダの Host フィールドの値
{
	errorCase(!host);

	return
		lineExp("<1,100,-.09AZaz>", host) ||
		lineExp("<1,100,-.09AZaz>:<1,5,09>", host);
}
