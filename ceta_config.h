//---------------------------------------------------------------------------
#ifndef ceta_configH
#define ceta_configH
//---------------------------------------------------------------------------
#include "misc\ch_config.h"
#include "ci_registry.h"
#include "recent_file.h"

/////////////////////////////////////////////////////////////////////////////
//
// Ceta用にフォームの位置やディレクトリ設定などへのアクセス手段を追加した
// クラスです。最終的なアプリケーション設定情報格納クラスです。
//

class TCetaConfig : public TCHConfig {

public:
	TCetaConfig(
    	AnsiString CmdLineBuffer,
        AnsiString fpExeFileBuffer,
        AnsiString fpIniFileBuffer);
    ~TCetaConfig() { delete RFList; }

	AnaMap<TCIRegistry, const AnsiString> CIRegistryList;
    TRecentFileList *RFList; // ファイル履歴保持

	void LoadConfig();
	void LoadDirectoryStatus();
    void LoadErrorStatus();
    void LoadWindowStatus();
	void LoadCIRegistryList();
	void LoadRFList();

    void SaveConfig();
	void SaveDirectoryStatus();
    void SaveErrorStatus();
    void SaveWindowStatus();
	void SaveCIRegistryList();
    void SaveRFList();

	AnsiString GetCmdLine() { return _CmdLine; }
    AnsiString GetAppDir() { return _AppDir; }
    AnsiString GetfpAppFile() { return _fpAppFile; }

    AnsiString GetDocDir() { return _DocDir; }
    void SetDocDir(AnsiString buf) { _DocDir = buf; }
    AnsiString GetTemplateDir() { return _TemplateDir; }
    void SetTemplateDir(AnsiString buf) { _TemplateDir = buf; }


    bool GetErrorOutput() { return _ErrorOutput; }
    void SetErrorOutput(bool buf) { _ErrorOutput = buf; }
    AnsiString GetErrorOutputColor() { return _ErrorOutputColor; }
    void SetErrorOutputColor(AnsiString buf) { _ErrorOutputColor = buf; }
    AnsiString GetErrorOutputBkColor() { return _ErrorOutputBkColor; }
    void SetErrorOutputBkColor(AnsiString buf) { _ErrorOutputBkColor = buf; }
    int GetErrorOutputHeight() { return _ErrorOutputHeight; }
    void SetErrorOutputHeight(int buf) { _ErrorOutputHeight = buf; }

    int GetWTop() { return _WTop; }
    void SetWTop(int buf) { _WTop = buf; }
    int GetWLeft() { return _WLeft; }
    void SetWLeft(int buf) { _WLeft = buf; }
    int GetWWidth() { return _WWidth; }
    void SetWWidth(int buf) { _WWidth = buf; }
    int GetWHeight() { return _WHeight; }
    void SetWHeight(int buf) { _WHeight = buf; }

private:

	AnsiString _CmdLine;
	void SetCmdLine(AnsiString buf) { _CmdLine = buf; }
    AnsiString _AppDir;
    void SetAppDir(AnsiString buf) { _AppDir = buf; }
    AnsiString _fpAppFile;
    void SetfpAppFile(AnsiString buf) { _fpAppFile = buf; }

    AnsiString _DocDir;
	AnsiString _TemplateDir;

    //メッセージ表示エリアVisible, Color, BkColor, Height.
	bool _ErrorOutput;
    AnsiString _ErrorOutputColor;
	AnsiString _ErrorOutputBkColor;
	int _ErrorOutputHeight;

    //TCetaMainWindowの位置と寸法
	int _WTop;
	int _WLeft;
	int _WWidth;
	int _WHeight;

};

#endif
