#include "C:\Dev\Main\Post2\libs\AuthorizationService.h"
#include "C:\Dev\Main\Post2\libs\Template.h"

char *ProcTemplateVar(char *var)
{
	ucTrim(var);

	if(!_stricmp(var, "oshirase-title"))
	{
		return LoadFileRec_Def("C:\\appdata\\barnatsutobi\\oshirase\\title", "���m�点");
	}
	if(!_stricmp(var, "oshirase-body"))
	{
		return LoadFileRec_Def("C:\\appdata\\barnatsutobi\\oshirase\\body", "�����ɂ��m�点�{�����L�q���ĉ������B\n�Q�s��\n�R�s��");
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
