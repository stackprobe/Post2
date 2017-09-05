/*
	Service.dat �Ɉȉ���ǉ�

		POST <���̎��s�t�@�C���̏ꏊ>\Post2.exe ROOT-DIR RECV-BODY-SIZE-MAX

			ROOT-DIR ...

				�T�[�r�X�v���O������z�u�����f�B���N�g��
				���΃p�X�s��

			RECV-BODY-SIZE-MAX ...

				���N�G�X�g�{�f�B�̍ő�T�C�Y [�o�C�g]
				�l�� == 1 �` IMAX_64

		�ǉ���

			POST "<BASE>Post2.exe" "<BASE>Service" 2000000

	- - - -

	���s�����T�[�r�X�v���O�����̃p�X

		ROOT-DIR \ REQ-FILE

			REQ-FILE ... ���N�G�X�gURL�̃p�X������ lineToFairLocalPath() ���� changeExt(, "exe") ����������

		�����̃t�@�C�������݂��Ȃ���΁A�X�� changeExt(, "bat") ����������

		�����̃t�@�C�������݂��Ȃ���΁A�����ɐؒf����B

	���s���̃J�����g�ɑ��݂���t�@�C��

		IP.httdat    ... IP�A�h���X
		Recv.httdat  ... ��̃t�@�C��
		Send.httdat  ... ��̃t�@�C��
		Tight.httdat ... ��̃t�@�C�� (���݂��Ȃ��ꍇ������)

		Method.htt2dat       ... ���\�b�h   ex. POST
		Path.htt2dat         ... �p�X       ex. /cgi-bin/index.cgi
		HTTP_Version.htt2dat ... �o�[�W���� ex. HTTP/1.1
		RawHeader.htt2dat    ... �w�b�_�S��, �擪�s���܂�
		HeaderKeys.htt2dat   ... �w�b�_�̃L�[�̃��X�g
		HeaderValues.htt2dat ... �w�b�_�̒l�̃��X�g
		BodySize.htt2dat     ... �{�f�B�T�C�Y, readFirstValue64 �œǂݍ��ނ��ƁB
		Chunked.htt2dat      ... Chunked �̏ꍇ���݂���B
		Body.htt2dat         ... �{�f�B�S��
		Type.htt2dat         ... Content-Type �̒l, ������� 0 �o�C�g�̃t�@�C��
		ResHeader.htt2dat    ... �����w�b�_, 0 �o�C�g�̃t�@�C��
		ResBody.htt2dat      ... �����{�f�B, 0 �o�C�g�̃t�@�C��

	����ׂ�����

		ResHeader.htt2dat
		ResBody.htt2dat

		... ���X�V����B

		�� ResHeader.htt2dat �� Content-Type ��ǉ����邱�ƁB
		�� ResHeader.htt2dat �̍Ō�̋�s�͕s�v�B
		�� ResBody.htt2dat �̃T�C�Y�� Content-Length �ɂ���B
*/

#include "C:\Factory\Common\all.h"
#include "C:\Factory\Common\Options\FileTools.h"

#define IP_FILE "IP.httdat"
#define RECV_FILE "Recv.httdat"
#define SEND_FILE "Send.httdat"
#define TIGHT_FILE "Tight.httdat"

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
#define RES_HEADER_FIRST_LINE_FILE "ResHeaderFirstLine.htt2dat"
#define RES_HEADER_FILE "ResHeader.htt2dat"
#define RES_BODY_FILE "ResBody.htt2dat"

// ---- args ----

static char *RootDir;
static uint64 RecvBodySizeMax;

// ----

static time_t SockTimeoutSec;
static char *SendFileFullPath;

static void DoDisconnect(void)
{
	LOGPOS();
	removeFileAtTermination(SendFileFullPath);
}

// ---- tools ----

