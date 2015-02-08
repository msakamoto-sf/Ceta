//---------------------------------------------------------------------------
#ifndef ci_registryH
#define ci_registryH
//---------------------------------------------------------------------------
class TCIRegistry {
public:
    TCIRegistry(AnsiString CIName, //���W���[���o�^��
                AnsiString fpCIFile, //���W���[���t�@�C���t���p�X��
                AnsiString CIExtensions) //�Ή��g���q������
    {
	    SetCIName(CIName);
		SetfpCIFile(fpCIFile);
	    SetCIExtensions(CIExtensions);
    }
    ~TCIRegistry() {}

    AnsiString GetfpCIFile() { return _fpCIFile; }
	void SetfpCIFile(AnsiString buf) { _fpCIFile = buf; }
	AnsiString GetCIName() { return _CIName; }
    void SetCIName(AnsiString buf) { _CIName = buf; }
    AnsiString GetCIExtensions() { return _CIExtensions; }
    void SetCIExtensions(AnsiString buf) { _CIExtensions = buf; }

	TCIRegistry *next;
	bool Compare(const AnsiString ext){ return _CIExtensions.Pos(ext); }

private:
	AnsiString _fpCIFile;
    AnsiString _CIName;
    AnsiString _CIExtensions;
};

#endif

