//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "replace_set.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Trp_set_w *rp_set_w;

//置換設定用ウインドウ
//---------------------------------------------------------------------------
__fastcall Trp_set_w::Trp_set_w(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Trp_set_w::CBExRegexpClick(TObject *Sender)
{
	if(CBExRegexp->Checked) CBExFuzzy->Enabled = true;
    else CBExFuzzy->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall Trp_set_w::ESearchChange(TObject *Sender)
{
	if((ESearch->Text.Length() > 0) && (EReplace->Text.Length() > 0))
		btn_ReplaceStart->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall Trp_set_w::RBstEditorClick(TObject *Sender)
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

