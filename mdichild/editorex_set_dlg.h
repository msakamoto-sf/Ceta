//---------------------------------------------------------------------------
#ifndef editorex_set_dlgH
#define editorex_set_dlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "HEditor.hpp"
#include "EditorEx.hpp"
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <FileCtrl.hpp>
#include "CSPIN.h"
#include "CGRID.h"

#include "..\misc\config.h"


/* 文字列からパラメータ記述をみて、TFontStylesを設定。*/
void SetStrFontStyles(TFontStyles *,AnsiString);

/* フォントスタイルを見て、パラメータ文字列を生成。*/
AnsiString FontStyleToStr(TFontStyles);

/* TColor型Color定数を、ColorGrid内のIndexに変換。
   対応していなければ-1を返す。*/
int ColorConstConv(TColor);

/* ColorGridで選択されているIndexをColor定数に変換。
   flagは１ならFGを、0ならBGを返す。 */
TColor ColorGridConv(bool,TCColorGrid *);

//---------------------------------------------------------------------------
class TEditorExSetDialog : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPageControl *ES_TabPage;
	TTabSheet *TabFont;
	TLabel *Font_LFont;
	TListBox *Font_LBFont;
	TCheckBox *Font_CBVpitch;
	TLabel *Font_LSize;
	TEdit *Font_EBSize;
	TListBox *Font_LBSize;
	TGroupBox *Font_GBScroll;
	TRadioButton *Font_RBssNone;
	TRadioButton *Font_RBssHorizontal;
	TRadioButton *Font_RBssVertical;
	TRadioButton *Font_RBssBoth;
	TButton *editorex_set_dlg_OK;
	TTabSheet *TabCaret;
	TCheckBox *Caret_CBFreeCaret;
	TCheckBox *Caret_CBAutoIndent;
	TCheckBox *Caret_CBBackSpace;
	TCheckBox *Caret_CBInTab;
	TCheckBox *Caret_CBKeepCaret;
	TCheckBox *Caret_CBLockScroll;
	TCheckBox *Caret_CBNextLine;
	TCheckBox *Caret_CBTabIndent;
	TCheckBox *Caret_CBPrevSpace;
	TCheckBox *Caret_CBSoftTab;
	TLabel *Caret_LTabSpace;
	TCSpinEdit *Caret_CSETabSpace;
	TGroupBox *Caret_GBCaretStyle;
	TRadioButton *Caret_RBcsDefault;
	TRadioButton *Caret_RBcsBrief;
	TCheckBox *Caret_CBRowSelect;
	TCheckBox *Caret_CBSelMove;
	TGroupBox *Caret_GBSelDrag;
	TRadioButton *Caret_RBdmManual;
	TRadioButton *Caret_RBdmAuto;
	TCheckBox *Caret_CBAutoCursor;
	TGroupBox *Caret_GBCursor;
	TLabel *Caret_LDefalt;
	TComboBox *Caret_CBXDefaultC;
	TLabel *Caret_LDragSel;
	TComboBox *Caret_CBXDragC;
	TLabel *Caret_LDragSelCopy;
	TComboBox *Caret_CBXDragSelC;
	TLabel *Caret_LInSelCursor;
	TComboBox *Caret_CBXInSelC;
	TLabel *Caret_LLeftMargin;
	TComboBox *Caret_CBXLMC;
	TTabSheet *TabVM;
	TListBox *VMM_LBColor;
	TCColorGrid *VMM_ColorGrid;
	TGroupBox *VMM_GBStyle;
	TCheckBox *VMM_CBfsBold;
	TCheckBox *VMM_CBfsUnderline;
	TCheckBox *VMM_CBfsItalic;
	TCheckBox *VMM_CBMail;
	TCheckBox *VMM_CBUrl;
	TCheckBox *VMM_CBCCCode;
	TLabel *VMM_LHexPrefix;
	TLabel *VMM_LQuotaion;
	TLabel *VMM_LCommenter;
	TEdit *VMM_EBHexPrefix;
	TEdit *VMM_EBQuotation;
	TEdit *VMM_EBComment;
	TButton *VMM_BTNSameBk;
	TButton *VMM_BTNSameCol;
	TButton *VMM_BTNSameStyle;
	TGroupBox *VMM_GBMarks;
	TCheckBox *VMM_CBRetMark;
	TCheckBox *VMM_CBEofMark;
	TCheckBox *VMM_CBUnderline;
	TTabSheet *TabLbRlM;
	TListBox *LbRl_LBlbRl;
	TCColorGrid *LbRl_ColorGrid;
	TGroupBox *LbRl_GBRuler;
	TCheckBox *LbRl_CBRVisible;
	TCheckBox *LbRl_CBREdge;
	TGroupBox *LbRl_GBGaugeRange;
	TRadioButton *LbRl_RB10;
	TRadioButton *LbRl_RB08;
	TGroupBox *LbRl_GBLeftbar;
	TCheckBox *LbRl_CBLVisible;
	TCheckBox *LbRl_CBLEdge;
	TCheckBox *LbRl_CBLShowNumber;
	TCheckBox *LbRl_CBLZeroBase;
	TCheckBox *LbRl_CBLZeroLead;
	TGroupBox *LbRl_GBShowNumber;
	TRadioButton *LbRl_RBnmRow;
	TRadioButton *LbRl_RBnmLine;
	TLabel *LbRl_LColumn;
	TCSpinEdit *LbRl_CSEColumn;
	TLabel *LbRl_LLeftMargin;
	TCSpinEdit *LbRl_CSELeftMargin;
	TLabel *LbRl_LRightMargin;
	TCSpinEdit *LbRl_CSERightMargin;
	TTabSheet *TabReserveWord;
	TTabSheet *TabWordWrap;
	TCheckBox *WW_CBWordWrap;
	TGroupBox *WW_GBWrapOption;
	TCheckBox *WW_CBFollowPunc;
	TCheckBox *WW_CBFollowRet;
	TCheckBox *WW_CBLeading;
	TCheckBox *WW_CBWordBreak;
	TCheckBox *WW_CBFitWrap;
	TLabel *WW_LWrapByte;
	TCSpinEdit *WW_CSEWrapByte;
	TLabel *WW_LFollowStr;
	TEdit *WW_EBFollowStr;
	TButton *WW_BTNFollowStr;
	TLabel *WW_LLeadStr;
	TEdit *WW_EBLeadStr;
	TButton *WW_BTNLeadStr;
	TLabel *WW_LPunctationStr;
	TEdit *WW_EBPunctationStr;
	TButton *WW_BTNPunctationStr;
	TButton *editorex_set_dlg_Cancel;
	TTabSheet *TabBracket;
	TListBox *BRK_LBColor;
	TCColorGrid *BRK_ColorGrid;
	TGroupBox *BRK_GBStyle;
	TCheckBox *BRK_CBfsBold;
	TCheckBox *BRK_CBfsItalic;
	TCheckBox *BRK_CBfsUnderline;
	TButton *BRK_BTNSameBk;
	TButton *BRK_BTNSameColor;
	TButton *BRK_BTNSameStyle;
	TButton *BRK_BTNNew;
	TButton *BRK_BTNRemove;
	TLabel *Label3;
	TEdit *BRK_EBLeftB;
	TLabel *Label4;
	TButton *BRK_BTNUpdate;
	TEdit *BRK_EBRightB;
	TButton *editorex_set_dlg_Help;
	TCheckBox *Caret_CBFreeRow;
	TLabel *Caret_LBTopMarginCursor;
	TComboBox *Caret_CBXTMC;
	TGroupBox *LbRlM_GBMargin;
	TLabel *LbRlM_LTop;
	TLabel *LbRlM_LLeft;
	TLabel *LbRlM_LLine;
	TLabel *LbRlM_LCharacter;
	TCSpinEdit *LbRlM_CSETop;
	TCSpinEdit *LbRlM_CSELeft;
	TCSpinEdit *LbRlM_CSELine;
	TCSpinEdit *LbRlM_CSECharacter;
	TCheckBox *VMM_CBWrapMark;
	TEditorEx *VMM_TEtest;
	TEditorEx *Font_TEtest;
	TEditorEx *Caret_TEtest;
	TEditorEx *LbRl_TEtest;
	TEditorEx *BRK_TEtest;
	TEditorEx *RWL_TEtest;
	TCheckBox *VMM_CBSpaceMark;
	TCheckBox *VMM_CBDBSpaceMark;
	TCheckBox *VMM_CBTabMark;
	TGroupBox *VMM_GBHistStyle;
	TRadioButton *VMM_RBhsSelect;
	TRadioButton *VMM_RBhsDraw;
	TRadioButton *VMM_RBhsCaret;
	TCheckBox *VMM_CBFindMark;
	TMemo *Memo1;
	TButton *RWL_BTNLoad;
	TButton *RWL_BTNSave;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;

	/* Font Tab Sheet Section */
    void __fastcall BuildFontList(void);
	void __fastcall Font_CBVpitchClick(TObject *Sender);
	void __fastcall Font_LBFontClick(TObject *Sender);
	void __fastcall Font_LBSizeClick(TObject *Sender);
	void __fastcall Font_EBSizeChange(TObject *Sender);
	void __fastcall Font_RBssNoneClick(TObject *Sender);
	void __fastcall Font_RBssHorizontalClick(TObject *Sender);
	void __fastcall Font_RBssVerticalClick(TObject *Sender);
	void __fastcall Font_RBssBothClick(TObject *Sender);

	/* Caret Tab Sheet Section */
	void __fastcall EnumCursor(AnsiString s);
	void __fastcall Caret_CBXDefaultCChange(TObject *Sender);

	/* View Marks Tab Sheet Section */
	void __fastcall VMM_ColorGridMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall VMM_LBColorClick(TObject *Sender);
	void __fastcall VMM_CBfsBoldClick(TObject *Sender);
	void __fastcall VMM_CBMailClick(TObject *Sender);
	void __fastcall VMM_CBUrlClick(TObject *Sender);
	void __fastcall VMM_CBCCCodeClick(TObject *Sender);
	void __fastcall VMM_CBRetMarkClick(TObject *Sender);
	void __fastcall VMM_CBEofMarkClick(TObject *Sender);
	void __fastcall VMM_CBUnderlineClick(TObject *Sender);
	void __fastcall VMM_CBWrapMarkClick(TObject *Sender);
	void __fastcall VMM_CBSpaceMarkClick(TObject *Sender);
    void __fastcall VMM_CBDBSpaceMarkClick(TObject *Sender);
	void __fastcall VMM_CBTabMarkClick(TObject *Sender);
	void __fastcall VMM_EBHexPrefixChange(TObject *Sender);
	void __fastcall VMM_EBQuotationChange(TObject *Sender);
	void __fastcall VMM_EBCommentChange(TObject *Sender);
	void __fastcall VMM_BTNSameBkClick(TObject *Sender);
	void __fastcall VMM_BTNSameColClick(TObject *Sender);
	void __fastcall VMM_BTNSameStyleClick(TObject *Sender);
	void __fastcall VMM_RBhsSelectClick(TObject *Sender);
	void __fastcall VMM_CBFindMarkClick(TObject *Sender);

	/* LeftBar and Ruler Tab Sheet Section */
	void __fastcall LbRl_CBLVisibleClick(TObject *Sender);
	void __fastcall LbRl_CBLEdgeClick(TObject *Sender);
	void __fastcall LbRl_CBLShowNumberClick(TObject *Sender);
	void __fastcall LbRl_CBLZeroBaseClick(TObject *Sender);
	void __fastcall LbRl_CBLZeroLeadClick(TObject *Sender);
	void __fastcall LbRl_RBnmRowClick(TObject *Sender);
	void __fastcall LbRl_RBnmLineClick(TObject *Sender);
	void __fastcall LbRl_CSEColumnChange(TObject *Sender);
	void __fastcall LbRl_CSELeftMarginChange(TObject *Sender);
	void __fastcall LbRl_CSERightMarginChange(TObject *Sender);
	void __fastcall LbRl_CBRVisibleClick(TObject *Sender);
	void __fastcall LbRl_CBREdgeClick(TObject *Sender);
	void __fastcall LbRl_RB10Click(TObject *Sender);
	void __fastcall LbRl_RB08Click(TObject *Sender);
	void __fastcall LbRl_LBlbRlClick(TObject *Sender);
	void __fastcall LbRl_ColorGridMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall LbRlM_CSETopChange(TObject *Sender);
	void __fastcall LbRlM_CSELeftChange(TObject *Sender);
	void __fastcall LbRlM_CSELineChange(TObject *Sender);
	void __fastcall LbRlM_CSECharacterChange(TObject *Sender);

    /* Brackets Tab Sheet Section */
	void __fastcall BRK_LBColorClick(TObject *Sender);
	void __fastcall BRK_ColorGridMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall BRK_CBfsBoldClick(TObject *Sender);
	void __fastcall BRK_BTNNewClick(TObject *Sender);
	void __fastcall BRK_BTNRemoveClick(TObject *Sender);
	void __fastcall BRK_EBLeftBChange(TObject *Sender);
	void __fastcall BRK_BTNUpdateClick(TObject *Sender);
	void __fastcall BRK_BTNSameBkClick(TObject *Sender);
	void __fastcall BRK_BTNSameColorClick(TObject *Sender);
	void __fastcall BRK_BTNSameStyleClick(TObject *Sender);

	/* Word Wrap Tab Sheet Section */
	void __fastcall WW_BTNFollowStrClick(TObject *Sender);
	void __fastcall WW_BTNLeadStrClick(TObject *Sender);
	void __fastcall WW_BTNPunctationStrClick(TObject *Sender);
	void __fastcall RWL_BTNLoadClick(TObject *Sender);
	void __fastcall RWL_BTNSaveClick(TObject *Sender);


	void __fastcall editorex_set_dlg_HelpClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall editorex_set_dlg_OKClick(TObject *Sender);

private:	// ユーザー宣言
	AnsiString _fpCIFile;
    TConfig *Config;

	AnsiString FollowStr_default;
    AnsiString LeadStr_default;
    AnsiString PunctationStr_default;

    // 読み込み関数群
    void LoadConfig();
    void LoadFont();
    void LoadCaret();
    void LoadVMM();
    void LoadLbRl();
    void LoadBRK();
    void LoadRWL();
    void LoadWW();

    // 書き込み関数群
    void SaveConfig();
    void SaveFont();
    void SaveCaret();
    void SaveVMM();
    void SaveLbRl();
    void SaveBRK();
    void SaveRWL();
    void SaveWW();

public:		// ユーザー宣言
	__fastcall TEditorExSetDialog(TComponent* Owner, AnsiString fpCIFileBuffer);

    AnsiString GetfpCIFile() { return _fpCIFile; }
    void SetfpCIFile(AnsiString value) { _fpCIFile = value; }
};
//---------------------------------------------------------------------------
extern PACKAGE TEditorExSetDialog *EditorExSetDialog;
//---------------------------------------------------------------------------
#endif
