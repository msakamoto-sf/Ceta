//---------------------------------------------------------------------------
#ifndef Ceta_main_windowH
#define Ceta_main_windowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <StdActns.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <SysUtils.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#include "ceta_config_dlg.h"
#include "ci_select_dlg.h"
#include <ExtCtrls.hpp>
//WM_COPYDATA感知定数
#define SIGNATURE_APP_COPYDATA 1000
#define szIniFileName "ceta.ini"
#define szStdErrFileName "cmderr.txt"

#include "mdichild\ana_mdichild.h"
#include "mdichild\editorex_set_dlg.h"

class TCetaMainWindow : public TForm
{
__published:	// IDE 管理のコンポーネント
	TMainMenu *MainMenu1;
	TMenuItem *MFile;
	TMenuItem *MFileNew;
	TMenuItem *MFileOpen;
	TMenuItem *MFileTemplate;
	TMenuItem *MFileReload;
	TMenuItem *MFileSave;
	TMenuItem *MFileSaveAs;
	TMenuItem *MFileClose;
	TMenuItem *MRFList;
	TMenuItem *MFileS1;
	TMenuItem *MFilePrint;
	TMenuItem *MFileS2;
	TMenuItem *MFileQuit;
	TMenuItem *MCommand;
	TMenuItem *MOption;
	TMenuItem *MOptionCetaConfig;
	TMenuItem *MWindow;
	TMenuItem *MWindowHorizontal;
	TMenuItem *MWindowVertical;
	TMenuItem *MWindowCascade;
	TMenuItem *MWindowErrorMsg;
	TMenuItem *MHelp;
	TMenuItem *MHelpS1;
	TMenuItem *MHelpAbout;
	TActionList *ActionList1;
	TAction *AFileNew;
	TAction *AFileOpen;
	TAction *AFileReload;
	TAction *AFileSave;
	TAction *AFileSaveAs;
	TAction *AFileClose;
	TAction *AFilePrint;
	TWindowCascade *AWindowCascade;
	TWindowTileHorizontal *AWindowHorizontal;
	TWindowTileVertical *AWindowVertical;
	TAction *AWindowErrorMsg;
	TToolBar *ToolBar1;
	TImageList *ImageList1;
	TToolButton *TBFileNew;
	TToolButton *TBFileOpen;
	TToolButton *TBFileSave;
	TToolButton *TBFileReload;
	TToolButton *ToolButton5;
	TAction *AEditUndo;
	TAction *AEditRedo;
	TAction *AEditCut;
	TAction *AEditCopy;
	TAction *AEditPaste;
	TAction *ASearchFind;
	TAction *ASearchReplace;
	TToolButton *TBEditUndo;
	TToolButton *TBEditRedo;
	TToolButton *TBEditCut;
	TToolButton *TBEditCopy;
	TToolButton *TBEditPaste;
	TToolButton *ToolButton11;
	TToolButton *TBSearchFind;
	TToolButton *TBSearchReplace;
	TToolButton *ToolButton14;
	TToolButton *TBWindowHorizontal;
	TToolButton *TBWindowVertical;
	TToolButton *TBWindowCascade;
	TToolButton *TBWindowErrorMsg;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TMenuItem *MCIRegistry;
	TToolButton *TBFilePrint;
	TMemo *MMError;
	TSplitter *Splitter1;
	void __fastcall AFileOpenExecute(TObject *Sender);
	void __fastcall AFileNewExecute(TObject *Sender);
	void __fastcall MOptionCetaConfigClick(TObject *Sender);
	void __fastcall MFileQuitClick(TObject *Sender);
	void __fastcall MHelpAboutClick(TObject *Sender);


    void __fastcall HelpClick(TObject *Sender);
    void __fastcall CommandClick(TObject *Sender);
    void __fastcall CIRegistryClick(TObject *Sender);
	void __fastcall RFListClick(TObject *Sender);

	void __fastcall AFileSaveExecute(TObject *Sender);
	void __fastcall AFileCloseExecute(TObject *Sender);
	void __fastcall AFileReloadExecute(TObject *Sender);

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall AFileSaveAsExecute(TObject *Sender);
	void __fastcall AFilePrintExecute(TObject *Sender);
	void __fastcall ASearchFindExecute(TObject *Sender);
	void __fastcall ASearchReplaceExecute(TObject *Sender);
	void __fastcall AEditUndoExecute(TObject *Sender);
	void __fastcall AEditRedoExecute(TObject *Sender);
	void __fastcall AEditCutExecute(TObject *Sender);
	void __fastcall AEditCopyExecute(TObject *Sender);
	void __fastcall AEditPasteExecute(TObject *Sender);
	void __fastcall Splitter1CanResize(TObject *Sender, int &NewSize,
          bool &Accept);
	void __fastcall Splitter1Moved(TObject *Sender);
	void __fastcall AWindowErrorMsgExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// ユーザー宣言
	void FileSelect(AnsiString fname, AnsiString t_fname = "");
	TCIRegistry* CISelect(AnsiString fname, AnsiString t_fname = "");
	void RegisterRecent(AnsiString);

    void ClearAnyMenu(TMenuItem * miBuffer, int offset = 0);
	void UpdateMainWindowInternalStatus();
    void LoadMainWindowInternalStatus();
    bool FirstActive;
    void BuildHelp();
    void ReBuildHelp();
    void BuildCommand();
    void ReBuildCommand();
    void BuildCIRegistry();
    void ReBuildCIRegistry();

	void BuildRFList();
	void ReBuildRFList();

    void BuildTemplateMenu(TMenuItem *tm, AnsiString TemplateDir);

public:		// ユーザー宣言
	__fastcall TCetaMainWindow(TComponent* Owner);

	//CreateWindowExパラメータのオーバーライド
	void __fastcall CreateParams(TCreateParams &Params);
	//メッセージハンドラのオーバーライド
	void __fastcall WMDropFiles(TWMDropFiles &Message);
	void __fastcall WMCopyData(TWMCopyData &Message);
BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_COPYDATA,TWMCopyData,WMCopyData)
	MESSAGE_HANDLER(WM_DROPFILES,TWMDropFiles,WMDropFiles);
END_MESSAGE_MAP(TForm)
	void __fastcall FirstFileLoad();

	TCetaConfig *Config;
    void __fastcall AppIdle(TObject *Sender,bool &Done);
    void __fastcall MTemplateClick(TObject *Sender);

};
//---------------------------------------------------------------------------
extern PACKAGE TCetaMainWindow *CetaMainWindow;
//---------------------------------------------------------------------------
#endif
