//---------------------------------------------------------------------------
#ifndef analistH
#define analistH
//---------------------------------------------------------------------------

/***************************************************************
�u�E�v�Ŏg����̂ɓK�����ėp���X�g�\���e���v���[�g�N���X

�e���v���[�g�N���X�̏���
�Epublic�ȃ����o�ŁA�������g�̃N���X�|�C���^�unext�v�����邱�ƁB
�i��j
class HogeClass {
...
public:
...
HogeClass* next;
...
};
******************************************************************/

/* �錾�� */

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


/* ��`���i�������j */

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

    //������template class���擾����B
    buf1 = Index(max_index);

	//class �`�F�C���ɒǉ�����B
	if(buf1 != NULL) {
		buf1->next = buf;
	} else {
		head = buf;
	}
	//�����܂�̏I�[NULL
	buf->next = NULL;
	return true;
}

template <class AnaClass>
bool AnaList<AnaClass>::Update(int index, AnaClass *buf)
{
	//�X�V�O�̌Â��N���X�|�C���^���擾�B
	AnaClass *old = Index(index);
    if(old == NULL) return false;
	//�Â��N���X�|�C���^��next��V�����N���X�|�C���^�ɓo�^�B
	buf->next = old->next;
	//�Â��N���X�|�C���^�̈��O�A�܂�next�ŌÂ������w���Ă�����̂��擾�B
	AnaClass *before = Index(index-1);
	//���O��next��V������Ɍq����B
    if(before != NULL) before->next = buf;
    else head = buf; // index = 0�̎��́Ahead���X�V���邱�ƂɂȂ�B
    //���̒i�K�ŌÂ��̂͗p�����B
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
	//�Ώۂ��擪�Ȃ�Ahead�|�C���^���q���ς���B
	if(buf == head) {
		//�v�f��������Ȃ��ꍇ�Abufp1��NULL�ŁAhead��NULL�ɂȂ�B
		//�܂�A�v�f�����ŃI�b�P�[�B
		head = bufp1;
	    delete buf;
    	return true;
	}
	//�Ώۂ��擪�łȂ���΁A�f���Ɍq���ς���B
	else bufm1->next = bufp1;
	delete buf;
	return true;
}

template <class AnaClass>
bool AnaList<AnaClass>::Backward(int index)
{
	//���ɓ�O�A��O�A�ΏہA����B
	AnaClass *bufm2,*bufm1,*buf,*bufp1;
	bufm2 = Index(index-2);
	bufm1 = Index(index-1);
	buf = Index(index);
	bufp1 = Index(index+1);
    if(buf == NULL) return false;

	//�Ώۂ��擪�Ȃ�A�������Ȃ��ėǂ��B�A��B
	if(buf == head) return false;
	//��O���擪�|�C���^�Ȃ�A�擪�|�C���^���q���ς���B
	if(bufm1 == head) head = buf;
	//����ȊO�Ȃ�A���ɖ��͂Ȃ��i�͂��j�B
	else bufm2->next = buf;
	buf->next = bufm1;
	bufm1->next = bufp1; //���Ƃ���オNULL���Ƃ��Ă��A���ǏI�[NULL��O.K.
	return true;
}

template <class AnaClass>
bool AnaList<AnaClass>::Forward(int index)
{
	//���ɁA��O�A�ΏہA����A����B
	AnaClass *bufm1,*buf,*bufp1,*bufp2;
	bufm1 = Index(index-1);
	buf = Index(index);
	bufp1 = Index(index+1);
	bufp2 = Index(index+2);
	if(buf == NULL) return false;

	//�Ώۂ������i���オNULL�j�Ȃ�A�������Ȃ��ėǂ��B�A��B
	if(buf->next == NULL) return false;
	//�Ώۂ��擪�Ȃ�A�N���X����head�|�C���^���q���ς���B
	if(buf == head) head = bufp1;
	//�Ώۂ��擪�łȂ���΁A�f���ɒ��O�|�C���^���q���ς���B
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
