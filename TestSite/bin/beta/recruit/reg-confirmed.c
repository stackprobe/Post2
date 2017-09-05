#include "libs\all4P2.h"

static char *Url;
static char *Ident;
static char *File;
static char *MailAddress;

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
	char *text;

	Url = GetBodyValue("url");

	{
		char *p = mbs_stristr(Url, "?ident=");

		errorCase(!p);

		Ident = strx(p + 7);
	}

	errorCase(!IsFairUUID(Ident));

	File = combine("C:\\temp\\data\\Post2\\recruit\\entry2", Ident);

	errorCase_m(!existFile(File), "エントリーは失効しました。");

	{
		char *text = LoadFileRec(File);

		removeFile(File); // エントリー削除

		Ident = MakeUUID(1); // g
		File = combine("C:\\temp\\data\\Post2\\recruit\\register", Ident); // g

		// text 修正_参照
		{
			text = EnvSetValue(text, "Time", "");
			MailAddress = EnvGetValue(text, "MailAddress");

			errorCase(!IsFairMailAddress(MailAddress));
		}

		SaveFileRec_cx(File, text);
	}

	// mailaddress 修正
	{
		char *mafile = combine_cx("C:\\temp\\data\\Post2\\recruit\\mailaddress", xcout("[%s].txt", MailAddress));

		errorCase(!existFile(mafile)); // 2bs: reg-send.c にて登録されているはず！

		SaveFileRec_xx(mafile, xcout("File=%s", File));
	}

	ProcTemplate();
}
