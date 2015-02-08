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
// �ėp�ݒ�t�@�C���ǂݏo���֐�
//

/**
 *  �ݒ�t�@�C�����當�����ǂݍ��݁A�Ԃ��B
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
 * �ݒ�t�@�C�����琮���l��ǂݍ��݁A�Ԃ��B
 */
int TConfig::GetIniFileInt(
	AnsiString Section, AnsiString Key, int Default)
{
	int result = GetPrivateProfileInt(
    	Section.c_str(),
        Key.c_str(),
        Default,
        GetfpIniFile().c_str());
    // GetPrivateProfile�͎��s����ƃ[����Ԃ��B
    // ���̂��߁Aresult == 0 == Default �Ȃ��result�Ԃ��B
    //if(result > 0 || result == Default) return result;
    // ����result <= 0 ���ADefault != 0�Ȃ�΁BDefault�Ԃ��B
    //������Ɏ擾�ł��Ă��Ȃ��B
	//else return Default;
    //�E�E�E�ƁA�s�������Ƃ��낾���B�u����ɓǂ݂Ƃꂽ�l�v���̂��[���̏ꍇ�B
    //���l����ƁA�璷�����p���čЂ��ƂȂ�B�Ƃ����킯�ŁA�O���B
    return result;
}

/**
 * �ݒ�t�@�C�����當���񃊃X�g���o�b�t�@�ɓǂݍ��ށB
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
// �ėp�ݒ�t�@�C�������o���֐�
//

/**
 * ������̏����o��
 */
bool TConfig::WriteIniFileString(
	AnsiString Section, AnsiString Key, AnsiString Value)
{
	return WritePrivateProfileString(
    	Section.c_str(), Key.c_str(), Value.c_str(),
        GetfpIniFile().c_str());
}

/**
 * �����l�̏����o��
 */
bool TConfig::WriteIniFileInt(
	AnsiString Section, AnsiString Key, int Value)
{
	return WritePrivateProfileString(
    	Section.c_str(), Key.c_str(), IntToStr(Value).c_str(),
        GetfpIniFile().c_str());
}

/**
 * �����񃊃X�g�̏����o��
 */
void TConfig::WriteIniFileStringList(TStrings *Source,
	AnsiString Section, AnsiString Key)
{
	int i = 0;
	AnsiString buf;

	buf = GetIniFileString(Section, Key + IntToStr(i), "");

	//�擾������""(�擾���s���̃f�t�H���g�j�łȂ�����A
	//�l�Ƃ��Ė����ȕ�����i""�j���Z�b�g��������B�܂�A�N���A��ƁB
	while(buf != "") {
		WriteIniFileString(Section, Key + IntToStr(i), "");
		i++;
		buf = GetIniFileString(Section, Key + IntToStr(i), "");
	}
	//�����o���B�L�[�J�E���^��i���[���ɖ߂��B
	for(i = 0; i < Source->Count; i++) {
		WriteIniFileString(Section, Key + IntToStr(i), Source->Strings[i]);
	}
	return;
}

