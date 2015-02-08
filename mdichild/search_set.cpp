//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "search_set.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Ts_set_w *s_set_w;
//Search SETting Window
//検索設定用ウインドウ
//---------------------------------------------------------------------------
__fastcall Ts_set_w::Ts_set_w(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Ts_set_w::CBExRegexpClick(TObject *Sender)
{
	if(CBExRegexp->Checked) CBExFuzzy->Enabled = true;
    else CBExFuzzy->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall Ts_set_w::ESearchChange(TObject *Sender)
{
if(ESearch->Text.Length() > 0) btn_SearchStart->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall Ts_set_w::RBstEditorClick(TObject *Sender)
{
if(RBstEditor->Checked) {
	RBstEditor->Checked = true;
    RBstEditorEx->Checked = false;
} else {
	RBstEditor->Checked = false;
    RBstEditorEx->Checked = true;
}
}
//---------------------------------------------------------------------------

