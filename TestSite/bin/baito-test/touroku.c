#include "C:\Factory\Common\all.h"
#include "C:\Factory\Common\Options\CRandom.h"

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

// ----

static autoList_t *FormKeys;
static autoList_t *FormValues;

static char *GetFormValue(char *key)
{
	return refLine(FormValues, findLineCase(FormKeys, key, 1));
}
static char *x_GetFormValue(char *key)
{
	return strx(GetFormValue(key));
}
static int FormValueIs(char *key, char *value)
{
	return !_stricmp(GetFormValue(key), value);
}
static char *GetHomeDir(void)
{
	static char *dir;

	if(!dir)
	{
		dir = getSelfDir();
		dir = strx(dir);
		dir = replaceLine(dir, "\\bin\\", "\\home\\", 1);

		errorCase(replaceLine_getLastReplacedCount() != 1);
	}
	return dir;
}
static void ErrorFnlz(void)
{
	if(errorOccurred)
	{
		writeOneLine(RES_HEADER_FILE, "Content-Type: text/plain; charset=Shift_JIS");
		writeOneLine_cx(RES_BODY_FILE, xcout("Error: %s (%u) %s", errorPosSource, errorPosLineno, errorPosFunction));
		termination(0);
	}
}

// ----

#define KYUUJIN_DATA_DIR "C:\\temp\\data\\Post2_Test\\baito-test"
#define KYUUJIN_MAX 8
#define REC_LINENUM 10 // 1�̃��R�[�h�̍s��

// ----

static void SetMaxLen(char *str, uint lenmax)
{
	if(lenmax < strlen(str))
		str[lenmax] = '\0';
}
static char *UrlDecode(char *url)
{
	autoBlock_t *buff = newBlock();
	char *p;

	for(p = url; *p; p++)
	{
		if(*p == '%' && m_ishexadecimal(p[1]) && m_ishexadecimal(p[2]))
		{
			addByte(buff, m_c2i(p[1]) * 16 + m_c2i(p[2]));
			p += 2;
		}
		else
			addByte(buff, *p);
	}
	url = unbindBlock2Line(buff);
	line2JLine(url, 1, 0, 0, 1);
	ucTrim(url);
	return url;
}

static char *Temp_Index;
static char *Temp_KyuujinItem;
static char *Temp_TableRow;

static autoList_t *RLines;
static uint RIndex;

