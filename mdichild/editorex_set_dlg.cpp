//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "editorex_set_dlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "CGRID"
#pragma link "HEditor"
#pragma link "EditorEx"
#pragma resource "*.dfm"
TEditorExSetDialog *EditorExSetDialog;
//---------------------------------------------------------------------------
__fastcall TEditorExSetDialog::TEditorExSetDialog(TComponent* Owner, AnsiString fpCIFileBuffer)
	 : TForm(Owner)
{
   	Caption = fpCIFileBuffer + AnsiString(" の文字色設定");
   	SetfpCIFile(fpCIFileBuffer);
	FollowStr_default = AnsiString("、。，．・？！゛゜ヽヾゝゞ々ー）］｝」』!),.:;?]}｡｣､･ｰﾞﾟ");
	LeadStr_default = AnsiString("（［｛「『([{｢");
	PunctationStr_default = AnsiString("、。，．,.｡､");
	Config = new TConfig(fpCIFileBuffer);
}


/////////////////////////////////////////////////////////////////////////////
//
// 外部公開向け汎用関数群
//

/* 文字列からパラメータ記述をみて、TFontStylesを設定。*/
void SetStrFontStyles(TFontStyles *fs,AnsiString d)
{
	fs->Clear();
	if(d.AnsiPos("fsBold")) *fs << fsBold;
	if(d.AnsiPos("fsItalic")) *fs << fsItalic;
	if(d.AnsiPos("fsUnderline")) *fs << fsUnderline;
	return;
}
/* フォントスタイルを見て、パラメータ文字列を生成。*/
AnsiString FontStyleToStr(TFontStyles fs)
{
	AnsiString s;
	s = "";
	if(fs.Contains(fsBold)) s += "fsBold";
	if(fs.Contains(fsItalic)) s += "fsItalic";
	if(fs.Contains(fsUnderline)) s += "fsUnderline";
	return s;
}
/* TColor型Color定数を、ColorGrid内のIndexに変換。
	対応していなければ-1を返す。*/
int ColorConstConv(TColor cl)
{
	switch(cl) {
    	case clBlack:return 0;
        case clMaroon:return 1;
        case clGreen:return 2;
        case clOlive:return 3;
        case clNavy:return 4;
        case clPurple:return 5;
        case clTeal:return 6;
        case clSilver:return 7;
        case clGray:return 8;
        case clRed:return 9;
        case clLime:return 10;
        case clYellow:return 11;
        case clBlue:return 12;
        case clFuchsia:return 13;
        case clAqua:return 14;
        case clWhite:return 15;
        default:return -1;
    }
}
/* ColorGridで選択されているIndexをColor定数に変換。
	flagは１ならFGを、0ならBGを返す。*/
TColor ColorGridConv(bool flag,TCColorGrid *cg)
{
	TColor color_array[16]
    	 = {clBlack,clMaroon,clGreen,clOlive,
	    	clNavy,clPurple,clTeal,clSilver,
            clGray,clRed,clLime,clYellow,
            clBlue,clFuchsia,clAqua,clWhite};
    if(flag) return color_array[cg->ForegroundIndex];
    else return color_array[cg->BackgroundIndex];
}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// タブシート関連イベントハンドラセクション
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Font タブシート関連ハンドラ
//

//Get installed Font names.
int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf,
	NEWTEXTMETRIC *lpntm,int FontType,LPARAM lParam)
{
	TEditorExSetDialog *dlg;
	TStrings *Items;
    dlg = (TEditorExSetDialog *)lParam;
    Items = dlg->Font_LBFont->Items;
    if(lpelf->elfLogFont.lfFaceName[0] == '@') return 1;
    if(!dlg->Font_CBVpitch->Checked) {
    	if(lpntm->tmPitchAndFamily & 0x01) return 1;
    }
    Items->Add(lpelf->elfLogFont.lfFaceName);
	return 1;
}

void __fastcall TEditorExSetDialog::BuildFontList(void)
{
	HDC dc;
    Font_LBFont->Items->Clear();
    dc = GetDC(0);
    EnumFontFamilies(dc,NULL,(FONTENUMPROC)EnumFontFamProc,(LPARAM)(this));
    ReleaseDC(0,dc);
}

void __fastcall TEditorExSetDialog::Font_CBVpitchClick(TObject *Sender)
{
	int i;
	BuildFontList();
	i = Font_LBFont->Items->IndexOf(Font_TEtest->Font->Name);
	if(i >= 0) Font_LBFont->ItemIndex = 1;
	else {
		Font_LBFont->ItemIndex = 0;
    	Font_TEtest->Font->Name = Font_LBFont->Items->Strings[0];
	}
	return;
}
void __fastcall TEditorExSetDialog::Font_LBFontClick(TObject *Sender)
{
	Font_TEtest->Font->Name = Font_LBFont->Items->Strings[Font_LBFont->ItemIndex];
}
void __fastcall TEditorExSetDialog::Font_LBSizeClick(TObject *Sender)
{
	Font_EBSize->Text = Font_LBSize->Items->Strings[Font_LBSize->ItemIndex];
}
void __fastcall TEditorExSetDialog::Font_EBSizeChange(TObject *Sender)
{
	Font_TEtest->Font->Size = StrToIntDef(Font_EBSize->Text,10);
}
void __fastcall TEditorExSetDialog::Font_RBssNoneClick(TObject *Sender)
{
	Font_RBssNone->Checked = 1;
	Font_TEtest->ScrollBars = ssNone;
}
void __fastcall TEditorExSetDialog::Font_RBssHorizontalClick(TObject *Sender)
{
	Font_RBssHorizontal->Checked = 1;
	Font_TEtest->ScrollBars = ssHorizontal;
}
void __fastcall TEditorExSetDialog::Font_RBssVerticalClick(TObject *Sender)
{
	Font_RBssVertical->Checked = 1;
	Font_TEtest->ScrollBars = ssVertical;
}
void __fastcall TEditorExSetDialog::Font_RBssBothClick(TObject *Sender)
{
	Font_RBssBoth->Checked = 1;
	Font_TEtest->ScrollBars = ssBoth;
}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Caret タブシート関連ハンドラ
//
// （実際の設定は各チェックボックスのCheckedプロパティなど
//  から直接読み書きするのでここでは詳しく記述しない。）
//

void __fastcall TEditorExSetDialog::EnumCursor(AnsiString s)
{
	Caret_CBXDefaultC->Items->Add(s);
    Caret_CBXDragC->Items->Add(s);
    Caret_CBXDragSelC->Items->Add(s);
    Caret_CBXInSelC->Items->Add(s);
    Caret_CBXLMC->Items->Add(s);
    Caret_CBXTMC->Items->Add(s);
	return;
}

void __fastcall TEditorExSetDialog::Caret_CBXDefaultCChange(TObject *Sender)
{
TComboBox *buf;
buf = dynamic_cast<TComboBox *>(Sender);
switch(buf->Tag) {
	case 1:
    	Caret_TEtest->Caret->Cursors->DefaultCursor = StringToCursor(buf->Items->Strings[buf->ItemIndex]);
        break;
    case 2:
    	Caret_TEtest->Caret->Cursors->DragSelCursor = StringToCursor(buf->Items->Strings[buf->ItemIndex]);
        break;
    case 3:
    	Caret_TEtest->Caret->Cursors->DragSelCopyCursor = StringToCursor(buf->Items->Strings[buf->ItemIndex]);
        break;
    case 4:
    	Caret_TEtest->Caret->Cursors->InSelCursor = StringToCursor(buf->Items->Strings[buf->ItemIndex]);
        break;
    case 5:
    	Caret_TEtest->Caret->Cursors->LeftMarginCursor = StringToCursor(buf->Items->Strings[buf->ItemIndex]);
        break;
    case 6:
    	Caret_TEtest->Caret->Cursors->TopMarginCursor = StringToCursor(buf->Items->Strings[buf->ItemIndex]);
        break;
}
}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// View Marks タブシート関連ハンドラ
//

