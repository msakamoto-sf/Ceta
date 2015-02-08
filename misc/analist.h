//---------------------------------------------------------------------------
#ifndef analistH
#define analistH
//---------------------------------------------------------------------------

/***************************************************************
「界」で使われるのに適した汎用リスト構造テンプレートクラス

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
******************************************************************/

/* 宣言部 */

template <class AnaClass> class AnaList {
public:
	AnaClass *head;

	AnaList();
	~AnaList();

	AnaClass* operator[](int);
    AnaClass* Index(int);
	int Count(void);
	bool Append(AnaClass *);
	bool Update(int, AnaClass *);
	bool Delete(int);
	bool Backward(int);
	bool Forward(int);
	void Clear();
};


/* 定義部（実装部） */

template <class AnaClass>
AnaList<AnaClass>::AnaList()
{
	head = NULL;
}

template <class AnaClass>
AnaList<AnaClass>::~AnaList()
{
	Clear();
}

template <class AnaClass>
AnaClass* AnaList<AnaClass>::operator[](int index)
{
	return Index(index);
}

template <class AnaClass>
AnaClass* AnaList<AnaClass>::Index(int index)
{
	if(index < 0) return NULL;
	int i;
	AnaClass *buf1, *buf2;
	buf1 = head;
	for(i = 0;(i < index) && (buf1 != NULL);i++) {
		buf2 = buf1->next;
	    buf1 = buf2;
	}
	return buf1;
}

template <class AnaClass>
int AnaList<AnaClass>::Count(void)
{
	int i = 0;
	AnaClass *buf1, *buf2;

	buf1 = head;
	while(buf1 != NULL) {
		buf2 = buf1->next;
	    buf1 = buf2;
    	i++;
	}
	return i;
}

template <class AnaClass>
bool AnaList<AnaClass>::Append(AnaClass *buf)
{

	int max_index = Count() - 1;
    AnaClass *buf1;

    //末尾のtemplate classを取得する。
    buf1 = Index(max_index);

	//class チェインに追加する。
	if(buf1 != NULL) {
		buf1->next = buf;
	} else {
		head = buf;
	}
	//おきまりの終端NULL
	buf->next = NULL;
	return true;
}

template <class AnaClass>
bool AnaList<AnaClass>::Update(int index, AnaClass *buf)
{
	//更新前の古いクラスポインタを取得。
	AnaClass *old = Index(index);
    if(old == NULL) return false;
	//古いクラスポインタのnextを新しいクラスポインタに登録。
	buf->next = old->next;
	//古いクラスポインタの一つ手前、つまりnextで古い方を指しているものを取得。
	AnaClass *before = Index(index-1);
	//一つ手前のnextを新しいやつに繋げる。
    if(before != NULL) before->next = buf;
    else head = buf; // index = 0の時は、headを更新することになる。
    //この段階で古いのは用無し。
    delete old;
    return true;
}

template <class AnaClass>
bool AnaList<AnaClass>::Delete(int index)
{
	if(index < 0) return false;
	AnaClass *bufm1,*buf,*bufp1;
	bufm1 = Index(index-1);
	buf = Index(index);
	bufp1 = Index(index+1);
    if(buf == NULL) return false;
	//対象が先頭なら、headポインタも繋ぎ変える。
	if(buf == head) {
		//要素が一個しかない場合、bufp1はNULLで、headもNULLになる。
		//つまり、要素無しでオッケー。
		head = bufp1;
	    delete buf;
    	return true;
	}
	//対象が先頭でなければ、素直に繋ぎ変える。
	else bufm1->next = bufp1;
	delete buf;
	return true;
}

template <class AnaClass>
bool AnaList<AnaClass>::Backward(int index)
{
	//順に二つ前、一つ前、対象、一つ後ろ。
	AnaClass *bufm2,*bufm1,*buf,*bufp1;
	bufm2 = Index(index-2);
	bufm1 = Index(index-1);
	buf = Index(index);
	bufp1 = Index(index+1);
    if(buf == NULL) return false;

	//対象が先頭なら、何もしなくて良い。帰る。
	if(buf == head) return false;
	//一つ前が先頭ポインタなら、先頭ポインタも繋ぎ変える。
	if(bufm1 == head) head = buf;
	//それ以外なら、特に問題はない（はず）。
	else bufm2->next = buf;
	buf->next = bufm1;
	bufm1->next = bufp1; //たとえ一つ後がNULLだとしても、結局終端NULLでO.K.
	return true;
}

template <class AnaClass>
bool AnaList<AnaClass>::Forward(int index)
{
	//順に、一つ前、対象、一つ後ろ、二つ後ろ。
	AnaClass *bufm1,*buf,*bufp1,*bufp2;
	bufm1 = Index(index-1);
	buf = Index(index);
	bufp1 = Index(index+1);
	bufp2 = Index(index+2);
	if(buf == NULL) return false;

	//対象が末尾（直後がNULL）なら、何もしなくて良い。帰る。
	if(buf->next == NULL) return false;
	//対象が先頭なら、クラス内のheadポインタも繋ぎ変える。
	if(buf == head) head = bufp1;
	//対象が先頭でなければ、素直に直前ポインタも繋ぎ変える。
	else bufm1->next = bufp1;
	bufp1->next = buf;
	buf->next = bufp2;
	return true;
}

template <class AnaClass>
void AnaList<AnaClass>::Clear(void)
{
	if(head == NULL) return;
	AnaClass *buf1,*buf2;
	buf1 = head;
	while(buf1 != NULL) {
	    buf2 = buf1->next;
		delete buf1;
	    buf1 = buf2;
	}
    head = NULL;
	return;
}


#endif
