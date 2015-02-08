//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("ceta.res");
USEFORM("ceta_main_window.cpp", CetaMainWindow);
USEFORM("ci_select_dlg.cpp", CISelectDlg);
USEUNIT("misc\ver_info.cpp");
USEUNIT("ceta_config.cpp");
USEFORM("ceta_config_dlg.cpp", CetaConfigDialog);
USEUNIT("misc\ch_config.cpp");
USEUNIT("misc\config.cpp");
USEFILE("ceta.ini");
USEFORM("mdichild\ana_mdichild.cpp", AnaMDIChild);
USEFORM("mdichild\editorex_set_dlg.cpp", EditorExSetDialog);
USEFORM("mdichild\search_set.cpp", s_set_w);
USEFORM("mdichild\replace_set.cpp", rp_set_w);
USEFORM("misc\command_op.cpp", CommandExeDialog);
//---------------------------------------------------------------------------
#include "ceta_main_window.h"
#define _MUTEX_APP_SERIAL "23isdflkjsas09rlkdaf"

//設定ファイルディレクトリが記述された環境変数名
#define ENV_APP_INI "CETA_INIDIR"

/* WM_COPYDATA（ショートカットへのドラップとか、SendToショートカット送りとか）*/
void CopyDataToOld(void) {
	HWND hwnd;
	hwnd = FindWindow("TCetaMainWindow",NULL);
	if(hwnd != NULL) {
		int pc = ParamCount();
		int len_pl = 0; //String Length
		TStringList *pl_buf; //Parameter List
        
		pl_buf = new TStringList;
		pl_buf->Clear();
		for(int i = 1; i <= pc; i++) {
			pl_buf->Add(ParamStr(i));
			len_pl += ParamStr(i).Length() + 2; //CR + LF = 2bytes.
		}

		COPYDATASTRUCT cds;
		cds.dwData = SIGNATURE_APP_COPYDATA;
		cds.cbData = len_pl+1; //+ NULL Terminator
		cds.lpData = GlobalAlloc(GPTR,cds.cbData);
		strncpy((char *)cds.lpData, pl_buf->GetText(), cds.cbData);

        SendMessage(hwnd,WM_COPYDATA,(WPARAM)hwnd,(LPARAM)&cds);

		delete pl_buf;
		GlobalFree(cds.lpData);
	}

	return;
}

AnsiString GetfpAppIniFile()
{
	AnsiString buf(getenv(ENV_APP_INI));
    AnsiString env_ini_file = buf + "\\" + AnsiString(szIniFileName);
    AnsiString exe_ini_file = ExtractFilePath(ParamStr(0)) + AnsiString(szIniFileName);

    if(buf == "") return exe_ini_file;

    // Check Ignore Ini Environment(I2E) Command Line Option.
    for (int i=0;i<=ParamCount();i++)
	{
    	// if I2E Option is specified, Ignore Environment
        //  Variable About Initial File Directory.
		if (LowerCase(ParamStr(i)) == "-I2E") return exe_ini_file;
	}
	return env_ini_file;

}

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HANDLE mx;
	mx = OpenMutex(MUTEX_ALL_ACCESS, false, _MUTEX_APP_SERIAL);
	if(mx != NULL) {
		CloseHandle(mx);
		CopyDataToOld();
		return 1;
	}
	mx = CreateMutex(NULL, false, _MUTEX_APP_SERIAL);

	try
	{
		Application->Initialize();
		Application->Title = "Ceta";
		Application->HelpFile = "C:\\Origin\\ceta\\help\\ceta.hlp";
		TCetaConfig Config(CmdLine, ParamStr(0), GetfpAppIniFile());
        Config.LoadConfig();

		Application->CreateForm(__classid(TCetaMainWindow), &CetaMainWindow);
		CetaMainWindow->Config = &Config;
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
   	ReleaseMutex(mx);
	return 0;
}
//---------------------------------------------------------------------------