void __fastcall TEditorExSetDialog::VMM_LBColorClick(TObject *Sender)
{
int i;
TColor b,c;
TFontStyles Styles;
Styles.Clear();
switch(VMM_LBColor->ItemIndex) {
	case 0:
    	b = VMM_TEtest->Color;
        c = VMM_TEtest->Font->Color;
        Styles = VMM_TEtest->Font->Style;
        break;
    case 1:
    	b = VMM_TEtest->View->Colors->Ank->BkColor;
        c = VMM_TEtest->View->Colors->Ank->Color;
        Styles = VMM_TEtest->View->Colors->Ank->Style;
        break;
    case 2:
    	b = VMM_TEtest->View->Colors->Comment->BkColor;
        c = VMM_TEtest->View->Colors->Comment->Color;
        Styles = VMM_TEtest->View->Colors->Comment->Style;
        break;
    case 3:
    	b = VMM_TEtest->View->Colors->DBCS->BkColor;
        c = VMM_TEtest->View->Colors->DBCS->Color;
        Styles = VMM_TEtest->View->Colors->DBCS->Style;
        break;
    case 4:
    	b = VMM_TEtest->View->Colors->Int->BkColor;
        c = VMM_TEtest->View->Colors->Int->Color;
        Styles = VMM_TEtest->View->Colors->Int->Style;
        break;
    case 5:
    	b = VMM_TEtest->View->Colors->Str->BkColor;
        c = VMM_TEtest->View->Colors->Str->Color;
        Styles = VMM_TEtest->View->Colors->Str->Style;
        break;
    case 6:
    	b = VMM_TEtest->View->Colors->Symbol->BkColor;
        c = VMM_TEtest->View->Colors->Symbol->Color;
        Styles = VMM_TEtest->View->Colors->Symbol->Style;
        break;
    case 7:
    	b = VMM_TEtest->View->Colors->Reserve->BkColor;
        c = VMM_TEtest->View->Colors->Reserve->Color;
        Styles = VMM_TEtest->View->Colors->Reserve->Style;
        break;
    case 8:
    	b = VMM_TEtest->View->Colors->Url->BkColor;
        c = VMM_TEtest->View->Colors->Url->Color;
        Styles = VMM_TEtest->View->Colors->Url->Style;
        break;
    case 9:
    	b = VMM_TEtest->View->Colors->Mail->BkColor;
        c = VMM_TEtest->View->Colors->Mail->Color;
        Styles = VMM_TEtest->View->Colors->Mail->Style;
        break;
    case 10:
    	b = VMM_TEtest->View->Colors->Select->BkColor;
        c = VMM_TEtest->View->Colors->Select->Color;
        Styles = VMM_TEtest->View->Colors->Select->Style;
        break;
    case 11:
    	b = clWhite;
        c = VMM_TEtest->Marks->EofMark->Color;
        Styles.Clear();
    	break;
    case 12:
    	b = clWhite;
        c = VMM_TEtest->Marks->RetMark->Color;
        Styles.Clear();;
    	break;
    case 13:
    	b = clWhite;
        c = VMM_TEtest->Marks->Underline->Color;
        Styles.Clear();
		break;
    case 14:
    	b = clWhite;
        c = VMM_TEtest->Marks->WrapMark->Color;
        Styles.Clear();
        break;
    case 15:
    	b = clWhite;
        c = VMM_TEtest->ExMarks->FindMark->Color;
        Styles.Clear();
        break;
    case 16:
    	b = clWhite;
        c = VMM_TEtest->ExMarks->SpaceMark->Color;
        Styles.Clear();
        break;
    case 17:
       	b = clWhite;
        c = VMM_TEtest->ExMarks->DBSpaceMark->Color;
        Styles.Clear();
        break;
	case 18:
    	b = clWhite;
        c = VMM_TEtest->ExMarks->TabMark->Color;
        Styles.Clear();
        break;
    default:
    	b = clWhite;
        c = clBlack;
        Styles.Clear();
        break;
}
//BackgroundEnabled
if((VMM_LBColor->ItemIndex >= 11) && (VMM_LBColor->ItemIndex <= 18))
	VMM_ColorGrid->BackgroundEnabled = false;
else
	VMM_ColorGrid->BackgroundEnabled = true;

//Color Index
i = ColorConstConv(b);
if(i == -1) i = 15;//clWhite
VMM_ColorGrid->BackgroundIndex = i;
i = ColorConstConv(c);
if(i == -1) i = 0;//clBlack
VMM_ColorGrid->ForegroundIndex = i;

//Style Adjustment
VMM_CBfsBold->OnClick = NULL;
VMM_CBfsUnderline->OnClick = NULL;
VMM_CBfsItalic->OnClick = NULL;
VMM_CBfsBold->Checked = Styles.Contains(fsBold);
VMM_CBfsUnderline->Checked = Styles.Contains(fsUnderline);
VMM_CBfsItalic->Checked = Styles.Contains(fsItalic);
VMM_CBfsBold->OnClick = VMM_CBfsBoldClick;
VMM_CBfsUnderline->OnClick = VMM_CBfsBoldClick;
VMM_CBfsItalic->OnClick = VMM_CBfsBoldClick;

if((VMM_LBColor->ItemIndex >= 11) && (VMM_LBColor->ItemIndex <= 18)) {
	VMM_CBfsBold->Enabled = false;
    VMM_CBfsUnderline->Enabled = false;
    VMM_CBfsItalic->Enabled = false;
} else {
	VMM_CBfsBold->Enabled = true;
    VMM_CBfsUnderline->Enabled = true;
    VMM_CBfsItalic->Enabled = true;
}
}

void __fastcall TEditorExSetDialog::VMM_ColorGridMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
TColor b,c;
c = ColorGridConv(1,VMM_ColorGrid);
b = ColorGridConv(0,VMM_ColorGrid);
switch(VMM_LBColor->ItemIndex) {
	case 0:
    	VMM_TEtest->Color = b;
        VMM_TEtest->Font->Color = c;
    	break;
    case 1:
    	VMM_TEtest->View->Colors->Ank->BkColor = b;
        VMM_TEtest->View->Colors->Ank->Color = c;
        break;
    case 2:
        VMM_TEtest->View->Colors->Comment->BkColor = b;
        VMM_TEtest->View->Colors->Comment->Color = c;
        break;
    case 3:
    	VMM_TEtest->View->Colors->DBCS->BkColor = b;
        VMM_TEtest->View->Colors->DBCS->Color = c;
        break;
    case 4:
		VMM_TEtest->View->Colors->Int->BkColor = b;
        VMM_TEtest->View->Colors->Int->Color = c;
        break;
    case 5:
    	VMM_TEtest->View->Colors->Str->BkColor = b;
        VMM_TEtest->View->Colors->Str->Color = c;
        break;
    case 6:
    	VMM_TEtest->View->Colors->Symbol->BkColor = b;
        VMM_TEtest->View->Colors->Symbol->Color = c;
        break;
    case 7:
    	VMM_TEtest->View->Colors->Reserve->BkColor = b;
        VMM_TEtest->View->Colors->Reserve->Color = c;
        break;
    case 8:
    	VMM_TEtest->View->Colors->Url->BkColor = b;
        VMM_TEtest->View->Colors->Url->Color = c;
        break;
    case 9:
    	VMM_TEtest->View->Colors->Mail->BkColor = b;
        VMM_TEtest->View->Colors->Mail->Color = c;
        break;
    case 10:
    	VMM_TEtest->View->Colors->Select->BkColor = b;
        VMM_TEtest->View->Colors->Select->Color = c;
		break;
    case 11:
    	VMM_TEtest->Marks->EofMark->Color = c;
        break;
    case 12:
        VMM_TEtest->Marks->RetMark->Color = c;
        break;
    case 13:
    	VMM_TEtest->Marks->Underline->Color = c;
		break;
    case 14:
    	VMM_TEtest->Marks->WrapMark->Color = c;
        break;
    case 15:
        VMM_TEtest->ExMarks->FindMark->Color = c;
		break;
    case 16:
    	VMM_TEtest->ExMarks->SpaceMark->Color = c;
        break;
    case 17:
    	VMM_TEtest->ExMarks->DBSpaceMark->Color = c;
        break;
    case 18:
    	VMM_TEtest->ExMarks->TabMark->Color = c;
        break;
    default:break;
}
}

void __fastcall TEditorExSetDialog::VMM_CBfsBoldClick(TObject *Sender)
{
TFontStyles result;
result.Clear();
if(VMM_CBfsBold->Checked) result << fsBold;
if(VMM_CBfsUnderline->Checked) result << fsUnderline;
if(VMM_CBfsItalic->Checked) result << fsItalic;

switch(VMM_LBColor->ItemIndex) {
	case 0:VMM_TEtest->Font->Style = result;break;
    case 1:VMM_TEtest->View->Colors->Ank->Style = result;break;
    case 2:VMM_TEtest->View->Colors->Comment->Style = result;break;
 	case 3:VMM_TEtest->View->Colors->DBCS->Style = result;break;
	case 4:VMM_TEtest->View->Colors->Int->Style = result;break;
	case 5:VMM_TEtest->View->Colors->Str->Style = result;break;
	case 6:VMM_TEtest->View->Colors->Symbol->Style = result;break;
	case 7:VMM_TEtest->View->Colors->Reserve->Style = result;break;
	case 8:VMM_TEtest->View->Colors->Url->Style = result;break;
	case 9:VMM_TEtest->View->Colors->Mail->Style = result;break;
	case 10:VMM_TEtest->View->Colors->Select->Style = result;break;
    default:break;
}
return;
}

void __fastcall TEditorExSetDialog::VMM_BTNSameBkClick(TObject *Sender)
{
TColor b;
b = ColorGridConv(0,VMM_ColorGrid);
if(MessageBox(Handle,"全ての背景色を現在選択されている背景色で統一しますか？","背景色の統一",MB_OKCANCEL) == IDOK) {
	VMM_TEtest->Color = b;
	VMM_TEtest->View->Colors->Ank->BkColor = b;
	VMM_TEtest->View->Colors->Comment->BkColor = b;
	VMM_TEtest->View->Colors->DBCS->BkColor = b;
	VMM_TEtest->View->Colors->Int->BkColor = b;
	VMM_TEtest->View->Colors->Str->BkColor = b;
	VMM_TEtest->View->Colors->Symbol->BkColor = b;
	VMM_TEtest->View->Colors->Reserve->BkColor = b;
	VMM_TEtest->View->Colors->Url->BkColor = b;
	VMM_TEtest->View->Colors->Mail->BkColor = b;
	VMM_TEtest->View->Colors->Select->BkColor = b;
}
}

void __fastcall TEditorExSetDialog::VMM_BTNSameColClick(TObject *Sender)
{
TColor c;
c = ColorGridConv(1,VMM_ColorGrid);
if(MessageBox(Handle,"全ての文字色を現在選択されている文字色で統一しますか？","文字色の統一",MB_OKCANCEL) == IDOK) {
	VMM_TEtest->Font->Color = c;
	VMM_TEtest->View->Colors->Ank->Color = c;
	VMM_TEtest->View->Colors->Comment->Color = c;
	VMM_TEtest->View->Colors->DBCS->Color = c;
	VMM_TEtest->View->Colors->Int->Color = c;
	VMM_TEtest->View->Colors->Str->Color = c;
	VMM_TEtest->View->Colors->Symbol->Color = c;
	VMM_TEtest->View->Colors->Reserve->Color = c;
	VMM_TEtest->View->Colors->Url->Color = c;
	VMM_TEtest->View->Colors->Mail->Color = c;
	VMM_TEtest->View->Colors->Select->Color = c;
}
}

void __fastcall TEditorExSetDialog::VMM_BTNSameStyleClick(TObject *Sender)
{
TFontStyles result;
result.Clear();
if(VMM_CBfsBold->Checked) result << fsBold;
if(VMM_CBfsUnderline->Checked) result << fsUnderline;
if(VMM_CBfsItalic->Checked) result << fsItalic;

if(MessageBox(Handle,"全てのスタイルを現在選択されているスタイルで統一しますか？","スタイルの統一",MB_OKCANCEL) == IDOK) {
	VMM_TEtest->Font->Style = result;
	VMM_TEtest->View->Colors->Ank->Style = result;
	VMM_TEtest->View->Colors->Comment->Style = result;
	VMM_TEtest->View->Colors->DBCS->Style = result;
	VMM_TEtest->View->Colors->Int->Style = result;
	VMM_TEtest->View->Colors->Str->Style = result;
	VMM_TEtest->View->Colors->Symbol->Style = result;
	VMM_TEtest->View->Colors->Reserve->Style = result;
	VMM_TEtest->View->Colors->Url->Style = result;
	VMM_TEtest->View->Colors->Mail->Style = result;
	VMM_TEtest->View->Colors->Select->Style = result;
}
}

