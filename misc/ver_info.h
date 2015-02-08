//---------------------------------------------------------------------------
#ifndef ver_infoH
#define ver_infoH
//---------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>

class TVerInfo
{
	// �t�@�C�����t���p�X
	char *_fpFileName;
    // �o�[�W�������擾���[�g�֐�
	void Init();

    // �o�[�W�������L�[������
    AnsiString _FileHeader;
	AnsiString GetFileTranslation(LPVOID);
	// �ʃo�[�W�������ėp�擾�֐�
	AnsiString GetAnyInfo(LPVOID, AnsiString);
	// �S�o�[�W�������擾�֐�
	void SetAllFileVersion(LPVOID);

    // �ʂ̃o�[�W������񃁃��o�ƃZ�b�^
	AnsiString _CompanyName;
	AnsiString _FileDescription;
	AnsiString _FileVersion;
	AnsiString _InternalName;
	AnsiString _LegalCopyright;
	AnsiString _OriginalFilename;
	AnsiString _ProductName;
	AnsiString _ProductVersion;

	long _MajorVersion;
	long _MinorVersion;
	long _ReleaseVersion;
	long _BuildVersion;

    void SetCompanyName(LPVOID lpBuffer)
    	{ _CompanyName = GetAnyInfo(lpBuffer, "CompanyName"); }
	void SetFileDescription(LPVOID lpBuffer)
    	{ _FileDescription = GetAnyInfo(lpBuffer, "FileDescription"); }
	void SetFileVersion(LPVOID lpBuffer)
    	{ _FileVersion = GetAnyInfo(lpBuffer, "FileVersion"); }
    void SetInternalName(LPVOID lpBuffer)
    	{ _InternalName = GetAnyInfo(lpBuffer, "InternalName"); }
    void SetLegalCopyright(LPVOID lpBuffer)
    	{ _LegalCopyright = GetAnyInfo(lpBuffer, "LegalCopyright"); }
	void SetOriginalFilename(LPVOID lpBuffer)
    	{ _OriginalFilename = GetAnyInfo(lpBuffer, "OriginalFilename"); }
    void SetProductName(LPVOID lpBuffer)
    	{ _ProductName = GetAnyInfo(lpBuffer, "ProductName"); }
    void SetProductVersion(LPVOID lpBuffer)
    	{ _ProductVersion = GetAnyInfo(lpBuffer, "ProductVersion"); }

    void ParseProductVersion(AnsiString pv)
		{ sscanf(pv.c_str(), "%ld.%ld.%ld.%ld",
        	&_MajorVersion, &_MinorVersion, &_ReleaseVersion, &_BuildVersion); }

public:
	TVerInfo(char *_fpFileNameBuffer);
	~TVerInfo(){}

    //�ʂ̃o�[�W�������̃Q�b�^
    AnsiString GetCompanyName() { return _CompanyName; }
	AnsiString GetFileDescription() { return _FileDescription; }
	AnsiString GetFileVersion() { return _FileVersion; }
    AnsiString GetInternalName() { return _InternalName; }
    AnsiString GetLegalCopyright() { return _LegalCopyright; }
	AnsiString GetOriginalFilename() { return _OriginalFilename; }
    AnsiString GetProductName() { return _ProductName; }
    AnsiString GetProductVersion() { return _ProductVersion; }

	int GetMajorVersion() { return (int)_MajorVersion; }
	int GetMinorVersion() { return (int)_MinorVersion; }
	int GetReleaseVersion() { return (int)_ReleaseVersion; }
	int GetBuildVersion() { return (int)_BuildVersion; }
};

#endif
