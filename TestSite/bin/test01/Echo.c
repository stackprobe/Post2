#include "C:\Factory\Common\all.h"

#define METHOD_FILE "Method.htt2dat"
#define REQ_PATH_FILE "Path.htt2dat"
#define HTTP_VERSION_FILE "HTTP_Version.htt2dat"
#define RAW_HEADER_FILE "RawHeader.htt2dat"
#define HEADER_KEYS_FILE "HeaderKeys.htt2dat"
#define HEADER_VALUES_FILE "HeaderValues.htt2dat"
#define BODY_SIZE_FILE "BodySize.htt2dat"
#define CHUNKED_FLAG "Chunked.htt2dat"
#define REQ_TYPE_FILE "Type.htt2dat"
#define BODY_FILE "Body.htt2dat"
#define RES_HEADER_FILE "ResHeader.htt2dat"
#define RES_BODY_FILE "ResBody.htt2dat"

static char *Method;
static char *ReqPath;
static char *HTTP_Version;
static char *RawHeader;
static autoList_t *HeaderKeys;
static autoList_t *HeaderValues;
static uint64 BodySize;
static int ChunkedFlag;
static char *ReqType;
static char *Body;

static void WriteRow(FILE *fp, char *name, char *value)
{
	writeLine(fp, "<TR>");
	writeLine(fp, "<TD>");
	writeLine(fp, name);
	writeLine(fp, "</TD>");
	writeLine(fp, "<TD>");
	writeLine(fp, "<PRE>");
	writeLine(fp, value);
	writeLine(fp, "</PRE>");
	writeLine(fp, "</TD>");
	writeLine(fp, "</TR>");
}
static void WriteRow_cx(FILE *fp, char *name, char *value)
{
	WriteRow(fp, name, value);
	memFree(value);
}
int main(int argc, char **argv)
{
	FILE *fp;
	uint index;

	Method = readText_b(METHOD_FILE);
	ReqPath = readText_b(REQ_PATH_FILE);
	HTTP_Version = readText_b(HTTP_VERSION_FILE);
	RawHeader = readText_b(RAW_HEADER_FILE);
	HeaderKeys = readLines(HEADER_KEYS_FILE);
	HeaderValues = readLines(HEADER_VALUES_FILE);
	BodySize = readFirstValue64(BODY_SIZE_FILE);
	ChunkedFlag = existFile(CHUNKED_FLAG);
	Body = readText_b(BODY_FILE);
	ReqType = readText_b(REQ_TYPE_FILE);

	errorCase(getCount(HeaderKeys) != getCount(HeaderValues)); // 同じはず。

	// ノーマライズが必要なのはこれだけ {

	toAsciiLine(RawHeader, 1, 1, 1);
	line2JLine(Body, 1, 1, 1, 1);

	// }

	writeOneLine(RES_HEADER_FILE, "Content-Type: text/html; charset=Shift_JIS");

	fp = fileOpen(RES_BODY_FILE, "wt");

	writeLine(fp, "<HTML>");
	writeLine(fp, "<BODY>");
	writeLine(fp, "<TABLE BORDER=\"1\">");

	WriteRow(fp, "Method", Method);
	WriteRow(fp, "ReqPath", ReqPath);
	WriteRow(fp, "HTTP_Version", HTTP_Version);
	WriteRow(fp, "RawHeader", RawHeader);

	writeLine(fp, "<TR>");
	writeLine(fp, "<TD>Header</TD>");
	writeLine(fp, "<TD>");
	writeLine(fp, "<TABLE BORDER=\"1\">");

	for(index = 0; index < getCount(HeaderKeys); index++)
	{
		WriteRow(fp, getLine(HeaderKeys, index), getLine(HeaderValues, index));
	}
	writeLine(fp, "</TABLE>");
	writeLine(fp, "</TD>");
	writeLine(fp, "</TR>");

	WriteRow_cx(fp, "BodySize", xcout("%I64u", BodySize));
	WriteRow_cx(fp, "ChunkedFlag", xcout("%d", ChunkedFlag));
	WriteRow(fp, "ReqType", ReqType);
	WriteRow(fp, "Body", Body);

	writeLine(fp, "</TABLE>");
	writeLine(fp, "<a href=\".\">戻る</a>");
	writeLine(fp, "</BODY>");
	writeLine(fp, "</HTML>");

	fileClose(fp);

	termination(0);
}