void __fastcall TEditorExSetDialog::VMM_CBMailClick(TObject *Sender)
{
VMM_TEtest->View->Mail = VMM_CBMail->Checked;
}
void __fastcall TEditorExSetDialog::VMM_CBUrlClick(TObject *Sender)
{
VMM_TEtest->View->Url = VMM_CBUrl->Checked;
}
void __fastcall TEditorExSetDialog::VMM_CBCCCodeClick(TObject *Sender)
{
VMM_TEtest->View->ControlCode = VMM_CBCCCode->Checked;
}
void __fastcall TEditorExSetDialog::VMM_EBHexPrefixChange(TObject *Sender)
{
VMM_TEtest->Lines->Strings[1] = "0123456789  " + VMM_EBHexPrefix->Text + "AF";
LbRl_TEtest->Lines->Strings[1] = "0123456789  " + VMM_EBHexPrefix->Text + "AF";
VMM_TEtest->View->HexPrefix = VMM_EBHexPrefix->Text;
}
void __fastcall TEditorExSetDialog::VMM_EBQuotationChange(TObject *Sender)
{
AnsiString s;
s = VMM_EBQuotation->Text;
if(s.Length() > 0) VMM_EBQuotation->Text = s[1];
VMM_TEtest->Lines->Strings[0] = "選択領域  " + VMM_EBQuotation->Text + "String" + VMM_EBQuotation->Text + "  #13#10";
LbRl_TEtest->Lines->Strings[0] = "選択領域  " + VMM_EBQuotation->Text + "String" + VMM_EBQuotation->Text + "  #13#10";
VMM_TEtest->View->Quotation = VMM_EBQuotation->Text;
}
void __fastcall TEditorExSetDialog::VMM_EBCommentChange(TObject *Sender)
{
VMM_TEtest->Lines->Strings[2] = "Editor1.View.Brackets[0]  " + VMM_EBComment->Text + " Commentline";
LbRl_TEtest->Lines->Strings[2] = "Editor1.View.Brackets[0]  " + VMM_EBComment->Text + " Commentline";
VMM_TEtest->View->Commenter = VMM_EBComment->Text;
}
void __fastcall TEditorExSetDialog::VMM_RBhsSelectClick(TObject *Sender)
{
    if(VMM_RBhsDraw->Checked) VMM_TEtest->HitStyle = hsDraw;
    else if(VMM_RBhsCaret->Checked) VMM_TEtest->HitStyle = hsCaret;
   	else VMM_TEtest->HitStyle = hsSelect;
}

void __fastcall TEditorExSetDialog::VMM_CBRetMarkClick(TObject *Sender)
{ VMM_TEtest->Marks->RetMark->Visible = VMM_CBRetMark->Checked; }

void __fastcall TEditorExSetDialog::VMM_CBEofMarkClick(TObject *Sender)
{ VMM_TEtest->Marks->EofMark->Visible = VMM_CBEofMark->Checked; }

void __fastcall TEditorExSetDialog::VMM_CBUnderlineClick(TObject *Sender)
{ VMM_TEtest->Marks->Underline->Visible = VMM_CBUnderline->Checked; }

void __fastcall TEditorExSetDialog::VMM_CBWrapMarkClick(TObject *Sender)
{ VMM_TEtest->Marks->WrapMark->Visible = VMM_CBWrapMark->Checked; }

void __fastcall TEditorExSetDialog::VMM_CBFindMarkClick(TObject *Sender)
{ VMM_TEtest->ExMarks->FindMark->Visible = VMM_CBFindMark->Checked; }

void __fastcall TEditorExSetDialog::VMM_CBSpaceMarkClick(TObject *Sender)
{ VMM_TEtest->ExMarks->SpaceMark->Visible = VMM_CBSpaceMark->Checked; }

void __fastcall TEditorExSetDialog::VMM_CBDBSpaceMarkClick(TObject *Sender)
{ VMM_TEtest->ExMarks->DBSpaceMark->Visible = VMM_CBDBSpaceMark->Checked; }

void __fastcall TEditorExSetDialog::VMM_CBTabMarkClick(TObject *Sender)
{ VMM_TEtest->ExMarks->TabMark->Visible = VMM_CBTabMark->Checked; }


//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Leftbar Ruler Margin タブシート関連ハンドラ
//

void __fastcall TEditorExSetDialog::LbRl_LBlbRlClick(TObject *Sender)
{
int i;
switch(LbRl_LBlbRl->ItemIndex) {
	case 0://Ruler
    	LbRl_ColorGrid->BackgroundEnabled = true;
    	if((i = ColorConstConv(LbRl_TEtest->Ruler->Color)) == -1) {
        	LbRl_ColorGrid->ForegroundIndex = 0;
        } else LbRl_ColorGrid->ForegroundIndex = i;
        if((i = ColorConstConv(LbRl_TEtest->Ruler->BkColor)) == -1) {
        	LbRl_ColorGrid->BackgroundIndex = 15;
        } else LbRl_ColorGrid->BackgroundIndex = i;
    	break;
    case 1://Ruler->MarkColor
    	LbRl_ColorGrid->BackgroundEnabled = false;
    	if((i = ColorConstConv(LbRl_TEtest->Ruler->MarkColor)) == -1) {
        	LbRl_ColorGrid->ForegroundIndex = 0;
        } else LbRl_ColorGrid->ForegroundIndex = i;
        break;
    case 2://Leftbar
    	LbRl_ColorGrid->BackgroundEnabled = true;
    	if((i = ColorConstConv(LbRl_TEtest->Leftbar->Color)) == -1) {
        	LbRl_ColorGrid->ForegroundIndex = 0;
        } else LbRl_ColorGrid->ForegroundIndex = i;
        if((i = ColorConstConv(LbRl_TEtest->Leftbar->BkColor)) == -1) {
        	LbRl_ColorGrid->BackgroundIndex = 15;
        } else LbRl_ColorGrid->BackgroundIndex = i;
    	break;
    default:break;
}
}
void __fastcall TEditorExSetDialog::LbRl_ColorGridMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
TColor b,c;
c = ColorGridConv(1,LbRl_ColorGrid);
b = ColorGridConv(0,LbRl_ColorGrid);
switch(LbRl_LBlbRl->ItemIndex) {
	case 0:
        LbRl_TEtest->Ruler->Color = c;
    	LbRl_TEtest->Ruler->BkColor = b;
    	break;
    case 1:
        LbRl_TEtest->Ruler->MarkColor = c;
        break;
    case 2:
        LbRl_TEtest->Leftbar->Color = c;
    	LbRl_TEtest->Leftbar->BkColor = b;
        break;
    default:break;
}
}
void __fastcall TEditorExSetDialog::LbRl_CBLVisibleClick(TObject *Sender)
{
if(LbRl_TEtest->Leftbar->Visible)
LbRl_TEtest->Leftbar->Visible = false;
else
LbRl_TEtest->Leftbar->Visible = true;
}
void __fastcall TEditorExSetDialog::LbRl_CBLEdgeClick(TObject *Sender)
{
if(LbRl_TEtest->Leftbar->Edge)
LbRl_TEtest->Leftbar->Edge = false;
else
LbRl_TEtest->Leftbar->Edge = true;
}
void __fastcall TEditorExSetDialog::LbRl_CBLShowNumberClick(TObject *Sender)
{
if(LbRl_TEtest->Leftbar->ShowNumber)
LbRl_TEtest->Leftbar->ShowNumber = false;
else
LbRl_TEtest->Leftbar->ShowNumber = true;
}
void __fastcall TEditorExSetDialog::LbRl_CBLZeroBaseClick(TObject *Sender)
{
if(LbRl_TEtest->Leftbar->ZeroBase)
LbRl_TEtest->Leftbar->ZeroBase = false;
else
LbRl_TEtest->Leftbar->ZeroBase = true;
}
void __fastcall TEditorExSetDialog::LbRl_CBLZeroLeadClick(TObject *Sender)
{
if(LbRl_TEtest->Leftbar->ZeroLead)
LbRl_TEtest->Leftbar->ZeroLead = false;
else
LbRl_TEtest->Leftbar->ZeroLead = true;
}
void __fastcall TEditorExSetDialog::LbRl_RBnmRowClick(TObject *Sender)
{
LbRl_TEtest->Leftbar->ShowNumberMode = nmRow;
}
void __fastcall TEditorExSetDialog::LbRl_RBnmLineClick(TObject *Sender)
{
LbRl_TEtest->Leftbar->ShowNumberMode = nmLine;
}
void __fastcall TEditorExSetDialog::LbRl_CSEColumnChange(TObject *Sender)
{
LbRl_TEtest->Leftbar->Column = LbRl_CSEColumn->Value;
}
void __fastcall TEditorExSetDialog::LbRl_CSELeftMarginChange(TObject *Sender)
{
LbRl_TEtest->Leftbar->LeftMargin = LbRl_CSELeftMargin->Value;
}
void __fastcall TEditorExSetDialog::LbRl_CSERightMarginChange(TObject *Sender)
{
LbRl_TEtest->Leftbar->RightMargin = LbRl_CSERightMargin->Value;
}
void __fastcall TEditorExSetDialog::LbRl_CBRVisibleClick(TObject *Sender)
{
if(LbRl_TEtest->Ruler->Visible)
LbRl_TEtest->Ruler->Visible = false;
else
LbRl_TEtest->Ruler->Visible = true;
}
void __fastcall TEditorExSetDialog::LbRl_CBREdgeClick(TObject *Sender)
{
if(LbRl_TEtest->Ruler->Edge)
LbRl_TEtest->Ruler->Edge = false;
else
LbRl_TEtest->Ruler->Edge = true;
}
void __fastcall TEditorExSetDialog::LbRl_RB10Click(TObject *Sender)
{
LbRl_TEtest->Ruler->GaugeRange = 10;
}
void __fastcall TEditorExSetDialog::LbRl_RB08Click(TObject *Sender)
{
LbRl_TEtest->Ruler->GaugeRange = 8;
}
void __fastcall TEditorExSetDialog::LbRlM_CSETopChange(TObject *Sender)
{
LbRl_TEtest->Margin->Top = LbRlM_CSETop->Value;
}
void __fastcall TEditorExSetDialog::LbRlM_CSELeftChange(TObject *Sender)
{
LbRl_TEtest->Margin->Left = LbRlM_CSELeft->Value;
}
void __fastcall TEditorExSetDialog::LbRlM_CSELineChange(TObject *Sender)
{
LbRl_TEtest->Margin->Line = LbRlM_CSELine->Value;
}
void __fastcall TEditorExSetDialog::LbRlM_CSECharacterChange(TObject *Sender)
{
LbRl_TEtest->Margin->Character = LbRlM_CSECharacter->Value;
}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Brackets タブシート関連ハンドラ
//

