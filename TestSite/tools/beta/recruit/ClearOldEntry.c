#include "C:\Dev\Main\Post2\libs\Common\all.h"

static time_t CurrTime;

int main(int argc, char **argv)
{
	LOGPOS();

	CurrTime = time(NULL);

	createPath("C:\\temp\\data\\Post2\\recruit\\entry1", 'd');
	createPath("C:\\temp\\data\\Post2\\recruit\\entry2", 'd');

	// entry1
	{
		autoList_t *files = lsFiles("C:\\temp\\data\\Post2\\recruit\\entry1");
		char *file;
		uint index;

		foreach(files, file, index)
		{
			char *text = LoadFileRec(file);
			time_t t;

			t = toValue64(EnvGetValue(text, "Time"));
			cout("t: %I64d\n", t); // test

			errorCase(t == 0); // ? Time Ç™ñ≥Ç¢ÅB

			if(t + 3600L < CurrTime)
			{
				cout("e1Tmout: %s\n", file);
				removeFile(file);
			}
			memFree(text);
		}
	}

	LOGPOS();

	// entry2
	{
		autoList_t *files = lsFiles("C:\\temp\\data\\Post2\\recruit\\entry2");
		char *file;
		uint index;

		foreach(files, file, index)
		{
			char *text = LoadFileRec(file);
			time_t t;

			t = toValue64(EnvGetValue(text, "Time"));
			cout("t: %I64d\n", t); // test

			errorCase(t == 0); // ? Time Ç™ñ≥Ç¢ÅB

			if(t + 86400L < CurrTime)
			{
				cout("e2Tmout: %s\n", file);
				removeFile(file);
			}
			memFree(text);
		}
	}

	LOGPOS();
}
