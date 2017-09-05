#include "C:\Dev\Main\Post2\libs\Service.h"
#include "C:\Dev\Main\Post2\libs\Template.h"

char *ProcTemplateVar(char *var)
{
	ucTrim(var);

	if(!_stricmp(var, "oshirase-title"))
	{
		return LoadFileRec("C:\\appdata\\barnatsutobi\\oshirase\\title");
	}
	if(!_stricmp(var, "oshirase-body"))
	{
		char *text = LoadFileRec("C:\\appdata\\barnatsutobi\\oshirase\\body");

		text = replaceLine(text, "\n", "<br/>", 0);

		return text;
	}
	if(!_stricmp(var, "oshirase-stamp"))
	{
		return LoadFileRec("C:\\appdata\\barnatsutobi\\oshirase\\stamp");
	}
	error();
	return NULL; // dummy
}
void Post2_Main(void)
{
	ProcTemplate();
}
