//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EditorEx"
#pragma link "HEditor"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{

if(OpenDialog1->Execute()) {
	TEditorExSetDialog *dlg = new TEditorExSetDialog(this, OpenDialog1->FileName);
    dlg->ShowModal();
    delete dlg;
}

}
//---------------------------------------------------------------------------
 