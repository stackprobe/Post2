#include "all.h"

#define SAVE_FILE "C:\\temp\\data\\Post2\\recruit\\RecruitList.txt"
#define IDENT_PER_PAGE 8

void RL_Load(void)
{
	LoadFileLines(SAVE_FILE);
}
void RL_Save(void)
{
	SaveFileLines(SAVE_FILE);
}
int RL_Has(char *ident)
{
	errorCase(!IsFairUUID(ident));

	return LHasLine(ident);
}
void RL_Add(char *ident)
{
	errorCase(!IsFairUUID(ident));

	LAddLine(ident);
}
void RL_Remove(char *ident)
{
	errorCase(!IsFairUUID(ident));

	LRemoveLine(ident);
}

// ---- seeker 全ページ更新 ----

static char *RootDir;
static autoList_t *Idents;

static char *GetPageFile(uint pageIndex)
{
	if(pageIndex)
	{
		return xcout("%04u.html", pageIndex);
	}
	return strx("index.html");
}
static char *c_GetPageFile(uint pageIndex)
{
	static char *stock;
	memFree(stock);
	return stock = GetPageFile(pageIndex);
}
static void UpdatePage(uint pageIndex, uint startPos, uint endPos, int isFirstPage, int isLastPage)
{
	char *htmlFile = combine_cx(RootDir, GetPageFile(pageIndex));
	uint index;
	autoList_t *dest = newList();

	addElement(dest, (uint)xcout("<html>"));
	addElement(dest, (uint)xcout("<body>"));

	if(!isFirstPage)
		addElement(dest, (uint)xcout("<a href=\"%s\">前のページ</a><br/>", c_GetPageFile(pageIndex - 1)));

	if(!isLastPage)
		addElement(dest, (uint)xcout("<a href=\"%s\">次のページ</a><br/>", c_GetPageFile(pageIndex + 1)));

	for(index = startPos; index < endPos; index++)
	{
		char *ident = (char *)getElement(Idents, index);

		if(ident)
		{
			char *profileFile;
			char *profile;

			errorCase(!IsFairUUID(ident));
			profileFile = combine("C:\\temp\\data\\Post2\\recruit\\register", ident);
			errorCase(!existFile(profileFile));
			profile = LoadFileRec(profileFile);

			addElement(dest, (uint)xcout("<hr/>"));

			addElement(dest, (uint)xcout("お店の名前：%s<br/>", c_EnvGetValue(profile, "TenpoName")));
			addElement(dest, (uint)xcout("住所：%s<br/>",       c_EnvGetValue(profile, "TenpoBasho")));
			addElement(dest, (uint)xcout("連絡先：%s<br/>",     c_EnvGetValue(profile, "TenpoTel")));

			memFree(profileFile);
			memFree(profile);
		}
	}
	addElement(dest, (uint)xcout("<hr/>"));
	addElement(dest, (uint)xcout("</body>"));
	addElement(dest, (uint)xcout("</html>"));

	cout("HTML_FILE: %s\n", htmlFile);

	writeLines(htmlFile, dest);

	memFree(htmlFile);
	releaseDim(dest, 1);
}
void RL_UpdateAllPage(char *rootDir)
{
	uint index;

	LOGPOS();

	errorCase(!existDir(rootDir));

	RootDir = rootDir;
	Idents = LGetLines(); // 参照用！

	LOGPOS();

	while(!getCount(Idents) || getCount(Idents) % IDENT_PER_PAGE)
	{
		addElement(Idents, (uint)NULL);
	}
	LOGPOS();

	for(index = 0; index * IDENT_PER_PAGE < getCount(Idents); index++)
	{
		UpdatePage(
			index,
			index * IDENT_PER_PAGE,
			(index + 1) * IDENT_PER_PAGE,
			!index,
			(index + 1) * IDENT_PER_PAGE == getCount(Idents)
			);
	}
	LOGPOS();

	for(; ; index++)
	{
		char *file = combine_cx(RootDir, GetPageFile(index));

		if(!existFile(file))
		{
			memFree(file);
			break;
		}
		removeFile_x(file);
	}
	LOGPOS();

	removeZero(Idents); // 復元

	RootDir = NULL;
	Idents = NULL;

	LOGPOS();
}

// ----
