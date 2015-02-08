//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "command_op.h"
#include <stdio.h>
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TCommandExeDialog *CommandExeDialog;
//---------------------------------------------------------------------------
#define BUF_SIZE 1024
/////////////////////////////////////////////////////////////////////////////
//
// Constructor
//

__fastcall TCommandExeDialog::TCommandExeDialog(HWND pHandle, //TComponent* AOwner,
	    const char *fpExecFile, // ���s�t�@�C���t���p�X��
	    const char *CommandLine, // �R�}���h���C��(�}�N���g�p�\)
	    const char *fpTargetFile, // �Ώۃt�@�C�����i��̂݁j
        const char *CommandEnv, // ���ϐ�(|�A���`���j
	    const char *fpStdErrFile, // ���ʏo�̓t�@�C���t���p�X��
	    const bool IsStdErrOut, // ���ʏo�͂̐���
		const bool IsBracketDQ // �t�@�C������""�ň͂�(Bracket)
) : TForm(pHandle) //AOwner)
{
	EfpExecFile->Text = AnsiString(fpExecFile);
    ECommandLine->Text = AnsiString(CommandLine);
    EfpTargetFile->Text = AnsiString(fpTargetFile);
    ECommandEnv->Lines->Append(AnsiString(CommandEnv));
    EfpStdErrFile->Caption = AnsiString(fpStdErrFile);
    CBStdErrOut->Checked = IsStdErrOut;
    CBBracketDQ->Checked = IsBracketDQ;
}

//
/////////////////////////////////////////////////////////////////////////////
//
// �v���Z�X���s�⏕�֐��Q
//

void ExpandEnvStr(TStrings *EnvList, char *env_buf, int buf_size)
{
	AnsiString buf = ""; //���ݒ蕶����ꎞ�o�b�t�@
	ZeroMemory(env_buf, buf_size);
    char windir[MAX_PATH];
    GetWindowsDirectory(windir, MAX_PATH);

	for(int i = 0;i < EnvList->Count;i++)
		buf += EnvList->Strings[i] + "|";

	//%WINDIR% �����ۂ�Windows�f�B���N�g���ɒu��
	TReplaceFlags f;
	f.Clear();
	f << rfReplaceAll;
	f << rfIgnoreCase;
	buf = StringReplace(buf, "%WINDIR%", AnsiString(windir), f);

	//�������ŉ��߂�env_str��C������ɕϊ��B|��\0�ɒ����B
	if(EnvList->Strings[0] == "") return;
	strncpy(env_buf, buf.c_str(), buf_size);
	for(int i = 0; i < buf_size; i++)
    	if(env_buf[i] == '|') env_buf[i] = '\0';
	return;
}

void InitSecurityAtt(SECURITY_DESCRIPTOR *sd, SECURITY_ATTRIBUTES *sa)
{
	InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(sd, true, NULL, false);
	sa->nLength = sizeof(SECURITY_ATTRIBUTES);
	sa->bInheritHandle = true;
	sa->lpSecurityDescriptor = sd;

}

void ExpandCommandLineMacros(AnsiString fpExecFile, AnsiString CLBuffer,
	AnsiString fpTargetFile, bool IsBracketDQ, char *cmdline_buf, int buf_size)
{
AnsiString Buffer = CLBuffer;

AnsiString lfc = fpExecFile;
AnsiString sfc = ExtractShortPathName(lfc);
AnsiString lc = ExtractFileName(lfc);
AnsiString sc = ExtractFileName(sfc);
AnsiString lft = fpTargetFile;
AnsiString sft = ExtractShortPathName(lft);
AnsiString lt = ExtractFileName(lft);
AnsiString st = ExtractFileName(sft);
AnsiString ldt = ExtractFileDir(lft);
char file[MAXFILE];
fnsplit(lft.c_str(),NULL,NULL,file,NULL);
AnsiString lnt = ldt + "\\" + AnsiString(file);
AnsiString lot = AnsiString(file);

TReplaceFlags f;
f.Clear();
f << rfReplaceAll;
f << rfIgnoreCase;

if(IsBracketDQ) {
	Buffer = StringReplace(Buffer, "%lfc", "\"%lfc\"", f);
	Buffer = StringReplace(Buffer, "%sfc", "\"%sfc\"", f);
	Buffer = StringReplace(Buffer, "%lc", "\"%lc\"", f);
	Buffer = StringReplace(Buffer, "%sc", "\"%sc\"", f);
	Buffer = StringReplace(Buffer, "%lft", "\"%lft\"", f);
	Buffer = StringReplace(Buffer, "%sft", "\"%sft\"", f);
	Buffer = StringReplace(Buffer, "%lt", "\"%lt\"", f);
	Buffer = StringReplace(Buffer, "%st", "\"%st\"", f);
	Buffer = StringReplace(Buffer, "%ldt", "\"%ldt\"", f);
	Buffer = StringReplace(Buffer, "%lnt", "\"%lnt\"", f);
	Buffer = StringReplace(Buffer, "%lot", "\"%lot\"", f);
}

Buffer = StringReplace(Buffer, "%lfc", lfc, f);
Buffer = StringReplace(Buffer, "%sfc", sfc, f);
Buffer = StringReplace(Buffer, "%lc", lc, f);
Buffer = StringReplace(Buffer, "%sc", sc, f);
Buffer = StringReplace(Buffer, "%lft", lft, f);
Buffer = StringReplace(Buffer, "%sft", sft, f);
Buffer = StringReplace(Buffer, "%lt", lt, f);
Buffer = StringReplace(Buffer, "%st", st, f);
Buffer = StringReplace(Buffer, "%ldt", ldt, f);
Buffer = StringReplace(Buffer, "%lnt", lnt, f);
Buffer = StringReplace(Buffer, "%lot", lot, f);

StrLCopy(cmdline_buf, Buffer.c_str(), buf_size);
}

