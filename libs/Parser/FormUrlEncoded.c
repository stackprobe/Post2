#include "all.h"

void ParseFormUrlEncoded(void)
{
	char *sBody = ab_toLine(Req.Body);
	autoList_t *parts;
	char *part;
	uint part_index;

	parts = tokenize(sBody, '&');

	foreach(parts, part, part_index)
	{
		autoList_t *tokens = tokenize(part, '=');

		if(getCount(tokens) == 2)
		{
			char *key   = getLine(tokens, 0);
			char *value = getLine(tokens, 1);

			DecodeUrl(key);
			DecodeUrl(value);

			AddBodyValue_xx(strx(key), strx(value));
		}
		releaseDim(tokens, 1);
	}
	memFree(sBody);
	releaseDim(parts, 1);
}
