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
 * �����o�^���X�g�̃��X�g�\���B
 */
void __fastcall TCISelectDlg::UpdateListBox(void)
{
	//�Ƃ肠�������X�g���N���A
	ListBox1->Items->Clear();
	TCIRegistry *buf1,*buf2;
	AnsiString item_buf = ""; //���X�g�{�b�N�X�ɉ����镶����p�o�b�t�@
	//�g���q�̔����o��
	AnsiString _ext = ExtractFileExt(_fname);

	buf1 = _CIRegistryList->Index(0);
	while(buf1 != NULL) {
   		item_buf = buf1->GetCIName() + "("+ buf1->GetCIExtensions() + ")";
		ListBox1->Items->Add(item_buf);
	    buf2 = buf1; //���݃|�C���^��ۑ��B
	    buf1 = buf2->next; //�c���[����i�߂�B
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
