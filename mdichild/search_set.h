//---------------------------------------------------------------------------
#ifndef search_setH
#define search_setH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class Ts_set_w : public TForm
{
__published:	// IDE 管理のコンポーネント
    TLabel *Label1;
	TButton *btn_SearchStart;
	TButton *btn_Help;
	TGroupBox *GroupBox3;
	TCheckBox *CBExMatchCase;
	TCheckBox *CBExWholeWord;
	TCheckBox *CBExRegexp;
	TCheckBox *CBExFuzzy;
	TEdit *ESearch;
	TGroupBox *GroupBox1;
	TRadioButton *RBstEditor;
	TRadioButton *RBstEditorEx;
	TCheckBox *CBMatchCase;
	TCheckBox *CBDown;
	TCheckBox *CBWholeWord;
	TCheckBox *CBNoMatchZenkaku;
	TCheckBox *CBIncludeCRLF;
	TCheckBox *CBIncludeSpace;
	void __fastcall CBExRegexpClick(TObject *Sender);
	void __fastcall ESearchChange(TObject *Sender);
	void __fastcall RBstEditorClick(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
    __fastcall Ts_set_w(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Ts_set_w *s_set_w;
//---------------------------------------------------------------------------
#endif
