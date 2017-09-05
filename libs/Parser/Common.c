#include "all.h"

void DecodeUrl(char *url)
{
	char *rp = url;
	char *wp = url;

	while(*rp)
	{
		if(*rp == '%' && rp[1] && rp[2])
		{
			*wp = m_c2i(rp[1]) << 4 | m_c2i(rp[2]);
			rp += 2;
		}
		else if(*rp == '+')
			*wp = ' ';
		else
			*wp = *rp;

		rp++;
		wp++;
	}
	*wp = '\0';
}
