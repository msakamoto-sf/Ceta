//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#include "..\mdichild\editorex_set_dlg.h"
#include "EditorEx.hpp"
#include "HEditor.hpp"
#include "CSPIN.h"
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TOpenDialog *OpenDialog1;
	TButton *Button1;
	TEditorEx *EditorEx1;
	TCSpinEdit *CSpinEdit1;
	void __fastcall Button1Click(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
