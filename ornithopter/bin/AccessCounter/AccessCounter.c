#include "C:\Dev\Main\Post2\libs\Service.h"

void Post2_Main(void)
{
	char *id = "default";
	char *file;
	uint64 counter;

	if(hasArgs(1))
		id = nextArg();

	id = lineToFairLocalPath(id, 100);
	file = combine("C:\\appdata\\Post2.AccessCounter\\Counter.txt", id);

	createPath(file, 'f');
	counter = toValue64_x(readFirstLine(file));
	counter++;

	if(IMAX_64 < counter) // ƒJƒ“ƒXƒg
		counter = 0;

	Res.Body = ab_fromLine_x(xcout("%I64u", counter));

	writeOneLineNoRet_b_cx(file, xcout("%I64u", counter));

	memFree(id);
	memFree(file);
}
