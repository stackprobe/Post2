/*
	Password.txt

		実行ファイルと同じ場所に置くこと。

		書式
			readResourceLines()

		各行
			ユーザー名とパスワードを ':' で連結する。

		記述例
			user01:1111
			admin:9999
			scott:tiger
*/

#include "AuthorizationService.h"

#define FORBIDDEN_PERIODS_FILE "C:\\appdata\\Post2.AuthorizationService\\ForbiddenPeriods.txt"
#define MISTAKE_COUNTERS_FILE  "C:\\appdata\\Post2.AuthorizationService\\MistakeCounters.txt"

#define FORBIDDEN_SEC 86400
#define MISTAKE_LIMIT 10

static char *GetPasswordFile(void)
{
	return combine_cx(getSelfDir(), xcout("Password_%s.txt", getEnvLine("ComputerName")));
}

// ---- MistakeUserPw ----

static char *MUP_User;

static void MUP_SetUserPw(char *userPw)
{
	autoBlock_t *enc = ab_fromLine(userPw);
	autoBlock_t *dec;
	char *sDec;
	char *p;

	dec = decodeBase64(enc);
	sDec = ab_toLine(dec);

	p = strchr(sDec, ':');

	if(p)
	{
		*p = '\0';
		MUP_User = strx(sDec);
	}
	else
		MUP_User = NULL;

	releaseAutoBlock(enc);
	releaseAutoBlock(dec);
	memFree(sDec);

	cout("MUP_User: <%u>\n", m_01(MUP_User));
}

static autoList_t *MUP_Keys;
static autoList_t *MUP_Values;

static void MUP_LoadKeyValues(char *file)
{
	autoList_t *lines;
	char *line;
	uint index;

	LOGPOS();

	MUP_Keys = newList();
	MUP_Values = newList();

	if(!existFile(file))
		return;

	lines = readLines(file);

	foreach(lines, line, index)
	{
		addElement(MUP_Keys, (uint)line);
		addElement(MUP_Values, (uint)getLine(lines, ++index));
	}
	releaseAutoList(lines);

	LOGPOS();
}
static void MUP_ReleaseKeyValues(void)
{
	releaseDim(MUP_Keys, 1);
	releaseDim(MUP_Values, 1);
}
static void MUP_SaveKeyValues(char *file)
{
	FILE *fp;
	char *key;
	uint index;

	LOGPOS();

	createPath(file, 'X');

	fp = fileOpen(file, "wt");

	foreach(MUP_Keys, key, index)
	{
		writeLine(fp, key);
		writeLine(fp, getLine(MUP_Values, index));
	}
	fileClose(fp);

	MUP_ReleaseKeyValues();

	LOGPOS();
}
static int IsForbiddenUserPw(char *userPw)
{
	uint index;
	time_t period;
	time_t currTime;

	LOGPOS();
	MUP_SetUserPw(userPw);

	if(!MUP_User)
		return 0;

	MUP_LoadKeyValues(FORBIDDEN_PERIODS_FILE);

	index = findLine(MUP_Keys, MUP_User);

	if(index == getCount(MUP_Keys))
	{
		MUP_ReleaseKeyValues();

		LOGPOS();
		return 0;
	}
	period = (time_t)toValue64(getLine(MUP_Values, index));
	currTime = time(NULL);

	cout("period: %I64u (%I64u)\n", period, currTime);

	if(currTime < period)
	{
		MUP_ReleaseKeyValues();

		LOGPOS();
		return 1;
	}
	memFree((char *)fastDesertElement(MUP_Keys, index));
	memFree((char *)fastDesertElement(MUP_Values, index));

	MUP_SaveKeyValues(FORBIDDEN_PERIODS_FILE);

	LOGPOS();
	return 0;
}
static void ReportCorrectUserPw(char *userPw)
{
	uint index;

	LOGPOS();
	MUP_SetUserPw(userPw);

	if(!MUP_User)
		return;

	MUP_LoadKeyValues(MISTAKE_COUNTERS_FILE);

	index = findLine(MUP_Keys, MUP_User);

	if(index < getCount(MUP_Keys))
	{
		memFree((char *)fastDesertElement(MUP_Keys, index));
		memFree((char *)fastDesertElement(MUP_Values, index));

		MUP_SaveKeyValues(MISTAKE_COUNTERS_FILE);
	}
	else
	{
		MUP_ReleaseKeyValues();
	}
}
static void ReportMistakeUserPw(char *userPw)
{
	int overMistake = 0;

	LOGPOS();
	MUP_SetUserPw(userPw);

	if(!MUP_User)
		return;

	// find user
	{
		autoList_t *userPwList = readResourceLines(GetPasswordFile()); // gomi
		char *line;
		uint index;

		foreach(userPwList, line, index)
		{
			ne_strchr(line, ':')[0] = '\0';

			if(!strcmp(line, MUP_User))
				break;
		}
		releaseDim(userPwList, 1);

		cout("line: <%u>\n", m_01(line));

		if(!line) // ? not found
			return;
	}

	MUP_LoadKeyValues(MISTAKE_COUNTERS_FILE);

	// increment mistake-counter
	{
		uint index = findLine(MUP_Keys, MUP_User);

		if(index < getCount(MUP_Keys))
		{
			uint count = toValue(getLine(MUP_Values, index));

			if(count < MISTAKE_LIMIT)
			{
				strzp_x((char **)directGetPoint(MUP_Values, index), xcout("%u", count + 1));
			}
			else
			{
				memFree((char *)fastDesertElement(MUP_Keys, index));
				memFree((char *)fastDesertElement(MUP_Values, index));

				// add to forbidden users
				{
					FILE *fp = fileOpen(FORBIDDEN_PERIODS_FILE, "wt");

					writeLine(fp, MUP_User);
					writeLine_x(fp, xcout("%I64u", time(NULL) + FORBIDDEN_SEC));

					fileClose(fp);
				}
			}
		}
		else
		{
			addElement(MUP_Keys, (uint)strx(MUP_User));
			addElement(MUP_Values, (uint)strx("0"));
		}
	}

	MUP_SaveKeyValues(MISTAKE_COUNTERS_FILE);
}

