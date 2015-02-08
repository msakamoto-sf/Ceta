//---------------------------------------------------------------------------
#include <vcl.h>
#include <dir.h>
#pragma hdrstop

#include "ana_mdichild.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HEditor"
#pragma link "EditorEx"
#pragma link "HKeyMacro"
#pragma resource "*.dfm"
TAnaMDIChild *AnaMDIChild;
//---------------------------------------------------------------------------
__fastcall TAnaMDIChild::TAnaMDIChild(TComponent* Owner,
    	AnsiString fpFileName, AnsiString fpCIFile, AnsiString fpTemplateFile)
    : TForm(Owner)
{
	SetfpFileName(fpFileName);
    SetfpCIFile(fpCIFile);
    _fpTemplateFile = fpTemplateFile;

	search_styles.Clear();
	ex_search_styles.Clear();
	f_find_type = true;
	f_replace_all = false;

	SetFindStrBuffer("");
    SetReplaceStrBuffer("");
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//
//
// AnaMDIChild 汎用インターフェイス関数群
//
//
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// GetFileName
//
AnsiString TAnaMDIChild::GetFileName()
{ return GetfpFileName(); }

/////////////////////////////////////////////////////////////////////////////
//
// FileLoad
//
void TAnaMDIChild::FileLoad(AnsiString fname)
{
	if(_fpTemplateFile != "") EditorEx1->Lines->LoadFromFile(_fpTemplateFile);
	else {
		if(FileExists(GetfpFileName()))
    		EditorEx1->Lines->LoadFromFile(GetfpFileName());
	    else EditorEx1->Lines->Clear();
    }

	Caption = GetfpFileName();
	EditorEx1->Modified = false;
}

/////////////////////////////////////////////////////////////////////////////
//
// FileReload
//
void TAnaMDIChild::FileReload()
{
	int r_buf = EditorEx1->Row;
	EditorEx1->Lines->LoadFromFile(GetfpFileName());
    EditorEx1->Row = r_buf;
}

/////////////////////////////////////////////////////////////////////////////
//
// FileSave
//
void TAnaMDIChild::FileSave(AnsiString fpFileNameBuffer)
{
	if(fpFileNameBuffer == "")
    	EditorEx1->Lines->SaveToFile(GetfpFileName());
    else {
    	EditorEx1->Lines->SaveToFile(fpFileNameBuffer);
        SetfpFileName(fpFileNameBuffer);
	}
   	//ウインドウキャプション、変更フラグのクリア
	Caption = GetfpFileName();
	EditorEx1->Modified = false;
}

/////////////////////////////////////////////////////////////////////////////
//
// FilePrint
//
void TAnaMDIChild::FilePrint()
{
	MessageBox(Handle, "Not Supported.", "未サポート", MB_OK | MB_ICONINFORMATION);
	return;
}

/////////////////////////////////////////////////////////////////////////////
//
// EditUndo
//
void TAnaMDIChild::EditUndo()
{ MEditUndoClick(this); }

/////////////////////////////////////////////////////////////////////////////
//
// EditRedo
//
void TAnaMDIChild::EditRedo()
{ MEditRedoClick(this); }

/////////////////////////////////////////////////////////////////////////////
//
// EditCut
//
void TAnaMDIChild::EditCut()
{ MEditCutClick(this); }

/////////////////////////////////////////////////////////////////////////////
//
// EditCopy
//
void TAnaMDIChild::EditCopy()
{ MEditCopyClick(this); }

/////////////////////////////////////////////////////////////////////////////
//
// EditPaste
//
void TAnaMDIChild::EditPaste()
{ MEditPasteClick(this); }

/////////////////////////////////////////////////////////////////////////////
//
// SearchFind
//
void TAnaMDIChild::SearchFind()
{ MSearchFindClick(this); }

/////////////////////////////////////////////////////////////////////////////
//
// SearchReplace
//
void TAnaMDIChild::SearchReplace()
{ MSearchReplaceClick(this); }

/////////////////////////////////////////////////////////////////////////////
//
// UpdateStatus
//
void TAnaMDIChild::UpdateStatus()
{
	UpdateStatusInternal();
}

/////////////////////////////////////////////////////////////////////////////
//
// IdlingHandler
//
void TAnaMDIChild::IdlingHandler()
{
	StatusBar1->Panels->Items[0]->Text = IntToStr(EditorEx1->Row) + " 行";
	StatusBar1->Panels->Items[1]->Text = IntToStr(EditorEx1->Col) + " 桁";
	if(EditorEx1->Modified) StatusBar1->Panels->Items[3]->Text = "変更あり";
	else StatusBar1->Panels->Items[3]->Text = "";
	if(EditorEx1->ReadOnly) {
    	StatusBar1->Panels->Items[4]->Text = "RO";
        StatusBar1->Panels->Items[4]->Bevel = pbLowered;
    } else {
    	StatusBar1->Panels->Items[4]->Text = "RW";
        StatusBar1->Panels->Items[4]->Bevel = pbNone;
    }
    StatusBar1->Panels->Items[6]->Text = "検索バッファ>> " + GetFindStrBuffer();

    switch(KeyMacro1->Status) {
        case msWaiting:
        	MEditMacroRec->Enabled = true;
            MEditMacroExec->Enabled = true;
        	MEditMacroClear->Enabled = true;
        	break;
        case msRunning:
        	MEditMacroRec->Enabled = false;
            MEditMacroExec->Enabled = false;
            MEditMacroClear->Enabled = false;
            break;
    	case msNone:
        case msRecording:
        default:
			MEditMacroRec->Enabled = true;
            MEditMacroExec->Enabled = false;
            MEditMacroClear->Enabled = false;
    }
}

//
//
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//
//
// ここまで。
//
//
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// 置換 内部実装関数群
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// eReplace(), ReplaceEditor() : Internal Function
//

void TAnaMDIChild::eReplace(void)
{
	TEditorHitStyle hit_style_buf;
	TPoint CaretPoint;
    TForm *MsgForm;
	int result;
	TMsgDlgButtons mdb;

    // hsCaret による検索一致文字列表現は、置き換え確認ダイアログが出たとき
    // TEditor がフォーカスを失い、キャレットが消えるので、置き換えには
    // 使えないので HitStyle の保存・復帰処理を行う。
    hit_style_buf = EditorEx1->HitStyle;
	EditorEx1->HitStyle = hsDraw;

	try {
	while(EditorEx1->Search(GetFindStrBuffer(), search_styles)) {
    	if(search_styles.Contains(sfrReplaceConfirm)) {
			// get screen position
			GetCaretPos(&CaretPoint);
			CaretPoint = EditorEx1->ClientToScreen(CaretPoint);
			// TMessageForm.Create fc Dialogs.pas
            mdb.Clear();mdb << mbYes << mbNo << mbCancel << mbAll;
			MsgForm = CreateMessageDialog(
    			GetFindStrBuffer() + " を\n" + GetReplaceStrBuffer() + "に置き換えますか？",
				mtConfirmation, mdb);
            try {
            	MsgForm->Top = CaretPoint.y - MsgForm->Height - EditorEx1->RowHeight;
                if(MsgForm->Top < 0)
                	MsgForm->Top = CaretPoint.y + EditorEx1->RowHeight;
                MsgForm->Left = CaretPoint.x + 2;
                if(MsgForm->Left + MsgForm->Width > Screen->Width)
                	MsgForm->Left = Screen->Width - MsgForm->Width - 4;

                result = MsgForm->ShowModal();
            } __finally {
            	MsgForm->Free();
            }
            switch(result) {
            	case mrYes: ReplaceEditor();break;
                case mrNo: break;
                case mrCancel:
                	EditorEx1->HitStyle = hit_style_buf;
                    return;
            	case mrAll:
            		ReplaceEditor();
                    f_replace_all = true;
                    search_styles >> sfrReplaceConfirm;
                	break;
            }
    	} else ReplaceEditor();

		if(!f_replace_all) break;
    }
	} __finally {
    	EditorEx1->HitStyle = hit_style_buf;
    }
	return;
}

void TAnaMDIChild::ReplaceEditor(void)
{
	if(EditorEx1->HitStyle != hsSelect) EditorEx1->HitToSelected();
	EditorEx1->SelText = GetReplaceStrBuffer();
	if(!search_styles.Contains(sfrDown))
    	EditorEx1->SelStart -= GetReplaceStrBuffer().Length();
}

//
/////////////////////////////////////////////////////////////////////////////
//
// Special
//
/////////////////////////////////////////////////////////////////////////////

void TAnaMDIChild::UpdateStatusInternal(void)
{
	LoadConfig();
	return;
}

void TAnaMDIChild::LoadConfig()
{
	Config = new TConfig(GetfpCIFile());
    LoadFont();
    LoadCaret();
    LoadVMM();
    LoadLbRl();
    LoadBRK();
    LoadRWL();
    LoadWW();
	delete Config;
}
void TAnaMDIChild::LoadFont()
{
	AnsiString Section = "Font";

	EditorEx1->Font->Name = Config->GetIniFileString(Section, "Font_Name", "ＭＳ ゴシック");
	EditorEx1->Font->Size = Config->GetIniFileInt(Section, "Font_Size", 11);
    AnsiString scroll = Config->GetIniFileString(Section, "ScrollBar", "ssNone");
	if(scroll == "ssNone") {
	    EditorEx1->ScrollBars = ssNone;
	} else if(scroll == "ssHorizontal") {
	    EditorEx1->ScrollBars = ssHorizontal;
	} else if(scroll == "ssVertical") {
	    EditorEx1->ScrollBars = ssVertical;
	} else if(scroll == "ssBoth") {
	    EditorEx1->ScrollBars = ssBoth;
	}
}
void TAnaMDIChild::LoadCaret()
{
	AnsiString Section = "Caret";

	EditorEx1->Caret->FreeCaret = Config->GetIniFileInt(Section, "Caret_FreeCaret", true);
	EditorEx1->Caret->FreeRow = Config->GetIniFileInt(Section, "Caret_FreeRow", false);
	EditorEx1->Caret->AutoIndent = Config->GetIniFileInt(Section, "Caret_AutoIndent", true);
	EditorEx1->Caret->BackSpaceUnIndent = Config->GetIniFileInt(Section, "Caret_BackSpaceUnIndent", true);
	EditorEx1->Caret->InTab = Config->GetIniFileInt(Section, "Caret_InTab", 0);
	EditorEx1->Caret->KeepCaret = Config->GetIniFileInt(Section, "Caret_KeepCaret", 0);
	EditorEx1->Caret->LockScroll = Config->GetIniFileInt(Section, "Caret_LockScroll", 0);
	EditorEx1->Caret->NextLine = Config->GetIniFileInt(Section, "Caret_NextLine", 0);
	EditorEx1->Caret->TabIndent = Config->GetIniFileInt(Section, "Caret_TabIndent", 0);
	EditorEx1->Caret->PrevSpaceIndent = Config->GetIniFileInt(Section, "Caret_PrevSpaceIndent", 0);
	EditorEx1->Caret->SoftTab = Config->GetIniFileInt(Section, "Caret_SoftTab", 0);
	EditorEx1->Caret->TabSpaceCount = Config->GetIniFileInt(Section, "Caret_TabSpaceCount", 8);

	if(Config->GetIniFileString(Section, "Caret_Style", "csBrief")  == "csBrief")
		EditorEx1->Caret->Style = csBrief;
	else EditorEx1->Caret->Style = Heditor::csDefault;
	EditorEx1->Caret->RowSelect = Config->GetIniFileInt(Section, "Caret_RowSelect", true);
	EditorEx1->Caret->SelMove = Config->GetIniFileInt(Section, "Caret_SelMove", true);
	if(Config->GetIniFileString(Section, "Caret_SelDragMode", "dmAutomatic") == "dmAutomatic")
		EditorEx1->Caret->SelDragMode = dmAutomatic;
	else EditorEx1->Caret->SelDragMode = dmManual;

	EditorEx1->Caret->AutoCursor = Config->GetIniFileInt(Section, "Caret_AutoCursor", true);
	EditorEx1->Caret->Cursors->DefaultCursor = StringToCursor(
    	Config->GetIniFileString(Section, "Caret_Cursors_DefaultCursor", "crIBeam"));
	EditorEx1->Caret->Cursors->DragSelCursor = StringToCursor(
		Config->GetIniFileString(Section, "Caret_Cursors_DragSelCopyCursor", "crDrag"));
	EditorEx1->Caret->Cursors->DragSelCopyCursor = StringToCursor(
    	Config->GetIniFileString(Section, "Caret_Cursors_DragSelCursor", "crDrag"));
	EditorEx1->Caret->Cursors->InSelCursor = StringToCursor(
    	Config->GetIniFileString(Section, "Caret_Cursors_InSelCursor", "crDefault"));
	EditorEx1->Caret->Cursors->LeftMarginCursor = StringToCursor(
    	Config->GetIniFileString(Section, "Caret_Cursors_LeftMarginCursor", "crArrow"));
	EditorEx1->Caret->Cursors->TopMarginCursor = StringToCursor(
    	Config->GetIniFileString(Section, "Caret_Cursors_TopMarginCursor", "crArrow"));
}
void TAnaMDIChild::LoadVMM()
{
	AnsiString Section = "ViewMarks";

	EditorEx1->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Color_BkColor", "clWhite"));
	EditorEx1->Font->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Color_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->Font->Style),Config->GetIniFileString(Section, "Colors_Color_Style", ""));
	EditorEx1->View->Colors->Ank->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Ank_BkColor", "clWhite"));
	EditorEx1->View->Colors->Ank->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Ank_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Ank->Style),Config->GetIniFileString(Section, "Colors_Ank_Style", ""));
	EditorEx1->View->Colors->Comment->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Comment_BkColor", "clWhite"));
	EditorEx1->View->Colors->Comment->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Comment_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Comment->Style),Config->GetIniFileString(Section, "Colors_Comment_Style", ""));
	EditorEx1->View->Colors->DBCS->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_DBCS_BkColor", "clWhite"));
	EditorEx1->View->Colors->DBCS->Color = StringToColor(Config->GetIniFileString(Section, "Colors_DBCS_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->DBCS->Style),Config->GetIniFileString(Section, "Colors_DBCA_Style", ""));
	EditorEx1->View->Colors->Int->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Int_BkColor", "clWhite"));
	EditorEx1->View->Colors->Int->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Int_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Int->Style),Config->GetIniFileString(Section, "Colors_Int_Style", ""));
	EditorEx1->View->Colors->Str->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Str_BkColor", "clWhite"));
	EditorEx1->View->Colors->Str->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Str_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Str->Style),Config->GetIniFileString(Section, "Colors_Str_Style", ""));
	EditorEx1->View->Colors->Symbol->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Symbol_BkColor", "clWhite"));
	EditorEx1->View->Colors->Symbol->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Symbol_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Symbol->Style),Config->GetIniFileString(Section, "Colors_Symbol_Style", ""));
	EditorEx1->View->Colors->Reserve->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Reserve_BkColor", "clWhite"));
	EditorEx1->View->Colors->Reserve->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Reserve_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Reserve->Style),Config->GetIniFileString(Section, "Colors_Reserve_Style", ""));
	EditorEx1->View->Colors->Url->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Url_BkColor", "clWhite"));
	EditorEx1->View->Colors->Url->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Url_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Url->Style),Config->GetIniFileString(Section, "Colors_Url_Style", ""));
	EditorEx1->View->Colors->Mail->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Mail_BkColor", "clWhite"));
	EditorEx1->View->Colors->Mail->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Mail_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Mail->Style),Config->GetIniFileString(Section, "Colors_Mail_Style", ""));
	EditorEx1->View->Colors->Select->BkColor = StringToColor(Config->GetIniFileString(Section, "Colors_Select_BkColor", "clWhite"));
	EditorEx1->View->Colors->Select->Color = StringToColor(Config->GetIniFileString(Section, "Colors_Select_Color", "clBlack"));
	SetStrFontStyles(&(EditorEx1->View->Colors->Select->Style),Config->GetIniFileString(Section, "Colors_Select_Style", ""));

	EditorEx1->View->HexPrefix = Config->GetIniFileString(Section, "Hexprefix", "");
	EditorEx1->View->Quotation = Config->GetIniFileString(Section, "Quotation", "");
	EditorEx1->View->Commenter = Config->GetIniFileString(Section, "Commenter", "");

	EditorEx1->View->Mail = Config->GetIniFileInt(Section, "Mail", 0);
	EditorEx1->View->Url = Config->GetIniFileInt(Section, "Url", 0);
	EditorEx1->View->ControlCode = Config->GetIniFileInt(Section, "ControlCode", 0);

	EditorEx1->Marks->EofMark->Visible =
		Config->GetIniFileInt(Section, "Marks_EofMark_Visible", 0);
	EditorEx1->Marks->EofMark->Color = StringToColor(
		Config->GetIniFileString(Section, "Marks_EofMark_Color", "clBlack"));
	EditorEx1->Marks->RetMark->Visible =
		Config->GetIniFileInt(Section, "Marks_RetMark_Visible", 0);
    EditorEx1->Marks->RetMark->Color = StringToColor(
		Config->GetIniFileString(Section, "Marks_RetMark_Color", "clBlack"));
	EditorEx1->Marks->Underline->Visible
	 = Config->GetIniFileInt(Section, "Marks_Underline_Visible", 0);
	EditorEx1->Marks->Underline->Color = StringToColor(
    	Config->GetIniFileString(Section, "Marks_Underline_Color", "clBlack"));
	EditorEx1->Marks->WrapMark->Visible =
    	Config->GetIniFileInt(Section, "Marks_WrapMark_Visible", 0);
	EditorEx1->Marks->WrapMark->Color = StringToColor(
		Config->GetIniFileString(Section, "Marks_WrapMark_Color", "clBlack"));
	EditorEx1->ExMarks->FindMark->Visible =
		Config->GetIniFileInt(Section, "Marks_FindMark_Visible", 0);
	EditorEx1->ExMarks->FindMark->Color = StringToColor(
		Config->GetIniFileString(Section, "Marks_FindMark_Color", "clBlack"));
	EditorEx1->ExMarks->SpaceMark->Visible =
		Config->GetIniFileInt(Section, "Marks_SpaceMark_Visible", 0);
	EditorEx1->ExMarks->SpaceMark->Color = StringToColor(
		Config->GetIniFileString(Section, "Marks_SpaceMark_Color", "clBlack"));
	EditorEx1->ExMarks->DBSpaceMark->Visible =
		Config->GetIniFileInt(Section, "Marks_DBSpaceMark_Visible", 0);
	EditorEx1->ExMarks->DBSpaceMark->Color = StringToColor(
		Config->GetIniFileString(Section, "Marks_DBSpaceMark_Color", "clBlack"));
	EditorEx1->ExMarks->TabMark->Visible =
		Config->GetIniFileInt(Section, "Marks_TabMark_Visible", 0);
    EditorEx1->ExMarks->TabMark->Color = StringToColor(
		Config->GetIniFileString(Section, "Marks_TabMark_Color", "clBlack"));

	if(Config->GetIniFileString(Section, "HitStyle", "hsDraw") == "hsDraw")
		EditorEx1->HitStyle = hsDraw;
	else if(Config->GetIniFileString(Section, "HitStyle", "hsDraw") == "hsCaret")
		EditorEx1->HitStyle = hsCaret;
	else EditorEx1->HitStyle = hsSelect;
}
void TAnaMDIChild::LoadLbRl()
{
	AnsiString Section = "LeftbarRuler";

	EditorEx1->Leftbar->BkColor
    	= StringToColor(Config->GetIniFileString(Section, "Leftbar_BkColor", "clBtnFace"));
	EditorEx1->Leftbar->Color
    	= StringToColor(Config->GetIniFileString(Section, "Leftbar_Color", "clWindowText"));
	EditorEx1->Leftbar->Column = Config->GetIniFileInt(Section, "Leftbar_Column", 4);
	EditorEx1->Leftbar->Edge = Config->GetIniFileInt(Section, "Leftbar_Edge", true);
	EditorEx1->Leftbar->LeftMargin = Config->GetIniFileInt(Section, "Leftbar_LeftMargin", 8);
	EditorEx1->Leftbar->RightMargin = Config->GetIniFileInt(Section, "Leftbar_RightMargin", 4);
	EditorEx1->Leftbar->ShowNumber = Config->GetIniFileInt(Section,"Leftbar_ShowNumber", true);

	if(Config->GetIniFileString(Section, "Leftbar_ShowNumberMode", "nmRow") == "nmRow")
		EditorEx1->Leftbar->ShowNumberMode = nmRow;
	else
		EditorEx1->Leftbar->ShowNumberMode = nmLine;
	EditorEx1->Leftbar->Visible = Config->GetIniFileInt(Section, "Leftbar_Visible", true);
	EditorEx1->Leftbar->ZeroBase = Config->GetIniFileInt(Section, "Leftbar_ZeroBase", 0);
	EditorEx1->Leftbar->ZeroLead = Config->GetIniFileInt(Section, "Leftbar_ZeroLead", 1);

	EditorEx1->Ruler->BkColor = StringToColor(
    	Config->GetIniFileString(Section, "Ruler_BkColor", "clBtnFace"));
	EditorEx1->Ruler->Color = StringToColor(
    	Config->GetIniFileString(Section, "Ruler_Color", "clBtnText"));
	EditorEx1->Ruler->Edge = Config->GetIniFileInt(Section, "Ruler_Edge", 1);
	if(Config->GetIniFileInt(Section, "Ruler_GaugeRange", 10) == 10)
		EditorEx1->Ruler->GaugeRange = 10;
	else EditorEx1->Ruler->GaugeRange = 8;
	EditorEx1->Ruler->MarkColor = StringToColor(
    	Config->GetIniFileString(Section, "Ruler_MarkColor", "clBtnText"));
	EditorEx1->Ruler->Visible = Config->GetIniFileInt(Section, "Ruler_Visible", true);

	EditorEx1->Margin->Top = Config->GetIniFileInt(Section, "Margin_Top", 2);
	EditorEx1->Margin->Left = Config->GetIniFileInt(Section, "Margin_Left", 19);
	EditorEx1->Margin->Line = Config->GetIniFileInt(Section, "Margin_Line", 0);
	EditorEx1->Margin->Character = Config->GetIniFileInt(Section, "Margin_Character", 0);

    EditorEx1->Imagebar->Visible = false;

}
void TAnaMDIChild::LoadBRK()
{
	EditorEx1->View->BeginUpdate();
	EditorEx1->View->Brackets->Clear();
	int j = 0;
    AnsiString Section = "Brackets";
	while(Config->GetIniFileString(Section, "Bracket_RBracket"+IntToStr(j), "") != "") {
		EditorEx1->View->Brackets->Add();
		/* Color, Style */
    	EditorEx1->View->Brackets->BracketItems[j]->ItemColor->BkColor
    		= StringToColor(Config->GetIniFileString(
            	Section, "Bracket_BkColor"+IntToStr(j), "clWhite"));
		EditorEx1->View->Brackets->BracketItems[j]->ItemColor->Color
    		= StringToColor(Config->GetIniFileString(
            	Section, "Bracket_Color"+IntToStr(j), "clBlack"));
	    SetStrFontStyles(&(EditorEx1->View->Brackets->BracketItems[j]->ItemColor->Style)
       		, Config->GetIniFileString(Section, "Bracket_Style"+IntToStr(j), ""));

		EditorEx1->View->Brackets->BracketItems[j]->LeftBracket
    		= Config->GetIniFileString(Section, "Bracket_LBracket"+IntToStr(j), "Left");
	    EditorEx1->View->Brackets->BracketItems[j]->RightBracket
    		= Config->GetIniFileString(Section, "Bracket_RBracket"+IntToStr(j), "Right");
	    j++;
	}
	EditorEx1->View->EndUpdate();
}
void TAnaMDIChild::LoadRWL()
{
	AnsiString Section = "ReserveWordList";
    AnsiString Key = "RWL";
    Config->GetIniFileStringList(EditorEx1->ReserveWordList, Section, Key);
}
void TAnaMDIChild::LoadWW()
{
	AnsiString Section = "WordWrap";

	EditorEx1->WordWrap = Config->GetIniFileInt(Section, "WordWrap", 0);
	EditorEx1->WrapOption->FollowPunctuation
	 = Config->GetIniFileInt(Section, "WrapOption_FollowPunctuation", 0);
	EditorEx1->WrapOption->FollowRetMark
	 = Config->GetIniFileInt(Section, "WrapOption_FollowRetMark", 0);
	EditorEx1->WrapOption->Leading
	 = Config->GetIniFileInt(Section, "WrapOption_Leading", 0);
	EditorEx1->WrapOption->WordBreak
	 = Config->GetIniFileInt(Section, "WrapOption_WordBreak", 0);
	EditorEx1->WrapOption->WrapByte
	 = Config->GetIniFileInt(Section, "WrapOption_WrapByte", 80);
	EditorEx1->WrapOption->FollowStr
	 = Config->GetIniFileString(Section, "WrapOption_FollowStr", "");
	EditorEx1->WrapOption->LeadStr
	 = Config->GetIniFileString(Section, "WrapOption_LeadStr", "");
	EditorEx1->WrapOption->PunctuationStr
	 = Config->GetIniFileString(Section, "WrapOption_PunctuationStr", "");
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//
//
//
// 以下、フォームのイベントハンドラが続く・・・。
//
//
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// キー入力、マクロ関連
//

void __fastcall TAnaMDIChild::EditorEx1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	static bool flag = 0;
	//「挿入」・「上書き」の切り替え
	if(Key == VK_INSERT) {
		if(flag) {
    		flag = 0;
	        EditorEx1->OverWrite = false;
    	    StatusBar1->Panels->Items[2]->Text = "挿入";
        	return;
	    } else {
    		flag = 1;
        	EditorEx1->OverWrite = true;
	        StatusBar1->Panels->Items[2]->Text = "上書き";
    	    return;
	    }
	}
}

