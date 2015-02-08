//---------------------------------------------------------------------------
#ifndef recent_fileH
#define recent_fileH
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// 「最近開いたファイル」履歴保持・・・に使えるかも。
// 実際はTStringListを必要最低限度のみ適当にラッピングしたクラス。
//

class TRecentFileList {
	TStringList *_RecentFileList;
    int _Max; //履歴保持最大数

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
 