// ----

void Post2_Main(void)
{
	char *auth = GetHeaderValue("Authorization");
	char *authUserPw = NULL;

	LOGPOS();
	cout("auth.1: [%s]\n", auth);

	if(startsWithICase(auth, "Basic "))
		auth += 6;

	cout("auth.2: [%s]\n", auth);

	if(IsForbiddenUserPw(auth))
	{
		Res.Body = ab_fromLine_x(xcout(
			"<html>"
			"<body style=\"color: #c00000; font-family: メイリオ; font-size: large;\">"
			"P2AS_Forbidden<hr/>%s @ %s"
			"</body>"
			"</html>"
			,Req.Path
			,makeJStamp(NULL, 0) // gomi
			));

		LOGPOS();
		return;
	}

	// auth 判定
	{
		char *file = GetPasswordFile();
		autoList_t *userPwList;
		char *userPw;
		uint index;

		cout("PASSWORD_FILE: %s\n", file);

		userPwList = readResourceLines(file);

		foreach(userPwList, userPw, index)
		{
			autoBlock_t *dec = ab_fromLine(userPw);
			autoBlock_t *enc;
			char *sEnc;

			cout("userPw: [%s]\n", userPw);

			enc = encodeBase64(dec);
			sEnc = ab_toLine(enc);

			cout("sEnc: [%s]\n", sEnc);

			if(!strcmp(sEnc, auth))
			{
				cout("BINGO!\n");
				authUserPw = strx(userPw);
			}
			releaseAutoBlock(dec);
			releaseAutoBlock(enc);
			memFree(sEnc);

			if(authUserPw)
				break;
		}
		memFree(file);
		releaseDim(userPwList, 1);
	}

	if(authUserPw)
	{
		LOGPOS();
		Post2_Auth_Main(authUserPw);
		LOGPOS();
		memFree(authUserPw);

		ReportCorrectUserPw(auth);
	}
	else
	{
		Res.StatusCode = 401;
		addElement(Res.HeaderLines, (uint)"WWW-Authenticate: Basic realm=\"Post2\"");
		Res.Body = ab_fromLine_x(xcout(
			"<html>"
			"<body style=\"color: #c00000; font-family: メイリオ; font-size: large;\">"
			"401<hr/>%s @ %s"
			"</body>"
			"</html>"
			,Req.Path
			,makeJStamp(NULL, 0) // gomi
			));

		ReportMistakeUserPw(auth);
	}
	LOGPOS();
}