void __fastcall TAnaMDIChild::KeyMacro1Change(TObject *Sender)
{
	if(KeyMacro1->Status == msNone) {
    	StatusBar1->Panels->Items[5]->Bevel = pbRaised;
        StatusBar1->Panels->Items[5]->Text = "キー操作モード(無し)";
    } else if(KeyMacro1->Status == msRecording) {
    	StatusBar1->Panels->Items[5]->Bevel = pbNone;
        StatusBar1->Panels->Items[5]->Text = "キー操作モード(記録中)";
    } else if(KeyMacro1->Status == msWaiting) {
    	StatusBar1->Panels->Items[5]->Bevel = pbNone;
        StatusBar1->Panels->Items[5]->Text = "キー操作モード(待機中)";
    } else if(KeyMacro1->Status == msRunning) {
    	StatusBar1->Panels->Items[5]->Bevel = pbLowered;
        StatusBar1->Panels->Items[5]->Text = "キー操作モード(実行中)";
    }
   	return;
}

/////////////////////////////////////////////////////////////////////////////
//
// フォーム関連
//

void __fastcall TAnaMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}

bool __fastcall TAnaMDIChild::CloseQuery(void)
{
	if(!EditorEx1->Modified) return true;
	switch(MessageBox(Handle,
    	AnsiString("ファイル" + GetfpFileName() + "\nは変更されています。上書き保存しますか？").c_str(),
		"確認",
        MB_YESNOCANCEL | MB_ICONQUESTION))
    {
		case ID_YES: FileSave();return true;
		case ID_NO: return true;
		case ID_CANCEL:default: return false;
    }
}

