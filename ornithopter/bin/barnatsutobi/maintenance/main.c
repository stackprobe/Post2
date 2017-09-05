#include "C:\Dev\Main\Post2\libs\AuthorizationService.h"
#include "C:\Dev\Main\Post2\libs\Template.h"

char *ProcTemplateVar(char *var)
{
	ucTrim(var);

	if(!_stricmp(var, "oshirase-title"))
	{
		return LoadFileRec_Def("C:\\appdata\\barnatsutobi\\oshirase\\title", "お知らせ");
	}
	if(!_stricmp(var, "oshirase-body"))
	{
		return LoadFileRec_Def("C:\\appdata\\barnatsutobi\\oshirase\\body", "ここにお知らせ本文を記述して下さい。\n２行目\n３行目");
	}
	if(!_stricmp(var, "oshirase-stamp"))
	{
		stampData_t *sd = getStampDataTime(time(NULL));

		return xcout(
			"%04u/%02u/%02u %02u:%02u"
			,sd->year
			,sd->month
			,sd->day
			,sd->hour
			,sd->minute
			);
	}
	error();
	return NULL; // dummy
}
void Post2_Auth_Main(char *userPw)
{
	ProcTemplate();
}
