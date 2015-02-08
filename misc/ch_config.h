//---------------------------------------------------------------------------
#ifndef ch_configH
#define ch_configH
//---------------------------------------------------------------------------
#include "command_set.h"
#include "help_set.h"
#include "analist.h"
#include "anamap.h"
#include "config.h"
#include <iostream.h>
#include "command_op.h"

/////////////////////////////////////////////////////////////////////////////
//
// �R�}���h�A�w���v���X�g�̓ǂݏ������\�b�h�ƁA
// ���X�g�\�����̂ւ̃A�N�Z�X��i��ǉ������N���X�ł��B
// C(ommand)H(elp)Config�N���X
//

class TCHConfig : public TConfig {
public:
	TCHConfig(AnsiString fpIniFileBuffer) : TConfig(fpIniFileBuffer) {}
    ~TCHConfig() {}

	AnaList<CommandSet> CommandSetList;
    AnaList<HelpSet> HelpSetList;

    void LoadCHConfig();
    void CH_HelpExec(int index, HWND Handle);
    bool CH_CommandExec(int index, AnsiString fpTargetFile, AnsiString fpStdErrFile, HWND Handle);

protected:
    void LoadCommandSetList();
    void LoadHelpSetList();

    void SaveCHConfig();
    void SaveCommandSetList();
    void SaveHelpSetList();
};

#endif
