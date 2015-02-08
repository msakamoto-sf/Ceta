//---------------------------------------------------------------------------
#ifndef configH
#define configH
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// .ini形式ファイルへの基本的なアクセス手段を提供する基底クラス。
// .iniファイルへのフルパスを受け取り、整数値と文字列のAnsiString形式での読み
//  書きメソッドを提供する。
// 生のままだと、使いづらいと思うので適宜派生して使ってあげてください。
// エラー処理は基本的に、無しです。
//
// GetPrivateProfileString, GetPrivateProfileInt() API に関する注意事項
// を本ヘッダファイル末尾に添付してありますので、内部に興味のある方はご一読
// ください。
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
// GetPrivateProfileString, GetPrivateProfileInt() APIに関する注意事項
//
// この二つの関数は文字列型と、整数値型を読み込むAPIとして非常に有用です。
// しかし、デフォルト値の扱いについてWin32 Programmer's Reference記載の
// 記述と大きな違いが存在し、しかも重大です。
//
// まず、GetPrivateProfileString()関数ですが、戻り値は「取得できた文字列を
// バッファに書き込めた文字数」です。
// 与えられたキー値やセクション文字列がNULLであったり、取得できた文字列が
// 大きすぎる場合、与えられたバッファサイズ-1 or -2 の戻り値になります。
// （ここら辺はマニュアル参照。）
//
// 問題はデフォルト値が返ってきた（とされた）ときの挙動です。
//
// Win2k(NO SP) + BCB4 Pro の環境で、デフォルト値が戻ってこない現象が発生
// しました。与えたバッファに何も書かれなかったのです。
//
// エラーメッセージ表示エリアの文字色は文字列として設定ファイルに書き込み、
// StringToColor()で変換しているのですが、NULL文字列が変換できないとエラーが
// 発生したのがきっかけです。
//
// ともあれ、なぜかデフォルト値が正常に返されない現象が起こってしまったわけです。
//
// 仕方ないので、戻り値がゼロ。すなわち、バッファに何も書き込まれなかった場合に
// AnsiString型のデフォルト値を返せるよう、GetIniFileStringメソッドを実装
// しました。
//
// 次にGetPrivateProfileInt()関数ですが、これは「ゼロ以上の整数値」を返します。
// もしキー値に対応している値が負数ならば、ゼロを返します。
//
// ここでもデフォルト値が有効にならず、ゼロが返される現象が発生しました。
// そのため、取得された値を検査し、ゼロ以上、またはゼロではあるが「デフォルト値
// それ自体もゼロ」という場合は取得された値を返し、もしそうでなければデフォル
// ト値はゼロでなく、かつ、取得に失敗したと考えられるのでデフォルト値を返す実装
// にGetIniFileIntメソッドを組み立てました。
//
// ・・・はずでしたが。「正常に読みとれた値それ自体がゼロ」だった場合は、もう
// どうしようもなくなる点に気づきましたので、結局そのまま返すことにしました。
//
// 結果としてデフォルト値が確実に働くようになり、.ini設定ファイルにアクセスする
// 基本メソッドを提供するという本クラスの目的を十二分に達することができました。
//
/////////////////////////////////////////////////////////////////////////////
#endif