static char *GetKyuujin(void)
{
	char *buff = strx("");
	uint index;

	if(getCount(RLines) <= RIndex)
		return "<br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/>"; // �󗓁A���ƍ��������킹��B

	for(index = 0; index < REC_LINENUM; index++)
	{
		char *line = refLine(RLines, RIndex++);

		switch(index)
		{
		case 0: line = xcout("<u>%s</u>", line); break;
		case 1: line = xcout("���@�ꏊ�@�@�F%s", line); break;
		case 2: line = xcout("���@��W���e�F%s", line); break;
		case 3: line = xcout("���@���������F<a href=\"mailto:%s\">%s</a>", line, line); break;
		case 4: line = xcout("���@���������F%s", line); break;
		case 5: line = xcout("���@�d�b�ԍ��F%s", line); break;

		default:
			line = NULL;
			break;
		}
		if(line)
		{
			char *itmBuff = strx(Temp_KyuujinItem);

			itmBuff = replaceLine(itmBuff, "<KYUUJIN_TEXT/>", line, 1);

			buff = addLine(buff, itmBuff);
		}
	}
	return buff;
}
int main(int argc, char **argv)
{
	FILE *fp;

	Method = readText_b(METHOD_FILE);
	ReqPath = readText_b(REQ_PATH_FILE);
	HTTP_Version = readText_b(HTTP_VERSION_FILE);
	RawHeader = readText_b(RAW_HEADER_FILE);
	HeaderKeys = readLines(HEADER_KEYS_FILE);
	HeaderValues = readLines(HEADER_VALUES_FILE);
	BodySize = readFirstValue64(BODY_SIZE_FILE);
	ChunkedFlag = existFile(CHUNKED_FLAG);
	ReqType = readText_b(REQ_TYPE_FILE);
	Body = readText_b(BODY_FILE);

	// nmlz {

	toAsciiLine(RawHeader, 1, 1, 1);
	line2JLine(Body, 1, 1, 1, 1);

	// }

	addFinalizer(ErrorFnlz);

	// ----

	FormKeys = newList();
	FormValues = newList();

	{
		autoList_t *parts = tokenize(Body, '&');
		char *part;
		uint index;

		foreach(parts, part, index)
		{
			autoList_t *tokens = tokenize(part, '=');
			char *key;
			char *value;

			key   = getLine(tokens, 0);
			value = getLine(tokens, 1);

			key   = UrlDecode(key);
			value = UrlDecode(value);

			addElement(FormKeys,   (uint)key);
			addElement(FormValues, (uint)value);

			releaseDim(tokens, 1);
		}
		releaseDim(parts, 1);
	}

	// ----

	{
		uint page = toValue(GetFormValue("page"));

		if(page != 0)
		{
			char *lFile = xcout("touroku-%04u.html", page);
			char *file;

			file = combine(getSelfDir(), lFile);
			writeOneLine(RES_HEADER_FILE, "Content-Type: text/html; charset=Shift_JIS");
			copyFile(file, RES_BODY_FILE);

			memFree(lFile);
			memFree(file);

			termination(0);
		}
	}

	if(FormValueIs("command", "touroku"))
	{
		char *namae  = x_GetFormValue("namae");
		char *basho  = x_GetFormValue("basho");
		char *naiyou = x_GetFormValue("naiyou");
		char *mail   = x_GetFormValue("mail");
		char *line   = x_GetFormValue("line");
		char *tel    = x_GetFormValue("tel");
		char *shimei = x_GetFormValue("shimei");
		char *juusho = x_GetFormValue("juusho");
		char *juushoTel = x_GetFormValue("juusho-tel");
		char *password  = x_GetFormValue("password");
		autoList_t *lines;

		{
			const uint LEN_MAX = 100;

			SetMaxLen(namae,  LEN_MAX);
			SetMaxLen(basho,  LEN_MAX);
			SetMaxLen(naiyou, LEN_MAX);
			SetMaxLen(mail,   LEN_MAX);
			SetMaxLen(line,   LEN_MAX);
			SetMaxLen(tel,    LEN_MAX);
			SetMaxLen(shimei, LEN_MAX);
			SetMaxLen(juusho, LEN_MAX);
			SetMaxLen(juushoTel, LEN_MAX);
			SetMaxLen(password,  LEN_MAX);
		}

		line2JLine(namae,  1, 0, 0, 0);
		line2JLine(basho,  1, 0, 0, 0);
		line2JLine(naiyou, 1, 0, 0, 0);
		line2JLine(mail,   1, 0, 0, 0);
		line2JLine(line,   1, 0, 0, 0);
		line2JLine(tel,    1, 0, 0, 0);
		line2JLine(shimei, 1, 0, 0, 0);
		line2JLine(juusho, 1, 0, 0, 0);
		line2JLine(juushoTel, 1, 0, 0, 0);
		toAsciiLine(password, 0, 0, 0);

		if(!*namae)  namae  = xcout("�X�����ݒ�%05u", getCryptoRand16());
		if(!*basho)  basho  = xcout("�ꏊ���ݒ�%05u", getCryptoRand16());
		if(!*naiyou) naiyou = xcout("���e���ݒ�%05u", getCryptoRand16());
		if(!*mail)   mail   = xcout("example@%05u.example.com", getCryptoRand16());
		if(!*line)   line   = xcout("���C�����ݒ�%05u", getCryptoRand16());
		if(!*tel)    tel    = xcout("�d�b�ԍ����ݒ�%05u", getCryptoRand16());
		if(!*shimei) shimei = "�������ݒ�";
		if(!*juusho) juusho = "�Z�����ݒ�";
		if(!*juushoTel) juushoTel = "�A���斢�ݒ�";
		if(!*password)  password  = "1111";

		{
			char *dataDir = KYUUJIN_DATA_DIR;
			char *dataFile;

			dataFile  = combine(dataDir, "touroku.dat");
			createPath(dataFile, 'f');
			lines = readLines(dataFile);

			if(getCount(lines) == 1) // ? ��
				lines = newList();

			addElement(lines, (uint)namae);
			addElement(lines, (uint)basho);
			addElement(lines, (uint)naiyou);
			addElement(lines, (uint)mail);
			addElement(lines, (uint)line);
			addElement(lines, (uint)tel);
			addElement(lines, (uint)shimei);
			addElement(lines, (uint)juusho);
			addElement(lines, (uint)juushoTel);
			addElement(lines, (uint)password);

			while(KYUUJIN_MAX * REC_LINENUM < getCount(lines))
				desertElement(lines, 0);

			writeLines(dataFile, lines);
		}

		{
			char *wDir = combine(GetHomeDir(), "active-page");
			char *wFile;
			char *buff = strx("");

			wFile = combine(wDir, "index.html");

			Temp_Index       = readText(combine(getSelfDir(), "template_index.html"));
			Temp_KyuujinItem = readText(combine(getSelfDir(), "template_kyuujin_item.html"));
			Temp_TableRow    = readText(combine(getSelfDir(), "template_table_row.html"));

			RLines = lines;
			RIndex = 0;

			while(RIndex < getCount(RLines))
			{
				char *tblRow = strx(Temp_TableRow);
				char *kyuujin1;
				char *kyuujin2;

				kyuujin1 = GetKyuujin();
				kyuujin2 = GetKyuujin();

				tblRow = replaceLine(tblRow, "<KYUUJIN_1/>", kyuujin1, 1);
				tblRow = replaceLine(tblRow, "<KYUUJIN_2/>", kyuujin2, 2);

				buff = addLine(buff, tblRow);
			}

			{
				char *idxBuff = strx(Temp_Index);

				idxBuff = replaceLine(idxBuff, "<TABLE_ROWS/>", buff, 1);

				writeOneLine(wFile, idxBuff);
			}
		}
	}

	writeOneLine(RES_HEADER_FILE, "Content-Type: text/html; charset=Shift_JIS");
	writeOneLine(RES_BODY_FILE,
		"<html>"
		"<head>"
		"<meta http-equiv=\"refresh\" content=\"0;URL=active-page\">"
		"</head>"
		"</html>"
		);

	termination(0);
}
