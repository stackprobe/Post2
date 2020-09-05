/*
	ToOneHtml.exe ���̓t�@�C�� �o�̓t�@�C��
*/

#include "C:\Factory\Common\all.h"

static char *GetTypeByExt(char *ext)
{
	if(!_stricmp(ext, "js"))
		return "text/javascript";

	if(!_stricmp(ext, "bmp"))
		return "image/bmp";

	if(!_stricmp(ext, "gif"))
		return "image/gif";

	if(!_stricmp(ext, "jpg"))
		return "image/jpeg";

	if(!_stricmp(ext, "jpeg"))
		return "image/jpeg";

	if(!_stricmp(ext, "png"))
		return "image/png";

	error(); // �s���Ȋg���q
	return NULL; // dummy
}
static char *GetOneHtml(char *file, uint depth)
{
	char *text = readText(file);
	char *p;
	char *srcFile;
	char *srcExt;
	autoBlock_t *eText;
	autoBlock_t *dest = newBlock();

	errorCase(30 < depth);

	addCwd_x(getParent_x(makeFullPath(file)));
	{
		for(p = text; updateTagRng(p, "src=\"", "\"", 0); )
		{
			ab_addBlock(dest, p, (uint)lastTagRng.innerBgn - (uint)p);
			p = lastTagRng.innerEnd;
LOGPOS();

			srcFile = strxl(lastTagRng.innerBgn, (uint)lastTagRng.innerEnd - (uint)lastTagRng.innerBgn);
			restoreYen(srcFile);
			errorCase(!existFile(srcFile));
			srcExt = getExt(srcFile);

			if(!_stricmp(srcExt, "html"))
			{
				char *srcText;
				autoBlock_t gab;
LOGPOS();

				srcText = GetOneHtml(srcFile, depth + 1);
				eText = encodeBase64(gndBlockLineVar(srcText, gab));

				ab_addLine(dest, "data:text/html;base64,");
				ab_addBytes(dest, eText);

				memFree(srcText);
				releaseAutoBlock(eText);
LOGPOS();
			}
			else
			{
				autoBlock_t *srcData = readBinary(srcFile);
LOGPOS();

				eText = encodeBase64(srcData);

				ab_addLine(dest, "data:");
				ab_addLine(dest, GetTypeByExt(srcExt));
				ab_addLine(dest, ";base64,");
				ab_addBytes(dest, eText);

				releaseAutoBlock(srcData);
				releaseAutoBlock(eText);
LOGPOS();
			}
LOGPOS();
			memFree(srcFile);
LOGPOS();
		}
LOGPOS();
		ab_addLine(dest, p);
LOGPOS();
	}
	unaddCwd();

	memFree(text);
	return unbindBlock2Line(dest);
}
static void ToOneHtml(char *rFile, char *wFile)
{
	writeOneLineNoRet_cx(wFile, GetOneHtml(rFile, 1));

	// Extra
	{
		char *text = readText(wFile);

		text = replaceLine(text, "<MB-SPC>", "�@", 0);

		writeOneLineNoRet(wFile, text);
		memFree(text);
	}
}
int main(int argc, char **argv)
{
	ToOneHtml(getArg(0), getArg(1));
}
