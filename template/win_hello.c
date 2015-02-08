#include <windows.h>
int WINAPI WinMain(HINSTANCE hThisInst,HINSTANCE hPrevInst,LPSTR lpszArgs,int nWinMode)
{
	MessageBox(HWND_DESKTOP,"Hello, World!","Test Message",MB_OK);
	return 0;
}