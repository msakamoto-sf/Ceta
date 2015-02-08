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
	FirstActive = true; // OnShow ���̏������ɗp����t���O�B
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
	// ������A����CI�ݒ肪����Ă��Ȃ������Ƃ��̗\�h���B
    // �e�X�g���́A���ۂ��蓾��B
    if(dlg == NULL) return NULL;

	TModalResult result = dlg->ShowModal();
    int i = dlg->ListBox1->ItemIndex;
    delete dlg;
	if(mrOk != result) return NULL;

	return Config->CIRegistryList.Index(i);
}

void TCetaMainWindow::RegisterRecent(AnsiString fname)
{
	//�t�@�C�������ɓo�^
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
	OpenDialog1->Title = "�t�@�C�����J��";
	OpenDialog1->Options << ofAllowMultiSelect;

	if(OpenDialog1->Execute())
		for(int I = 0; I < OpenDialog1->Files->Count;I ++)
		    FileSelect(OpenDialog1->Files->Strings[I]);

	OpenDialog1->Options >> ofAllowMultiSelect;
}
//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::AFileNewExecute(TObject *Sender)
{
	OpenDialog1->Title = "�V�K�쐬����t�@�C�����̓���";
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
	//����kmi->load_Commandset���I����Ă����Ԃ����肵�Ă���B
	TMenuItem *new_item;
	CommandSet *buf;

	for(int i = 0;i < Config->CommandSetList.Count() ;i++) {
		new_item = new TMenuItem(MCommand);
		new_item->OnClick = CommandClick;
	    buf = Config->CommandSetList.Index(i); //Commandset�̎��o��
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
        sprintf(str_buf, "�w�肳�ꂽ�w���v�t�@�C��\n%s\n�����݂��܂���B", buf.c_str());
    	MessageBox(Handle, str_buf, "�G���[", MB_OK | MB_ICONERROR);
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
	//����kmi->load_helpset���I����Ă����Ԃ����肵�Ă���B
	TMenuItem *new_item;
	HelpSet *buf;

	for(int i = 0;i < Config->HelpSetList.Count() ;i++) {
		new_item = new TMenuItem(MHelp);
		new_item->OnClick = HelpClick;
	    buf = Config->HelpSetList.Index(i); //helpset�̎��o��
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
	//����kmi->load_CIRegistryset���I����Ă����Ԃ����肵�Ă���B
	TMenuItem *new_item;
	TCIRegistry *buf;
	OpenDialog1->Filter = "";
    SaveDialog1->Filter = "";

	for(int i = 0;i < Config->CIRegistryList.Count() ;i++) {
		new_item = new TMenuItem(MCIRegistry);
		new_item->OnClick = CIRegistryClick;
	    buf = Config->CIRegistryList.Index(i); //CIRegistry�̎��o��
		new_item->Caption = buf->GetCIName();
		new_item->Name = "MCIRegistry" + IntToStr(i + 1);
	    new_item->Tag = i;

		OpenDialog1->Filter += buf->GetCIName() + "(" + buf->GetCIExtensions() + ")|"
        	+ buf->GetCIExtensions() + "|";
		SaveDialog1->Filter += buf->GetCIName() + "(" + buf->GetCIExtensions() + ")|"
        	+ buf->GetCIExtensions() + "|";

	    MCIRegistry->Insert(i,new_item);
	}
    OpenDialog1->Filter += "�S��(*.*)|*.*";
    SaveDialog1->Filter += "�S��(*.*)|*.*";
	return;
}

//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::MHelpAboutClick(TObject *Sender)
{ Application->HelpCommand(HELP_FINDER,0); }

//---------------------------------------------------------------------------

void __fastcall TCetaMainWindow::FormCreate(TObject *Sender)
{
	Application->OnIdle = AppIdle;
	Application->ShowHint = true;//�q���g�{�b�N�X��\���B
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
// CreateWindowEx�ɓn���p�����[�^�쐬�̃I�[�o�[���C�h
//

void __fastcall TCetaMainWindow::CreateParams(TCreateParams &Params)
{
	TForm::CreateParams(Params); //��{�N���X��CreateParams
    Params.ExStyle |= WS_EX_ACCEPTFILES; //WM_DROPFILES��L���ɂ���
}

/////////////////////////////////////////////////////////////////////////////
//
// WM_DROPFILES��������(�G�N�X�v���[������̃h���b�v)
//

void __fastcall TCetaMainWindow::WMDropFiles(TWMDropFiles &Message)
{
	char fn_buf[MAX_PATH];
    int count;
	count = DragQueryFile((void *)Message.Drop, DWORD(-1), fn_buf, sizeof(fn_buf));

	AnsiString fullpathname_buf;
    for(int i = 0; i < count;i++) {
		DragQueryFile((void *)Message.Drop, i,fn_buf, sizeof(fn_buf));
		/* ParamStr(),�Ђ��Ă�pl_buf�œn����Ă���̂�8.3�`���Ȃ̂ŁA
		FindFirst���ă����O�t�@�C�������擾����K�v������B*/
		TSearchRec sr;
		int iAttributes = faAnyFile;
		fullpathname_buf = ExtractFilePath(fn_buf); //�p�X�p�o�b�t�@�ۑ��B
		if(FindFirst(fn_buf, iAttributes, sr) == 0)
			fullpathname_buf += AnsiString(sr.FindData.cFileName);
		else
			fullpathname_buf = AnsiString(fn_buf);
        FindClose(sr);
		FileSelect(fullpathname_buf);
    }
    DragFinish((void *)Message.Drop); //�h���b�v�n���h���̉����Y�ꂸ�ɁI
}

/////////////////////////////////////////////////////////////////////////////
//
// WM_COPYDATA��������(�V���[�g�J�b�g�ւ̃h���b�v)
//

void __fastcall TCetaMainWindow::WMCopyData(TWMCopyData &msg)
{
	//������̎擾�i�o�b�t�@�I�[�o�[�t���[�ɋC�����āI�j
	char *str_pl_buf = (char *)GlobalAlloc(GPTR, msg.CopyDataStruct->cbData);
	strncpy(str_pl_buf, (char *)msg.CopyDataStruct->lpData, msg.CopyDataStruct->cbData);

	//TStringList::SetText()�ł�������StringList�^�ɒ������Ⴄ�B
	TStringList *pl_buf;
	pl_buf = new TStringList;
	pl_buf->SetText(str_pl_buf);

	if(msg.CopyDataStruct->dwData != SIGNATURE_APP_COPYDATA) return;

	AnsiString fullpathname_buf; //�����O�t�@�C�����p�t���p�X�o�b�t�@
	for(int i = 0; i < pl_buf->Count; i++) {
		/* ParamStr(),�Ђ��Ă�pl_buf�œn����Ă���̂�8.3�`���Ȃ̂ŁA
		FindFirst���ă����O�t�@�C�������擾����K�v������B*/
		TSearchRec sr;
		int iAttributes = faAnyFile;
		fullpathname_buf = ExtractFilePath(pl_buf->Strings[i]); //�p�X�p�o�b�t�@�ۑ��B
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
// ����N�����A�p�����[�^�ɓn����Ă����t�@�C���������[�f�B���O�B
//

void __fastcall TCetaMainWindow::FirstFileLoad()
{
	int pc = ParamCount();
	AnsiString fullpathname_buf; //�����O�t�@�C�����p�t���p�X�o�b�t�@
	for(int i = 1; i <= pc ;i++) {
		/* ParamStr()�œn����Ă���̂�8.3�`���Ȃ̂ŁA
		FindFirst���ă����O�t�@�C�������擾����K�v������B*/
		TSearchRec sr;
		int iAttributes = faAnyFile;
        fullpathname_buf = ExtractFilePath(ParamStr(i)); //�p�X�p�o�b�t�@�ۑ��B
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
    OpenDialog1->Title = "�e���v���[�g���g�p���č쐬����V�K�t�@�C�����̓���";
    if(OpenDialog1->Execute()) FileSelect(OpenDialog1->FileName, t_fname);
    return;
}
//---------------------------------------------------------------------------


