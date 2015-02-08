//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ver_info.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/////////////////////////////////////////////////////////////////////////////
//
// Public Function Implementation Section
//

/**
 * Constructor
 */
TVerInfo::TVerInfo(char *_fpFileNameBuffer)
  : _fpFileName(_fpFileNameBuffer)
{ Init(); }


/////////////////////////////////////////////////////////////////////////////
//
// Private Function Implementation Section
//

/**
 * バージョン情報取得ルート関数
 */
void TVerInfo::Init()
{
	DWORD dwSize;
	DWORD dwReserved;
	LPVOID lpBuffer;

	dwSize = GetFileVersionInfoSize(_fpFileName, &dwReserved);
	lpBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize );

	if(lpBuffer && GetFileVersionInfo(_fpFileName, 0, dwSize, lpBuffer))
		SetAllFileVersion(lpBuffer);

	if(lpBuffer) HeapFree(GetProcessHeap(), 0, lpBuffer);
}

/**
 * 言語情報取得関数
 */
AnsiString TVerInfo::GetFileTranslation(LPVOID lpBuffer) {
	DWORD FAR *translation;
	UINT dwLength;
	VerQueryValue(lpBuffer,
    			"\\VarFileInfo\\Translation",
		    	(LPVOID*)&translation, &dwLength);
	char szName[2048];

	wsprintf(szName, "\\StringFileInfo\\%04x%04x",
		LOWORD(*translation),HIWORD(*translation));
	return AnsiString(szName);
}

/**
 * 個別バージョン情報汎用取得関数
 */
AnsiString TVerInfo::GetAnyInfo(LPVOID lpBuffer, AnsiString AnyInfo)
{
	LPVOID lpStr;
    UINT dwLength;
	VerQueryValue(lpBuffer,
		(_FileHeader + "\\" + AnyInfo).c_str(),
		&lpStr,
		&dwLength);
	return AnsiString((LPTSTR)lpStr);
}

/**
 * 全バージョン情報取得関数
 */
void TVerInfo::SetAllFileVersion(LPVOID lpBuffer)
{
	_FileHeader = GetFileTranslation(lpBuffer);
	SetCompanyName(lpBuffer);
	SetFileDescription(lpBuffer);
    SetFileVersion(lpBuffer);
	SetInternalName(lpBuffer);
	SetLegalCopyright(lpBuffer);
	SetOriginalFilename(lpBuffer);
	SetProductName(lpBuffer);
	SetProductVersion(lpBuffer);
    ParseProductVersion(GetProductVersion());
}
