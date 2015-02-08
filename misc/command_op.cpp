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
	    const char *fpExecFile, // 実行ファイルフルパス名
	    const char *CommandLine, // コマンドライン(マクロ使用可能)
	    const char *fpTargetFile, // 対象ファイル名（一つのみ）
        const char *CommandEnv, // 環境変数(|連結形式）
	    const char *fpStdErrFile, // 結果出力ファイルフルパス名
	    const bool IsStdErrOut, // 結果出力の是非
		const bool IsBracketDQ // ファイル名を""で囲む(Bracket)
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
// プロセス実行補助関数群
//

void ExpandEnvStr(TStrings *EnvList, char *env_buf, int buf_size)
{
	AnsiString buf = ""; //環境設定文字列一時バッファ
	ZeroMemory(env_buf, buf_size);
    char windir[MAX_PATH];
    GetWindowsDirectory(windir, MAX_PATH);

	for(int i = 0;i < EnvList->Count;i++)
		buf += EnvList->Strings[i] + "|";

	//%WINDIR% を実際のWindowsディレクトリに置換
	TReplaceFlags f;
	f.Clear();
	f << rfReplaceAll;
	f << rfIgnoreCase;
	buf = StringReplace(buf, "%WINDIR%", AnsiString(windir), f);

	//こっちで改めてenv_strのC文字列に変換。|を\0に直す。
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
// 外部インターフェイス関数
//
/////////////////////////////////////////////////////////////////////////////
//

void AnaHelpExec(HWND Handle, char *fpHelpFile)
{
	WinHelp(Handle, fpHelpFile, HELP_CONTENTS,0);
}

bool AnaCommandExec(
	const char *fpExecFile, // 実行ファイルフルパス名
    const char *CommandLine, // コマンドライン(マクロ使用可能)
    const char *fpTargetFile, // 対象ファイル名（一つのみ）
    const char *CommandEnv, // 環境変数(|連結形式）
    const char *fpStdErrFile, // 結果出力ファイルフルパス名
    const bool IsStdErrOut, // 結果出力の是非
    const bool IsBracketDQ, // ファイル名を""で囲む(Bracket)
	const HWND Handle // とりあえずの親ウインドウ
)
{
	TCommandExeDialog *dlg = new TCommandExeDialog(Handle,
		fpExecFile, // 実行ファイルフルパス名
	    CommandLine, // コマンドライン(マクロ使用可能)
	    fpTargetFile, // 対象ファイル名（一つのみ）
        CommandEnv, // 環境変数(|連結形式）
	    fpStdErrFile, // 結果出力ファイルフルパス名
	    IsStdErrOut, // 結果出力の是非
        IsBracketDQ // ファイル名を""で囲む(Bracket)
        );
	int result = dlg->ShowModal();
    if(result != mrOk) {
    	delete dlg;
        return false;
    }

    //念のため実行コマンドの空チェック。
	if(dlg->EfpExecFile->Text == "" && dlg->EfpTargetFile->Text == "")
	{
    	MessageBox(Handle,"実行ファイルが指定されていません。","エラー",MB_OK | MB_ICONERROR);
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

	AnsiString CurrentDir_buf = GetCurrentDir(); //カレントディレクトリの取得
	//コンパイラの動作では標準でソースディレクトリにオブジェクトを生成する。
	if(!SetCurrentDir(ExtractFileDir(dlg->EfpTargetFile->Text)))
		SetCurrentDir(CurrentDir_buf); //失敗すればディレクトリを元に戻す。

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
        sprintf(str_buf,"最終的なコマンド：%s\nは実行ファイルが存在しないか、実行の際にエラーが発生しました。",cmdline_str);
        MessageBox(Handle,str_buf,"実行失敗",MB_OK | MB_ICONERROR);
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
	SetCurrentDir(CurrentDir_buf); //カレントディレクトリの復帰。

	delete dlg;
	return true;
}

//
/////////////////////////////////////////////////////////////////////////////
//
// 以下、イベントハンドラ
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

