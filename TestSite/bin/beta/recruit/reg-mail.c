#include "libs\all4P2.h"

// ---- conf ----

static uint RegisterMax;

// ----

static char *Ident;

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
	char *credentials = GetBodyValue("credentials");

	errorCase(_stricmp(credentials, "{7f5ac763-774f-44a9-b546-2589eacdfe67}"));

	// conf
	{
		autoList_t *lines = readResourceLines_x(changeExt(getSelfFile(), "conf"));
		uint c = 0;

		RegisterMax = toValue(refLine(lines, c++));

		releaseDim(lines, 1);
	}
	// “¯‚¶IP‚©‚ç‘å—Ê‚Ì“o˜^‚ğŒŸoE‘j~
	{
		char *mafile = combine_cx("C:\\temp\\data\\Post2\\recruit\\ip", xcout("[%s].txt", Req.IP));

		errorCase_m(existFile(mafile), "‚²g—p‚ÌIPƒAƒhƒŒƒX‚Í‹K§‚³‚ê‚Ä‚¢‚Ü‚·B");
		errorCase_m(100 <= lsCount("C:\\temp\\data\\Post2\\recruit\\ip"), "V‚µ‚¢“o˜^‚Í§ŒÀ‚³‚ê‚Ä‚¢‚Ü‚·B(IP)");

		createPath(mafile, 'F'); // ‚±‚ÌIP‚ğ“o˜^‚·‚éB

		memFree(mafile);
	}
	// “o˜^”ãŒÀƒ`ƒFƒbƒN
	{
		uint count =
			lsCount("C:\\temp\\data\\Post2\\recruit\\register") +
			lsCount("C:\\temp\\data\\Post2\\recruit\\entry1") +
			lsCount("C:\\temp\\data\\Post2\\recruit\\entry2");
			lsCount("C:\\temp\\data\\Post2\\recruit\\entry3");

		errorCase_m(RegisterMax <= count, "V‚µ‚¢“o˜^‚Í§ŒÀ‚³‚ê‚Ä‚¢‚Ü‚·B");

		count = lsCount("C:\\temp\\data\\Post2\\recruit\\send");

		errorCase_m(100 <= count, "V‚µ‚¢“o˜^‚Í§ŒÀ‚³‚ê‚Ä‚¢‚Ü‚·B(SEND)");
	}

	Ident = MakeUUID(1);

	SaveFileRec_xx(combine("C:\\temp\\data\\Post2\\recruit\\entry1", Ident), xcout("Time=%I64d", time(NULL)));

	ProcTemplate();
}
