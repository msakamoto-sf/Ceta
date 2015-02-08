//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ceta_main_window.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TCetaMainWindow *CetaMainWindow;
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// Constructor Implementation Section
//

__fastcall TCetaMainWindow::TCetaMainWindow(TComponent* Owner)
	: TForm(Owner)
{
	FirstActive = true; // OnShow 時の初期化に用いるフラグ。
}
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// Private Funciton Implementation Section
//

void TCetaMainWindow::FileSelect(AnsiString fname, AnsiString t_fname)
{
    TCIRegistry *buf;
	buf = CISelect(fname, t_fname);
    if(buf == NULL) return;

    AnaMDIChild = new TAnaMDIChild(this, fname, buf->GetfpCIFile(), t_fname);
    AnaMDIChild->FileLoad();
	RegisterRecent(fname);
	return;
}

TCIRegistry* TCetaMainWindow::CISelect(AnsiString fname, AnsiString t_fname)
{
	AnsiString s_fname = (t_fname == "") ? fname : t_fname;

	TCISelectDlg *dlg = NULL;
    dlg = new TCISelectDlg(this, s_fname, &(Config->CIRegistryList));
	// 万が一、何もCI設定がされていなかったときの予防線。
    // テスト中は、実際あり得る。
    if(dlg == NULL) return NULL;

	TModalResult result = dlg->ShowModal();
    int i = dlg->ListBox1->ItemIndex;
    delete dlg;
	if(mrOk != result) return NULL;

	return Config->CIRegistryList.Index(i);
}

void TCetaMainWindow::RegisterRecent(AnsiString fname)
{
	//ファイル履歴に登録
	Config->RFList->RegistryRecent(fname);
}

void TCetaMainWindow::ClearAnyMenu(TMenuItem * miBuffer, int offset)
{
	TMenuItem *buf;
	while(miBuffer->Count - offset) {
    	buf = miBuffer->Items[0];
        miBuffer->Delete(0);
        delete buf;
    }
}
/////////////////////////////////////////////////////////////////////////////
//
// Window Handlers
//

