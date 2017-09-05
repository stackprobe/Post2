#include "libs\all4P2.h"

static char *MailAddress;
static char *Password;
static char *MailAddressFile;
static char *MailAddressFileText;
static char *MailAddressFileText_File;
static char *Ident;
static char *ProfileFile;
static char *Profile;
static char *Profile_Password;

char *ProcTemplateVar(char *var)
{
	ucTrim(var);

	if(!_stricmp(var, "Ident"))
	{
		return strx(Ident);
	}
	error();
	return NULL; // dummy
}
void Post2_Main(void)
{
	MailAddress = GetBodyValue("mailaddress");
	errorCase_m(!IsFairMailAddress(MailAddress), "���[���A�h���X�܂��̓p�X���[�h���Ⴂ�܂��B");
	Password = GetBodyValue("password");
	errorCase_m(!IsFairPassword(Password), "���[���A�h���X�܂��̓p�X���[�h���Ⴂ�܂��B");
	MailAddressFile = combine_cx("C:\\temp\\data\\Post2\\recruit\\mailaddress", xcout("[%s].txt", MailAddress));
	errorCase_m(!existFile(MailAddressFile), "���[���A�h���X�܂��̓p�X���[�h���Ⴂ�܂��B"); // ? ���o�^�̃��[���A�h���X
	MailAddressFileText = LoadFileRec(MailAddressFile);
	MailAddressFileText_File = EnvGetValue(MailAddressFileText, "File");
	errorCase(!existFile(MailAddressFileText_File));
	Ident = strx(getLocal(MailAddressFileText_File));
	errorCase(!IsFairUUID(Ident));
	ProfileFile = combine("C:\\temp\\data\\Post2\\recruit\\register", Ident);
	errorCase_m(!existFile(ProfileFile), "�o�^�葱�����������Ă��܂���B");
	Profile = LoadFileRec(ProfileFile);
	Profile_Password = EnvGetValue(Profile, "Password");
	errorCase(!IsFairPassword(Profile_Password));
	errorCase_m(strcmp(Password, Profile_Password), "���[���A�h���X�܂��̓p�X���[�h���Ⴂ�܂��B");

	ProcTemplate();
}
