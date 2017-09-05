#include "all.h"

// ---- load file lines ----

static autoList_t *Lines;

void LoadFileLines(char *file)
{
	errorCase(m_isEmpty(file));

	if(Lines) // ? already loaded
		releaseDim(Lines, 1);

	createFileIfNotExist(file);

	Lines = readResourceLines(file);
}
void SaveFileLines(char *file)
{
	errorCase(!Lines); // ? unloaded
	errorCase(m_isEmpty(file));

	writeLines(file, Lines);
}
int LHasLine(char *target)
{
	char *line;
	uint index;

	errorCase(!Lines); // ? unloaded
	errorCase(m_isEmpty(target));

	foreach(Lines, line, index)
		if(!strcmp(line, target))
			return 1;

	return 0;
}
void LAddLine_x(char *line)
{
	errorCase(!Lines); // ? unloaded
	errorCase(m_isEmpty(line));

	addElement(Lines, (uint)line);
}
void LAddLine(char *line)
{
	errorCase(m_isEmpty(line));

	LAddLine_x(strx(line));
}
void LRemoveLine(char *target)
{
	char *line;
	uint index;

	errorCase(!Lines); // ? unloaded
	errorCase(m_isEmpty(target));

	foreach(Lines, line, index)
		if(!strcmp(line, target))
			line[0] = '\0';

	trimLines(Lines);
}
void LRemoveLine_x(char *target)
{
	LRemoveLine(target);
	memFree(target);
}
autoList_t *LGetLines(void) // ret: direct!
{
	errorCase(!Lines); // ? unloaded
	return Lines;
}

// ----