//
/////////////////////////////////////////////////////////////////////////////
//
// �O���C���^�[�t�F�C�X�֐�
//
/////////////////////////////////////////////////////////////////////////////
//

void AnaHelpExec(HWND Handle, char *fpHelpFile)
{
	WinHelp(Handle, fpHelpFile, HELP_CONTENTS,0);
}

bool AnaCommandExec(
	const char *fpExecFile, // ���s�t�@�C���t���p�X��
    const char *CommandLine, // �R�}���h���C��(�}�N���g�p�\)
    const char *fpTargetFile, // �Ώۃt�@�C�����i��̂݁j
    const char *CommandEnv, // ���ϐ�(|�A���`���j
    const char *fpStdErrFile, // ���ʏo�̓t�@�C���t���p�X��
    const bool IsStdErrOut, // ���ʏo�͂̐���
    const bool IsBracketDQ, // �t�@�C������""�ň͂�(Bracket)
	const HWND Handle // �Ƃ肠�����̐e�E�C���h�E
)
{
	TCommandExeDialog *dlg = new TCommandExeDialog(Handle,
		fpExecFile, // ���s�t�@�C���t���p�X��
	    CommandLine, // �R�}���h���C��(�}�N���g�p�\)
	    fpTargetFile, // �Ώۃt�@�C�����i��̂݁j
        CommandEnv, // ���ϐ�(|�A���`���j
	    fpStdErrFile, // ���ʏo�̓t�@�C���t���p�X��
	    IsStdErrOut, // ���ʏo�͂̐���
        IsBracketDQ // �t�@�C������""�ň͂�(Bracket)
        );
	int result = dlg->ShowModal();
    if(result != mrOk) {
    	delete dlg;
        return false;
    }

    //�O�̂��ߎ��s�R�}���h�̋�`�F�b�N�B
	if(dlg->EfpExecFile->Text == "" && dlg->EfpTargetFile->Text == "")
	{
    	MessageBox(Handle,"���s�t�@�C�����w�肳��Ă��܂���B","�G���[",MB_OK | MB_ICONERROR);
		delete dlg;
        return false;
    }

    char cmdline_str[BUF_SIZE];
	ExpandCommandLineMacros(
    	dlg->EfpExecFile->Text,
        dlg->ECommandLine->Text,
        dlg->EfpTargetFile->Text,
        dlg->CBBracketDQ->Checked,
        cmdline_str,
        BUF_SIZE);

    char env_str[BUF_SIZE];
    ExpandEnvStr(dlg->ECommandEnv->Lines, env_str, BUF_SIZE);

	SECURITY_DESCRIPTOR sd;
	SECURITY_ATTRIBUTES sa;
    InitSecurityAtt(&sd, &sa);

    HANDLE StdErrHandle;
    StdErrHandle = (dlg->CBStdErrOut->Checked) ?
		CreateFile(fpStdErrFile,
        	GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			&sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)
        : NULL;
	if(StdErrHandle == INVALID_HANDLE_VALUE) StdErrHandle = NULL;

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW | (dlg->CBStdErrOut->Checked ? STARTF_USESTDHANDLES : 0);
	si.wShowWindow = SW_SHOW;//SW_HIDE;
	si.hStdOutput = StdErrHandle;
	si.hStdError = StdErrHandle;

    Application->ProcessMessages();

	AnsiString CurrentDir_buf = GetCurrentDir(); //�J�����g�f�B���N�g���̎擾
	//�R���p�C���̓���ł͕W���Ń\�[�X�f�B���N�g���ɃI�u�W�F�N�g�𐶐�����B
	if(!SetCurrentDir(ExtractFileDir(dlg->EfpTargetFile->Text)))
		SetCurrentDir(CurrentDir_buf); //���s����΃f�B���N�g�������ɖ߂��B

	bool ProcessResult =
	CreateProcess(
		NULL,
		cmdline_str,
		&sa,
		&sa,
		dlg->CBStdErrOut->Checked,
		NORMAL_PRIORITY_CLASS,
		(strlen(env_str) ? env_str: 0),
		0,
		&si,
		&pi);
	if(!ProcessResult) {
    	char str_buf[2048];
        sprintf(str_buf,"�ŏI�I�ȃR�}���h�F%s\n�͎��s�t�@�C�������݂��Ȃ����A���s�̍ۂɃG���[���������܂����B",cmdline_str);
        MessageBox(Handle,str_buf,"���s���s",MB_OK | MB_ICONERROR);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		CloseHandle(StdErrHandle);
	    SetCurrentDir(CurrentDir_buf);
        delete dlg;
        return false;
    }
	dlg->CBStdErrOut->Checked ? WaitForSingleObject(pi.hProcess,INFINITE) : false;
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(StdErrHandle);
	SetCurrentDir(CurrentDir_buf); //�J�����g�f�B���N�g���̕��A�B

	delete dlg;
	return true;
}

