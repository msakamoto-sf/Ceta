//----------------------------------------------------------------------------
#ifndef command_opH
#define command_opH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------

void AnaHelpExec(HWND Handle, char *fpHelpFile);
bool AnaCommandExec(
	const char *fpExecFile, // 実行ファイルフルパス名
    const char *CommandLine, // コマンドライン(マクロ使用可能)
    const char *fpTargetFile, // 対象ファイル名（一つのみ）
	const char *CommandEnv, // 環境変数(|連結形式）
    const char *fpStdErrFile, // 結果出力ファイルフルパス名
    const bool IsStdErrOut, // 結果出力の是非
    const bool IsBracketDQ, // ファイル名を""で囲む(Bracket)
    const HWND Handle // とりあえずの親ウインドウ
);

class TCommandExeDialog : public TForm
{
__published:
	TPageControl *PageControl1;
	TTabSheet *TSCONormal;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label1;
	TCheckBox *CBStdErrOut;
	TMemo *ECommandEnv;
	TMemo *Memo2;
	TEdit *EfpTargetFile;
	TLabel *Label7;
	TButton *BTNRun;
	TButton *BTNCancel;
	TBevel *Bevel1;
	TEdit *EfpExecFile;
	TEdit *ECommandLine;
	TLabel *EfpStdErrFile;
	TLabel *Label8;
	TCheckBox *CBBracketDQ;
	TPopupMenu *PMCommandMacro;
	TMenuItem *N3;
	TMenuItem *N2;
	TMenuItem *MCMLFC;
	TMenuItem *MCMSFC;
	TMenuItem *MCMLC;
	TMenuItem *MCMSC;
	TMenuItem *N1;
	TMenuItem *MCMLFT;
	TMenuItem *MCMSFT;
	TMenuItem *MCMLT;
	TMenuItem *MCMST;
	TMenuItem *N4;
	TMenuItem *MCMLDT;
	TMenuItem *MCMLNT;
	TPopupMenu *PMCommandEnv;
	TMenuItem *PMCEPath;
	TMenuItem *PMCEWindir;
	TMenuItem *PMCELDir;
	TMenuItem *PMCESDir;
	TMenuItem *PMCEBracketdq;
	TMenuItem *N8;
	TMenuItem *PMCECut;
	TMenuItem *PMCECopy;
	TMenuItem *PMCEPaste;
	TMenuItem *PMCETemplate;
	TMenuItem *PMCESTemplate;
	TMenuItem *MCMLOT;
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

private:
public:
	virtual __fastcall TCommandExeDialog(HWND pHandle, //TComponent* AOwner,
	    const char *fpExecFile, // 実行ファイルフルパス名
	    const char *CommandLine, // コマンドライン(マクロ使用可能)
	    const char *fpTargetFile, // 対象ファイル名（一つのみ）
        const char *CommandEnv, // 環境変数(|連結形式）
	    const char *fpStdErrFile, // 結果出力ファイルフルパス名
	    const bool IsStdErrOut, // 結果出力の是非
		const bool IsBracketDQ); // ファイル名を""で囲む(Bracket)
};
//----------------------------------------------------------------------------
extern PACKAGE TCommandExeDialog *CommandExeDialog;
//----------------------------------------------------------------------------
#endif