void __fastcall TEditorExSetDialog::BRK_LBColorClick(TObject *Sender)
{
int i,j;
TColor c,b;
TFontStyles Styles;
i = BRK_LBColor->ItemIndex;
if(i >= 0) {
    BRK_EBLeftB->Enabled = true;
	BRK_EBRightB->Enabled = true;
	BRK_BTNRemove->Enabled = true;
    BRK_ColorGrid->BackgroundEnabled = true;
    BRK_ColorGrid->ForegroundEnabled = true;
    BRK_CBfsBold->Enabled = true;
    BRK_CBfsItalic->Enabled = true;
    BRK_CBfsUnderline->Enabled = true;
    b = BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->BkColor;
    c = BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->Color;
    Styles = BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->Style;
	j = ColorConstConv(b);
	if(j == -1) j = 15;
    BRK_ColorGrid->BackgroundIndex = j;
    j = ColorConstConv(c);
    if(j == -1) j = 0;
    BRK_ColorGrid->ForegroundIndex = j;
    BRK_EBLeftB->Text = BRK_TEtest->View->Brackets->BracketItems[i]->LeftBracket;
    BRK_EBRightB->Text = BRK_TEtest->View->Brackets->BracketItems[i]->RightBracket;
	BRK_CBfsBold->OnClick = NULL;
    BRK_CBfsItalic->OnClick = NULL;
    BRK_CBfsUnderline->OnClick = NULL;
	BRK_CBfsBold->Checked = Styles.Contains(fsBold);
	BRK_CBfsUnderline->Checked = Styles.Contains(fsUnderline);
	BRK_CBfsItalic->Checked = Styles.Contains(fsItalic);
	BRK_CBfsBold->OnClick = BRK_CBfsBoldClick;
	BRK_CBfsUnderline->OnClick = BRK_CBfsBoldClick;
	BRK_CBfsItalic->OnClick = BRK_CBfsBoldClick;
}
}

void __fastcall TEditorExSetDialog::BRK_ColorGridMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int i = BRK_LBColor->ItemIndex;
if(i >= 0) {
	BRK_TEtest->View->BeginUpdate();
	try {
		BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->BkColor
    		= ColorGridConv(0,BRK_ColorGrid);
	    BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->Color
    		= ColorGridConv(1,BRK_ColorGrid);
	} catch(...) {}
    BRK_TEtest->View->EndUpdate();
}
return;
}

void __fastcall TEditorExSetDialog::BRK_CBfsBoldClick(TObject *Sender)
{
TFontStyles result;
result.Clear();
if(BRK_CBfsBold->Checked) result << fsBold;
if(BRK_CBfsUnderline->Checked) result << fsUnderline;
if(BRK_CBfsItalic->Checked) result << fsItalic;

int i = BRK_LBColor->ItemIndex;
if(i >= 0) {
	BRK_TEtest->View->BeginUpdate();
    try {
    	BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->Style = result;
    } catch(...) {}
    BRK_TEtest->View->EndUpdate();
}
return;
}

void __fastcall TEditorExSetDialog::BRK_BTNNewClick(TObject *Sender)
{
if(BRK_LBColor->Items->Count == 14) {
	MessageBox(Handle,"14個以上の囲み文字を登録することはできません","設定ミスのお知らせ",MB_OK);
    return;
}
BRK_TEtest->View->Brackets->Add();
BRK_LBColor->Items->Add("");
BRK_TEtest->Lines->Add("Closed String(被囲み文字)");
BRK_LBColor->ItemIndex = BRK_LBColor->Items->Count - 1;
BRK_LBColorClick(Sender);
return;
}

void __fastcall TEditorExSetDialog::BRK_BTNRemoveClick(TObject *Sender)
{
int i = BRK_LBColor->ItemIndex;
if(i >= 0) {
	BRK_LBColor->Items->Delete(i);
    BRK_TEtest->Lines->Delete(i);
	BRK_TEtest->View->Brackets->BracketItems[i]->Free();
    if(i > BRK_LBColor->Items->Count -1)
    	i = BRK_LBColor->Items->Count -1;
    if(i >= 0) {
    	BRK_LBColor->ItemIndex = i;
        BRK_LBColorClick(Sender);
    } else {
    	BRK_EBLeftB->Text = "";
        BRK_EBRightB->Text = "";
        BRK_EBLeftB->Enabled = false;
        BRK_EBRightB->Enabled = false;
        BRK_BTNRemove->Enabled = false;
        BRK_ColorGrid->BackgroundEnabled = false;
        BRK_ColorGrid->ForegroundEnabled = false;
		BRK_CBfsBold->Checked = false;
		BRK_CBfsItalic->Checked = false;
		BRK_CBfsUnderline->Checked = false;
        BRK_CBfsBold->Enabled = false;
        BRK_CBfsItalic->Enabled = false;
		BRK_CBfsUnderline->Enabled = false;
    }
}
}

void __fastcall TEditorExSetDialog::BRK_EBLeftBChange(TObject *Sender)
{
	BRK_BTNUpdate->Enabled
    	= (BRK_EBLeftB->Text != BRK_EBRightB->Text)
        && (BRK_EBLeftB->Text != "")
        && (BRK_EBRightB->Text != "");
return;
}

void __fastcall TEditorExSetDialog::BRK_BTNUpdateClick(TObject *Sender)
{
int i = BRK_LBColor->ItemIndex;
if(i >= 0) {
	if((BRK_EBLeftB->Text == BRK_EBRightB->Text) || (BRK_EBLeftB->Text == "") || (BRK_EBRightB->Text == "")) {
    	MessageBox(Handle,"空白 ・ 同一の左右囲み文字(Bracket)は指定出来ません。","設定ミスのお知らせ",MB_OK);
        return;
    }
    BRK_TEtest->View->BeginUpdate();
    try {
    	BRK_TEtest->View->Brackets->BracketItems[i]->LeftBracket = BRK_EBLeftB->Text;
        BRK_TEtest->View->Brackets->BracketItems[i]->RightBracket = BRK_EBRightB->Text;
		BRK_LBColor->Items->Strings[i] = BRK_EBLeftB->Text + " " + BRK_EBRightB->Text;
        BRK_TEtest->Lines->Strings[i] = BRK_EBLeftB->Text + " Closed String(被囲み文字) " + BRK_EBRightB->Text;
	} catch(...) {}
    BRK_TEtest->View->EndUpdate();
}
return;
}

void __fastcall TEditorExSetDialog::BRK_BTNSameBkClick(TObject *Sender)
{
TColor b;
int i = BRK_LBColor->ItemIndex;
if(i >= 0) {
	if(BRK_TEtest->View->Brackets->Count > 0) {
		b = BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->BkColor;
	    if(MessageBox(Handle,"全ての背景色を現在選択されている背景色で統一しますか？","背景色の統一",MB_OKCANCEL) == IDOK)
	    	BRK_TEtest->View->Brackets->SameBkColor(b);
	}
}
}

void __fastcall TEditorExSetDialog::BRK_BTNSameColorClick(TObject *Sender)
{
TColor c;
int i = BRK_LBColor->ItemIndex;
if(i >= 0) {
	if(BRK_TEtest->View->Brackets->Count > 0) {
		c = BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->Color;
	    if(MessageBox(Handle,"全ての文字色を現在選択されている文字色で統一しますか？","文字色の統一",MB_OKCANCEL) == IDOK)
	    	BRK_TEtest->View->Brackets->SameColor(c);
	}
}
}

void __fastcall TEditorExSetDialog::BRK_BTNSameStyleClick(TObject *Sender)
{
TFontStyles style;
int i = BRK_LBColor->ItemIndex;
if(i >= 0) {
	if(BRK_TEtest->View->Brackets->Count > 0) {
		style = BRK_TEtest->View->Brackets->BracketItems[i]->ItemColor->Style;
	    if(MessageBox(Handle,"全てのスタイルを現在選択されているスタイルで統一しますか？","スタイルの統一",MB_OKCANCEL) == IDOK)
	    	BRK_TEtest->View->Brackets->SameStyle(style);
	}

}
}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Reserve Word List タブシート関連ハンドラ
//

void __fastcall TEditorExSetDialog::RWL_BTNLoadClick(TObject *Sender)
{
	OpenDialog1->InitialDir = ExtractFileDir(GetfpCIFile());
	if(OpenDialog1->Execute() && FileExists(OpenDialog1->FileName)) {
		RWL_TEtest->Lines->LoadFromFile(OpenDialog1->FileName);
		SaveDialog1->FileName = OpenDialog1->FileName;
	}
}

void __fastcall TEditorExSetDialog::RWL_BTNSaveClick(TObject *Sender)
{
	if(SaveDialog1->Execute()) {
		RWL_TEtest->Lines->SaveToFile(SaveDialog1->FileName);
		OpenDialog1->FileName = SaveDialog1->FileName;
	}
}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// WordWrap タブシート関連ハンドラ
//
//（実際の設定は各チェックボックスのCheckedプロパティなど
// から直接読み書きするのでここでは詳しく記述しない。）
//

void __fastcall TEditorExSetDialog::WW_BTNFollowStrClick(TObject *Sender)
{
WW_EBFollowStr->Text = this->FollowStr_default;
}
void __fastcall TEditorExSetDialog::WW_BTNLeadStrClick(TObject *Sender)
{
WW_EBLeadStr->Text = this->LeadStr_default;
}
void __fastcall TEditorExSetDialog::WW_BTNPunctationStrClick(TObject *Sender)
{
WW_EBPunctationStr->Text = this->PunctationStr_default;
}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Read : 読み込み
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// 読み込みルート関数
//

void TEditorExSetDialog::LoadConfig()
{
    LoadFont();
    LoadCaret();
    LoadVMM();
    LoadLbRl();
    LoadBRK();
    LoadRWL();
    LoadWW();
}