static char *NextCRLF(char *p)
{
	for(; ; p++)
	{
		errorCase(!*p); // ? ������Ȃ��B
		errorCase(*p == '\n'); // ? ! CR-LF

		if(*p == '\r')
		{
			errorCase(p[1] != '\n'); // ? ! CR-LF
			break;
		}
	}
	return p;
}
static char *ReadToCRLF_IgnoreTopCr(FILE *fp)
{
	autoBlock_t *buff = newBlock();

	for(; ; )
	{
		int chr = readChar(fp);

		if(chr == EOF)
		{
			releaseAutoBlock(buff);
			return NULL;
		}
		if(chr == '\r' && getSize(buff))
			break;

		errorCase(512000 < getSize(buff)); // ? 1�s_�ł��߂�
		addByte(buff, chr);
	}
	errorCase(readChar(fp) != '\n'); // ? ! CR-LF
	return unbindBlock2Line(buff);
}
static char *Block2Line(autoBlock_t *block)
{
	addByte(block, '\0');
	return b(block);
}

// ---- DoParseHeader ----

static char *DPH_Text;
static autoList_t *DPH_Lines;
static char *DPH_FirstLine;
static char *DPH_Method;
static char *DPH_Path;
static char *DPH_HTTPVersion;
static autoList_t *DPH_Keys;
static autoList_t *DPH_Values;
static uint64 DPH_BodySize;
static int DPH_Chunked;
static char *DPH_ReqType;

static void DPH_ParseLines(void)
{
	char *p = DPH_Text;

	DPH_Lines = newList();

	while(*p)
	{
		char *q = NextCRLF(p);
		char *line;

		*q = '\0'; // �j��
		line = strx(p);
//		*q = '\r'; // ����
		addElement(DPH_Lines, (uint)line);
		p = q + 2;
	}
}
static void DPH_ParseFirstLine(void)
{
	char *line = getLine(DPH_Lines, 0);
	autoList_t *tokens;

	tokens = tokenize(line, ' ');

	errorCase(getCount(tokens) != 3);

	DPH_Method      = getLine(tokens, 0);
	DPH_Path        = getLine(tokens, 1);
	DPH_HTTPVersion = getLine(tokens, 2);

	toAsciiLine(DPH_Method,      0, 0, 0);
	toAsciiLine(DPH_Path,        0, 0, 0);
	toAsciiLine(DPH_HTTPVersion, 0, 0, 0);
}
static void DPH_ParseFields(void)
{
	uint index;

	DPH_Keys = newList();
	DPH_Values = newList();

	for(index = 1; index + 1 < getCount(DPH_Lines); index++) // �ŏI�s�͋�s
	{
		char *line = getLine(DPH_Lines, index);

		toAsciiLine(line, 0, 1, 1);

		if(line[0] <= ' ' && 1 <= getCount(DPH_Values)) // unfolding
		{
			char *value = (char *)unaddElement(DPH_Values);

			ucTrimEdge(line);

			value = addChar(value, ' ');
			value = addLine(value, line);

			addElement(DPH_Values, (uint)value);
		}
		else
		{
			char *p = strchr(line, ':');
			char *key;
			char *value;

			errorCase(!p);

			*p = '\0';
			key = strx(line);
			value = strx(p + 1);

			ucTrimEdge(key);
			ucTrimEdge(value);

			addElement(DPH_Keys, (uint)key);
			addElement(DPH_Values, (uint)value);
		}
	}
}
static void DPH_CheckFields(void)
{
	char *key;
	uint index;

	foreach(DPH_Keys, key, index)
	{
		if(!_stricmp(key, "Content-Length"))
		{
			DPH_BodySize = toValue64(getLine(DPH_Values, index));
		}
		else if(!_stricmp(key, "Transfer-Encoding") && !_stricmp(getLine(DPH_Values, index), "chunked"))
		{
			DPH_Chunked = 1;
		}
		else if(!_stricmp(key, "Content-Type"))
		{
			DPH_ReqType = strx(getLine(DPH_Values, index));
		}
	}
}
static void DoParseHeader(void)
{
	autoBlock_t *recvData;
	uint rPos;

	recvData = readBinary(RECV_FILE);

	for(rPos = 0; ; rPos++)
	{
		if(getSize(recvData) < rPos + 4) // ? ��M������
		{
			errorCase(512000 < getSize(recvData)); // ? �w�b�__�ł��߂�
			errorCase(getFileWriteTime(IP_FILE) + SockTimeoutSec < time(NULL)); // ? �ڑ����Ă��� SockTimeoutSec �b�ȏ�o�� -> ���Ԋ|����߂��A�؂�B
			termination(0);
		}
		if(
			b(recvData)[rPos + 0] == '\r' &&
			b(recvData)[rPos + 1] == '\n' &&
			b(recvData)[rPos + 2] == '\r' &&
			b(recvData)[rPos + 3] == '\n'
			)
			break;
	}
	rPos += 4;
	setSize(recvData, rPos);
	DeleteFileDataPart(RECV_FILE, 0, rPos);

	writeBinary(RAW_HEADER_FILE, recvData);

	DPH_Text = unbindBlock2Line(recvData);
	DPH_ParseLines();
	DPH_ParseFirstLine();
	DPH_ParseFields();
	DPH_CheckFields();

	if(startsWithICase(DPH_Path, "http://"))
	{
		DPH_Path += 7;
		DPH_Path = strchrEnd(DPH_Path, '/');
	}
	if(*DPH_Path != '/')
		DPH_Path = xcout("/%s", DPH_Path);

	if(DPH_Chunked)
		errorCase(1 <= DPH_BodySize); // ? chunked �Ȃ̂� Content-Length ���w�肳��Ă���B
	else
		errorCase(RecvBodySizeMax < DPH_BodySize); // ? Content-Length �ł��߂�

	if(!DPH_ReqType)
		DPH_ReqType = strx("");

	writeOneLineNoRet_b(METHOD_FILE, DPH_Method);
	writeOneLineNoRet_b(REQ_PATH_FILE, DPH_Path);
	writeOneLineNoRet_b(HTTP_VERSION_FILE, DPH_HTTPVersion);
	writeLines(HEADER_KEYS_FILE, DPH_Keys);
	writeLines(HEADER_VALUES_FILE, DPH_Values);
	writeOneValue64(BODY_SIZE_FILE, DPH_BodySize);

	if(DPH_Chunked)
		createFile(CHUNKED_FLAG);

	writeOneLineNoRet_b(REQ_TYPE_FILE, DPH_ReqType);

	errorCase(getCount(readLines(HEADER_KEYS_FILE)) != getCount(readLines(HEADER_VALUES_FILE))); // test
}

