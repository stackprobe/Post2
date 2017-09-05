/*
	char *ProcTemplateVar(char *var);

		var
			not NULL
			変更して良い。
			開放不可、再配置不可

		戻り値
			NULL 不可
			呼び出し側で開放する。
*/

#include "Template.h"

void ProcTemplate_T_x(char *text)
{
	autoBlock_t *buff = newBlock();
	char *p;
	char *q;
	char *ret;

	for(p = text; *p; )
	{
		if(!memcmp(p, "<%", 2))
		{
			p += 2;
			q = strstr(p, "%>");
			errorCase(!q);
			*q = '\0';

			ret = ProcTemplateVar(p);
			errorCase(!ret);
			ab_addLine(buff, ret);
			memFree(ret);

			p = q + 2;
		}
		else
		{
			addByte(buff, *p);
			p++;
		}
	}
	memFree(text);

	Res.Body = buff;
}
void ProcTemplate_T(char *text)
{
	ProcTemplate_T_x(strx(text));
}
void ProcTemplate_F(char *tFile)
{
	cout("TEMPLATE_FILE: %s\n", tFile);

	ProcTemplate_T_x(readText_b(tFile));
}
void ProcTemplate(void)
{
	char *dir = combine(getSelfDir(), "template");
	char *file;
	char *tFile;

	file = combine(dir, getLocal(getSelfFile()));
	tFile = changeExt(file, "html");

	ProcTemplate_F(tFile);

	memFree(dir);
	memFree(file);
	memFree(tFile);
}
