//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ch_config.h"
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// �R�}���h�A�w���v���s�p�C���^�[�t�F�C�X�֐�
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
// �ǂݏo��
//

/**
 * �R�}���h�A�w���v�ݒ�ǂݏo�����[�g�֐�
 */
void TCHConfig::LoadCHConfig()
{
	LoadCommandSetList();
	LoadHelpSetList();
}


/**
 * �R�}���h�ݒ��ǂݍ��݁A���X�g���\�z����B
 */
void TCHConfig::LoadCommandSetList()
{
	//�Â����X�g�̃N���A
    CommandSetList.Clear();
	int i = 0;  //�L�[���̃J�E���g�A�b�v�p�B
	CommandSet *buf;

	AnsiString name_buf, file_buf, line_buf, env_buf;
	bool err_buf, bracket_buf;
	AnsiString Section = "Command";

	//���[�v�̍\����A�Ƃ肠������Ԑ擪�̐ݒ���e�X�g�ǂݏo������K�v������B
	//�擾�ł��Ȃ������ꍇ�A�f�t�H���g�Ƃ���""���R�s�[�����B
	name_buf = GetIniFileString(Section, AnsiString("CommandName") + IntToStr(i), "");
	file_buf = GetIniFileString(Section, AnsiString("CommandFile") + IntToStr(i), "");
	line_buf = GetIniFileString(Section, AnsiString("CommandLine") + IntToStr(i), "");
	env_buf = GetIniFileString(Section, AnsiString("CommandEnv") + IntToStr(i), "");
	err_buf = GetIniFileInt(Section, AnsiString("CommandErr") + IntToStr(i), 0);
	bracket_buf = GetIniFileInt(Section, AnsiString("BracketDQ") + IntToStr(i), 0);

	//�擾������""(�擾���s���̃f�t�H���g�j�łȂ�����񂷁B
	while(name_buf != "") {
    	try {
		buf = new CommandSet(name_buf, file_buf, line_buf, env_buf, err_buf, bracket_buf);
		} catch (std::bad_alloc) {
    		MessageBox(HWND_DESKTOP,"�������m�ۃG���[","�G���[",MB_OK | MB_ICONERROR);
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
 * �w���v�ݒ��ǂݍ��݁A���X�g���\�z����B
 */
void TCHConfig::LoadHelpSetList()
{
	//�Â����X�g�̃N���A
    HelpSetList.Clear();
	int i = 0;  //�L�[���̃J�E���g�A�b�v�p�B
	HelpSet *buf;

	AnsiString name_buf, file_buf;
	AnsiString Section = "Help";

	//���[�v�̍\����A�Ƃ肠������Ԑ擪�̐ݒ���e�X�g�ǂݏo������K�v������B
	//�擾�ł��Ȃ������ꍇ�A�f�t�H���g�Ƃ���""���R�s�[�����B
	name_buf = GetIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");
	file_buf = GetIniFileString(Section, AnsiString("HelpFile") + IntToStr(i), "");

	//�擾������""(�擾���s���̃f�t�H���g�j�łȂ�����񂷁B
	while(name_buf != "") {
		try {
			buf = new HelpSet(name_buf, file_buf);
		} catch (std::bad_alloc) {
    		MessageBox(HWND_DESKTOP,"�������m�ۃG���[","�G���[",MB_OK | MB_ICONERROR);
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
// �����o��
//

/**
 * �R�}���h�A�w���v�ݒ�t�@�C�������o�����[�g�֐�
 */
void TCHConfig::SaveCHConfig()
{
    SaveCommandSetList();
    SaveHelpSetList();
}

/**
 * �R�}���h���X�g�̏����o��
 */
void TCHConfig::SaveCommandSetList()
{
	int i = 0;
	AnsiString Section = "Command";
	AnsiString name_buf;

	//�Ƃ肠�����A�܂��ȑO�̐ݒ���N���A���Ȃ���΂Ȃ�Ȃ��B
	name_buf = GetIniFileString(Section, AnsiString("CommandName") + IntToStr(i), "");

	//�擾������""(�擾���s���̃f�t�H���g�j�łȂ�����A
	//�l�Ƃ��Ė����ȕ�����i""�j���Z�b�g��������B�܂�A�N���A��ƁB
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
	//�����o���B�L�[�J�E���^��i���[���ɖ߂��B
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
 * �w���v���X�g�̏����o��
 */
void TCHConfig::SaveHelpSetList()
{
	int i = 0;
	AnsiString Section = "Help";
	AnsiString name_buf;

	//�Ƃ肠�����A�܂��ȑO�̐ݒ���N���A���Ȃ���΂Ȃ�Ȃ��B
	name_buf = GetIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");

	//�擾������""(�擾���s���̃f�t�H���g�j�łȂ�����A
	//�l�Ƃ��Ė����ȕ�����i""�j���Z�b�g��������B�܂�A�N���A��ƁB
	while(name_buf != "") {
		WriteIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("HelpFile") + IntToStr(i), "");
		i++;
		name_buf = GetIniFileString(Section, AnsiString("HelpName") + IntToStr(i), "");
	}
	//�����o���B�L�[�J�E���^��i���[���ɖ߂��B
	for(i = 0; i < HelpSetList.Count(); i++) {
		HelpSet *buf = HelpSetList.Index(i);
		WriteIniFileString(Section, AnsiString("HelpName") + IntToStr(i),
    		buf->GetHelpName());
		WriteIniFileString(Section, AnsiString("HelpFile") + IntToStr(i),
    		buf->GetHelpFile());
	}
	return;
}
