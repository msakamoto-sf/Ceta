//---------------------------------------------------------------------------
#ifndef anamapH
#define anamapH
//---------------------------------------------------------------------------
#include "analist.h"


/***************************************************************
「界」で使われるのに適した汎用連想配列
　　　「もどき」リスト構造テンプレートクラス
（なんで「もどき」かというとSearch()というメソッドで連想配列
「風」に使えるようにしているに過ぎないから。）

汎用リスト構造テンプレートクラス、AnaClassから派生しています。

テンプレートクラスの条件
・publicなメンバで、自分自身のクラスポインタ「next」があること。
（例）
class HogeClass {
...
public:
...
HogeClass* next;
...
};

・publicなメンバ関数で、次の仕様のCompare()という名の関数があること。
０．返値の型は bool 型であること。
１．引数として Comparator 型の引数を一つ取ること。
２．デフォルト引数は使わない。
３．自分自身の何かしらの識別メンバとその引数とを比べ、一致していれば
「true」を。一致しなければ「false」を返すこと。
４．引数名は何でもよい。

（例）
class HogeClass {
...
AnsiString extensions;
...
public:
bool Compare(AnsiString ext) { return extensions.Pos(ext); }
...
};

Search()メソッドについて。

概要：連想配列もどき風に使う。便利なのは、Compare関数の実装によって
いろいろな比較ができるようになること。
使用法：上記の例なら、クラスのインスタンスは

AnaMap<HogeClass, AnsiString> ext_list;

とする。Comparatorというのは要するに比較するときに使う型のこと。
拡張子が"txt"に対応しているHogeClassのインデックスを取得するには、

int index = hwnd_list.Search("txt");
とする。

返値：
　比較され、一致するものがあればリスト中でのインデックス値を返す。
　リストが空であれば -1 を返す。
　一致するものが一つもなければ、0を返す。

　このように幅広い「一致」を実装できるようにしている。
　それもそのはずで、元々拡張子から対応モジュールを取得するために
実装されたテンプレートクラスである。
******************************************************************/

/* 宣言部 */
template<class AnaClass, class Comparator>
class AnaMap : public AnaList<AnaClass>
{
public:
	AnaMap() {};
	int SearchIndex(Comparator);
	AnaClass* Search(Comparator);
};

/* 定義部（実装部） */
template<class AnaClass, class Comparator>
int AnaMap<AnaClass, Comparator>::SearchIndex(Comparator cmp)
{
	//もし先頭がNULLならまだ何のモジュールもロードされていないので
	//当然-1を返す。
	if(head == NULL) return -1;

	AnaClass *buf1,*buf2;
	buf1 = head;
    int i = 0;
	while(!(buf1->Compare(cmp))) {
		buf2 = buf1->next;
	    buf1 = buf2;
        //もしも終端まできてしまったら、該当なし。という意味で、
        //先頭モジュールのインデックス 0 をデフォルトとして投げキス。
	    if(buf1 == NULL) return 0;
		i++;
	}
	return i;
}

template<class AnaClass, class Comparator>
AnaClass* AnaMap<AnaClass, Comparator>::Search(Comparator cmp)
{
	int i = SearchIndex(cmp);
    if(i == -1) return NULL;
    return Index(i);
}

#endif
