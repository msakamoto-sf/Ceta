//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ceta_config_dlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGRID"
#pragma link "SHBFFEX"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TCetaConfigDialog *CetaConfigDialog;
//---------------------------------------------------------------------------
__fastcall TCetaConfigDialog::TCetaConfigDialog(TComponent* Owner,
	AnsiString CmdLineBuffer, AnsiString fpExeFileBuffer,
    AnsiString fpIniFileBuffer) : TForm(Owner)
{
	Config = new TCetaConfig(CmdLineBuffer,
    	fpExeFileBuffer, fpIniFileBuffer);
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::FormDestroy(TObject *Sender)
{
	delete Config;
}
//---------------------------------------------------------------------------
void __fastcall TCetaConfigDialog::HeaderMenuSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	if(Section->Text == "OK") CDOkSelected();
	if(Section->Text == "適用") CDApplySelected();
	if(Section->Text == "キャンセル") CDCancelSelected();
	if(Section->Text == "ヘルプ") CDHelpSelected();
}

void TCetaConfigDialog::CDOkSelected()
{
	CDApplySelected();
    this->ModalResult = mrOk;
}

void TCetaConfigDialog::CDApplySelected()
{
	Config->SaveConfig();
}

void TCetaConfigDialog::CDCancelSelected()
{
	this->ModalResult = mrCancel;
}

void TCetaConfigDialog::CDHelpSelected()
{
	Application->HelpCommand(HELP_CONTEXT, 23);
}