// ---- DoRecvBody ----

static void DoRecvBody(void)
{
	uint64 bodySz = readFirstValue64(BODY_SIZE_FILE);
	uint64 recvFSz;

restart:
	recvFSz = getFileSize(RECV_FILE);

	if(recvFSz < bodySz) // ? ��M������
	{
	continue_read:
		errorCase(getFileWriteTime(RECV_FILE) + SockTimeoutSec < time(NULL)); // ? ��M�̖��ʐM��Ԃ� SockTimeoutSec �b�ȏ㑱���Ă���B-> �x�����A�؂�B
		termination(0);
	}
	if(existFile(CHUNKED_FLAG))
	{
		FILE *fp = fileOpen(RECV_FILE, "rb");
		char *line;
		uint64 nextSz;

		fileSeek(fp, SEEK_SET, bodySz);
		line = ReadToCRLF_IgnoreTopCr(fp);
		fileClose(fp);

		if(!line) // ? ���̃T�C�Y_��M������
			goto continue_read;

		DeleteFileDataPart(RECV_FILE, bodySz, strlen(line) + 2);

		// chunk-extension �폜
		{
			char *p = strchr(line, ';');

			if(p)
				*p = '\0';
		}

		ucTrimEdge(line);
		toAsciiLine(line, 0, 0, 0);

		nextSz = toValue64Digits(line, hexadecimal);

		memFree(line);

		if(1 <= nextSz) // ? chunk �܂�����
		{
			errorCase(IMAX_64 < nextSz); // ? ���̃T�C�Y_�ł��߂�
			bodySz += nextSz;
			errorCase(RecvBodySizeMax < bodySz); // ? �{�f�B_�ł��߂�
			writeOneValue64(BODY_SIZE_FILE, bodySz);
			goto restart;
		}
		setFileSize(RECV_FILE, bodySz); // chunk footer �폜
	}
	else
	{
		errorCase(bodySz < recvFSz); // ? ���M�f�[�^ Content-Length ��蒷���B
	}
	moveFile(RECV_FILE, BODY_FILE);
	createFile(RECV_FILE);
}

