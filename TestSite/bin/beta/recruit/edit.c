#include "libs\all4P2.h"

static char *Ident;
static char *ProfileFile;
static char *Profile;

char *ProcTemplateVar(char *var)
{
	ucTrim(var);

	if(!_stricmp(var, "Ident"))
	{
		return strx(Ident);
	}
	if(!_stricmp(var, "Selected_Koukai"))
	{
		return !strcmp(c_EnvGetValue(Profile, "Status"), STATUS_KOUKAI) ? "selected" : "";
	}
	if(!_stricmp(var, "Selected_Hikoukai"))
	{
		return !strcmp(c_EnvGetValue(Profile, "Status"), STATUS_KOUKAI) ? "" : "selected";
	}
	return EnvGetValue(Profile, var);
}
void Post2_Main(void)
{
	Ident = GetBodyValue("ident");
	errorCase(!IsFairUUID(Ident));
	ProfileFile = combine("C:\\temp\\data\\Post2\\recruit\\register", Ident);
	errorCase_m(!existFile(ProfileFile), "エントリーは失効しています。");
	Profile = LoadFileRec(ProfileFile);

	ProcTemplate();
}
