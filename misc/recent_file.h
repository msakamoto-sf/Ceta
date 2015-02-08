//---------------------------------------------------------------------------
#ifndef recent_fileH
#define recent_fileH
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// �u�ŋߊJ�����t�@�C���v����ێ��E�E�E�Ɏg���邩���B
// ���ۂ�TStringList��K�v�Œ���x�̂ݓK���Ƀ��b�s���O�����N���X�B
//

class TRecentFileList {
	TStringList *_RecentFileList;
    int _Max; //����ێ��ő吔

public:
	TRecentFileList(int Max = 20) {
    	_RecentFileList = new TStringList();
        _Max = Max;
    }
    ~TRecentFileList() { delete _RecentFileList; }

	void RegistryRecent(AnsiString fpNewFile) {
		int ibuf;
	    if(Count() > _Max) return;
        ibuf = _RecentFileList->IndexOf(fpNewFile);
	    if(-1 != ibuf) _RecentFileList->Delete(ibuf);
	    _RecentFileList->Insert(0, fpNewFile);
	}

    void SetMax(int value) { _Max = value; }
    int GetMax() { return _Max; }

    void Append(AnsiString fpNewFile) {
    	if(Count() > _Max) return;
        _RecentFileList->Append(fpNewFile);
    }
    void Clear() { _RecentFileList->Clear(); }
    int Count() { return _RecentFileList->Count; }
    AnsiString Index(int index) {
    	if(index > _Max || index >= _RecentFileList->Count) return AnsiString("");
    	return _RecentFileList->Strings[index];
    }
};
#endif
 