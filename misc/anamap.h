//---------------------------------------------------------------------------
#ifndef anamapH
#define anamapH
//---------------------------------------------------------------------------
#include "analist.h"


/***************************************************************
�u�E�v�Ŏg����̂ɓK�����ėp�A�z�z��
�@�@�@�u���ǂ��v���X�g�\���e���v���[�g�N���X
�i�Ȃ�Łu���ǂ��v���Ƃ�����Search()�Ƃ������\�b�h�ŘA�z�z��
�u���v�Ɏg����悤�ɂ��Ă���ɉ߂��Ȃ�����B�j

�ėp���X�g�\���e���v���[�g�N���X�AAnaClass����h�����Ă��܂��B

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

�Epublic�ȃ����o�֐��ŁA���̎d�l��Compare()�Ƃ������̊֐������邱�ƁB
�O�D�Ԓl�̌^�� bool �^�ł��邱�ƁB
�P�D�����Ƃ��� Comparator �^�̈��������邱�ƁB
�Q�D�f�t�H���g�����͎g��Ȃ��B
�R�D�������g�̉�������̎��ʃ����o�Ƃ��̈����Ƃ��ׁA��v���Ă����
�utrue�v���B��v���Ȃ���΁ufalse�v��Ԃ����ƁB
�S�D�������͉��ł��悢�B

�i��j
class HogeClass {
...
AnsiString extensions;
...
public:
bool Compare(AnsiString ext) { return extensions.Pos(ext); }
...
};

Search()���\�b�h�ɂ��āB

�T�v�F�A�z�z����ǂ����Ɏg���B�֗��Ȃ̂́ACompare�֐��̎����ɂ����
���낢��Ȕ�r���ł���悤�ɂȂ邱�ƁB
�g�p�@�F��L�̗�Ȃ�A�N���X�̃C���X�^���X��

AnaMap<HogeClass, AnsiString> ext_list;

�Ƃ���BComparator�Ƃ����̂͗v����ɔ�r����Ƃ��Ɏg���^�̂��ƁB
�g���q��"txt"�ɑΉ����Ă���HogeClass�̃C���f�b�N�X���擾����ɂ́A

int index = hwnd_list.Search("txt");
�Ƃ���B

�Ԓl�F
�@��r����A��v������̂�����΃��X�g���ł̃C���f�b�N�X�l��Ԃ��B
�@���X�g����ł���� -1 ��Ԃ��B
�@��v������̂�����Ȃ���΁A0��Ԃ��B

�@���̂悤�ɕ��L���u��v�v�������ł���悤�ɂ��Ă���B
�@��������̂͂��ŁA���X�g���q����Ή����W���[�����擾���邽�߂�
�������ꂽ�e���v���[�g�N���X�ł���B
******************************************************************/

/* �錾�� */
template<class AnaClass, class Comparator>
class AnaMap : public AnaList<AnaClass>
{
public:
	AnaMap() {};
	int SearchIndex(Comparator);
	AnaClass* Search(Comparator);
};

/* ��`���i�������j */
template<class AnaClass, class Comparator>
int AnaMap<AnaClass, Comparator>::SearchIndex(Comparator cmp)
{
	//�����擪��NULL�Ȃ�܂����̃��W���[�������[�h����Ă��Ȃ��̂�
	//���R-1��Ԃ��B
	if(head == NULL) return -1;

	AnaClass *buf1,*buf2;
	buf1 = head;
    int i = 0;
	while(!(buf1->Compare(cmp))) {
		buf2 = buf1->next;
	    buf1 = buf2;
        //�������I�[�܂ł��Ă��܂�����A�Y���Ȃ��B�Ƃ����Ӗ��ŁA
        //�擪���W���[���̃C���f�b�N�X 0 ���f�t�H���g�Ƃ��ē����L�X�B
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
