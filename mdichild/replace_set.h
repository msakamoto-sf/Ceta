//---------------------------------------------------------------------------
#ifndef replace_setH
#define replace_setH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class Trp_set_w : public TForm
{
__published:	// IDE 管理のコンポーネント
    TLabel *Label1;
	TButton *btn_ReplaceStart;
	TButton *btn_Help;
	TGroupBox *GroupBox3;
	TCheckBox *CBExMatchCase;
	TCheckBox *CBExWholeWord;
	TCheckBox *CBExRegexp;
	TCheckBox *CBExFuzzy;
	TEdit *ESearch;
	TLabel *Label2;
	TEdit *EReplace;
	TGroupBox *GroupBox1;
	TCheckBox *CBMatchCase;
	TCheckBox *CBDown;
	TCheckBox *CBWholeWord;
	TCheckBox *CBNoMatchZenkaku;
	TCheckBox *CBIncludeCRLF;
	TCheckBox *CBIncludeSpace;
	TRadioButton *RBstEditorEx;
	TRadioButton *RBstEditor;
	TCheckBox *CBReplaceConfirm;
	TCheckBox *CBReplaceAll;
	void __fastcall CBExRegexpClick(TObject *Sender);
	void __fastcall ESearchChange(TObject *Sender);
	void __fastcall RBstEditorClick(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
    __fastcall Trp_set_w(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Trp_set_w *rp_set_w;
//---------------------------------------------------------------------------
#endif
