#include "C:\Dev\Main\Post2\libs\AuthorizationService.h"

void Post2_Auth_Main(char *userPw)
{
	Res.Body = ab_fromLine_x(xcout(
		"<html><body><h1>���[�U�[�E�p�X���[�h: [%s]</h1></body></html>"
		,userPw
		));
}