void __fastcall TAnaMDIChild::FormShow(TObject *Sender)
{
	UpdateStatusInternal();
}

/////////////////////////////////////////////////////////////////////////////
//
// Edit メニュー関連
//

void __fastcall TAnaMDIChild::MEditUndoClick(TObject *Sender)
{ EditorEx1->Undo(); }

void __fastcall TAnaMDIChild::MEditRedoClick(TObject *Sender)
{ EditorEx1->Redo(); }

void __fastcall TAnaMDIChild::MEditCutClick(TObject *Sender)
{ EditorEx1->CutToClipboard(); }

void __fastcall TAnaMDIChild::MEditCopyClick(TObject *Sender)
{ EditorEx1->CopyToClipboard(); }

void __fastcall TAnaMDIChild::MEditPasteClick(TObject *Sender)
{ EditorEx1->PasteFromClipboard(); }

void __fastcall TAnaMDIChild::MEditClearClick(TObject *Sender)
{ EditorEx1->Lines->Clear(); }

void __fastcall TAnaMDIChild::MEditSelectAllClick(TObject *Sender)
{ EditorEx1->SelectAll(); }

void __fastcall TAnaMDIChild::MEditReadOnlyClick(TObject *Sender)
{
	if(EditorEx1->ReadOnly) {
		EditorEx1->ReadOnly = false;
	    MEditReadOnly->Checked = false;
	} else {
		EditorEx1->ReadOnly = true;
	    MEditReadOnly->Checked = true;
	}
}

