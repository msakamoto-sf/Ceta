//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ceta_config.h"

//---------------------------------------------------------------------------

/**********************************************************
 * Public Function Implementation Section
 */

TCetaConfig::TCetaConfig(AnsiString CmdLineBuffer,
	AnsiString fpExeFileBuffer, AnsiString fpIniFileBuffer)
    : TCHConfig(fpIniFileBuffer)
{
	// アプリケーション実行ファイル名
  	SetfpAppFile(fpExeFileBuffer);
	SetAppDir(ExtractFileDir(GetfpAppFile()));
	// コマンドラインの取り出し
	SetCmdLine(CmdLineBuffer);
    // ファイル履歴の初期化
    RFList = new TRecentFileList();
    return;
}

////////////////////////////////////////////////////////////
//
// 読み出し
//

/**
 * 設定ファイル全面読み出しルート関数
 */
void TCetaConfig::LoadConfig()
{
	LoadDirectoryStatus();
    LoadErrorStatus();
    LoadWindowStatus();
	LoadCIRegistryList();
    LoadRFList();
	LoadCHConfig();
}

/**
 * ディレクトリ設定を読み込み、格納する。
 */
void TCetaConfig::LoadDirectoryStatus()
{
	AnsiString Section = "DirectoryStatus";
	SetDocDir(GetIniFileString(Section, "DocDir", "C:\\"));
	SetTemplateDir(GetIniFileString(Section, "TemplateDir", GetAppDir() + "\\template"));
}

/**
 * エラーメッセージ表示設定を読み込み、格納する。
 */
void TCetaConfig::LoadErrorStatus()
{
	AnsiString Section = "ErrorStatus";
	SetErrorOutput(GetIniFileInt(Section, "ErrorOutput", 1));
}

/**
 * メインウインドウの位置、サイズを読み込み、格納する。
 */
void TCetaConfig::LoadWindowStatus()
{
	AnsiString Section = "WindowStatus";
    SetWTop(GetIniFileInt(Section, "Top", 20));
	SetWLeft(GetIniFileInt(Section, "Left", 20));
	SetWWidth(GetIniFileInt(Section, "Width", 600));
	SetWHeight(GetIniFileInt(Section, "Height", 450));
}

/**
 * CI(ColorInformation)設定を読み込み、リストを構築する。
 */
void TCetaConfig::LoadCIRegistryList()
{
	//古いリストのクリア
    CIRegistryList.Clear();
	int i = 0;  //キー名のカウントアップ用。
	TCIRegistry *buf;

	AnsiString fpFileBuffer, NameBuffer, ExtensionsBuffer;
	AnsiString Section = "ColorInformation";

	//ループの構造上、とりあえず一番先頭の設定をテスト読み出しする必要がある。
	//取得できなかった場合、デフォルトとして""がコピーされる。
	NameBuffer = GetIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");
	fpFileBuffer = GetIniFileString(Section, AnsiString("fpCIFile") + IntToStr(i), "");
	ExtensionsBuffer = GetIniFileString(Section, AnsiString("CIExtensions") + IntToStr(i), "");

	//取得文字列が""(取得失敗時のデフォルト）でない限り回す。
	while(NameBuffer != "") {
		try {
			buf = new TCIRegistry(NameBuffer, fpFileBuffer, ExtensionsBuffer);
		} catch (std::bad_alloc) {
    		MessageBox(HWND_DESKTOP,"メモリ確保エラー","エラー",MB_OK | MB_ICONERROR);
			return;
		}
        CIRegistryList.Append(buf);
	    i++;
		NameBuffer = GetIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");
		fpFileBuffer = GetIniFileString(Section, AnsiString("fpCIFile") + IntToStr(i), "");
		ExtensionsBuffer = GetIniFileString(Section, AnsiString("CIExtensions") + IntToStr(i), "");
	}
	return;
}