//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PageControl1Change(TObject *Sender)
{
switch(dynamic_cast<TPageControl*>(Sender)->ActivePage->Tag) {
	case TS_CI:
    	OpenDialog1->Filter = "文字色設定ファイル(*.ini;*.*)|*.ini;*.ini";
    	OpenDialog1->InitialDir = Config->GetAppDir();
        break;
    case TS_COMMAND:
    	OpenDialog1->Filter = "実行ファイル(*.exe;*.com)|*.exe;*.com";
        OpenDialog1->InitialDir = Config->GetAppDir();
        break;
    case TS_HELP:
    	OpenDialog1->Filter = "ヘルプファイル(*.hlp;*.chm)|*.hlp;*.chm";
    	OpenDialog1->InitialDir = Config->GetAppDir();
        break;
    default:
    break;
}
}
//---------------------------------------------------------------------------
void __fastcall TCetaConfigDialog::PMFileReferOpenClick(TObject *Sender)
{
	if(!OpenDialog1->Execute()) return;
    _FileNameBuffer = OpenDialog1->FileName;
	switch(PageControl1->ActivePage->Tag) {
		case TS_CI:
    		EfpCIFile->Text = _FileNameBuffer;
        	break;
	    case TS_COMMAND:
			ECommandFile->Text = _FileNameBuffer;
        	break;
	    case TS_HELP:
			EHelpFile->Text = _FileNameBuffer;
	    case TS_OTHERS:
    	default:
	    break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCetaConfigDialog::HeaderMenuSectionTrack(
      THeaderControl *HeaderControl, THeaderSection *Section, int Width,
      TSectionTrackState State)
{ Section->Width = HeaderControl->Width / HeaderControl->Sections->Count; }
//---------------------------------------------------------------------------
void __fastcall TCetaConfigDialog::HeaderMenuSectionResize(
      THeaderControl *HeaderControl, THeaderSection *Section)
{ Section->Width = HeaderControl->Width / HeaderControl->Sections->Count; }
//---------------------------------------------------------------------------
void __fastcall TCetaConfigDialog::FormShow(TObject *Sender)
{
	Config->LoadConfig();
    BuildTSCI();
    BuildTSCommand();
    BuildTSHelp();
    BuildTSOthers();
}
//---------------------------------------------------------------------------
void TCetaConfigDialog::BuildTSCI()
{
	UpdateLBCI();
}

void TCetaConfigDialog::BuildTSCommand()
{
	UpdateLBCommand();
}

void TCetaConfigDialog::BuildTSHelp()
{
	UpdateLBHelp();
}

void TCetaConfigDialog::BuildTSOthers()
{
	EOTemplateDir->Caption = Config->GetTemplateDir();
    ERFLMax->Value = Config->RFList->GetMax();
	ReadVersionInfo();
}
//---------------------------------------------------------------------------

void TCetaConfigDialog::UpdateLBCI()
{
	LBCI->Items->Clear();
	TCIRegistry *buf;
	for(int i = 0; i < Config->CIRegistryList.Count(); i++) {
    	buf = Config->CIRegistryList.Index(i);
		LBCI->Items->Add(buf->GetCIName() + " ( "
        	+ ExtractFileName(buf->GetfpCIFile()) + " ) ");
	}
	return;
}

void __fastcall TCetaConfigDialog::LBCIClick(TObject *Sender)
{
	//既にCIRegistryはローディング済み、とする。
	int i = LBCI->ItemIndex;
	TCIRegistry *buf = Config->CIRegistryList.Index(i);
    EfpCIFile->Text = buf->GetfpCIFile();
    ECIName->Text = buf->GetCIName();
	ECIExtensions->Text = buf->GetCIExtensions();
}

void __fastcall TCetaConfigDialog::HMCISectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	if(Section->Text == "追加") AppendCIRegistry();
	if(Section->Text == "更新") UpdateCIRegistry();
	if(Section->Text == "削除") DeleteCIRegistry();
	if(Section->Text == "ヘルプ") HelpCIRegistry();
}

void TCetaConfigDialog::AppendCIRegistry()
{
    Config->CIRegistryList.Append(new TCIRegistry(
		"新規ファイルタイプ", "scratch.ini", "*.*"));
    UpdateLBCI();
    LBCI->ItemIndex = Config->CIRegistryList.Count() - 1;

    TCIRegistry *buf = Config->CIRegistryList.Index(LBCI->ItemIndex);

    ECIName->Text = buf->GetCIName();
    EfpCIFile->Text = buf->GetfpCIFile();
    ECIExtensions->Text = buf->GetCIExtensions();

	if(!OpenDialog1->Execute()) return;

	buf->SetfpCIFile(OpenDialog1->FileName);

    ECIName->Text = buf->GetCIName();
	EfpCIFile->Text = buf->GetfpCIFile();
    ECIExtensions->Text = buf->GetCIExtensions();
	UpdateCIRegistry();
    ECIName->SetFocus();
	return;
}

void TCetaConfigDialog::UpdateCIRegistry()
{
	int i = LBCI->ItemIndex;
	if(i == -1) return;
	if(ECIName->Text == "" || EfpCIFile->Text == "") {
		MessageBox(Handle,"ファイルタイプ名または文字色設定ファイルが指定されていません。","エラー",MB_OK | MB_ICONERROR);
	    return;
	}
	Config->CIRegistryList.Update(i, new TCIRegistry(
    ECIName->Text, EfpCIFile->Text, ECIExtensions->Text));
	UpdateLBCI();
	LBCI->ItemIndex = i;
}

void TCetaConfigDialog::DeleteCIRegistry()
{
	Config->CIRegistryList.Delete(LBCI->ItemIndex);
	UpdateLBCI();
}

void TCetaConfigDialog::HelpCIRegistry()
{
	Application->HelpCommand(HELP_CONTEXT, 21);
}

void __fastcall TCetaConfigDialog::HMCIBackwardSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	int i = LBCI->ItemIndex;
    if(i == -1 || i  == 0) return;
    Config->CIRegistryList.Backward(i);
	UpdateLBCI();
	LBCI->SetFocus();
    LBCI->ItemIndex = i - 1;
	return;
}

void __fastcall TCetaConfigDialog::HMCIForwardSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	int i = LBCI->ItemIndex;
    if(i == -1 || i  == Config->CIRegistryList.Count()) return;
    Config->CIRegistryList.Forward(i);
	UpdateLBCI();
	LBCI->SetFocus();
    LBCI->ItemIndex = i + 1;
	return;
}

//---------------------------------------------------------------------------

