//---------------------------------------------------------------------------
#ifndef ci_select_dlgH
#define ci_select_dlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include "misc\anamap.h"
#include "ci_registry.h"

class TCISelectDlg : public TForm
{
__published:	// IDE 管理のコンポーネント
	TLabel *Label1;
	TLabel *Label2;
	TButton *btn_ok;
	TButton *btn_Cancel;
	TListBox *ListBox1;
	void __fastcall FormShow(TObject *Sender);
private:	// ユーザー宣言
    AnsiString _fname;
	AnaMap<TCIRegistry, const AnsiString> *_CIRegistryList;

public:		// ユーザー宣言
	__fastcall TCISelectDlg(
    	TComponent* Owner,
        AnsiString fname,
        AnaMap<TCIRegistry, const AnsiString> *CIRegistryList);
	void __fastcall UpdateListBox(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TCISelectDlg *CISelectDlg;
//---------------------------------------------------------------------------
#endif