void __fastcall TAnaMDIChild::MEditJumpClick(TObject *Sender)
{
	EditorEx1->Row = StrToInt(
    	InputBox("指定行数に移動",
        	"行数：",
            IntToStr(EditorEx1->Row))
    );
}

void __fastcall TAnaMDIChild::MEditMacroRecClick(TObject *Sender)
{
	if(KeyMacro1->Status == msRecording || KeyMacro1->Status == msRunning)
			KeyMacro1->EndRec();
    else KeyMacro1->BeginRec();
}

void __fastcall TAnaMDIChild::MEditMacroExecClick(TObject *Sender)
{
	KeyMacro1->Execute();
}

void __fastcall TAnaMDIChild::MEditMacroClearClick(TObject *Sender)
{
	KeyMacro1->Clear();
}

/////////////////////////////////////////////////////////////////////////////
//
// Search メニュー関連
//

void __fastcall TAnaMDIChild::MSearchFindClick(TObject *Sender)
{

    if(EditorEx1->SelLength) SetFindStrBuffer(EditorEx1->SelText);

	//検索文字列入力DLGの表示
	Ts_set_w *dlg;
    dlg = new Ts_set_w(this);
    //検索文字列入力DLGの設定
    dlg->ESearch->Text = GetFindStrBuffer();
	//検索種類の読みとり
	if(this->f_find_type) {
    	dlg->RBstEditor->Checked = true;
        dlg->RBstEditorEx->Checked = false;
    } else {
    	dlg->RBstEditor->Checked = false;
        dlg->RBstEditorEx->Checked = true;
    }
	//Editor検索オプションの読みとり
	if(this->search_styles.Contains(sfrMatchCase)) dlg->CBMatchCase->Checked = true;
	if(this->search_styles.Contains(sfrDown)) dlg->CBDown->Checked = true;
	if(this->search_styles.Contains(sfrWholeWord)) dlg->CBWholeWord->Checked = true;
	if(this->search_styles.Contains(sfrNoMatchZenkaku)) dlg->CBNoMatchZenkaku->Checked = true;
	if(this->search_styles.Contains(sfrIncludeCRLF)) dlg->CBIncludeCRLF->Checked = true;
	if(this->search_styles.Contains(sfrIncludeSpace)) dlg->CBIncludeSpace->Checked = true;
	//EditorEx検索オプションの読みとり
    if(this->ex_search_styles.Contains(soMatchCase)) dlg->CBExMatchCase->Checked = true;
    if(this->ex_search_styles.Contains(soRegexp)) dlg->CBExRegexp->Checked = true;
    if(this->ex_search_styles.Contains(soFuzzy)) dlg->CBExFuzzy->Checked = true;
    if(this->ex_search_styles.Contains(soWholeWord)) dlg->CBExWholeWord->Checked = true;

    if(mrCancel == dlg->ShowModal()) { delete dlg; return; }

	//検索文字列、オプションの受け取り。
	SetFindStrBuffer(dlg->ESearch->Text);
    //検索種類の受け取り
    f_find_type = dlg->RBstEditor->Checked ? true : false;
	//Editor検索オプションの受け取り
    this->search_styles.Clear();
	if(dlg->CBMatchCase->Checked) this->search_styles << sfrMatchCase;
	if(dlg->CBDown->Checked) this->search_styles << sfrDown;
    if(dlg->CBWholeWord->Checked) this->search_styles << sfrWholeWord;
    if(dlg->CBNoMatchZenkaku->Checked) this->search_styles << sfrNoMatchZenkaku;
	if(dlg->CBIncludeCRLF->Checked) this->search_styles << sfrIncludeCRLF;
    if(dlg->CBIncludeSpace->Checked) this->search_styles << sfrIncludeSpace;
	//EditorEx検索オプションの受け取り
	this->ex_search_styles.Clear();
    if(dlg->CBExMatchCase->Checked) this->ex_search_styles << soMatchCase;
    if(dlg->CBExRegexp->Checked) this->ex_search_styles << soRegexp;
    if(dlg->CBExFuzzy->Checked) this->ex_search_styles << soFuzzy;
    if(dlg->CBExWholeWord->Checked) this->ex_search_styles << soWholeWord;

    delete dlg;

	if(f_find_type) { //Editor検索
        EditorEx1->Search(GetFindStrBuffer(), search_styles);
        return;
    } else { //EditorEx検索
		EditorEx1->ExSearchOptions = ex_search_styles;
		EditorEx1->FindString = GetFindStrBuffer();
		EditorEx1->Find();
		return;
    }
}