void __fastcall TCetaMainWindow::AFileOpenExecute(TObject *Sender)
{
	OpenDialog1->Title = "ファイルを開く";
	OpenDialog1->Options << ofAllowMultiSelect;

	if(OpenDialog1->Execute())
		for(int I = 0; I < OpenDialog1->Files->Count;I ++)
		    FileSelect(OpenDialog1->Files->Strings[I]);

	OpenDialog1->Options >> ofAllowMultiSelect;
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AFileNewExecute(TObject *Sender)
{
	OpenDialog1->Title = "新規作成するファイル名の入力";
	if(OpenDialog1->Execute()) FileSelect(OpenDialog1->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AFileSaveExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->FileSave();
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AFileSaveAsExecute(TObject *Sender)
{
	if(ActiveMDIChild) {
    	if(SaveDialog1->Execute())
        	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->
            	FileSave(SaveDialog1->FileName);
    }
}
//---------------------------------------------------------------------------
void __fastcall TCetaMainWindow::AFileCloseExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->Close();
}
//---------------------------------------------------------------------------
void __fastcall TCetaMainWindow::AFileReloadExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->FileReload();
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AFilePrintExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->FilePrint();
}
//---------------------------------------------------------------------------
void __fastcall TCetaMainWindow::ASearchFindExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->SearchFind();
}
//---------------------------------------------------------------------------
void __fastcall TCetaMainWindow::ASearchReplaceExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->SearchReplace();

}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AEditUndoExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->EditUndo();
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AEditRedoExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->EditRedo();
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AEditCutExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->EditCut();
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AEditCopyExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->EditCopy();
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AEditPasteExecute(TObject *Sender)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->EditPaste();
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::Splitter1CanResize(TObject *Sender,
      int &NewSize, bool &Accept)
{ if(NewSize <= 30) Accept = false; }
void __fastcall TCetaMainWindow::Splitter1Moved(TObject *Sender)
{ Splitter1->Top = MMError->Top; }
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AWindowErrorMsgExecute(TObject *Sender)
{
	if(AWindowErrorMsg->Checked) {
		MMError->Visible = false;
	    Splitter1->Visible = false;
        AWindowErrorMsg->Checked = false;
	} else {
		MMError->Visible = true;
    	Splitter1->Visible = true;
        AWindowErrorMsg->Checked = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::MOptionCetaConfigClick(TObject *Sender)
{
	Config->SaveConfig();

	TCetaConfigDialog *dlg = new TCetaConfigDialog(this,
		Config->GetCmdLine(),
        Config->GetfpAppFile(),
        Config->GetfpIniFile());
	TModalResult result = dlg->ShowModal();
    delete dlg;

	if(mrOk != result) return;


    Config->LoadDirectoryStatus();
    Config->LoadErrorStatus();
    Config->LoadCHConfig();
	Config->LoadCIRegistryList();
    Config->LoadRFList();

    ReBuildHelp();
    ReBuildCommand();
	ReBuildCIRegistry();
}

//---------------------------------------------------------------------------
void __fastcall TCetaMainWindow::RFListClick(TObject *Sender)
{
	AnsiString buf = dynamic_cast<TMenuItem *>(Sender)->Caption;
	FileSelect(buf);
    return;
}

void TCetaMainWindow::ReBuildRFList()
{
	ClearAnyMenu(MRFList);
    BuildRFList();
}

void TCetaMainWindow::BuildRFList()
{
	TMenuItem *new_item;
	AnsiString buf;

	for(int i = 0;i < Config->RFList->Count() ;i++) {
		new_item = new TMenuItem(MCommand);
		new_item->OnClick = RFListClick;
	    buf = Config->RFList->Index(i);
		new_item->Caption = buf;
		new_item->Name = "MRFList" + IntToStr(i + 1);
        new_item->ImageIndex = 1;
	    MRFList->Insert(i,new_item);
	}
	return;
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::CommandClick(TObject *Sender)
{
	int CommandIndex = dynamic_cast<TMenuItem *>(Sender)->Tag;
    int IsStdErrOut = (Config->CommandSetList.Index(CommandIndex))->GetCommandErr();
	AnsiString fpStdErrFile = Config->GetAppDir() + "\\" + szStdErrFileName;
    AnsiString fpTargetFile =
    	(ActiveMDIChild) ?
        dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->GetFileName()
        : AnsiString("");

	if(Config->CH_CommandExec(
    	CommandIndex,
        fpTargetFile,
		fpStdErrFile,
        Handle))
    	if(IsStdErrOut) MMError->Lines->LoadFromFile(fpStdErrFile);
    return;
}

void TCetaMainWindow::ReBuildCommand()
{
	ClearAnyMenu(MCommand);
    BuildCommand();
}

void TCetaMainWindow::BuildCommand()
{
	//既にkmi->load_Commandsetが終わっている状態を仮定している。
	TMenuItem *new_item;
	CommandSet *buf;

	for(int i = 0;i < Config->CommandSetList.Count() ;i++) {
		new_item = new TMenuItem(MCommand);
		new_item->OnClick = CommandClick;
	    buf = Config->CommandSetList.Index(i); //Commandsetの取り出し
		new_item->Caption = buf->GetCommandName();
		new_item->Name = "MCommand" + IntToStr(i + 1);
	    new_item->Tag = i;
        new_item->ImageIndex = 12;
	    MCommand->Insert(i,new_item);
	}
	return;
}

//---------------------------------------------------------------------------
void __fastcall TCetaMainWindow::HelpClick(TObject *Sender)
{
	int HelpIndex = dynamic_cast<TMenuItem *>(Sender)->Tag;

    AnsiString buf =  (Config->HelpSetList.Index(HelpIndex))->GetHelpFile();
    if(!FileExists(buf)) {
    	char str_buf[1024];
        sprintf(str_buf, "指定されたヘルプファイル\n%s\nが存在しません。", buf.c_str());
    	MessageBox(Handle, str_buf, "エラー", MB_OK | MB_ICONERROR);
        return;
    }
    Config->CH_HelpExec(HelpIndex, Handle);
	return;

}

void TCetaMainWindow::ReBuildHelp()
{
	ClearAnyMenu(MHelp, 2);
    BuildHelp();
}

void TCetaMainWindow::BuildHelp()
{
	//既にkmi->load_helpsetが終わっている状態を仮定している。
	TMenuItem *new_item;
	HelpSet *buf;

	for(int i = 0;i < Config->HelpSetList.Count() ;i++) {
		new_item = new TMenuItem(MHelp);
		new_item->OnClick = HelpClick;
	    buf = Config->HelpSetList.Index(i); //helpsetの取り出し
		new_item->Caption = buf->GetHelpName();
		new_item->Name = "MHelp" + IntToStr(i + 1);
	    new_item->Tag = i;
        new_item->ImageIndex = 19;
	    MHelp->Insert(i,new_item);
	}
	return;
}

//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::CIRegistryClick(TObject *Sender)
{
	int CIRegistryIndex = dynamic_cast<TMenuItem *>(Sender)->Tag;

    TEditorExSetDialog *dlg = new TEditorExSetDialog(
    	this, (Config->CIRegistryList.Index(CIRegistryIndex))->GetfpCIFile());
	if(dlg->ShowModal() == mrOk) {
    	if(ActiveMDIChild)
        	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->UpdateStatus();
    }
    delete dlg;
}

void TCetaMainWindow::ReBuildCIRegistry()
{
	ClearAnyMenu(MCIRegistry);
    BuildCIRegistry();
}

void TCetaMainWindow::BuildCIRegistry()
{
	//既にkmi->load_CIRegistrysetが終わっている状態を仮定している。
	TMenuItem *new_item;
	TCIRegistry *buf;
	OpenDialog1->Filter = "";
    SaveDialog1->Filter = "";

	for(int i = 0;i < Config->CIRegistryList.Count() ;i++) {
		new_item = new TMenuItem(MCIRegistry);
		new_item->OnClick = CIRegistryClick;
	    buf = Config->CIRegistryList.Index(i); //CIRegistryの取り出し
		new_item->Caption = buf->GetCIName();
		new_item->Name = "MCIRegistry" + IntToStr(i + 1);
	    new_item->Tag = i;

		OpenDialog1->Filter += buf->GetCIName() + "(" + buf->GetCIExtensions() + ")|"
        	+ buf->GetCIExtensions() + "|";
		SaveDialog1->Filter += buf->GetCIName() + "(" + buf->GetCIExtensions() + ")|"
        	+ buf->GetCIExtensions() + "|";

	    MCIRegistry->Insert(i,new_item);
	}
    OpenDialog1->Filter += "全て(*.*)|*.*";
    SaveDialog1->Filter += "全て(*.*)|*.*";
	return;
}

//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::MHelpAboutClick(TObject *Sender)
{ Application->HelpCommand(HELP_FINDER,0); }

//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::FormCreate(TObject *Sender)
{
	Application->OnIdle = AppIdle;
	Application->ShowHint = true;//ヒントボックスを表示。
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Config->SaveConfig();
    ClearAnyMenu(MFileTemplate);
	ClearAnyMenu(MCommand);
    ClearAnyMenu(MHelp, 2);
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AppIdle(TObject *Sender,bool &Done)
{
	if(ActiveMDIChild)
    	dynamic_cast<TAnaMDIChild*>(ActiveMDIChild)->IdlingHandler();
    UpdateMainWindowInternalStatus();
	Done = true; //false;
}

//---------------------------------------------------------------------------


void __fastcall TCetaMainWindow::MFileQuitClick(TObject *Sender)
{
//	Config->SaveConfig();
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::FormShow(TObject *Sender)
{
	if(!FirstActive) return;
    LoadMainWindowInternalStatus();
	BuildCommand();
	BuildHelp();
	BuildCIRegistry();
	FirstFileLoad();
    FirstActive = false;
}
//---------------------------------------------------------------------------

void TCetaMainWindow::UpdateMainWindowInternalStatus()
{
    Config->SetErrorOutput(AWindowErrorMsg->Checked);
    Config->SetWLeft(this->Left);
    Config->SetWTop(this->Top);
    Config->SetWWidth(this->Width);
	Config->SetWHeight(this->Height);
    ClearAnyMenu(MFileTemplate);
	BuildTemplateMenu(MFileTemplate, Config->GetTemplateDir());
    ClearAnyMenu(MRFList);
    BuildRFList();
}

void TCetaMainWindow::LoadMainWindowInternalStatus()
{
    AWindowErrorMsg->Checked = Config->GetErrorOutput();
    MMError->Visible = AWindowErrorMsg->Checked;
    this->Left = Config->GetWLeft();
    this->Top = Config->GetWTop();
    this->Width = Config->GetWWidth();
    this->Height = Config->GetWHeight();
}

/////////////////////////////////////////////////////////////////////////////
//
// CreateWindowExに渡すパラメータ作成のオーバーライド
//

void __fastcall TCetaMainWindow::CreateParams(TCreateParams &Params)
{
	TForm::CreateParams(Params); //基本クラスのCreateParams
    Params.ExStyle |= WS_EX_ACCEPTFILES; //WM_DROPFILESを有効にする
}

/////////////////////////////////////////////////////////////////////////////
//
// WM_DROPFILES応答部分(エクスプローラからのドラップ)
//

void __fastcall TCetaMainWindow::WMDropFiles(TWMDropFiles &Message)
{
	char fn_buf[MAX_PATH];
    int count;
	count = DragQueryFile((void *)Message.Drop, DWORD(-1), fn_buf, sizeof(fn_buf));

	AnsiString fullpathname_buf;
    for(int i = 0; i < count;i++) {
		DragQueryFile((void *)Message.Drop, i,fn_buf, sizeof(fn_buf));
		/* ParamStr(),ひいてはpl_bufで渡されてくるのは8.3形式なので、
		FindFirstしてロングファイル名を取得する必要がある。*/
		TSearchRec sr;
		int iAttributes = faAnyFile;
		fullpathname_buf = ExtractFilePath(fn_buf); //パス用バッファ保存。
		if(FindFirst(fn_buf, iAttributes, sr) == 0)
			fullpathname_buf += AnsiString(sr.FindData.cFileName);
		else
			fullpathname_buf = AnsiString(fn_buf);
        FindClose(sr);
		FileSelect(fullpathname_buf);
    }
    DragFinish((void *)Message.Drop); //ドラップハンドルの解放を忘れずに！
}

/////////////////////////////////////////////////////////////////////////////
//
// WM_COPYDATA応答部分(ショートカットへのドラップ)
//

void __fastcall TCetaMainWindow::WMCopyData(TWMCopyData &msg)
{
	//文字列の取得（バッファオーバーフローに気をつけて！）
	char *str_pl_buf = (char *)GlobalAlloc(GPTR, msg.CopyDataStruct->cbData);
	strncpy(str_pl_buf, (char *)msg.CopyDataStruct->lpData, msg.CopyDataStruct->cbData);

	//TStringList::SetText()でさくっとStringList型に直しちゃう。
	TStringList *pl_buf;
	pl_buf = new TStringList;
	pl_buf->SetText(str_pl_buf);

	if(msg.CopyDataStruct->dwData != SIGNATURE_APP_COPYDATA) return;

	AnsiString fullpathname_buf; //ロングファイル名用フルパスバッファ
	for(int i = 0; i < pl_buf->Count; i++) {
		/* ParamStr(),ひいてはpl_bufで渡されてくるのは8.3形式なので、
		FindFirstしてロングファイル名を取得する必要がある。*/
		TSearchRec sr;
		int iAttributes = faAnyFile;
		fullpathname_buf = ExtractFilePath(pl_buf->Strings[i]); //パス用バッファ保存。
		if(FindFirst(pl_buf->Strings[i], iAttributes, sr) == 0)
			fullpathname_buf += AnsiString(sr.FindData.cFileName);
		else
			fullpathname_buf = pl_buf->Strings[i];
        FindClose(sr);
		FileSelect(fullpathname_buf);
    }

	delete pl_buf;
	GlobalFree(str_pl_buf);
    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// 初回起動時、パラメータに渡されてきたファイル名をローディング。
//

void __fastcall TCetaMainWindow::FirstFileLoad()
{
	int pc = ParamCount();
	AnsiString fullpathname_buf; //ロングファイル名用フルパスバッファ
	for(int i = 1; i <= pc ;i++) {
		/* ParamStr()で渡されてくるのは8.3形式なので、
		FindFirstしてロングファイル名を取得する必要がある。*/
		TSearchRec sr;
		int iAttributes = faAnyFile;
        fullpathname_buf = ExtractFilePath(ParamStr(i)); //パス用バッファ保存。
		if(FindFirst(ParamStr(i), iAttributes, sr) == 0)
        	fullpathname_buf += AnsiString(sr.FindData.cFileName);
        else
        	fullpathname_buf = ParamStr(i);
		FindClose(sr);
		FileSelect(fullpathname_buf);
    }
	return;
}

void TCetaMainWindow::BuildTemplateMenu(TMenuItem *tm, AnsiString TemplateDir)
{
	WIN32_FIND_DATA FindFileData;

    AnsiString FileNameW = TemplateDir + "\\*.*";
    HANDLE handle = FindFirstFile(FileNameW.c_str(), &FindFileData);
    if(handle == INVALID_HANDLE_VALUE) return;

	TMenuItem *new_item;
	int i = 0;
    while(FindNextFile(handle, &FindFileData)) {
    	if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) continue;
		new_item = new TMenuItem(tm);
        new_item->Caption = FindFileData.cFileName;
        new_item->Name = "MTemplate" + IntToStr(i+1);
        new_item->ImageIndex = 0;
        new_item->OnClick = MTemplateClick;
        tm->Insert(i, new_item);
        i++;
    }
    if(!i) tm->Enabled = false;
    FindClose(handle);
}

void __fastcall TCetaMainWindow::MTemplateClick(TObject *Sender)
{
	AnsiString t_fname =
    	Config->GetTemplateDir() + "\\" + dynamic_cast<TMenuItem*>(Sender)->Caption;
    OpenDialog1->Title = "テンプレートを使用して作成する新規ファイル名の入力";
    if(OpenDialog1->Execute()) FileSelect(OpenDialog1->FileName, t_fname);
    return;
}
//---------------------------------------------------------------------------


