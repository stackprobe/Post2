#include "all.h"

char *LoadFileRec_Def(char *file, char *defval)
{
	char *ret;

//	errorCase(m_isEmpty(file));
	errorCase(!startsWithICase(file, "C:\\appdata\\"));
	errorCase(!defval);

	if(existFile(file))
		ret = readText_b(file);
	else
		ret = strx(defval);

	return ret;
}
char *LoadFileRec(char *file)
{
	return LoadFileRec_Def(file, "");
}
void SaveFileRec(char *file, char *text)
{
//	errorCase(m_isEmpty(file));
	errorCase(!startsWithICase(file, "C:\\appdata\\"));
	errorCase(!text);

	if(*text)
	{
		createPath(file, 'X');
		writeOneLineNoRet_b(file, text);
	}
	else
		removeFileIfExist(file);
}

// c_
char *c_LoadFileRec(char *file)
{
	static char *stock;
	memFree(stock);
	return stock = LoadFileRec(file);
}

// _x
void SaveFileRec_cx(char *file, char *text)
{
	SaveFileRec(file, text);
	memFree(text);
}
void SaveFileRec_xc(char *file, char *text)
{
	SaveFileRec(file, text);
	memFree(file);
}
void SaveFileRec_xx(char *file, char *text)
{
	SaveFileRec(file, text);
	memFree(file);
	memFree(text);
}
