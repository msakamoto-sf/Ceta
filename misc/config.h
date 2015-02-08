//---------------------------------------------------------------------------
#ifndef configH
#define configH
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// .ini�`���t�@�C���ւ̊�{�I�ȃA�N�Z�X��i��񋟂�����N���X�B
// .ini�t�@�C���ւ̃t���p�X���󂯎��A�����l�ƕ������AnsiString�`���ł̓ǂ�
//  �������\�b�h��񋟂���B
// ���̂܂܂��ƁA�g���Â炢�Ǝv���̂œK�X�h�����Ďg���Ă����Ă��������B
// �G���[�����͊�{�I�ɁA�����ł��B
//
// GetPrivateProfileString, GetPrivateProfileInt() API �Ɋւ��钍�ӎ���
// ��{�w�b�_�t�@�C�������ɓY�t���Ă���܂��̂ŁA�����ɋ����̂�����͂����
// ���������B
//

class TConfig {

public:
    TConfig(AnsiString fpIniFile);
    ~TConfig() {};
    AnsiString GetfpIniFile() { return _fpIniFile; }
    AnsiString GetIniDir() { return _IniDir; }

	AnsiString GetIniFileString(AnsiString, AnsiString, AnsiString);
    int GetIniFileInt(AnsiString, AnsiString, int);
	void GetIniFileStringList(TStrings*, AnsiString, AnsiString);

    bool WriteIniFileString(AnsiString, AnsiString, AnsiString);
	bool WriteIniFileInt(AnsiString, AnsiString, int);
    void WriteIniFileStringList(TStrings*, AnsiString, AnsiString);


private:
    AnsiString _fpIniFile;
    AnsiString _IniDir;
    void SetfpIniFile(AnsiString value) { _fpIniFile = value; }
    void SetIniDir(AnsiString value) { _IniDir = value; }

};

//
//
/////////////////////////////////////////////////////////////////////////////
//
// GetPrivateProfileString, GetPrivateProfileInt() API�Ɋւ��钍�ӎ���
//
// ���̓�̊֐��͕�����^�ƁA�����l�^��ǂݍ���API�Ƃ��Ĕ��ɗL�p�ł��B
// �������A�f�t�H���g�l�̈����ɂ���Win32 Programmer's Reference�L�ڂ�
// �L�q�Ƒ傫�ȈႢ�����݂��A�������d��ł��B
//
// �܂��AGetPrivateProfileString()�֐��ł����A�߂�l�́u�擾�ł����������
// �o�b�t�@�ɏ������߂��������v�ł��B
// �^����ꂽ�L�[�l��Z�N�V����������NULL�ł�������A�擾�ł���������
// �傫������ꍇ�A�^����ꂽ�o�b�t�@�T�C�Y-1 or -2 �̖߂�l�ɂȂ�܂��B
// �i������ӂ̓}�j���A���Q�ƁB�j
//
// ���̓f�t�H���g�l���Ԃ��Ă����i�Ƃ��ꂽ�j�Ƃ��̋����ł��B
//
// Win2k(NO SP) + BCB4 Pro �̊��ŁA�f�t�H���g�l���߂��Ă��Ȃ����ۂ�����
// ���܂����B�^�����o�b�t�@�ɉ���������Ȃ������̂ł��B
//
// �G���[���b�Z�[�W�\���G���A�̕����F�͕�����Ƃ��Đݒ�t�@�C���ɏ������݁A
// StringToColor()�ŕϊ����Ă���̂ł����ANULL�����񂪕ϊ��ł��Ȃ��ƃG���[��
// ���������̂����������ł��B
//
// �Ƃ�����A�Ȃ����f�t�H���g�l������ɕԂ���Ȃ����ۂ��N�����Ă��܂����킯�ł��B
//
// �d���Ȃ��̂ŁA�߂�l���[���B���Ȃ킿�A�o�b�t�@�ɉ����������܂�Ȃ������ꍇ��
// AnsiString�^�̃f�t�H���g�l��Ԃ���悤�AGetIniFileString���\�b�h������
// ���܂����B
//
// ����GetPrivateProfileInt()�֐��ł����A����́u�[���ȏ�̐����l�v��Ԃ��܂��B
// �����L�[�l�ɑΉ����Ă���l�������Ȃ�΁A�[����Ԃ��܂��B
//
// �����ł��f�t�H���g�l���L���ɂȂ炸�A�[�����Ԃ���錻�ۂ��������܂����B
// ���̂��߁A�擾���ꂽ�l���������A�[���ȏ�A�܂��̓[���ł͂��邪�u�f�t�H���g�l
// ���ꎩ�̂��[���v�Ƃ����ꍇ�͎擾���ꂽ�l��Ԃ��A���������łȂ���΃f�t�H��
// �g�l�̓[���łȂ��A���A�擾�Ɏ��s�����ƍl������̂Ńf�t�H���g�l��Ԃ�����
// ��GetIniFileInt���\�b�h��g�ݗ��Ă܂����B
//
// �E�E�E�͂��ł������B�u����ɓǂ݂Ƃꂽ�l���ꎩ�̂��[���v�������ꍇ�́A����
// �ǂ����悤���Ȃ��Ȃ�_�ɋC�Â��܂����̂ŁA���ǂ��̂܂ܕԂ����Ƃɂ��܂����B
//
// ���ʂƂ��ăf�t�H���g�l���m���ɓ����悤�ɂȂ�A.ini�ݒ�t�@�C���ɃA�N�Z�X����
// ��{���\�b�h��񋟂���Ƃ����{�N���X�̖ړI���\�񕪂ɒB���邱�Ƃ��ł��܂����B
//
/////////////////////////////////////////////////////////////////////////////
#endif

