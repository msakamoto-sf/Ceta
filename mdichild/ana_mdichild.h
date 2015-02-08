//---------------------------------------------------------------------------
#ifndef ana_mdichildH
#define ana_mdichildH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <dir.h>
#include "EditorEx.hpp"
#include "HEditor.hpp"
#include "HKeyMacro.hpp"
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>

#include "search_set.h"
#include "replace_set.h"
#include "..\misc\config.h"

class TAnaMDIChild : public TForm
{
__published:	// IDE 管理のコンポーネント
    TStatusBar *StatusBar1;
	TEditorEx *EditorEx1;
	TKeyMacro *KeyMacro1;
	TMainMenu *MainMenu1;
	TMenuItem *MEdit;
	TMenuItem *MEditUndo;
	TMenuItem *MEditRedo;
	TMenuItem *N1;
	TMenuItem *MEditCut;
	TMenuItem *MEditCopy;
	TMenuItem *MEditPaste;
	TMenuItem *MEditClear;
	TMenuItem *N2;
	TMenuItem *K1;
	TMenuItem *MEditMacroRec;
	TMenuItem *MEditSelectAll;
	TMenuItem *MEditJump;
	TMenuItem *MEditMacroExec;
	TMenuItem *MEditMacroClear;
	TMenuItem *MSearch;
	TMenuItem *MSearchFind;
	TMenuItem *MSearchFindNext;
	TMenuItem *MSearchFindPrev;
	TMenuItem *MSearchReplace;
	TImageList *ImageList1;
	TMenuItem *MEditReadOnly;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    bool __fastcall CloseQuery(void);
	void __fastcall EditorEx1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall KeyMacro1Change(TObject *Sender);

	//Edit Menu Handling
    void __fastcall MEditUndoClick(TObject *Sender);
    void __fastcall MEditRedoClick(TObject *Sender);
    void __fastcall MEditCutClick(TObject *Sender);
    void __fastcall MEditCopyClick(TObject *Sender);
    void __fastcall MEditPasteClick(TObject *Sender);
	void __fastcall MEditClearClick(TObject *Sender);
	void __fastcall MEditSelectAllClick(TObject *Sender);
	void __fastcall MEditReadOnlyClick(TObject *Sender);
   	void __fastcall MEditJumpClick(TObject *Sender);
	void __fastcall MEditMacroRecClick(TObject *Sender);
	void __fastcall MEditMacroExecClick(TObject *Sender);
	void __fastcall MEditMacroClearClick(TObject *Sender);

	//Search Menu Handling
	void __fastcall MSearchFindClick(TObject *Sender);
	void __fastcall MSearchFindNextClick(TObject *Sender);
	void __fastcall MSearchFindPrevClick(TObject *Sender);
	void __fastcall MSearchReplaceClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// ユーザー宣言
	AnsiString _fpFileName; // 編集ファイル名
    AnsiString GetfpFileName() { return _fpFileName; }
    void SetfpFileName(AnsiString value) { _fpFileName = value; }

    AnsiString _fpCIFile; // ColorInformationファイル名フルパス
	AnsiString GetfpCIFile() { return _fpCIFile; }
    void SetfpCIFile(AnsiString value) { _fpCIFile = value; }

    AnsiString _fpTemplateFile;

	int caret_x,caret_y; // キャレット位置
	bool f_find_type; // 検索機能のタイプ(TEditor:true, TEditorEx:false)
	bool f_replace_all; // 全置換フラグ
	TSearchOptions search_styles;
    TExSearchOptions ex_search_styles;

	AnsiString _FindStrBuffer;
	AnsiString GetFindStrBuffer() { return _FindStrBuffer; }
    void SetFindStrBuffer(AnsiString value) { _FindStrBuffer = value; }

	AnsiString _ReplaceStrBuffer;
    AnsiString GetReplaceStrBuffer() { return _ReplaceStrBuffer; }
    void SetReplaceStrBuffer(AnsiString value) { _ReplaceStrBuffer = value; }

	// 検索機能内部実装
	void eReplace();
	void ReplaceEditor();

    // CIファイルリロード、文字色設定更新
    void UpdateStatusInternal();
    TConfig *Config;
    // CIファイルのロード
    void LoadConfig();
    void LoadFont();
    void LoadCaret();
    void LoadVMM();
    void LoadLbRl();
    void LoadBRK();
    void LoadRWL();
    void LoadWW();

public:		// ユーザー宣言
    __fastcall TAnaMDIChild(TComponent* Owner,
    	AnsiString fpFileName, AnsiString fpCIFile, AnsiString fpTemplateFile = "");

	AnsiString GetFileName();
    void FileLoad(AnsiString fname = "");
    void FileReload();
    void FileSave(AnsiString fpFileNameBuffer = "");
    void FilePrint();

    // 編集メニューインターフェイス関数群
    void EditUndo();
    void EditRedo();
    void EditCut();
    void EditCopy();
    void EditPaste();
	// 検索・置換用インターフェイス関数群
	void SearchFind();
	void SearchReplace();

    void UpdateStatus(); // 何らかの更新作業用の外部インターフェイス関数
	void IdlingHandler(); // アイドリング時処理用インターフェイス関数

};
//---------------------------------------------------------------------------
extern PACKAGE TAnaMDIChild *AnaMDIChild;
//---------------------------------------------------------------------------
#endif