void TEditorExSetDialog::LoadFont()
{
	int i;
	AnsiString Section = "Font";

	Font_CBVpitch->Checked = false;

	BuildFontList();
	for(i = 8;i <= 50;i++) Font_LBSize->Items->Add(IntToStr(i));

	i = Font_LBFont->Items->IndexOf(Config->GetIniFileString(Section, "Font_Name", "ＭＳ ゴシック"));
	if(i == -1) Font_LBFont->ItemIndex = 0;
	else Font_LBFont->ItemIndex = i;
	Font_TEtest->Font->Name = Font_LBFont->Items->Strings[Font_LBFont->ItemIndex];

	Font_EBSize->Text = Config->GetIniFileString(Section, "Font_Size", "11");
	i = Font_LBSize->Items->IndexOf(Font_EBSize->Text);
	if(i == -1) Font_LBSize->ItemIndex = 0;
	else Font_LBSize->ItemIndex = i;
	Font_TEtest->Font->Size = StrToInt(Font_EBSize->Text);

    AnsiString scroll = Config->GetIniFileString(Section, "ScrollBar", "ssNone");

	if(scroll == "ssNone") {
		Font_RBssNone->Checked = true;
		Font_TEtest->ScrollBars = ssNone;
	} else if(scroll == "ssHorizontal") {
		Font_RBssHorizontal->Checked = true;
		Font_TEtest->ScrollBars = ssHorizontal;
	} else if(scroll == "ssVertical") {
		Font_RBssVertical->Checked = true;
		Font_TEtest->ScrollBars = ssVertical;
	} else if(scroll == "ssBoth") {
		Font_RBssBoth->Checked = true;
		Font_TEtest->ScrollBars = ssBoth;
	}
}

void TEditorExSetDialog::LoadCaret()
{
	AnsiString Section = "Caret";
    
	Caret_CBFreeCaret->Checked = Config->GetIniFileInt(Section, "Caret_FreeCaret", true);
	Caret_CBFreeRow->Checked = Config->GetIniFileInt(Section, "Caret_FreeRow", false);
	Caret_CBAutoIndent->Checked = Config->GetIniFileInt(Section, "Caret_AutoIndent", true);
	Caret_CBBackSpace->Checked = Config->GetIniFileInt(Section, "Caret_BackSpaceUnIndent", true);
	Caret_CBInTab ->Checked = Config->GetIniFileInt(Section, "Caret_InTab", 0);
	Caret_CBKeepCaret->Checked = Config->GetIniFileInt(Section, "Caret_KeepCaret", 0);
	Caret_CBLockScroll->Checked = Config->GetIniFileInt(Section, "Caret_LockScroll", 0);
	Caret_CBNextLine->Checked = Config->GetIniFileInt(Section, "Caret_NextLine", 0);
	Caret_CBTabIndent->Checked = Config->GetIniFileInt(Section, "Caret_TabIndent", 0);
	Caret_CBPrevSpace->Checked = Config->GetIniFileInt(Section, "Caret_PrevSpaceIndent", 0);
	Caret_CBSoftTab->Checked = Config->GetIniFileInt(Section, "Caret_SoftTab", 0);
	Caret_CSETabSpace->Value = Config->GetIniFileInt(Section, "Caret_TabSpaceCount", 8);
	if(Config->GetIniFileString(Section, "Caret_Style", "csBrief")  == "csBrief")
    	Caret_RBcsBrief->Checked = true;
	else Caret_RBcsDefault->Checked = true;
	Caret_CBRowSelect->Checked = Config->GetIniFileInt(Section, "Caret_RowSelect", true);
	Caret_CBSelMove->Checked = Config->GetIniFileInt(Section, "Caret_SelMove", true);
	if(Config->GetIniFileString(Section, "Caret_SelDragMode", "dmAutomatic") == "dmAutomatic")
    	Caret_RBdmAuto->Checked = true;
	else Caret_RBdmManual->Checked = true;
	Caret_CBAutoCursor->Checked = Config->GetIniFileInt(Section, "Caret_AutoCursor", true);

	Caret_TEtest->SelStart = 12;
	Caret_TEtest->SelLength = 22;
	GetCursorValues(EnumCursor);

	Caret_CBXDefaultC->ItemIndex = Caret_CBXDefaultC->Items->IndexOf(
    	Config->GetIniFileString(Section, "Caret_Cursors_DefaultCursor", "crIBeam"));
	Caret_CBXDragC->ItemIndex = Caret_CBXDragC->Items->IndexOf(
    	Config->GetIniFileString(Section, "Caret_Cursors_DragSelCopyCursor", "crDrag"));
	Caret_CBXDragSelC->ItemIndex = Caret_CBXDragSelC->Items->IndexOf(
    	Config->GetIniFileString(Section, "Caret_Cursors_DragSelCursor", "crDrag"));
	Caret_CBXInSelC->ItemIndex = Caret_CBXInSelC->Items->IndexOf(
    	Config->GetIniFileString(Section, "Caret_Cursors_InSelCursor", "crDefault"));
	Caret_CBXLMC->ItemIndex = Caret_CBXLMC->Items->IndexOf(
    	Config->GetIniFileString(Section, "Caret_Cursors_LeftMarginCursor", "crArrow"));
	Caret_CBXTMC->ItemIndex = Caret_CBXTMC->Items->IndexOf(
    	Config->GetIniFileString(Section, "Caret_Cursors_TopMarginCursor", "crArrow"));

	Caret_TEtest->Caret->Cursors->DefaultCursor
	 = StringToCursor(Caret_CBXDefaultC->Items->Strings[Caret_CBXDefaultC->ItemIndex]);
	Caret_TEtest->Caret->Cursors->DragSelCursor
	 = StringToCursor(Caret_CBXDragC->Items->Strings[Caret_CBXDragC->ItemIndex]);
	Caret_TEtest->Caret->Cursors->DragSelCopyCursor
	 = StringToCursor(Caret_CBXDragSelC->Items->Strings[Caret_CBXDragSelC->ItemIndex]);
	Caret_TEtest->Caret->Cursors->InSelCursor
	 = StringToCursor(Caret_CBXInSelC->Items->Strings[Caret_CBXInSelC->ItemIndex]);
	Caret_TEtest->Caret->Cursors->LeftMarginCursor
	 = StringToCursor(Caret_CBXLMC->Items->Strings[Caret_CBXLMC->ItemIndex]);
	Caret_TEtest->Caret->Cursors->TopMarginCursor
	 = StringToCursor(Caret_CBXTMC->Items->Strings[Caret_CBXTMC->ItemIndex]);
}

void TEditorExSetDialog::LoadVMM()
{
	AnsiString Section = "ViewMarks";
	VMM_LBColor->ItemIndex = 0;
	VMM_TEtest->SelStart = 0;
	VMM_TEtest->SelLength = 8;

	VMM_TEtest->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Color_BkColor", "clWhite"));
	VMM_TEtest->Font->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Color_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->Font->Style),Config->GetIniFileString(Section, "Colors_Color_Style", ""));
	VMM_TEtest->View->Colors->Ank->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Ank_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Ank->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Ank_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Ank->Style),Config->GetIniFileString(Section, "Colors_Ank_Style", ""));
	VMM_TEtest->View->Colors->Comment->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Comment_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Comment->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Comment_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Comment->Style),Config->GetIniFileString(Section, "Colors_Comment_Style", ""));
	VMM_TEtest->View->Colors->DBCS->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_DBCS_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->DBCS->Color = StringToColor(Config->GetIniFileString(Section, "Colors_DBCS_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->DBCS->Style),Config->GetIniFileString(Section, "Colors_DBCA_Style", ""));
	VMM_TEtest->View->Colors->Int->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Int_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Int->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Int_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Int->Style),Config->GetIniFileString(Section, "Colors_Int_Style", ""));
	VMM_TEtest->View->Colors->Str->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Str_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Str->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Str_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Str->Style),Config->GetIniFileString(Section, "Colors_Str_Style", ""));
	VMM_TEtest->View->Colors->Symbol->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Symbol_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Symbol->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Symbol_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Symbol->Style),Config->GetIniFileString(Section, "Colors_Symbol_Style", ""));
	VMM_TEtest->View->Colors->Reserve->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Reserve_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Reserve->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Reserve_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Reserve->Style),Config->GetIniFileString(Section, "Colors_Reserve_Style", ""));
	VMM_TEtest->View->Colors->Url->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Url_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Url->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Url_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Url->Style),Config->GetIniFileString(Section, "Colors_Url_Style", ""));
	VMM_TEtest->View->Colors->Mail->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Mail_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Mail->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Mail_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Mail->Style),Config->GetIniFileString(Section, "Colors_Mail_Style", ""));
	VMM_TEtest->View->Colors->Select->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Select_BkColor", "clWhite"));
	VMM_TEtest->View->Colors->Select->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Select_Color", "clBlack"));
	SetStrFontStyles(&(VMM_TEtest->View->Colors->Select->Style),Config->GetIniFileString(Section, "Colors_Select_Style", ""));

	VMM_EBHexPrefix->Text = Config->GetIniFileString(Section, "Hexprefix", "");
	VMM_TEtest->View->HexPrefix = VMM_EBHexPrefix->Text;
	VMM_EBQuotation->Text = Config->GetIniFileString(Section, "Quotation", "");
	VMM_TEtest->View->Quotation = VMM_EBQuotation->Text;
	VMM_EBComment->Text = Config->GetIniFileString(Section, "Commenter", "");
	VMM_TEtest->View->Commenter = VMM_EBComment->Text;

	VMM_CBMail->Checked = Config->GetIniFileInt(Section, "Mail", 0);
	VMM_TEtest->View->Mail = VMM_CBMail->Checked;
	VMM_CBUrl->Checked = Config->GetIniFileInt(Section, "Url", 0);
	VMM_TEtest->View->Url = VMM_CBUrl->Checked;
	VMM_CBCCCode->Checked = Config->GetIniFileInt(Section, "ControlCode", 0);
	VMM_TEtest->View->ControlCode = VMM_CBCCCode->Checked;

	VMM_CBEofMark->Checked = Config->GetIniFileInt(Section, "Marks_EofMark_Visible", 0);
	VMM_TEtest->Marks->EofMark->Visible = VMM_CBEofMark->Checked;
	VMM_TEtest->Marks->EofMark->Color = StringToColor(Config->GetIniFileString(Section, "Marks_EofMark_Color", "clBlack"));

	VMM_CBRetMark->Checked = Config->GetIniFileInt(Section, "Marks_RetMark_Visible", 0);
	VMM_TEtest->Marks->RetMark->Visible = VMM_CBRetMark->Checked;
	VMM_TEtest->Marks->RetMark->Color = StringToColor(Config->GetIniFileString(Section, "Marks_RetMark_Color", "clBlack"));

	VMM_CBUnderline->Checked = Config->GetIniFileInt(Section, "Marks_Underline_Visible", 0);
	VMM_TEtest->Marks->Underline->Visible = VMM_CBUnderline->Checked;
	VMM_TEtest->Marks->Underline->Color = StringToColor(Config->GetIniFileString(Section, "Marks_Underline_Color", "clBlack"));
	VMM_CBWrapMark->Checked = Config->GetIniFileInt(Section, "Marks_WrapMark_Visible", 0);
	VMM_TEtest->Marks->WrapMark->Visible = VMM_CBWrapMark->Checked;
	VMM_TEtest->Marks->WrapMark->Color = StringToColor(Config->GetIniFileString(Section, "Marks_WrapMark_Color", "clBlack"));
	VMM_CBFindMark->Checked = Config->GetIniFileInt(Section, "Marks_FindMark_Visible", 0);
	VMM_TEtest->ExMarks->FindMark->Visible = VMM_CBFindMark->Checked;
	VMM_TEtest->ExMarks->FindMark->Color = StringToColor(Config->GetIniFileString(Section, "Marks_FindMark_Color", "clBlack"));
	VMM_CBSpaceMark->Checked = Config->GetIniFileInt(Section, "Marks_SpaceMark_Visible", 0);
	VMM_TEtest->ExMarks->SpaceMark->Visible = VMM_CBSpaceMark->Checked;
	VMM_TEtest->ExMarks->SpaceMark->Color = StringToColor(Config->GetIniFileString(Section, "Marks_SpaceMark_Color", "clBlack"));
	VMM_CBDBSpaceMark->Checked = Config->GetIniFileInt(Section, "Marks_DBSpaceMark_Visible", 0);
	VMM_TEtest->ExMarks->DBSpaceMark->Visible = VMM_CBDBSpaceMark->Checked;
	VMM_TEtest->ExMarks->DBSpaceMark->Color = StringToColor(Config->GetIniFileString(Section, "Marks_DBSpaceMark_Color", "clBlack"));
	VMM_CBTabMark->Checked = Config->GetIniFileInt(Section, "Marks_TabMark_Visible", 0);
	VMM_TEtest->ExMarks->TabMark->Visible = VMM_CBTabMark->Checked;
	VMM_TEtest->ExMarks->TabMark->Color = StringToColor(Config->GetIniFileString(Section, "Marks_TabMark_Color", "clBlack"));

	if(Config->GetIniFileString(Section, "HitStyle", "hsDraw") == "hsDraw") {
   		VMM_RBhsDraw->Checked = true;
		VMM_TEtest->HitStyle = hsDraw;
	} else if(Config->GetIniFileString(Section, "HitStyle", "hsDraw") == "hsCaret") {
		VMM_RBhsCaret->Checked = true;
		VMM_TEtest->HitStyle = hsCaret;
	} else {
		VMM_RBhsSelect->Checked = true;
		VMM_TEtest->HitStyle = hsSelect;
	}

	VMM_LBColor->ItemIndex = 0;
	VMM_LBColorClick(this);
}

