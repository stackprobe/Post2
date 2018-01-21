#include "C:\Factory\Common\all.h"

static autoList_t *ParseRHtml(char *rHtml)
{
	autoList_t *images = newList();
	char *p = rHtml;
	char *q;

	LOGPOS();

	for(; ; )
	{
		p = strstr(p, "thumbnail_src");

		if(!p)
			break;

		p = strstr(p, "https://");
//		p = strstr(p, "https://scontent.cdninstagram.com/"); // del @ 2017.9.5 -- ホスト名・ドメインは変わるっぽい。

		if(!p)
			break;

		q = strchr(p, '"');

		if(!q)
			break;

		*q = '\0';
		p = strx(p);
//		p = replaceLine(p, "640x640", "160x160", 0); // del @ 2018.1.21

		addElement(images, (uint)p);

		p = q + 1;
	}
	LOGPOS();
	return images;
}
static char *MkWHtml(autoList_t *images)
{
	char *template_index     = readText("template\\index.html");
	char *template_tableRow  = readText("template\\tableRow.html");
	char *template_tableCell = readText("template\\tableCell.html");
	autoBlock_t *buff = newBlock();
	uint rowidx;
	uint colidx;
	uint imageIndex = 0;

	LOGPOS();

	for(rowidx = 0; rowidx < 4; rowidx++)
	{
		autoBlock_t *rowBuff = newBlock();

		for(colidx = 0; colidx < 3; colidx++)
		{
			char *tmp = strx(template_tableCell);
			char *imgsrc = refLine(images, imageIndex);

			if(!*imgsrc)
				imgsrc = "not-found.png";

			imageIndex++;

			tmp = replaceLine(tmp, "__@__", "__\a__", 0);
			tmp = replaceLine(tmp, "__\a__HREF__", "https://instagram.com/barnatsutobi/", 0);
			tmp = replaceLine(tmp, "__\a__IMG-SRC__", imgsrc, 0);

			ab_addLine_x(rowBuff, tmp);
		}

		{
			char *tmp = strx(template_tableRow);

			tmp = replaceLine_cx(tmp, "__@__TABLE-CELLS__", unbindBlock2Line(rowBuff), 0);

			ab_addLine_x(buff, tmp);
		}
	}

	{
		char *tmp = strx(template_index);

		tmp = replaceLine_cx(tmp, "__@__TABLE-ROWS__", unbindBlock2Line(buff), 0);

		LOGPOS();
		return tmp;
	}
}
static void MkIndexHtml(char *rFile, char *wFile)
{
	char *rHtml;
	autoList_t *images;
	char *wHtml;

	cout("rFile: %s\n", rFile);
	cout("wFile: %s\n", wFile);

	LOGPOS();

	if(existFile(rFile))
	{
		LOGPOS();

		rHtml = readText(rFile);
		images = ParseRHtml(rHtml);

		if(getCount(images))
		{
			LOGPOS();

			wHtml = MkWHtml(images);
			writeOneLineNoRet(wFile, wHtml);
			memFree(wHtml);

			LOGPOS();
		}
		memFree(rHtml);
		releaseDim(images, 1);

		LOGPOS();
	}
	LOGPOS();
}
int main(int argc, char **argv)
{
	char *rFile;
	char *wFile;

	if(argIs("/E")) // Empty mode
	{
		char *wFile = nextArg();

		cout("wFile: %s\n", wFile);

		writeOneLineNoRet(wFile, MkWHtml(newList())); // g
		return;
	}
	rFile = nextArg();
	wFile = nextArg();

	MkIndexHtml(rFile, wFile);
}
