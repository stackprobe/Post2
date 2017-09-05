#include "C:\Dev\Main\Post2\libs\Service.h"

void Post2_Main(void)
{
	autoBlock_t *buff = newBlock();
	char *key;
	uint index;

	ab_addLine_x(buff, xcout("%s %s %s\r\n", Req.Method, Req.Path, Req.HTTP_Version));

	foreach(Req.HeaderKeys, key, index)
		ab_addLine_x(buff, xcout("%s: %s\r\n", key, getLine(Req.HeaderValues, index)));

	ab_addLine(buff, "\r\n");
	addBytes(buff, Req.Body);

	Res.Body = buff;
	Res.ContentType = "text/plain; charset=Shift_JIS";
}
