/*
	Service.dat に以下を追加

		POST <この実行ファイルの場所>\Post2.exe ROOT-DIR RECV-BODY-SIZE-MAX

			ROOT-DIR ...

				サービスプログラムを配置したディレクトリ
				相対パス不可

			RECV-BODY-SIZE-MAX ...

				リクエストボディの最大サイズ [バイト]
				値域 == 1 ～ IMAX_64

		追加例

			POST "<BASE>Post2.exe" "<BASE>Service" 2000000

	- - - -

	実行されるサービスプログラムのパス

		ROOT-DIR \ REQ-FILE

			REQ-FILE ... リクエストURLのパス部分を lineToFairLocalPath() して changeExt(, "exe") した文字列

		★このファイルが存在しなければ、更に changeExt(, "bat") した文字列

		★そのファイルも存在しなければ、直ちに切断する。

	実行時のカレントに存在するファイル

		IP.httdat    ... IPアドレス
		Recv.httdat  ... 空のファイル
		Send.httdat  ... 空のファイル
		Tight.httdat ... 空のファイル (存在しない場合もある)

		Method.htt2dat       ... メソッド   ex. POST
		Path.htt2dat         ... パス       ex. /cgi-bin/index.cgi
		HTTP_Version.htt2dat ... バージョン ex. HTTP/1.1
		RawHeader.htt2dat    ... ヘッダ全体, 先頭行も含む
		HeaderKeys.htt2dat   ... ヘッダのキーのリスト
		HeaderValues.htt2dat ... ヘッダの値のリスト
		BodySize.htt2dat     ... ボディサイズ, readFirstValue64 で読み込むこと。
		Chunked.htt2dat      ... Chunked の場合存在する。
		Body.htt2dat         ... ボディ全体
		Type.htt2dat         ... Content-Type の値, 無ければ 0 バイトのファイル
		ResHeader.htt2dat    ... 応答ヘッダ, 0 バイトのファイル
		ResBody.htt2dat      ... 応答ボディ, 0 バイトのファイル

	するべきこと

		ResHeader.htt2dat
		ResBody.htt2dat

		... を更新する。

		★ ResHeader.htt2dat に Content-Type を追加すること。ヘッダ最後の「空行」は不要
		★ ResBody.htt2dat のサイズを Content-Length にする。
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
		errorCase(!*p); // ? 見つからない。
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

		errorCase(512000 < getSize(buff)); // ? 1行_でか過ぎ
		addByte(buff, chr);
	}
	errorCase(readChar(fp) != '\n'); // ? ! CR-LF
	return unbindBlock2Line(buff);
}
static char *Block2Line(autoBlock_t *block)
{
	addByte(block, '\0');
	return b_(block);
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
static int DPH_Expect100Continue;

static void DPH_ParseLines(void)
{
	char *p = DPH_Text;

	DPH_Lines = newList();

	while(*p)
	{
		char *q = NextCRLF(p);
		char *line;

		*q = '\0'; // 破壊
		line = strx(p);
//		*q = '\r'; // 復元
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

	for(index = 1; index + 1 < getCount(DPH_Lines); index++) // 最終行は空行
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
		else if(!_stricmp(key, "Expect") && !_stricmp(getLine(DPH_Values, index), "100-continue"))
		{
			DPH_Expect100Continue = 1;
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
		if(getSize(recvData) < rPos + 4) // ? 受信未完了
		{
			errorCase(512000 < getSize(recvData)); // ? ヘッダ_でか過ぎ
			errorCase(getFileWriteTime(IP_FILE) + SockTimeoutSec < time(NULL)); // ? 接続してから SockTimeoutSec 秒以上経過 -> 時間掛かり過ぎ、切る。
			termination(0);
		}
		if(
			b_(recvData)[rPos + 0] == '\r' &&
			b_(recvData)[rPos + 1] == '\n' &&
			b_(recvData)[rPos + 2] == '\r' &&
			b_(recvData)[rPos + 3] == '\n'
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
		errorCase(1 <= DPH_BodySize); // ? chunked なのに Content-Length が指定されている。
	else
		errorCase(RecvBodySizeMax < DPH_BodySize); // ? Content-Length でか過ぎ

	if(!DPH_ReqType)
		DPH_ReqType = strx("");

	errorCase(DPH_Expect100Continue); // Expect: 100-continue 非対応

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

	if(recvFSz < bodySz) // ? 受信未完了
	{
	continue_read:
		errorCase(getFileWriteTime(RECV_FILE) + SockTimeoutSec < time(NULL)); // ? 受信の無通信状態が SockTimeoutSec 秒以上続いている。-> 遅すぎ、切る。
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

		if(!line) // ? 次のサイズ_受信未完了
			goto continue_read;

		DeleteFileDataPart(RECV_FILE, bodySz, strlen(line) + 2);

		// chunk-extension 削除
		{
			char *p = strchr(line, ';');

			if(p)
				*p = '\0';
		}

		ucTrimEdge(line);
		toAsciiLine(line, 0, 0, 0);

		nextSz = toValue64Digits(line, hexadecimal);

		memFree(line);

		if(1 <= nextSz) // ? chunk まだ続く
		{
			errorCase(IMAX_64 < nextSz); // ? 次のサイズ_でか過ぎ
			bodySz += nextSz;
			errorCase(RecvBodySizeMax < bodySz); // ? ボディ_でか過ぎ
			writeOneValue64(BODY_SIZE_FILE, bodySz);
			goto restart;
		}
		setFileSize(RECV_FILE, bodySz); // chunk footer 削除
	}
	else
	{
		errorCase(bodySz < recvFSz); // ? 送信データ Content-Length より長い。
	}
	moveFile(RECV_FILE, BODY_FILE);
	createFile(RECV_FILE);
}

// ---- DoParseRequest ----

static void DoParseRequest(void)
{
	// ここに追加！
}

// ---- DoService ----

static void DoService(void)
{
	char *file = readText_b(REQ_PATH_FILE);

	// クエリー除去
	{
		char *q = strchr(file, '?');

		if(q)
			*q = '\0';
	}

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

	// 下の方でエラーになるので、ここでチェックしなくてもいいや。
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
	errorCase(getFileWriteTime(SEND_FILE) + SockTimeoutSec < time(NULL)); // ? 送信の無通信状態が SockTimeoutSec 秒以上続いている。-> 遅すぎ、切る。
	createFile(RECV_FILE); // もう受信しない。
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
