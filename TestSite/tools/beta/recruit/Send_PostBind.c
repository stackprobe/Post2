#include "C:\Dev\Main\Post2\libs\Common\all.h"
#include "C:\Factory\Meteor\GmailSend.h"

static void DoSend(char *host, char *ident, char *mailaddress)
{
	LOGPOS();

	/*
		2bs
	*/
	errorCase(m_isEmpty(host));
	errorCase(m_isEmpty(ident));
	errorCase(m_isEmpty(mailaddress));

	GS_Clear();
	GS_AddTo(mailaddress);
	GS_SetUser("user0001@gmail.com");
	GS_SetFrom("user0001@gmail.com");
	GS_SetPassword("9999");
	GS_SetSubject("これはテストメールです");
	GS_SetBody_x(xcout(
		"これはテストメールです。\n"
		"このメールに心当たりが無い場合は破棄して下さい。\n"
		"以下のURLを開いて登録を続行して下さい。\n"
		"http://%s/beta/recruit/reg-confirmed.html?ident=%s"
		,host
		,ident
		));

	GS_Send();

	LOGPOS();
}
int main(int argc, char **argv)
{
	LOGPOS();

	createPath("C:\\temp\\data\\Post2\\recruit\\sending", 'd');

	LOGPOS();

	{
		autoList_t *files = lsFiles("C:\\temp\\data\\Post2\\recruit\\sending");
		char *file;
		uint index;
		uint stTime = now();

		foreach(files, file, index)
		{
			char *text = LoadFileRec(file);
			char *host;
			char *ident;
			char *mailaddress;

			removeFile(file);

			host        = EnvGetValue(text, "Host");
			ident       = EnvGetValue(text, "Ident");
			mailaddress = EnvGetValue(text, "MailAddress");

			DoSend(host, ident, mailaddress);

			memFree(text);
			memFree(host);
			memFree(ident);
			memFree(mailaddress);

			if(stTime + 30 < now()) // ? タイムアウト
			{
				cout("★全メール送信タイムアウト\n");
				break;
			}
		}
		releaseDim(files, 1);
	}

	LOGPOS();
}