void TCetaConfigDialog::UpdateLBCommand()
{
	LBCommand->Items->Clear();
	CommandSet *buf;
	for(int i = 0; i < Config->CommandSetList.Count(); i++) {
    	buf = Config->CommandSetList.Index(i);
		LBCommand->Items->Add(buf->GetCommandName() + " ( " +
        	ExtractFileName(buf->GetCommandFile()) + " ) ");
	}
	return;
}


void __fastcall TCetaConfigDialog::LBCommandClick(TObject *Sender)
{
	int i = LBCommand->ItemIndex;
	CommandSet *buf = Config->CommandSetList.Index(i);
    ECommandName->Text = buf->GetCommandName();
    ECommandFile->Text = buf->GetCommandFile();
	ECommandLine->Text = buf->GetCommandLine();
    ECommandEnv->Lines->Text = buf->GetCommandEnv();
    ECommandErr->Checked = buf->GetCommandErr();
    EBracketDQ->Checked = buf->GetBracketDQ();
}

void __fastcall TCetaConfigDialog::HMCommandSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	if(Section->Text == "追加") AppendCommandSet();
	if(Section->Text == "更新") UpdateCommandSet();
	if(Section->Text == "削除") DeleteCommandSet();
	if(Section->Text == "ヘルプ") HelpCommandSet();
}

void TCetaConfigDialog::AppendCommandSet()
{
    Config->CommandSetList.Append(new CommandSet("新規コマンド", "scratch.exe", "%lfc %lft", "", 0, 1));
    UpdateLBCommand();
    LBCommand->ItemIndex = Config->CommandSetList.Count() - 1;
	ECommandName->Text = (Config->CommandSetList.Index(LBCommand->ItemIndex))->GetCommandName();
    ECommandFile->Text = (Config->CommandSetList.Index(LBCommand->ItemIndex))->GetCommandFile();
	ECommandLine->Text = (Config->CommandSetList.Index(LBCommand->ItemIndex))->GetCommandLine();
	ECommandEnv->Lines->Text =
    	(Config->CommandSetList.Index(LBCommand->ItemIndex))->GetCommandEnv();
    ECommandErr->Checked =
    	(Config->CommandSetList.Index(LBCommand->ItemIndex))->GetCommandErr();
    EBracketDQ->Checked =
    	(Config->CommandSetList.Index(LBCommand->ItemIndex))->GetBracketDQ();

	if(OpenDialog1->Execute()) {
        (Config->CommandSetList.Index(LBCommand->ItemIndex))->SetCommandFile(OpenDialog1->FileName);
    	ECommandFile->Text = OpenDialog1->FileName;
        UpdateCommandSet();
        ECommandName->SetFocus();
	}
	return;
}


void TCetaConfigDialog::UpdateCommandSet()
{
	int i = LBCommand->ItemIndex;
	if(i == -1) return;
	if(ECommandName->Text == "" || ECommandFile->Text == "") {
		MessageBox(Handle,"登録名または実行ファイル名が指定されていません。","エラー",MB_OK | MB_ICONERROR);
	    return;
	}

	AnsiString EnvBuffer = ECommandEnv->Lines->Strings[0];
	for(int j = 1;j < ECommandEnv->Lines->Count;j++)
    	EnvBuffer += "|" + ECommandEnv->Lines->Strings[j];

	Config->CommandSetList.Update(i, new CommandSet(
    	ECommandName->Text, ECommandFile->Text,
        ECommandLine->Text, EnvBuffer, ECommandErr->Checked, EBracketDQ->Checked));
	UpdateLBCommand();
	LBCommand->ItemIndex = i;
}


void TCetaConfigDialog::DeleteCommandSet()
{
	Config->CommandSetList.Delete(LBCommand->ItemIndex);
	UpdateLBCommand();
}


void TCetaConfigDialog::HelpCommandSet()
{
	Application->HelpCommand(HELP_CONTEXT, 22);
}

void __fastcall TCetaConfigDialog::HMCommandBackwardSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	int i = LBCommand->ItemIndex;
    if(i == -1 || i  == 0) return;
    Config->CommandSetList.Backward(i);
	UpdateLBCommand();
	LBCommand->SetFocus();
    LBCommand->ItemIndex = i - 1;
	return;
}