void TEditorExSetDialog::LoadLbRl()
{
	AnsiString Section = "LeftbarRuler";

	LbRl_LBlbRl->ItemIndex = 0;
	LbRl_ColorGrid->ForegroundIndex = 0;
	LbRl_ColorGrid->BackgroundIndex = 15;

	LbRl_TEtest->Leftbar->Color
    	= StringToColor(Config->GetIniFileString(Section, "Leftbar_Color", "clWindowText"));
	LbRl_TEtest->Leftbar->BkColor
    	= StringToColor(Config->GetIniFileString(Section, "Leftbar_BkColor", "clBtnFace"));

	LbRl_TEtest->Leftbar->Column = Config->GetIniFileInt(Section, "Leftbar_Column", 4);
	LbRl_CSEColumn->Value = LbRl_TEtest->Leftbar->Column;

	LbRl_CBLEdge->Checked = Config->GetIniFileInt(Section, "Leftbar_Edge", true);
	LbRl_TEtest->Leftbar->Edge = LbRl_CBLEdge->Checked;

	LbRl_TEtest->Leftbar->LeftMargin = Config->GetIniFileInt(Section, "Leftbar_LeftMargin", 8);
	LbRl_CSELeftMargin->Value = LbRl_TEtest->Leftbar->LeftMargin;


	LbRl_TEtest->Leftbar->RightMargin = Config->GetIniFileInt(Section, "Leftbar_RightMargin", 4);
	LbRl_CSERightMargin->Value = LbRl_TEtest->Leftbar->RightMargin;

	LbRl_CBLShowNumber->Checked = Config->GetIniFileInt(Section,"Leftbar_ShowNumber", true);
	LbRl_TEtest->Leftbar->ShowNumber = LbRl_CBLShowNumber->Checked;

	if(Config->GetIniFileString(Section, "Leftbar_ShowNumberMode", "nmRow") == "nmRow") {
		LbRl_TEtest->Leftbar->ShowNumberMode = nmRow;
    	LbRl_RBnmRow->Checked = true;
	} else {
		LbRl_TEtest->Leftbar->ShowNumberMode = nmLine;
		LbRl_RBnmLine->Checked = true;
	}

	LbRl_CBLVisible->Checked = Config->GetIniFileInt(Section, "Leftbar_Visible", true);
	LbRl_TEtest->Leftbar->Visible = LbRl_CBLVisible->Checked;

	LbRl_CBLZeroBase->Checked = Config->GetIniFileInt(Section, "Leftbar_ZeroBase", 0);
	LbRl_TEtest->Leftbar->ZeroBase = LbRl_CBLZeroBase->Checked;

	LbRl_CBLZeroLead->Checked = Config->GetIniFileInt(Section, "Leftbar_ZeroLead", 1);
    LbRl_TEtest->Leftbar->ZeroLead = LbRl_CBLZeroLead->Checked;


	LbRl_TEtest->Ruler->Color = StringToColor(
    	Config->GetIniFileString(Section, "Ruler_Color", "clBtnText"));
	LbRl_TEtest->Ruler->BkColor = StringToColor(
    	Config->GetIniFileString(Section, "Ruler_BkColor", "clBtnFace"));

	LbRl_CBREdge->Checked = Config->GetIniFileInt(Section, "Ruler_Edge", 1);
	LbRl_TEtest->Ruler->Edge = LbRl_CBREdge->Checked;

	if(Config->GetIniFileInt(Section, "Ruler_GaugeRange", 10) == 10) {
		LbRl_TEtest->Ruler->GaugeRange = 10;
		LbRl_RB10->Checked = true;
	} else {
		LbRl_TEtest->Ruler->GaugeRange = 8;
		LbRl_RB08->Checked = true;
	}

	LbRl_TEtest->Ruler->MarkColor = StringToColor(
    	Config->GetIniFileString(Section, "Ruler_MarkColor", "clBtnText"));

	LbRl_CBRVisible->Checked = Config->GetIniFileInt(Section, "Ruler_Visible", true);
	LbRl_TEtest->Ruler->Visible = LbRl_CBRVisible->Checked;

	LbRl_TEtest->Margin->Top = Config->GetIniFileInt(Section, "Margin_Top", 2);
	LbRlM_CSETop->Value = LbRl_TEtest->Margin->Top;

	LbRl_TEtest->Margin->Left = Config->GetIniFileInt(Section, "Margin_Left", 19);
	LbRlM_CSELeft->Value = LbRl_TEtest->Margin->Left;

	LbRl_TEtest->Margin->Line = Config->GetIniFileInt(Section, "Margin_Line", 0);
	LbRlM_CSELine->Value = LbRl_TEtest->Margin->Line;

	LbRl_TEtest->Margin->Character = Config->GetIniFileInt(Section, "Margin_Character", 0);
	LbRlM_CSECharacter->Value = LbRl_TEtest->Margin->Character;

	LbRl_TEtest->Imagebar->Visible = false;
	LbRl_LBlbRl->ItemIndex = 0;
	LbRl_LBlbRlClick(this);
}

void TEditorExSetDialog::LoadBRK()
{
	int j = 0;
    AnsiString Section = "Brackets";

	BRK_LBColor->Items->Clear();
	BRK_TEtest->View->Brackets->Clear();
	BRK_TEtest->Lines->Clear();

	while(Config->GetIniFileString(Section, "Bracket_RBracket"+IntToStr(j), "") != "") {
		BRK_TEtest->View->Brackets->Add();
		BRK_TEtest->View->BeginUpdate();

		/* Color, Style */
    	BRK_TEtest->View->Brackets->BracketItems[j]->ItemColor->BkColor
    		= StringToColor(Config->GetIniFileString(
            	Section, "Bracket_BkColor"+IntToStr(j), "clWhite"));
		BRK_TEtest->View->Brackets->BracketItems[j]->ItemColor->Color
    		= StringToColor(Config->GetIniFileString(
            	Section, "Bracket_Color"+IntToStr(j), "clBlack"));
	    SetStrFontStyles(&(BRK_TEtest->View->Brackets->BracketItems[j]->ItemColor->Style)
       		, Config->GetIniFileString(Section, "Bracket_Style"+IntToStr(j), ""));

		/* Right, Left String */
		BRK_TEtest->View->Brackets->BracketItems[j]->LeftBracket
    		= Config->GetIniFileString(Section, "Bracket_LBracket"+IntToStr(j), "Left");
	    BRK_TEtest->View->Brackets->BracketItems[j]->RightBracket
    		= Config->GetIniFileString(Section, "Bracket_RBracket"+IntToStr(j), "Right");

		BRK_TEtest->View->EndUpdate();
		BRK_LBColor->Items->Add(
        	Config->GetIniFileString(Section, "Bracket_LBracket"+IntToStr(j), "Left")
    		+ " " +
            Config->GetIniFileString(Section, "Bracket_RBracket"+IntToStr(j), "Right"));
	    BRK_TEtest->Lines->Add(
        	Config->GetIniFileString(Section, "Bracket_LBracket"+IntToStr(j), "Left")
    		+ "Closed String(被囲み文字)" +
        	Config->GetIniFileString(Section, "Bracket_RBracket"+IntToStr(j), "Right"));
	    j++;
	}
	BRK_LBColor->ItemIndex = 0;
	BRK_LBColorClick(this);
}

