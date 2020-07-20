/*
	一定時間内の同じIPからのアクセスはカウントしない。
*/

#include "C:\Dev\Main\Post2\libs\Service.h"

#define IP_MAX 1000
#define TIMEOUT_SEC 300 // 5 min

static time_t CurrTime;

static autoList_t *TimeList;
static autoList_t *IPList;

static void LoadIPFile(char *ipFile)
{
	autoList_t *lines = readLines(ipFile);
	char *line;
	uint index;

	TimeList = newList();
	IPList = newList();

	foreach(lines, line, index)
	{
		autoList_t *tokens = tokenize(line, ' ');

		addElement(TimeList, (uint)strx(getLine(tokens, 0)));
		addElement(IPList,   (uint)strx(getLine(tokens, 1)));

		releaseDim(tokens, 1);
	}
	releaseDim(lines, 1);
}
static void EraseTimeout(void)
{
	char *sTime;
	uint index;

	errorCase(getCount(TimeList) != getCount(IPList)); // 2bs

	foreach(TimeList, sTime, index)
	{
		time_t lastAccessTime = toValue64(sTime);

		if(lastAccessTime + TIMEOUT_SEC < CurrTime) // ? タイムアウト
		{
			sTime[0] = '\0';
			getLine(IPList, index)[0] = '\0';
		}
	}
	trimLines(TimeList);
	trimLines(IPList);

	errorCase(getCount(TimeList) != getCount(IPList)); // 2bs
}
static void EraseOverflow(void)
{
	uint ofnum;
	uint index;

	if(IP_MAX < getCount(IPList))
		ofnum = getCount(IPList) - IP_MAX;
	else
		ofnum = 0;

	errorCase(getCount(TimeList) != getCount(IPList)); // 2bs

	for(index = 0; index < ofnum; index++)
	{
		getLine(TimeList, index)[0] = '\0';
		getLine(IPList,   index)[0] = '\0';
	}
	trimLines(TimeList);
	trimLines(IPList);

	errorCase(getCount(TimeList) != getCount(IPList)); // 2bs
}
static void SaveIPFile(char *ipFile)
{
	FILE *fp = fileOpen(ipFile, "wt");
	char *sTime;
	uint index;

	foreach(TimeList, sTime, index)
		writeLine_x(fp, xcout("%s %s", sTime, getLine(IPList, index)));

	fileClose(fp);
}
static int IsFirstAccess(char *ip, char *ipFile)
{
	uint index;
	int ret;

	cout("IsFirstAccess: %s ", ip);

	CurrTime = time(NULL);

	LoadIPFile(ipFile);
	EraseTimeout();
	EraseOverflow();

	index = findLine(IPList, ip);

	if(index < getCount(IPList)) // ? アクセス記録有り
	{
		ret = 0;
	}
	else // ? アクセス記録無し
	{
		ret = 1;

		addElement(TimeList, (uint)xcout("%I64d", CurrTime));
		addElement(IPList, (uint)strx(ip));
	}
	SaveIPFile(ipFile);

	cout("-> %d\n", ret);
	return ret;
}
void Post2_Main(void)
{
	char *id = "default";
	char *file;
	char *ipFile;
	uint64 counter;

	if(hasArgs(1))
		id = nextArg();

	id = lineToFairLocalPath(id, 100);
	file   = combine_cx("C:\\appdata\\Post2.AccessCounter2", xcout("%s.counter.txt", id));
	ipFile = combine_cx("C:\\appdata\\Post2.AccessCounter2", xcout("%s.ip.txt", id));

	createPath(file, 'f');
	createPath(ipFile, 'f');

	counter = toValue64_x(readFirstLine(file));

	if(IsFirstAccess(Req.IP, ipFile))
	{
		counter++;

		if(IMAX_64 < counter) // カンスト
			counter = 0;

		writeOneLineNoRet_b_cx(file, xcout("%I64u", counter));
	}
	Res.Body = ab_fromLine_x(xcout("%I64u", counter));

	memFree(id);
	memFree(file);
}