void __fastcall TCetaConfigDialog::HMCommandForwardSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	int i = LBCommand->ItemIndex;
    if(i == -1 || i  == Config->CommandSetList.Count()) return;
    Config->CommandSetList.Forward(i);
	UpdateLBCommand();
	LBCommand->SetFocus();
    LBCommand->ItemIndex = i + 1;
	return;
}

//---------------------------------------------------------------------------

void TCetaConfigDialog::UpdateLBHelp()
{
	LBHelp->Items->Clear();
	HelpSet *buf;
	for(int i = 0; i < Config->HelpSetList.Count(); i++) {
    	buf = Config->HelpSetList.Index(i);
		LBHelp->Items->Add(buf->GetHelpName() + " ( " +
        	ExtractFileName(buf->GetHelpFile()) + " ) ");
	}
	return;
}

void __fastcall TCetaConfigDialog::LBHelpClick(TObject *Sender)
{
	int i = LBHelp->ItemIndex;
	HelpSet *buf = Config->HelpSetList.Index(i);
    EHelpName->Text = buf->GetHelpName();
    EHelpFile->Text = buf->GetHelpFile();
}

void __fastcall TCetaConfigDialog::HMHelpSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	if(Section->Text == "追加") AppendHelpSet();
	if(Section->Text == "更新") UpdateHelpSet();
	if(Section->Text == "削除") DeleteHelpSet();
	if(Section->Text == "ヘルプ") HelpHelpSet();
}

void TCetaConfigDialog::AppendHelpSet()
{
    Config->HelpSetList.Append(new HelpSet("新規ヘルプ", "scratch.hlp"));
    UpdateLBHelp();
    LBHelp->ItemIndex = Config->HelpSetList.Count() - 1;
	EHelpName->Text = (Config->HelpSetList.Index(LBHelp->ItemIndex))->GetHelpName();
    EHelpFile->Text = (Config->HelpSetList.Index(LBHelp->ItemIndex))->GetHelpFile();

	if(OpenDialog1->Execute()) {
        (Config->HelpSetList.Index(LBHelp->ItemIndex))->SetHelpFile(OpenDialog1->FileName);
    	EHelpFile->Text = OpenDialog1->FileName;
        UpdateHelpSet();
        EHelpName->SetFocus();
	}
	return;
}

void TCetaConfigDialog::UpdateHelpSet()
{
	int i = LBHelp->ItemIndex;
	if(i == -1) return;
	if(EHelpName->Text == "" || EHelpFile->Text == "") {
		MessageBox(Handle,"登録名またはヘルプファイル名が指定されていません。","エラー",MB_OK | MB_ICONERROR);
	    return;
	}
	Config->HelpSetList.Update(i, new HelpSet(EHelpName->Text, EHelpFile->Text));
	UpdateLBHelp();
	LBHelp->ItemIndex = i;
}

void TCetaConfigDialog::DeleteHelpSet()
{
	Config->HelpSetList.Delete(LBHelp->ItemIndex);
	UpdateLBHelp();
}

void TCetaConfigDialog::HelpHelpSet()
{
	Application->HelpCommand(HELP_CONTEXT, 9);
}

void __fastcall TCetaConfigDialog::HMHelpBackwardSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	int i = LBHelp->ItemIndex;
    if(i == -1 || i  == 0) return;
    Config->HelpSetList.Backward(i);
	UpdateLBHelp();
	LBHelp->SetFocus();
    LBHelp->ItemIndex = i - 1;
	return;
}

void __fastcall TCetaConfigDialog::HMHelpForwardSectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
	int i = LBHelp->ItemIndex;
    if(i == -1 || i  == Config->HelpSetList.Count()) return;
    Config->HelpSetList.Forward(i);
	UpdateLBHelp();
	LBHelp->SetFocus();
    LBHelp->ItemIndex = i + 1;
	return;
}

//---------------------------------------------------------------------------