void TEditorExSetDialog::LoadRWL()
{
	AnsiString Section = "ReserveWordList";
    AnsiString Key = "RWL";
    Config->GetIniFileStringList(RWL_TEtest->Lines, Section, Key);
}

void TEditorExSetDialog::LoadWW()
{
	AnsiString Section = "WordWrap";

	WW_CBWordWrap->Checked = Config->GetIniFileInt(Section, "WordWrap", 0);
	WW_CBFollowPunc->Checked = Config->GetIniFileInt(Section, "WrapOption_FollowPunctuation", 0);
	WW_CBFollowRet->Checked = Config->GetIniFileInt(Section, "WrapOption_FollowRetMark", 0);
	WW_CBLeading->Checked = Config->GetIniFileInt(Section, "WrapOption_Leading", 0);
	WW_CBWordBreak->Checked = Config->GetIniFileInt(Section, "WrapOption_WordBreak", 0);
	WW_CSEWrapByte->Value = Config->GetIniFileInt(Section, "WrapOption_WrapByte", 80);

	WW_EBFollowStr->Text
    	= Config->GetIniFileString(Section, "WrapOption_FollowStr", FollowStr_default);
	WW_EBLeadStr->Text
    	= Config->GetIniFileString(Section, "WrapOption_LeadStr", LeadStr_default);
	WW_EBPunctationStr->Text
    	= Config->GetIniFileString(Section, "WrapOption_PunctuationStr", PunctationStr_default);

}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Write : 書き込み
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// 書き込みルート関数
//

void TEditorExSetDialog::SaveConfig()
{
	SaveFont();
	SaveCaret();
	SaveVMM();
	SaveLbRl();
	SaveBRK();
	SaveRWL();
	SaveWW();
}

void TEditorExSetDialog::SaveFont()
{
	AnsiString Section = "Font";

    Config->WriteIniFileString(Section, "Font_Name", Font_TEtest->Font->Name);
	Config->WriteIniFileInt(Section, "Font_Size", Font_TEtest->Font->Size);
	if(Font_TEtest->ScrollBars == ssNone) {
		Config->WriteIniFileString(Section, "ScrollBar", "ssNone");
	} else if(Font_TEtest->ScrollBars == ssHorizontal) {
		Config->WriteIniFileString(Section, "ScrollBar", "ssHorizontal");
	} else if(Font_TEtest->ScrollBars == ssVertical) {
		Config->WriteIniFileString(Section, "ScrollBar", "ssVertical");
	} else if(Font_TEtest->ScrollBars == ssBoth) {
		Config->WriteIniFileString(Section, "ScrollBar", "ssBoth");
	}
}
void TEditorExSetDialog::SaveCaret()
{
	AnsiString Section = "Caret";

	Config->WriteIniFileInt(Section, "Caret_AutoCursor", Caret_CBAutoCursor->Checked);
	Config->WriteIniFileInt(Section, "Caret_AutoIndent", Caret_CBAutoIndent->Checked);
	Config->WriteIniFileInt(Section, "Caret_BackSpaceUnIndent", Caret_CBBackSpace->Checked);
	Config->WriteIniFileString(Section, "Caret_Cursors_DefaultCursor",
		CursorToString(Caret_TEtest->Caret->Cursors->DefaultCursor));
	Config->WriteIniFileString(Section, "Caret_Cursors_DragSelCopyCursor",
		CursorToString(Caret_TEtest->Caret->Cursors->DragSelCopyCursor));
	Config->WriteIniFileString(Section, "Caret_Cursors_DragSelCursor",
		CursorToString(Caret_TEtest->Caret->Cursors->DragSelCursor));
	Config->WriteIniFileString(Section, "Caret_Cursors_InSelCursor",
		CursorToString(Caret_TEtest->Caret->Cursors->InSelCursor));
	Config->WriteIniFileString(Section, "Caret_Cursors_LeftMarginCursor",
		CursorToString(Caret_TEtest->Caret->Cursors->LeftMarginCursor));
	Config->WriteIniFileString(Section, "Caret_Cursors_TopMarginCursor",
		CursorToString(Caret_TEtest->Caret->Cursors->TopMarginCursor));
	Config->WriteIniFileInt(Section, "Caret_FreeCaret", Caret_CBFreeCaret->Checked);
	Config->WriteIniFileInt(Section, "Caret_FreeRow", Caret_CBFreeRow->Checked);
	Config->WriteIniFileInt(Section, "Caret_InTab", Caret_CBInTab->Checked);
	Config->WriteIniFileInt(Section, "Caret_KeepCaret", Caret_CBKeepCaret->Checked);
	Config->WriteIniFileInt(Section, "Caret_LockScroll", Caret_CBLockScroll->Checked);
	Config->WriteIniFileInt(Section, "Caret_NextLine", Caret_CBNextLine->Checked);
	Config->WriteIniFileInt(Section, "Caret_PrevSpaceIndent", Caret_CBPrevSpace->Checked);
	Config->WriteIniFileInt(Section, "Caret_RowSelect", Caret_CBRowSelect->Checked);
	if(Caret_RBdmManual->Checked)
    	Config->WriteIniFileString(Section, "Caret_SelDragMode", "dmManual");
	else
    	Config->WriteIniFileString(Section, "Caret_SelDragMode", "dmAutomatic");
	Config->WriteIniFileInt(Section, "Caret_SelMove", Caret_CBSelMove->Checked);
	Config->WriteIniFileInt(Section, "Caret_SoftTab", Caret_CBSoftTab->Checked);
	if(Caret_RBcsDefault->Checked)
    	Config->WriteIniFileString(Section, "Caret_Style", "csDefault");
	else
    	Config->WriteIniFileString(Section, "Caret_Style", "csBrief");
	Config->WriteIniFileInt(Section, "Caret_TabIndent", Caret_CBTabIndent->Checked);
	Config->WriteIniFileInt(Section, "Caret_TabSpaceCount", Caret_CSETabSpace->Value);
}

void TEditorExSetDialog::SaveVMM()
{
	AnsiString Section = "ViewMarks";

	Config->WriteIniFileString(Section, "Commenter", VMM_TEtest->View->Commenter);
	Config->WriteIniFileInt(Section, "ControlCode", VMM_CBCCCode->Checked);
	Config->WriteIniFileString(Section, "HexPrefix", VMM_TEtest->View->HexPrefix);
	Config->WriteIniFileInt(Section, "Mail", VMM_CBMail->Checked);
	Config->WriteIniFileString(Section, "Quotation", VMM_TEtest->View->Quotation);
	Config->WriteIniFileInt(Section, "Url", VMM_CBUrl->Checked);

	Config->WriteIniFileString(Section, "Colors_Color_BkColor",
		ColorToString(VMM_TEtest->Color));
	Config->WriteIniFileString(Section, "Colors_Color_Color",
		ColorToString(VMM_TEtest->Font->Color));
	Config->WriteIniFileString(Section, "Colors_Color_Style",
		FontStyleToStr(VMM_TEtest->Font->Style));
	Config->WriteIniFileString(Section, "Colors_Ank_BkColor",
		ColorToString(VMM_TEtest->View->Colors->Ank->BkColor));
	Config->WriteIniFileString(Section, "Colors_Ank_Color",
		ColorToString(VMM_TEtest->View->Colors->Ank->Color));
	Config->WriteIniFileString(Section, "Colors_Ank_Style",
		FontStyleToStr(VMM_TEtest->View->Colors->Ank->Style));
	Config->WriteIniFileString(Section, "Colors_Comment_BkColor", 
		ColorToString(VMM_TEtest->View->Colors->Comment->BkColor));
	Config->WriteIniFileString(Section, "Colors_Comment_Color", 
		ColorToString(VMM_TEtest->View->Colors->Comment->Color));
	Config->WriteIniFileString(Section, "Colors_Comment_Style", 
		FontStyleToStr(VMM_TEtest->View->Colors->Comment->Style));
	Config->WriteIniFileString(Section, "Colors_DBCS_BkColor", 
		ColorToString(VMM_TEtest->View->Colors->DBCS->BkColor));
	Config->WriteIniFileString(Section, "Colors_DBCS_Color", 
		ColorToString(VMM_TEtest->View->Colors->DBCS->Color));
	Config->WriteIniFileString(Section, "Colors_DBCS_Style", 
		FontStyleToStr(VMM_TEtest->View->Colors->DBCS->Style));
	Config->WriteIniFileString(Section, "Colors_Int_BkColor", 
		ColorToString(VMM_TEtest->View->Colors->Int->BkColor));
	Config->WriteIniFileString(Section, "Colors_Int_Color", 
		ColorToString(VMM_TEtest->View->Colors->Int->Color));
	Config->WriteIniFileString(Section, "Colors_Int_Style", 
		FontStyleToStr(VMM_TEtest->View->Colors->Int->Style));
	Config->WriteIniFileString(Section, "Colors_Str_BkColor", 
		ColorToString(VMM_TEtest->View->Colors->Str->BkColor));
	Config->WriteIniFileString(Section, "Colors_Str_Color", 
		ColorToString(VMM_TEtest->View->Colors->Str->Color));
	Config->WriteIniFileString(Section, "Colors_Str_Style", 
		FontStyleToStr(VMM_TEtest->View->Colors->Str->Style));
	Config->WriteIniFileString(Section, "Colors_Symbol_BkColor", 
		ColorToString(VMM_TEtest->View->Colors->Symbol->BkColor));
	Config->WriteIniFileString(Section, "Colors_Symbol_Color", 
		ColorToString(VMM_TEtest->View->Colors->Symbol->Color));
	Config->WriteIniFileString(Section, "Colors_Symbol_Style", 
		FontStyleToStr(VMM_TEtest->View->Colors->Symbol->Style));
	Config->WriteIniFileString(Section, "Colors_Reserve_BkColor", 
		ColorToString(VMM_TEtest->View->Colors->Reserve->BkColor));
	Config->WriteIniFileString(Section, "Colors_Reserve_Color", 
		ColorToString(VMM_TEtest->View->Colors->Reserve->Color));
	Config->WriteIniFileString(Section, "Colors_Reserve_Style",
		FontStyleToStr(VMM_TEtest->View->Colors->Reserve->Style));
	Config->WriteIniFileString(Section, "Colors_Url_BkColor",
		ColorToString(VMM_TEtest->View->Colors->Url->BkColor));
	Config->WriteIniFileString(Section, "Colors_Url_Color",
		ColorToString(VMM_TEtest->View->Colors->Url->Color));
	Config->WriteIniFileString(Section, "Colors_Url_Style",
		FontStyleToStr(VMM_TEtest->View->Colors->Url->Style));
	Config->WriteIniFileString(Section, "Colors_Mail_BkColor",
		ColorToString(VMM_TEtest->View->Colors->Mail->BkColor));
	Config->WriteIniFileString(Section, "Colors_Mail_Color",
		ColorToString(VMM_TEtest->View->Colors->Mail->Color));
	Config->WriteIniFileString(Section, "Colors_Mail_Style",
		FontStyleToStr(VMM_TEtest->View->Colors->Mail->Style));
	Config->WriteIniFileString(Section, "Colors_Select_BkColor",
		ColorToString(VMM_TEtest->View->Colors->Select->BkColor));
	Config->WriteIniFileString(Section, "Colors_Select_Color",
		ColorToString(VMM_TEtest->View->Colors->Select->Color));
	Config->WriteIniFileString(Section, "Colors_Select_Style",
		FontStyleToStr(VMM_TEtest->View->Colors->Select->Style));


	Config->WriteIniFileInt(Section, "Marks_EofMark_Visible",
		VMM_TEtest->Marks->EofMark->Visible);
	Config->WriteIniFileString(Section, "Marks_EofMark_Color",
		ColorToString(VMM_TEtest->Marks->EofMark->Color));

	Config->WriteIniFileInt(Section, "Marks_RetMark_Visible",
		VMM_TEtest->Marks->RetMark->Visible);
	Config->WriteIniFileString(Section, "Marks_RetMark_Color",
		ColorToString(VMM_TEtest->Marks->RetMark->Color));

	Config->WriteIniFileInt(Section, "Marks_Underline_Visible",
		VMM_TEtest->Marks->Underline->Visible);
	Config->WriteIniFileString(Section, "Marks_Underline_Color",
		ColorToString(VMM_TEtest->Marks->Underline->Color));
	Config->WriteIniFileInt(Section, "Marks_WrapMark_Visible",
		VMM_TEtest->Marks->WrapMark->Visible);
	Config->WriteIniFileString(Section, "Marks_WrapMark_Color",
		ColorToString(VMM_TEtest->Marks->WrapMark->Color));
	Config->WriteIniFileInt(Section, "Marks_FindMark_Visible",
		VMM_TEtest->ExMarks->FindMark->Visible);
	Config->WriteIniFileString(Section, "Marks_FindMark_Color",
		ColorToString(VMM_TEtest->ExMarks->FindMark->Color));
	Config->WriteIniFileInt(Section, "Marks_SpaceMark_Visible",
		VMM_TEtest->ExMarks->SpaceMark->Visible);
	Config->WriteIniFileString(Section, "Marks_SpaceMark_Color",
		ColorToString(VMM_TEtest->ExMarks->SpaceMark->Color));
	Config->WriteIniFileInt(Section, "Marks_DBSpaceMark_Visible",
		VMM_TEtest->ExMarks->DBSpaceMark->Visible);
	Config->WriteIniFileString(Section, "Marks_DBSpaceMark_Color",
		ColorToString(VMM_TEtest->ExMarks->DBSpaceMark->Color));
	Config->WriteIniFileInt(Section, "Marks_TabMark_Visible",
		VMM_TEtest->ExMarks->TabMark->Visible);
	Config->WriteIniFileString(Section, "Marks_TabMark_Color",
		ColorToString(VMM_TEtest->ExMarks->TabMark->Color));

	switch(VMM_TEtest->HitStyle) {
		case hsSelect:Config->WriteIniFileString(Section, "HitStyle", "hsSelect");break;
    	case hsDraw:Config->WriteIniFileString(Section, "HitStyle", "hsDraw");break;
	    case hsCaret:Config->WriteIniFileString(Section, "HitStyle", "hsCaret");break;
	}
}