// ---- DoParseRequest ----

static void DoParseRequest(void)
{
	// �����ɒǉ��I
}

// ---- DoService ----

static void DoService(void)
{
	char *file = readText_b(REQ_PATH_FILE);

	file = lineToFairRelPath_x(file, strlen(RootDir));
	file = combine_cx(RootDir, file);
	file = changeExt_xc(file, "exe");

	if(!existFile(file))
		file = changeExt_xc(file, "bat");

	{
		char *stamp = makeJStamp(NULL, 0);
		char *ip = readFirstLine(IP_FILE);

		cout("%s [%s] %s\n", stamp, ip, file);

		memFree(stamp);
		memFree(ip);
	}

	errorCase(!existFile(file));

	writeOneLine(RES_HEADER_FIRST_LINE_FILE, "HTTP/1.1 200 Happy Tea Time");
	createFile(RES_HEADER_FILE);
	createFile(RES_BODY_FILE);

	LOGPOS();

	_spawnl(_P_WAIT, file, file, NULL);

	LOGPOS();

	// ���̕��ŃG���[�ɂȂ�̂ŁA�����Ń`�F�b�N���Ȃ��Ă�������B
//	errorCase(!existFile(RES_HEADER_FILE));
//	errorCase(!existFile(RES_BODY_FILE));

	{
		FILE *fp = fileOpen(SEND_FILE, "wb");

		writeBinaryBlock_x(fp, readBinary(RES_HEADER_FIRST_LINE_FILE));

		writeToken_x(fp, xcout(
//			"HTTP/1.1 200 Happy Tea Time\r\n" // -> RES_HEADER_FIRST_LINE_FILE
			"Server: htt2\r\n"
			"Connection: close\r\n"
			"Content-Length: %I64u\r\n"
			,getFileSize(RES_BODY_FILE)
			));

		writeBinaryBlock_x(fp, readBinary(RES_HEADER_FILE));

		writeChar(fp, '\r');
		writeChar(fp, '\n');

		writeBinaryBlock_x(fp, readBinary(RES_BODY_FILE));

		fileClose(fp);
	}
}

// ---- DoDownload ----

static void DoDownload(void)
{
	if(getFileSize(SEND_FILE) == 0)
	{
		DoDisconnect();
	}
	errorCase(getFileWriteTime(SEND_FILE) + SockTimeoutSec < time(NULL)); // ? ���M�̖��ʐM��Ԃ� SockTimeoutSec �b�ȏ㑱���Ă���B-> �x�����A�؂�B
	createFile(RECV_FILE); // ������M���Ȃ��B
	termination(0);
}

// ----

static void ErrorFnlz(void)
{
	if(errorOccurred)
		DoDisconnect();

	termination(1);
}
static void Main2(void)
{
	errorCase(!existFile(IP_FILE));
	errorCase(!existFile(RECV_FILE));
	errorCase(!existFile(SEND_FILE));

	if(getFileSize(IP_FILE) == 0)
		return;

	SockTimeoutSec = existFile(TIGHT_FILE) ? 60L : 300L;
	cout("SockTimeoutSec: %I64d\n", SockTimeoutSec);

	SendFileFullPath = makeFullPath(SEND_FILE);
	addFinalizer(ErrorFnlz);

	RootDir = nextArg();
	RecvBodySizeMax = toValue64(nextArg());

	errorCase(!isAbsPath(RootDir));
	errorCase(!existDir(RootDir));
	errorCase(!m_isRange(RecvBodySizeMax, 1, IMAX_64));

	if(existFile(RES_BODY_FILE))
		DoDownload();

	if(!existFile(RAW_HEADER_FILE))
		DoParseHeader();

	if(!existFile(BODY_FILE))
		DoRecvBody();

	DoParseRequest();
	DoService();
//	DoDownload();
}
int main(int argc, char **argv)
{
	Main2();
	termination(0);
}