//
/////////////////////////////////////////////////////////////////////////////
//
// �ȉ��A�C�x���g�n���h��
//

void __fastcall TCommandExeDialog::MCMLFCClick(TObject *Sender)
{
	ECommandLine->SelText = "%lfc";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMSFCClick(TObject *Sender)
{
	ECommandLine->SelText = "%sfc";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMLCClick(TObject *Sender)
{
	ECommandLine->SelText = "%lc";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMSCClick(TObject *Sender)
{
	ECommandLine->SelText = "%sc";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMLFTClick(TObject *Sender)
{
	ECommandLine->SelText = "%lft";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMSFTClick(TObject *Sender)
{
	ECommandLine->SelText = "%sft";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMLTClick(TObject *Sender)
{
	ECommandLine->SelText = "%lt";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMSTClick(TObject *Sender)
{
	ECommandLine->SelText = "%st";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMLDTClick(TObject *Sender)
{
	ECommandLine->SelText = "%ldt";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMLNTClick(TObject *Sender)
{
	ECommandLine->SelText = "%lnt";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::MCMLOTClick(TObject *Sender)
{
	ECommandLine->SelText = "%lot";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCEPathClick(TObject *Sender)
{
	ECommandEnv->SelText = "PATH=";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCEWindirClick(TObject *Sender)
{
	ECommandEnv->SelText = "%WINDIR%";
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCELDirClick(TObject *Sender)
{
	ECommandEnv->SelText = ExtractFileDir(EfpExecFile->Text);
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCESDirClick(TObject *Sender)
{
  	ECommandEnv->SelText = ExtractFileDir(ExtractShortPathName(EfpExecFile->Text));
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCEBracketdqClick(TObject *Sender)
{
	ECommandEnv->SelText = AnsiString("\"") + ECommandEnv->SelText + AnsiString("\"");
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCECutClick(TObject *Sender)
{
	ECommandEnv->CutToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCECopyClick(TObject *Sender)
{
	ECommandEnv->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCEPasteClick(TObject *Sender)
{
	ECommandEnv->PasteFromClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCETemplateClick(TObject *Sender)
{
	ECommandEnv->SelText = AnsiString("PATH=\"%WINDIR%\";\"")
    	 + ExtractFileDir(EfpExecFile->Text) + AnsiString("\"");
}
//---------------------------------------------------------------------------

void __fastcall TCommandExeDialog::PMCESTemplateClick(TObject *Sender)
{
	ECommandEnv->SelText = AnsiString("PATH=\"%WINDIR%\";\"")
    	 + ExtractFileDir(ExtractShortPathName(EfpExecFile->Text)) + AnsiString("\"");
}
//---------------------------------------------------------------------------