void TEditorExSetDialog::SaveLbRl()
{
	AnsiString Section = "LeftbarRuler";
	Config->WriteIniFileString(Section, "Leftbar_BkColor"
		,ColorToString(LbRl_TEtest->Leftbar->BkColor));
	Config->WriteIniFileString(Section, "Leftbar_Color"
		,ColorToString(LbRl_TEtest->Leftbar->Color));
	Config->WriteIniFileInt(Section, "Leftbar_Column"
		,LbRl_TEtest->Leftbar->Column);
	Config->WriteIniFileInt(Section, "Leftbar_Edge"
		,LbRl_TEtest->Leftbar->Edge);
	Config->WriteIniFileInt(Section, "Leftbar_LeftMargin"
		,LbRl_TEtest->Leftbar->LeftMargin);
	Config->WriteIniFileInt(Section, "Leftbar_RightMargin"
		,LbRl_TEtest->Leftbar->RightMargin);
	Config->WriteIniFileInt(Section, "Leftbar_ShowNumber"
		,LbRl_TEtest->Leftbar->ShowNumber);

	if(LbRl_TEtest->Leftbar->ShowNumberMode == nmRow)
		Config->WriteIniFileString(Section, "Leftbar_ShowNumberMode", "nmRow");
	else
		Config->WriteIniFileString(Section, "Leftbar_ShowNumberMode", "nmLine");

	Config->WriteIniFileInt(Section, "Leftbar_Visible",
		LbRl_TEtest->Leftbar->Visible);
	Config->WriteIniFileInt(Section, "Leftbar_ZeroBase",
		LbRl_TEtest->Leftbar->ZeroBase);
	Config->WriteIniFileInt(Section, "Leftbar_ZeroLead",
		LbRl_TEtest->Leftbar->ZeroLead);

	Config->WriteIniFileString(Section, "Ruler_BkColor",
		ColorToString(LbRl_TEtest->Ruler->BkColor));
	Config->WriteIniFileString(Section, "Ruler_Color",
		ColorToString(LbRl_TEtest->Ruler->Color));

	Config->WriteIniFileInt(Section, "Ruler_Edge",
		LbRl_TEtest->Ruler->Edge);
	Config->WriteIniFileInt(Section, "Ruler_GaugeRange",
		LbRl_TEtest->Ruler->GaugeRange);
	Config->WriteIniFileString(Section, "Ruler_MarkColor",
		ColorToString(LbRl_TEtest->Ruler->MarkColor));
	Config->WriteIniFileInt(Section, "Ruler_Visible",
		LbRl_TEtest->Ruler->Visible);

	Config->WriteIniFileInt(Section, "Margin_Character", LbRlM_CSECharacter->Value);
	Config->WriteIniFileInt(Section, "Margin_Left", LbRlM_CSELeft->Value);
	Config->WriteIniFileInt(Section, "Margin_Line", LbRlM_CSELine->Value);
	Config->WriteIniFileInt(Section, "Margin_Top", LbRlM_CSETop->Value);
}

void TEditorExSetDialog::SaveBRK()
{
    AnsiString Section = "Brackets";
	int j = 0;

    AnsiString Key = "Bracket_RBracket";
	AnsiString buf = Config->GetIniFileString(Section, Key + IntToStr(j), "");

	//取得文字列が""(取得失敗時のデフォルト）でない限り、
	//値として無効な文字列（""）をセットし続ける。つまり、クリア作業。
	while(buf != "") {
		Config->WriteIniFileString(Section, Key + IntToStr(j), "");
		j++;
		buf = Config->GetIniFileString(Section, Key + IntToStr(j), "");
	}

	for(j = 0;j < BRK_TEtest->View->Brackets->Count;j++) {
	Config->WriteIniFileString(Section, "Bracket_BkColor"+IntToStr(j),
		ColorToString(BRK_TEtest->View->Brackets->BracketItems[j]->ItemColor->BkColor));
	Config->WriteIniFileString(Section, "Bracket_Color"+IntToStr(j),
		ColorToString(BRK_TEtest->View->Brackets->BracketItems[j]->ItemColor->Color));
	Config->WriteIniFileString(Section, "Bracket_Style"+IntToStr(j),
		FontStyleToStr(BRK_TEtest->View->Brackets->BracketItems[j]->ItemColor->Style));
	Config->WriteIniFileString(Section, "Bracket_LBracket"+IntToStr(j),
		BRK_TEtest->View->Brackets->BracketItems[j]->LeftBracket);
	Config->WriteIniFileString(Section, "Bracket_RBracket"+IntToStr(j),
		BRK_TEtest->View->Brackets->BracketItems[j]->RightBracket);
	}
}

void TEditorExSetDialog::SaveRWL()
{

	AnsiString Section = "ReserveWordList";
    AnsiString Key = "RWL";
    Config->WriteIniFileStringList(RWL_TEtest->Lines, Section, Key);
}

void TEditorExSetDialog::SaveWW()
{
	AnsiString Section = "WordWrap";

	Config->WriteIniFileInt(Section, "WordWrap", WW_CBWordWrap->Checked);
	Config->WriteIniFileInt(Section, "WrapOption_FollowPunctuation", WW_CBFollowPunc->Checked);
	Config->WriteIniFileInt(Section, "WrapOption_FollowRetMark", WW_CBFollowRet->Checked);
	Config->WriteIniFileString(Section, "WrapOption_FollowStr", WW_EBFollowStr->Text);
	Config->WriteIniFileInt(Section, "WrapOption_Leading", WW_CBLeading->Checked);
	Config->WriteIniFileString(Section, "WrapOption_LeadStr", WW_EBLeadStr->Text);
	Config->WriteIniFileString(Section, "WrapOption_PunctuationStr", WW_EBPunctationStr->Text);
	Config->WriteIniFileInt(Section, "WrapOption_WordBreak", WW_CBWordBreak->Checked);
	Config->WriteIniFileInt(Section, "WrapOption_WrapByte", WW_CSEWrapByte->Value);
}

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// ダイアログボックスの動作
//

void __fastcall TEditorExSetDialog::FormShow(TObject *Sender)
{
	LoadConfig();
}
//---------------------------------------------------------------------------

/* Help Button */
void __fastcall TEditorExSetDialog::editorex_set_dlg_HelpClick(TObject *Sender)
{ /* Application->HelpContext(24); */ }

void __fastcall TEditorExSetDialog::FormDestroy(TObject *Sender)
{
	delete Config;
}
//---------------------------------------------------------------------------

void __fastcall TEditorExSetDialog::editorex_set_dlg_OKClick(
      TObject *Sender)
{
	SaveConfig();
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------


