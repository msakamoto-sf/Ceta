//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ch_config.h"
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// コマンド、ヘルプ実行用インターフェイス関数
//

void TCHConfig::CH_HelpExec(int index, HWND Handle)
{
	HelpSet *buf = HelpSetList.Index(index);
    AnaHelpExec(Handle, buf->GetHelpFile().c_str());
    return;
}

bool TCHConfig::CH_CommandExec(int index,
	AnsiString fpTargetFile, AnsiString fpStdErrFile, HWND Handle)
{
	CommandSet *buf = CommandSetList.Index(index);
	return AnaCommandExec(
    	buf->GetCommandFile().c_str(),
        buf->GetCommandLine().c_str(),
		fpTargetFile.c_str(),
		buf->GetCommandEnv().c_str(),
        fpStdErrFile.c_str(),
        buf->GetCommandErr(),
        buf->GetBracketDQ(),
        Handle);
}

////////////////////////////////////////////////////////////
//
// 読み出し
//

/**
 * コマンド、ヘルプ設定読み出しルート関数
 */
void TCHConfig::LoadCHConfig()
{
	LoadCommandSetList();
	LoadHelpSetList();
}


/**
 * コマンド設定を読み込み、リストを構築する。
 */
void TCHConfig::LoadCommandSetList()
{
	//古いリストのクリア
    CommandSetList.Clear();
	int i = 0;  //キー名のカウントアップ用。
	CommandSet *buf;

	AnsiString name_buf, file_buf, line_buf, env_buf;
	bool err_buf, bracket_buf;
	AnsiString Section = "Command";

	//ループの構造上、とりあえず一番先頭の設定をテスト読み出しする必要がある。
	//取得できなかった場合、デフォルトとして""がコピーされる。
	name_buf = GetIniFileString(Section, AnsiString("CommandName") + IntToStr(i), "");
	file_buf = GetIniFileString(Section, AnsiString("CommandFile") + IntToStr(i), "");
	line_buf = GetIniFileString(Section, AnsiString("CommandLine") + IntToStr(i), "");
	env_buf = GetIniFileString(Section, AnsiString("CommandEnv") + IntToStr(i), "");
	err_buf = GetIniFileInt(Section, AnsiString("CommandErr") + IntToStr(i), 0);
	bracket_buf = GetIniFileInt(Section, AnsiString("BracketDQ") + IntToStr(i), 0);

	//取得文字列が""(取得失敗時のデフォルト）でない限り回す。
	while(name_buf != "") {
    	try {
		buf = new CommandSet(name_buf, file_buf, line_buf, env_buf, err_buf, bracket_buf);
		} catch (std::bad_alloc) {
    		MessageBox(HWND_DESKTOP,"メモリ確保エラー","エラー",MB_OK | MB_ICONERROR);
			return;
		}
		CommandSetList.Append(buf);
	    i++;
		name_buf = GetIniFileString(Section, AnsiString("CommandName") + IntToStr(i), "");
		file_buf = GetIniFileString(Section, AnsiString("CommandFile") + IntToStr(i), "");
		line_buf = GetIniFileString(Section, AnsiString("CommandLine") + IntToStr(i), "");
		env_buf = GetIniFileString(Section, AnsiString("CommandEnv") + IntToStr(i), "");
		err_buf = GetIniFileInt(Section, AnsiString("CommandErr") + IntToStr(i), 0);
		bracket_buf = GetIniFileInt(Section, AnsiString("BracketDQ") + IntToStr(i), 0);
	}
	return;
}

/**
 * ヘルプ設定を読み込み、リストを構築する。
 */