void TCetaConfigDialog::ReadVersionInfo()
{
    TVerInfo vi_buf(Config->GetfpAppFile().c_str());
    EVersionInfo->Lines->Add("");
    EVersionInfo->Lines->Add(vi_buf.GetFileDescription());
    EVersionInfo->Lines->Add(vi_buf.GetProductName() + " " + vi_buf.GetProductVersion());
    EVersionInfo->Lines->Add("");
    EVersionInfo->Lines->Add("作者：" + vi_buf.GetCompanyName());
    EVersionInfo->Lines->Add("ＨＰ：http://homepage2.nifty.com/ssfu/");
    EVersionInfo->Lines->Add("");
    EVersionInfo->Lines->Add(vi_buf.GetLegalCopyright());
    EVersionInfo->Lines->Add("");
	EVersionInfo->Lines->Add("内部名：" + vi_buf.GetInternalName() + " " + vi_buf.GetFileVersion());
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::BTNTemplateDirClick(TObject *Sender)
{
 	SHBrowseForFolderEx1->Caption = "テンプレートディレクトリを指定してください";
	SHBrowseForFolderEx1->SelectFolderName = Config->GetTemplateDir();
	if(SHBrowseForFolderEx1->Execute()) {
    	EOTemplateDir->Caption = SHBrowseForFolderEx1->FolderName;
        Config->SetTemplateDir(EOTemplateDir->Caption);
    }
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMLFCClick(TObject *Sender)
{
	ECommandLine->SelText = "%lfc";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMSFCClick(TObject *Sender)
{
	ECommandLine->SelText = "%sfc";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMLCClick(TObject *Sender)
{
	ECommandLine->SelText = "%lc";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMSCClick(TObject *Sender)
{
	ECommandLine->SelText = "%sc";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMLFTClick(TObject *Sender)
{
	ECommandLine->SelText = "%lft";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMSFTClick(TObject *Sender)
{
	ECommandLine->SelText = "%sft";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMLTClick(TObject *Sender)
{
	ECommandLine->SelText = "%lt";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMSTClick(TObject *Sender)
{
	ECommandLine->SelText = "%st";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMLDTClick(TObject *Sender)
{
	ECommandLine->SelText = "%ldt";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMLNTClick(TObject *Sender)
{
	ECommandLine->SelText = "%lnt";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::MCMLOTClick(TObject *Sender)
{
	ECommandLine->SelText = "%lot";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::BTNClearRecentFilesClick(
      TObject *Sender)
{
	Config->RFList->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::ERFLMaxChange(TObject *Sender)
{
	Config->RFList->SetMax(ERFLMax->Value);
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCEPathClick(TObject *Sender)
{
	ECommandEnv->SelText = "PATH=";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCEWindirClick(TObject *Sender)
{
	ECommandEnv->SelText = "%WINDIR%";
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCELDirClick(TObject *Sender)
{
	ECommandEnv->SelText = ExtractFileDir(ECommandFile->Text);
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCESDirClick(TObject *Sender)
{
	ECommandEnv->SelText = ExtractFileDir(ExtractShortPathName(ECommandFile->Text));
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCEBracketdqClick(TObject *Sender)
{
	ECommandEnv->SelText = AnsiString("\"") + ECommandEnv->SelText + AnsiString("\"");
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCECutClick(TObject *Sender)
{
	ECommandEnv->CutToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCECopyClick(TObject *Sender)
{
	ECommandEnv->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCEPasteClick(TObject *Sender)
{
	ECommandEnv->PasteFromClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCETemplateClick(TObject *Sender)
{
	ECommandEnv->SelText = AnsiString("PATH=\"%WINDIR%\";\"")
    	 + ExtractFileDir(ECommandFile->Text) + AnsiString("\"");
}
//---------------------------------------------------------------------------

void __fastcall TCetaConfigDialog::PMCESTemplateClick(TObject *Sender)
{
	ECommandEnv->SelText = AnsiString("PATH=\"%WINDIR%\";\"")
    	 + ExtractFileDir(ExtractShortPathName(ECommandFile->Text)) + AnsiString("\"");
}
//---------------------------------------------------------------------------
