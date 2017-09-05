#include "C:\Dev\Main\Post2\libs\Common\all.h"

int main(int argc, char **argv)
{
	LOGPOS();

	createPath("C:\\temp\\data\\Post2\\recruit\\send", 'd');
	createPath("C:\\temp\\data\\Post2\\recruit\\sending", 'd');

	LOGPOS();

	moveDir(
		"C:\\temp\\data\\Post2\\recruit\\send",
		"C:\\temp\\data\\Post2\\recruit\\sending"
		);

	LOGPOS();
}
