#include "libs\all4P2.h"

static char *Host;
static char *Ident;
static char *MailAddress;
static char *Password;
static char *Password2;
static time_t CurrTime;
static char *File;

static void DoSend(void)
{
	SaveFileRec_xx(combine_cx("C:\\temp\\data\\Post2\\recruit\\send", MakeUUID(1)), xcout(
		"Host=%s\n"
		"Ident=%s\n"
		"MailAddress=%s\n"
		,Host
		,Ident
		,MailAddress
		));
}
char *ProcTemplateVar(char *var)
{
	error();
	return NULL; // dummy
}
void Post2_Main(void)
{
	Host      = GetHeaderValue("Host");
	Ident       = GetBodyValue("ident");
	MailAddress = GetBodyValue("mailaddress");
	Password    = GetBodyValue("password");
	Password2   = GetBodyValue("password2");

	errorCase_m(!IsFairHost(Host),  "リクエストヘッダのホスト名に問題があります。");
	errorCase_m(!IsFairUUID(Ident), "エントリーがありません。");
	errorCase_m(!IsFairMailAddress(MailAddress), "メールアドレスに問題があります。");
	errorCase_m(!IsFairPassword(Password),   "パスワードに問題があります。");
	errorCase_m(!IsFairPassword(Password2),  "パスワード（確認用）に問題があります。");
	errorCase_m(strcmp(Password, Password2), "パスワードが一致しません。");

	CurrTime = time(NULL);
	File = combine("C:\\temp\\data\\Post2\\recruit\\entry1", Ident);

	errorCase_m(!existFile(File), "エントリーは失効しています。");

	removeFile(File); // 削除

	// メールアドレス規制
	{
		char *file = combine_cx("C:\\temp\\data\\Post2\\recruit\\mailaddress", xcout("[%s].txt", MailAddress));

		if(existFile(file))
		{
			char *text = LoadFileRec(file);
			char *regFile;

			regFile = EnvGetValue(text, "File");

			cout("regFile: %s\n", regFile);

			errorCase_m(existFile(regFile), "ご使用のメールアドレスは既に登録されています。");

			memFree(text);
			memFree(regFile);
		}
		memFree(file);
	}

	Ident = MakeUUID(1); // g
	File = combine("C:\\temp\\data\\Post2\\recruit\\entry2", Ident); // g

	// メールアドレス登録
	{
		char *file = combine_cx("C:\\temp\\data\\Post2\\recruit\\mailaddress", xcout("[%s].txt", MailAddress));

		SaveFileRec_cx(file, xcout("File=%s", File));

		memFree(file);
	}

	DoSend();

	SaveFileRec_cx(File, xcout(
		"Time=%I64d\n"
		"MailAddress=%s\n"
		"Password=%s\n"
		,CurrTime
		,MailAddress
		,Password
		));

	ProcTemplate();
}