void TCHConfig::LoadHelpSetList()
{
	//古いリストのクリア
    HelpSetList.Clear();
	int i = 0;  //キー名のカウントアップ用。
	HelpSet *buf;

	AnsiString name_buf, file_buf;
	AnsiString Section = "Help";

	//ループの構造上、とりあえず一番先頭の設定をテスト読み出しする必要がある。
	//取得できなかった場合、デフォルトとして""がコピーされる。
	name_buf = GetIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");
	file_buf = GetIniFileString(Section, AnsiString("HelpFile") + IntToStr(i), "");

	//取得文字列が""(取得失敗時のデフォルト）でない限り回す。
	while(name_buf != "") {
		try {
			buf = new HelpSet(name_buf, file_buf);
		} catch (std::bad_alloc) {
    		MessageBox(HWND_DESKTOP,"メモリ確保エラー","エラー",MB_OK | MB_ICONERROR);
			return;
		}
		HelpSetList.Append(buf);
	    i++;
		name_buf = GetIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");
		file_buf = GetIniFileString(Section, AnsiString("HelpFile") + IntToStr(i), "");
	}
	return;
}


////////////////////////////////////////////////////////////
//
// 書き出し
//

/**
 * コマンド、ヘルプ設定ファイル書き出しルート関数
 */
void TCHConfig::SaveCHConfig()
{
    SaveCommandSetList();
    SaveHelpSetList();
}

/**
 * コマンドリストの書き出し
 */
void TCHConfig::SaveCommandSetList()
{
	int i = 0;
	AnsiString Section = "Command";
	AnsiString name_buf;

	//とりあえず、まず以前の設定をクリアしなければならない。
	name_buf = GetIniFileString(Section, AnsiString("CommandName") + IntToStr(i), "");

	//取得文字列が""(取得失敗時のデフォルト）でない限り、
	//値として無効な文字列（""）をセットし続ける。つまり、クリア作業。
	while(name_buf != "") {
		WriteIniFileString(Section, AnsiString("CommandName") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("CommandFile") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("CommandLine") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("CommandEnv") + IntToStr(i), "");
		WriteIniFileInt(Section, AnsiString("CommandErr") + IntToStr(i), 0);
		WriteIniFileInt(Section, AnsiString("BracketDQ") + IntToStr(i), 0);
		i++;
		name_buf = GetIniFileString(Section, AnsiString("CommandName") + IntToStr(i), "");
	}
	//書き出し。キーカウンタのiをゼロに戻す。
	for(i = 0; i < CommandSetList.Count(); i++) {
		CommandSet *buf = CommandSetList.Index(i);
		WriteIniFileString(Section, AnsiString("CommandName") + IntToStr(i),
    		buf->GetCommandName());
		WriteIniFileString(Section, AnsiString("CommandFile") + IntToStr(i),
    		buf->GetCommandFile());
		WriteIniFileString(Section, AnsiString("CommandLine") + IntToStr(i),
    		buf->GetCommandLine());
		WriteIniFileString(Section, AnsiString("CommandEnv") + IntToStr(i),
    		buf->GetCommandEnv());
		WriteIniFileInt(Section, AnsiString("CommandErr") + IntToStr(i),
    		buf->GetCommandErr());
        WriteIniFileInt(Section, AnsiString("BracketDQ") + IntToStr(i),
        	buf->GetBracketDQ());
	}
	return;
}

/**
 * ヘルプリストの書き出し
 */
void TCHConfig::SaveHelpSetList()
{
	int i = 0;
	AnsiString Section = "Help";
	AnsiString name_buf;

	//とりあえず、まず以前の設定をクリアしなければならない。
	name_buf = GetIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");

	//取得文字列が""(取得失敗時のデフォルト）でない限り、
	//値として無効な文字列（""）をセットし続ける。つまり、クリア作業。
	while(name_buf != "") {
		WriteIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("HelpFile") + IntToStr(i), "");
		i++;
		name_buf = GetIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");
	}
	//書き出し。キーカウンタのiをゼロに戻す。
	for(i = 0; i < HelpSetList.Count(); i++) {
		HelpSet *buf = HelpSetList.Index(i);
		WriteIniFileString(Section, AnsiString("HelpName") + IntToStr(i),
    		buf->GetHelpName());
		WriteIniFileString(Section, AnsiString("HelpFile") + IntToStr(i),
    		buf->GetHelpFile());
	}
	return;
}
