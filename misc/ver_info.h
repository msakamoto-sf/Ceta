//---------------------------------------------------------------------------
#ifndef ver_infoH
#define ver_infoH
//---------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>

class TVerInfo
{
	// ファイル名フルパス
	char *_fpFileName;
    // バージョン情報取得ルート関数
	void Init();

    // バージョン情報キー文字列
    AnsiString _FileHeader;
	AnsiString GetFileTranslation(LPVOID);
	// 個別バージョン情報汎用取得関数
	AnsiString GetAnyInfo(LPVOID, AnsiString);
	// 全バージョン情報取得関数
	void SetAllFileVersion(LPVOID);

    // 個別のバージョン情報メンバとセッタ
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

    //個別のバージョン情報のゲッタ
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
