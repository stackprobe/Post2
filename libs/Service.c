/*
	Req ... Body とフラグ・整数 () を除いて表示可能文字列である。

	Req 内の文字列   保証される正規化
	------------------------------------------
	IP               xcout("%u.%u.%u.%u", ...)
	Method           toAsciiLine(, 0, 0, 0)
	Path             toAsciiLine(, 0, 0, 0)
	HTTP_Version     toAsciiLine(, 0, 0, 0)
	HeaderKeys       ucTrimEdge(toAsciiLine(, 0, 1, 1))
	HeaderValues     ucTrimEdge(toAsciiLine(, 0, 1, 1)) ★フォルディングされた行は ' ' で連結する。
	Type             ucTrimEdge(toAsciiLine(, 0, 1, 1))
*/

#include "Service.h"

#define IP_FILE "IP.httdat"

static char *BootDir;

Req_t Req;
Res_t Res;

char *GetBootDir(void)
{
	return BootDir;
}
char *GetHeaderValue(char *key)
{
	errorCase(m_isEmpty(key));

	return refLine(Req.HeaderValues, findLineCase(Req.HeaderKeys, key, 1));
}
static void ErrorFnlz(void)
{
	if(errorOccurred)
	{
		changeCwd(BootDir);

		writeOneLine(RES_HEADER_FILE, "Content-Type: text/html; charset=Shift_JIS");
		writeOneLine_cx(RES_BODY_FILE, xcout(
			"<html><body style=\"color: #c00000; font-family: メイリオ; font-size: large;\">エラー<br/>%s (%u) %s<br/>%s<hr/>%s @ %s</body></html>"
			,errorPosSource
			,errorPosLineno
			,errorPosFunction
			,errorPosMessage
			,Req.Path
			,makeJStamp(NULL, 0) // gomi
			));

		termination(0);
	}
}
int main(int argc, char **argv)
{
	Req.IP = readFirstLine(IP_FILE);
	Req.Method = readText_b(METHOD_FILE);
	Req.Path = readText_b(REQ_PATH_FILE);
	Req.HTTP_Version = readText_b(HTTP_VERSION_FILE);
	Req.HeaderKeys = readLines(HEADER_KEYS_FILE);
	Req.HeaderValues = readLines(HEADER_VALUES_FILE);
	Req.BodySize = readFirstValue64(BODY_SIZE_FILE);
	Req.ChunkedFlag = existFile(CHUNKED_FLAG);
	Req.Type = readText_b(REQ_TYPE_FILE);
	Req.Body = readBinary(BODY_FILE);

	Res.HeaderLines = newList();

	BootDir = getCwd();

	addFinalizer(ErrorFnlz);

	Post2_Main();

	changeCwd(BootDir);

	if(Res.StatusCode)
		writeOneLine_cx(RES_HEADER_FIRST_LINE_FILE, xcout("HTTP/1.1 %u Powdered Green Tea", Res.StatusCode));

	if(!Res.ContentType)
		Res.ContentType = "text/html; charset=Shift_JIS";

	addElement(Res.HeaderLines, (uint)xcout("Content-Type: %s", Res.ContentType));

	if(!Res.Body)
		Res.Body = ab_fromLine_x(xcout(
			"<html><body style=\"font-family: メイリオ; font-size: large;\">リクエストを処理しました。<hr/>%s @ %s</body></html>"
			,Req.Path
			,makeJStamp(NULL, 0) // gomi
			));

	writeLines(RES_HEADER_FILE, Res.HeaderLines);
	writeBinary(RES_BODY_FILE, Res.Body);

	termination(0);
}
