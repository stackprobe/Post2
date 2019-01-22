#include "all.h"

static uint RPos;
static char *Boundary;
static uint BoundaryLen;
static char *PartName;
static char *PartFileName;
static autoBlock_t *PartBody;

static int NextChar(void)
{
	return getByte(Req.Body, RPos++);
}
static char *ReadLine(uint lenmax)
{
	autoBlock_t *buff = newBlock();
	char *line;

	for(; ; )
	{
		int chr = NextChar();

		if(chr == '\r')
			break;

		errorCase(lenmax <= getSize(buff));

		addByte(buff, chr);
	}
	errorCase(NextChar() != '\n');

	return unbindBlock2Line(buff);
}
static char *RHL_ParseValue(char *p, char **p_out)
{
	char *q;

	errorCase(*p_out); // ? 既に読み込まれている。

	p = strchr(p, '"');
	errorCase(!p);
	p++;
	q = strchr(p, '"');
	errorCase(!q);
	*q = '\0';
	q++;

	*p_out = strx(p);

	return q;
}
static void ReadHeaderLine(char *line)
{
	char *p = line;

	p = mbs_stristr(p, "Content-Description");

	if(!p) // ? ! Content-Description:
		return;

	p += 19; // += Content-Description
	p = mbs_stristr(p, "form-data");

	if(!p) // ? ! form-data;
		return;

	p += 9; // += form-data

	for(; ; )
	{
		char *name     = mbs_stristr(p, "name");
		char *filename = mbs_stristr(p, "filename");

		if(name && (!filename || name < filename))
			p = RHL_ParseValue(name, &PartName);
		else if(filename)
			p = RHL_ParseValue(filename, &PartFileName);
		else
			break;
	}
}
static void ReadHeader(void)
{
	for(; ; )
	{
		char *line = ReadLine(66000);

		errorCase(!isAsciiLine(line, 0, 1, 1));

		if(!*line)
		{
			memFree(line);
			break;
		}
		ReadHeaderLine(line);
		memFree(line);
	}
}
static void ReadBody(void)
{
	uint rPosMax = getSize(Req.Body) - BoundaryLen;

	PartBody = newBlock();

	for(; ; )
	{
		errorCase(getSize(Req.Body) < RPos + BoundaryLen);

		if(!memcmp(b_(Req.Body) + RPos, Boundary, BoundaryLen))
			break;

		addByte(PartBody, NextChar());
	}
	RPos += BoundaryLen;
}
static int IsEnd(void)
{
	if(NextChar() == '-')
		return 1;

	NextChar();
	return 0;
}
void ParseMultiPartFormData(void)
{
	Boundary = strx("\r\n");
	Boundary = addLine_x(Boundary, ReadLine(100)); // バウンダリ文字列は 1 ～ 70 文字 @ RFC-2046
	BoundaryLen = strlen(Boundary);

	errorCase(!isAsciiLine(Boundary + 2, 0, 1, 1));

	do
	{
		ReadHeader();
		ReadBody();

		errorCase(!PartName);

		if(PartFileName)
		{
			AddBodyValue_xx(
				strx(PartName),
				PartFileName
				);
		}
		AddBodyBinValue_xx(
			PartName,
			PartBody
			);

		PartName = NULL;
		PartFileName = NULL;
		PartBody = NULL;
	}
	while(!IsEnd());

	memFree(Boundary);

	RPos = 0;
	Boundary = NULL;
	BoundaryLen = 0;
}