void __fastcall TAnaMDIChild::MSearchFindNextClick(TObject *Sender)
{
	if(f_find_type) { //Editor版検索
		search_styles << sfrDown; //後方検索指示
        EditorEx1->Search(GetFindStrBuffer(), search_styles);
        return;
	} else {
    	EditorEx1->FindString = GetFindStrBuffer();
        EditorEx1->FindNext();
        return;
    }
}

void __fastcall TAnaMDIChild::MSearchFindPrevClick(TObject *Sender)
{
	if(f_find_type) { //Editor版検索
		search_styles >> sfrDown; //前方検索指示
        EditorEx1->Search(GetFindStrBuffer(), search_styles);
        return;
	} else {
		EditorEx1->FindString = GetFindStrBuffer();
		EditorEx1->FindPrev();
        return;
    }
}

void __fastcall TAnaMDIChild::MSearchReplaceClick(TObject *Sender)
{
    if(EditorEx1->SelLength) SetFindStrBuffer(EditorEx1->SelText);

	//置換文字列入力DLGの表示
	Trp_set_w *dlg;
    dlg = new Trp_set_w(this);
    //置換文字列入力DLGの設定
    dlg->ESearch->Text = GetFindStrBuffer();
    dlg->EReplace->Text = GetReplaceStrBuffer();
	//置換種類の読みとり
	if(f_find_type) {
    	dlg->RBstEditor->Checked = true;
        dlg->RBstEditorEx->Checked = false;
    } else {
    	dlg->RBstEditor->Checked = false;
        dlg->RBstEditorEx->Checked = true;
    }
	//全置換フラグの読みとり
	dlg->CBReplaceAll->Checked = f_replace_all;
	//Editor検索オプションの読みとり
	if(search_styles.Contains(sfrMatchCase)) dlg->CBMatchCase->Checked = true;
	if(search_styles.Contains(sfrDown)) dlg->CBDown->Checked = true;
	if(search_styles.Contains(sfrWholeWord)) dlg->CBWholeWord->Checked = true;
	if(search_styles.Contains(sfrNoMatchZenkaku)) dlg->CBNoMatchZenkaku->Checked = true;
	if(search_styles.Contains(sfrIncludeCRLF)) dlg->CBIncludeCRLF->Checked = true;
	if(search_styles.Contains(sfrIncludeSpace)) dlg->CBIncludeSpace->Checked = true;
	if(search_styles.Contains(sfrReplaceConfirm)) dlg->CBReplaceConfirm->Checked = true;
	//EditorEx検索オプションの読みとり
    if(ex_search_styles.Contains(soMatchCase)) dlg->CBExMatchCase->Checked = true;
    if(ex_search_styles.Contains(soRegexp)) dlg->CBExRegexp->Checked = true;
    if(ex_search_styles.Contains(soFuzzy)) dlg->CBExFuzzy->Checked = true;
    if(ex_search_styles.Contains(soWholeWord)) dlg->CBExWholeWord->Checked = true;

    if(mrCancel == dlg->ShowModal()) { delete dlg; return; }

	//置換文字列、オプションの受け取り。
	SetFindStrBuffer(dlg->ESearch->Text);
	SetReplaceStrBuffer(dlg->EReplace->Text);
    //検索種類の受け取り
    f_find_type = dlg->RBstEditor->Checked ? true : false;
    //全置換フラグの受け取り
	f_replace_all = dlg->CBReplaceAll->Checked;
	//Editor検索オプションの受け取り
    search_styles.Clear();
	if(dlg->CBMatchCase->Checked) search_styles << sfrMatchCase;
	if(dlg->CBDown->Checked) search_styles << sfrDown;
    if(dlg->CBWholeWord->Checked) search_styles << sfrWholeWord;
    if(dlg->CBNoMatchZenkaku->Checked) search_styles << sfrNoMatchZenkaku;
	if(dlg->CBIncludeCRLF->Checked) search_styles << sfrIncludeCRLF;
    if(dlg->CBIncludeSpace->Checked) search_styles << sfrIncludeSpace;
	if(dlg->CBReplaceConfirm->Checked) search_styles << sfrReplaceConfirm;
	//EditorEx検索オプションの受け取り
	ex_search_styles.Clear();
    if(dlg->CBExMatchCase->Checked) ex_search_styles << soMatchCase;
    if(dlg->CBExRegexp->Checked) ex_search_styles << soRegexp;
    if(dlg->CBExFuzzy->Checked) ex_search_styles << soFuzzy;
    if(dlg->CBExWholeWord->Checked) ex_search_styles << soWholeWord;

    delete dlg;

    if(f_find_type) { //Editor置換
    	eReplace();
		return;
    } else { //EditorEx置換
    	EditorEx1->ExSearchOptions = ex_search_styles;
    	EditorEx1->FindString = GetFindStrBuffer();
		if(f_replace_all) EditorEx1->ReplaceAll(GetReplaceStrBuffer(),false);
		else EditorEx1->Replace(GetReplaceStrBuffer(),false);
	    return;
    }
}
//---------------------------------------------------------------------------

