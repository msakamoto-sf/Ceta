//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "config.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <System.hpp>
#include <dir.h>
#define BUF_SIZE 1024


/***************************************************************
 * Public Function Implementation Section
 */

/**
 * Constructor
 */
TConfig::TConfig(AnsiString fpIniFile)
{
	SetfpIniFile(fpIniFile);
    SetIniDir(ExtractFileDir(fpIniFile));
}

/***************************************************************
 * Private Function Implementation Section
 */

////////////////////////////////////////////////////////////
//
// 汎用設定ファイル読み出し関数
//

/**
 *  設定ファイルから文字列を読み込み、返す。
 */
AnsiString TConfig::GetIniFileString(
	AnsiString Section, AnsiString Key, AnsiString Default)
{
	char buf[BUF_SIZE];
	if(GetPrivateProfileString(
    	Section.c_str(),
        Key.c_str(),
        Default.c_str(),
        buf,
        BUF_SIZE,
        GetfpIniFile().c_str()))
    { return AnsiString(buf); }
    else
    { return Default; }
}

/**
 * 設定ファイルから整数値を読み込み、返す。
 */
int TConfig::GetIniFileInt(
	AnsiString Section, AnsiString Key, int Default)
{
	int result = GetPrivateProfileInt(
    	Section.c_str(),
        Key.c_str(),
        Default,
        GetfpIniFile().c_str());
    // GetPrivateProfileは失敗するとゼロを返す。
    // そのため、result == 0 == Default ならばresult返し。
    //if(result > 0 || result == Default) return result;
    // もしresult <= 0 かつ、Default != 0ならば。Default返し。
    //←正常に取得できていない。
	//else return Default;
    //・・・と、行きたいところだが。「正常に読みとれた値」自体がゼロの場合。
    //を考えると、冗長さが却って災いとなる。というわけで、外す。
    return result;
}

/**
 * 設定ファイルから文字列リストをバッファに読み込む。
 */
void TConfig::GetIniFileStringList(TStrings *Destination,
	AnsiString Section, AnsiString Key)
{
	Destination->Clear();
	int i = 0;
	AnsiString buf;

	buf = GetIniFileString(Section, Key + IntToStr(i), "");

	while(buf != "") {
    	Destination->Append(buf);
	    i++;
		buf = GetIniFileString(Section, Key + IntToStr(i), "");
	}
	return;
}

////////////////////////////////////////////////////////////
//
// 汎用設定ファイル書き出し関数
//

/**
 * 文字列の書き出し
 */
bool TConfig::WriteIniFileString(
	AnsiString Section, AnsiString Key, AnsiString Value)
{
	return WritePrivateProfileString(
    	Section.c_str(), Key.c_str(), Value.c_str(),
        GetfpIniFile().c_str());
}

/**
 * 整数値の書き出し
 */
bool TConfig::WriteIniFileInt(
	AnsiString Section, AnsiString Key, int Value)
{
	return WritePrivateProfileString(
    	Section.c_str(), Key.c_str(), IntToStr(Value).c_str(),
        GetfpIniFile().c_str());
}

/**
 * 文字列リストの書き出し
 */
void TConfig::WriteIniFileStringList(TStrings *Source,
	AnsiString Section, AnsiString Key)
{
	int i = 0;
	AnsiString buf;

	buf = GetIniFileString(Section, Key + IntToStr(i), "");

	//取得文字列が""(取得失敗時のデフォルト）でない限り、
	//値として無効な文字列（""）をセットし続ける。つまり、クリア作業。
	while(buf != "") {
		WriteIniFileString(Section, Key + IntToStr(i), "");
		i++;
		buf = GetIniFileString(Section, Key + IntToStr(i), "");
	}
	//書き出し。キーカウンタのiをゼロに戻す。
	for(i = 0; i < Source->Count; i++) {
		WriteIniFileString(Section, Key + IntToStr(i), Source->Strings[i]);
	}
	return;
}

