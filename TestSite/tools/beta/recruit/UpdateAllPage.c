#include "C:\Dev\Main\Post2\libs\Common\all.h"
#include "C:\Dev\Main\Post2\TestSite\bin\beta\recruit\libs\all.h"

int main(int argc, char **argv)
{
	char *dir = combine(GetHomeDir_From("\\tools\\"), "seeker");

	LOGPOS();

	RL_Load();
	RL_UpdateAllPage(dir);

	LOGPOS();

	memFree(dir);
}
