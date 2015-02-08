//---------------------------------------------------------------------------
#ifndef ceta_config_dlgH
#define ceta_config_dlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

#include "ceta_config.h"
#include "ver_info.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "CGRID.h"
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include "SHBFFEX.hpp"
#include "CSPIN.h"

typedef enum { TS_CI, TS_COMMAND, TS_HELP, TS_OTHERS } SelectedTabPageId;

class TCetaConfigDialog : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPageControl *PageControl1;
	THeaderControl *HeaderMenu;
	TTabSheet *TSCommand;
	TTabSheet *TSHelp;
	TTabSheet *TSOthers;
	TTabSheet *TSCI;
	TListBox *LBCommand;
	THeaderControl *HMCommand;
	TLabel *Label1;
	TEdit *ECommandName;
	TEdit *ECommandFile;
	TLabel *Label2;
	TLabel *Label3;
	TMemo *ECommandEnv;
	TCheckBox *ECommandErr;
	TBevel *Bevel1;
	TLabel *Label4;
	TEdit *ECommandLine;
	TPopupMenu *PMFileRefer;
	TMenuItem *PMFileReferOpen;
	THeaderControl *HMHelp;
	TListBox *LBHelp;
	TListBox *LBCI;
	THeaderControl *HMCI;
	TBevel *Bevel2;
	TBevel *Bevel4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *EfpCIFile;
	TLabel *Label8;
	TEdit *ECIName;
	TLabel *Label9;
	TEdit *ECIExtensions;
	TBevel *Bevel7;
	TGroupBox *GBXTemplateDir;
	TLabel *EOTemplateDir;
	TButton *BTNTemplateDir;
	TGroupBox *GBXVersionInfo;
	TMemo *EVersionInfo;
	TImage *Image1;
	TOpenDialog *OpenDialog1;
	THeaderControl *HMCIBackward;
	THeaderControl *HMCIForward;
	THeaderControl *HMCommandBackward;
	THeaderControl *HMCommandForward;
	THeaderControl *HMHelpBackward;
	THeaderControl *HMHelpForward;
	TSHBrowseForFolderEx *SHBrowseForFolderEx1;
	TMemo *Memo2;
	TPopupMenu *PMCommandMacro;
	TMenuItem *MCMLFC;
	TMenuItem *MCMSFC;
	TMenuItem *MCMLC;
	TMenuItem *MCMSC;
	TMenuItem *N1;
	TMenuItem *MCMLFT;
	TMenuItem *MCMSFT;
	TMenuItem *MCMLT;
	TMenuItem *MCMST;
	TCheckBox *EBracketDQ;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *MCMLDT;
	TMenuItem *N4;
	TMenuItem *MCMLNT;
	TGroupBox *GroupBox1;
	TLabel *Label10;
	TButton *BTNClearRecentFiles;
	TCSpinEdit *ERFLMax;
	TPopupMenu *PMCommandEnv;
	TMenuItem *PMCEWindir;
	TMenuItem *PMCELDir;
	TMenuItem *PMCESDir;
	TMenuItem *PMCEPath;
	TMenuItem *PMCEBracketdq;
	TMenuItem *N8;
	TMenuItem *PMCECut;
	TMenuItem *PMCECopy;
	TMenuItem *PMCEPaste;
	TLabel *Label11;
	TEdit *EHelpName;
	TEdit *EHelpFile;
	TMenuItem *PMCETemplate;
	TMenuItem *PMCESTemplate;
	TMenuItem *MCMLOT;
	void __fastcall HeaderMenuSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall PMFileReferOpenClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall HeaderMenuSectionTrack(THeaderControl *HeaderControl,
          THeaderSection *Section, int Width, TSectionTrackState State);
	void __fastcall HeaderMenuSectionResize(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall HMHelpSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall LBHelpClick(TObject *Sender);
	void __fastcall HMHelpBackwardSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall HMHelpForwardSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall LBCommandClick(TObject *Sender);
	void __fastcall HMCommandSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall HMCommandBackwardSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall HMCommandForwardSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall LBCIClick(TObject *Sender);
	void __fastcall HMCISectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall HMCIBackwardSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall HMCIForwardSectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall BTNTemplateDirClick(TObject *Sender);
	void __fastcall MCMLFCClick(TObject *Sender);
	void __fastcall MCMSFCClick(TObject *Sender);
	void __fastcall MCMLCClick(TObject *Sender);
	void __fastcall MCMSCClick(TObject *Sender);
	void __fastcall MCMLFTClick(TObject *Sender);
	void __fastcall MCMSFTClick(TObject *Sender);
	void __fastcall MCMLTClick(TObject *Sender);
	void __fastcall MCMSTClick(TObject *Sender);
	void __fastcall MCMLDTClick(TObject *Sender);
	void __fastcall MCMLNTClick(TObject *Sender);
	void __fastcall BTNClearRecentFilesClick(TObject *Sender);
	void __fastcall ERFLMaxChange(TObject *Sender);
	void __fastcall PMCEPathClick(TObject *Sender);
	void __fastcall PMCEWindirClick(TObject *Sender);
	void __fastcall PMCELDirClick(TObject *Sender);
	void __fastcall PMCESDirClick(TObject *Sender);
	void __fastcall PMCEBracketdqClick(TObject *Sender);
	void __fastcall PMCECutClick(TObject *Sender);
	void __fastcall PMCECopyClick(TObject *Sender);
	void __fastcall PMCEPasteClick(TObject *Sender);
	void __fastcall PMCETemplateClick(TObject *Sender);
	void __fastcall PMCESTemplateClick(TObject *Sender);
	void __fastcall MCMLOTClick(TObject *Sender);
private:	// ユーザー宣言

	TCetaConfig *Config;
	SelectedTabPageId tpId;
    AnsiString _FileNameBuffer;
    void BuildTSCI();
    void BuildTSCommand();
    void BuildTSHelp();
    void BuildTSOthers();
    void UpdateLBCommand();
    void AppendCommandSet();
    void UpdateCommandSet();
    void DeleteCommandSet();
    void HelpCommandSet();
    void UpdateLBHelp();
    void AppendHelpSet();
    void UpdateHelpSet();
    void DeleteHelpSet();
    void HelpHelpSet();
    void ReadVersionInfo();
    void UpdateLBCI();
    void AppendCIRegistry();
    void UpdateCIRegistry();
    void DeleteCIRegistry();
    void HelpCIRegistry();
    void CDOkSelected();
    void CDApplySelected();
    void CDCancelSelected();
    void CDHelpSelected();

public:		// ユーザー宣言
	__fastcall TCetaConfigDialog(TComponent* Owner, AnsiString CmdLineBuffer,
    	AnsiString fpExeFileBuffer, AnsiString fpIniFileBuffer);

};
//---------------------------------------------------------------------------
extern PACKAGE TCetaConfigDialog *CetaConfigDialog;
//---------------------------------------------------------------------------
#endif
