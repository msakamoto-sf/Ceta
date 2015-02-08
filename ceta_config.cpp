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
	// �A�v���P�[�V�������s�t�@�C����
  	SetfpAppFile(fpExeFileBuffer);
	SetAppDir(ExtractFileDir(GetfpAppFile()));
	// �R�}���h���C���̎��o��
	SetCmdLine(CmdLineBuffer);
    // �t�@�C�������̏�����
    RFList = new TRecentFileList();
    return;
}

////////////////////////////////////////////////////////////
//
// �ǂݏo��
//

/**
 * �ݒ�t�@�C���S�ʓǂݏo�����[�g�֐�
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
 * �f�B���N�g���ݒ��ǂݍ��݁A�i�[����B
 */
void TCetaConfig::LoadDirectoryStatus()
{
	AnsiString Section = "DirectoryStatus";
	SetDocDir(GetIniFileString(Section, "DocDir", "C:\\"));
	SetTemplateDir(GetIniFileString(Section, "TemplateDir", GetAppDir() + "\\template"));
}

/**
 * �G���[���b�Z�[�W�\���ݒ��ǂݍ��݁A�i�[����B
 */
void TCetaConfig::LoadErrorStatus()
{
	AnsiString Section = "ErrorStatus";
	SetErrorOutput(GetIniFileInt(Section, "ErrorOutput", 1));
}

/**
 * ���C���E�C���h�E�̈ʒu�A�T�C�Y��ǂݍ��݁A�i�[����B
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
 * CI(ColorInformation)�ݒ��ǂݍ��݁A���X�g���\�z����B
 */
void TCetaConfig::LoadCIRegistryList()
{
	//�Â����X�g�̃N���A
    CIRegistryList.Clear();
	int i = 0;  //�L�[���̃J�E���g�A�b�v�p�B
	TCIRegistry *buf;

	AnsiString fpFileBuffer, NameBuffer, ExtensionsBuffer;
	AnsiString Section = "ColorInformation";

	//���[�v�̍\����A�Ƃ肠������Ԑ擪�̐ݒ���e�X�g�ǂݏo������K�v������B
	//�擾�ł��Ȃ������ꍇ�A�f�t�H���g�Ƃ���""���R�s�[�����B
	NameBuffer = GetIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");
	fpFileBuffer = GetIniFileString(Section, AnsiString("fpCIFile") + IntToStr(i), "");
	ExtensionsBuffer = GetIniFileString(Section, AnsiString("CIExtensions") + IntToStr(i), "");

	//�擾������""(�擾���s���̃f�t�H���g�j�łȂ�����񂷁B
	while(NameBuffer != "") {
		try {
			buf = new TCIRegistry(NameBuffer, fpFileBuffer, ExtensionsBuffer);
		} catch (std::bad_alloc) {
    		MessageBox(HWND_DESKTOP,"�������m�ۃG���[","�G���[",MB_OK | MB_ICONERROR);
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
	//�Â����X�g�̃N���A
	RFList->Clear();
	int i = 0;  //�L�[���̃J�E���g�A�b�v�p�B
	AnsiString buf;

	AnsiString Section = "RecentFileList";
	//����ێ��ő吔�̏�����
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
// �����o��
//

/**
 * �ݒ�t�@�C���S�ʏ����o�����[�g�֐�
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
 * �f�B���N�g���ݒ�̏����o��
 */
void TCetaConfig::SaveDirectoryStatus()
{
	AnsiString Section = "DirectoryStatus";
    WriteIniFileString(Section, "DocDir", GetDocDir());
    WriteIniFileString(Section, "TemplateDir", GetTemplateDir());
}

/**
 * �G���[���b�Z�[�W�\���ݒ�̏����o��
 */
void TCetaConfig::SaveErrorStatus()
{
	AnsiString Section = "ErrorStatus";
	WriteIniFileInt(Section, "ErrorOutput", GetErrorOutput());
}

/**
 * ���C���E�C���h�E�̈ʒu�A�T�C�Y�̏����o��
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
 * �������X�g�̏����o��
 */
void TCetaConfig::SaveCIRegistryList()
{
	int i = 0;
	AnsiString Section = "ColorInformation";
	AnsiString NameBuffer;

	//�Ƃ肠�����A�܂��ȑO�̐ݒ���N���A���Ȃ���΂Ȃ�Ȃ��B
	NameBuffer = GetIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");

	//�擾������""(�擾���s���̃f�t�H���g�j�łȂ�����A
	//�l�Ƃ��Ė����ȕ�����i""�j���Z�b�g��������B�܂�A�N���A��ƁB
	while(NameBuffer != "") {
		WriteIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("fpCIFile") + IntToStr(i), "");
		WriteIniFileString(Section, AnsiString("CIExtensions") + IntToStr(i), "");
		i++;
		NameBuffer = GetIniFileString(Section, AnsiString("CIName") + IntToStr(i), "");
	}
	//�����o���B�L�[�J�E���^��i���[���ɖ߂��B
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

    //����ێ��ő吔�̏�������
	WriteIniFileInt(Section, AnsiString("Max"), RFList->GetMax());

	buf = GetIniFileString(Section, AnsiString("RFList") + IntToStr(i), "");
	//�擾������""(�擾���s���̃f�t�H���g�j�łȂ�����A
	//�l�Ƃ��Ė����ȕ�����i""�j���Z�b�g��������B�܂�A�N���A��ƁB
	while(buf != "") {
		WriteIniFileString(Section, AnsiString("RFList") + IntToStr(i), "");
		i++;
		buf = GetIniFileString(Section, AnsiString("RFList") + IntToStr(i), "");
	}
	//�����o���B�L�[�J�E���^��i���[���ɖ߂��B
	for(i = 0; i < RFList->Count(); i++) {
    	buf = RFList->Index(i);
		WriteIniFileString(Section, AnsiString("RFList") + IntToStr(i), buf);
	}
	return;
}



