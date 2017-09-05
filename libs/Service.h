#pragma once

#include "C:\Factory\Common\all.h"
#include "Common\all.h"

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

typedef struct Req_st
{
	char *IP;
	char *Method;
	char *Path;
	char *HTTP_Version;
	autoList_t *HeaderKeys;
	autoList_t *HeaderValues;
	uint64 BodySize;
	int ChunkedFlag;
	char *Type;
	autoBlock_t *Body;
}
Req_t;

typedef struct Res_st
{
	uint StatusCode;
	char *ContentType;
	autoList_t *HeaderLines;
	autoBlock_t *Body;
}
Res_t;

extern Req_t Req;
extern Res_t Res;

char *GetBootDir(void);
char *GetHeaderValue(char *key);

void Post2_Main(void); // external
