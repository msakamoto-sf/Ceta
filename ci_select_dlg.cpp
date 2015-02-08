//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ci_select_dlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCISelectDlg *CISelectDlg;
//---------------------------------------------------------------------------
__fastcall TCISelectDlg::TCISelectDlg(
	TComponent* Owner, AnsiString fname,
    AnaMap<TCIRegistry, const AnsiString> *CIRegistryList)
	 : TForm(Owner)
{
	_fname = fname;
	_CIRegistryList = CIRegistryList;
}
//---------------------------------------------------------------------------
/**
 * 結夢登録リストのリスト表示。
 */
void __fastcall TCISelectDlg::UpdateListBox(void)
{
	//とりあえずリストをクリア
	ListBox1->Items->Clear();
	TCIRegistry *buf1,*buf2;
	AnsiString item_buf = ""; //リストボックスに加える文字列用バッファ
	//拡張子の抜き出し
	AnsiString _ext = ExtractFileExt(_fname);

	buf1 = _CIRegistryList->Index(0);
	while(buf1 != NULL) {
   		item_buf = buf1->GetCIName() + "("+ buf1->GetCIExtensions() + ")";
		ListBox1->Items->Add(item_buf);
	    buf2 = buf1; //現在ポインタを保存。
	    buf1 = buf2->next; //ツリーを一つ進める。
	}
	int i = _CIRegistryList->SearchIndex(_ext);
	if(i != -1) ListBox1->ItemIndex = i;

	return;
}

void __fastcall TCISelectDlg::FormShow(TObject *Sender)
{
	Label2->Caption = ExtractFileName(_fname);
	UpdateListBox();
}
//---------------------------------------------------------------------------
