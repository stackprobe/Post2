#include "all.h"

char *GetHomeDir_From(char *from)
{
	static char *dir;

	if(!dir)
	{
		dir = getSelfDir();
		dir = strx(dir);
		dir = replaceLine(dir, from /* "\\bin\\" */, "\\home\\", 1);

		errorCase(replaceLine_getLastReplacedCount() != 1);
	}
	return dir;
}
char *GetHomeDir(void)
{
	return GetHomeDir_From("\\bin\\");
}
