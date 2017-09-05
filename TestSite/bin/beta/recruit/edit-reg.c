#include "libs\all4P2.h"

static char *Ident;
static char *ProfileFile;
static char *Profile;

static void Update(char *rName, char *wName)
{
	char *value = GetBodyValue(rName);

	value = strx(value);
	errorCase_m(!To_IsFairProfileToken(value), "入力フィールドに問題があります。");

	Profile = EnvSetValue(Profile, wName, value);

	memFree(value);
}
char *ProcTemplateVar(char *var)
{
	ucTrim(var);

	if(!_stricmp(var, "Ident"))
	{
		return strx(Ident);
	}
	error();
	return NULL; // dummy
}
void Post2_Main(void)
{
	Ident = GetBodyValue("ident");
	errorCase(!IsFairUUID(Ident));
	ProfileFile = combine("C:\\temp\\data\\Post2\\recruit\\register", Ident);
	errorCase_m(!existFile(ProfileFile), "エントリーは失効しました。");
	Profile = LoadFileRec(ProfileFile);

	Update("master-name", "MasterName");
	Update("master-kana", "MasterKana");
	Update("tenpo-name",  "TenpoName");
	Update("tenpo-basho", "TenpoBasho");
	Update("tenpo-tel",   "TenpoTel");
	Update("status",      "Status");

	SaveFileRec(ProfileFile, Profile);

	// seeker 全ページ更新
	{
		char *status = EnvGetValue(Profile, "Status");
		char *rootDir = combine(GetHomeDir(), "seeker");

		RL_Load();

		if(!_stricmp(status, STATUS_KOUKAI))
		{
			if(!RL_Has(Ident))
			{
				RL_Add(Ident);
				RL_Save();
				RL_UpdateAllPage(rootDir);
			}
		}
		else
		{
			RL_Remove(Ident);
			RL_Save();
			RL_UpdateAllPage(rootDir);
		}
	}

	ProcTemplate();
}