void TCetaConfig::LoadRFList()
{
	//古いリストのクリア
	RFList->Clear();
	int i = 0;  //キー名のカウントアップ用。
	AnsiString buf;

	AnsiString Section = "RecentFileList";
	//履歴保持最大数の初期化
	RFList->SetMax(GetIniFileInt(Section, AnsiString("Max"), 20));

	buf = GetIniFileString(Section, AnsiString("RFList") + IntToStr(i), "");
	while(buf != "") {
		RFList->Append(buf);
	    i++;
		buf = GetIniFileString(Section, AnsiString("RFList") + IntToStr(i), "");
	}
	return;
}


////////////////////////////////////////////////////////////
//
// 書き出し
//

/**
 * 設定ファイル全面書き出しルート関数
 */
void TCetaConfig::SaveConfig()
{
	SaveDirectoryStatus();
    SaveErrorStatus();
    SaveWindowStatus();
	SaveCIRegistryList();
	SaveCHConfig();
    SaveRFList();
}


/**
 * ディレクトリ設定の書き出し
 */
void TCetaConfig::SaveDirectoryStatus()
{
	AnsiString Section = "DirectoryStatus";
    WriteIniFileString(Section, "DocDir", GetDocDir());
    WriteIniFileString(Section, "TemplateDir", GetTemplateDir());
}

/**
 * エラーメッセージ表示設定の書き出し
 */
void TCetaConfig::SaveErrorStatus()
{
	AnsiString Section = "ErrorStatus";
	WriteIniFileInt(Section, "ErrorOutput", GetErrorOutput());
}

/**
 * メインウインドウの位置、サイズの書き出し
 */
void TCetaConfig::SaveWindowStatus()
{
	AnsiString Section = "WindowStatus";
	WriteIniFileInt(Section, "Top", GetWTop());
	WriteIniFileInt(Section, "Left", GetWLeft());
	WriteIniFileInt(Section, "Width", GetWWidth());
	WriteIniFileInt(Section, "Height", GetWHeight());
}

/**
 * 結夢リストの書き出し
 */
void TCetaConfig::SaveCIRegistryList()
{
	int i = 0;
	AnsiString Section = "ColorInformation";
	AnsiString NameBuffer;

	//とりあえず、まず以前の設定をクリアしなければならない。
	NameBuffer = GetIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");

	//取得文字列が""(取得失敗時のデフォルト）でない限り、
	//値として無効な文字列（""）をセットし続ける。つまり、クリア作業。
	while(NameBuffer != "") {
		WriteIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("fpCIFile") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("CIExtensions") + IntToStr(i), "");
		i++;
		NameBuffer = GetIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");
	}
	//書き出し。キーカウンタのiをゼロに戻す。
	for(i = 0; i < CIRegistryList.Count(); i++) {
		TCIRegistry *buf = CIRegistryList.Index(i);
		WriteIniFileString(Section, AnsiString("CIName") + IntToStr(i),
    		buf->GetCIName());
		WriteIniFileString(Section, AnsiString("fpCIFile") + IntToStr(i),
    		buf->GetfpCIFile());
		WriteIniFileString(Section, AnsiString("CIExtensions") + IntToStr(i),
    		buf->GetCIExtensions());
	}
	return;
}

void TCetaConfig::SaveRFList()
{
	int i = 0;
	AnsiString Section = "RecentFileList";
	AnsiString buf;

    //履歴保持最大数の書き込み
	WriteIniFileInt(Section, AnsiString("Max"), RFList->GetMax());

	buf = GetIniFileString(Section, AnsiString("RFList") + IntToStr(i), "");
	//取得文字列が""(取得失敗時のデフォルト）でない限り、
	//値として無効な文字列（""）をセットし続ける。つまり、クリア作業。
	while(buf != "") {
		WriteIniFileString(Section, AnsiString("RFList") + IntToStr(i), "");
		i++;
		buf = GetIniFileString(Section, AnsiString("RFList") + IntToStr(i), "");
	}
	//書き出し。キーカウンタのiをゼロに戻す。
	for(i = 0; i < RFList->Count(); i++) {
    	buf = RFList->Index(i);
		WriteIniFileString(Section, AnsiString("RFList") + IntToStr(i), buf);
	}
	return;
}



