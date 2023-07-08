#include <math.h>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

void ROL(unsigned int& s, unsigned short cx); //32λ��ѭ������ʵ�ֺ���
void ltob(unsigned int& i); //B\L��ת������UINT����
unsigned int* MD5(const char* mStr); //MD5���ܺ�������ִ���������

//4����㺯��
inline unsigned int F(unsigned int X, unsigned int Y, unsigned int Z)
{
    return (X & Y) | ((~X) & Z);
}
inline unsigned int G(unsigned int X, unsigned int Y, unsigned int Z)
{
    return (X & Z) | (Y & (~Z));
}
inline unsigned int H(unsigned int X, unsigned int Y, unsigned int Z)
{
    return X ^ Y ^ Z;
}
inline unsigned int I(unsigned int X, unsigned int Y, unsigned int Z)
{
    return Y ^ (X | (~Z));
}

//32λ��ѭ������(�������)ʵ�ֺ���
void ROL(unsigned int& s, unsigned short cx)
{
    if (cx > 32)cx %= 32;
    s = (s << cx) | (s >> (32 - cx));
    return;
}

//B\L��ת������UINT����
void ltob(unsigned int& i)
{
    unsigned int tmp = i;//���渱��
    byte* psour = (byte*)&tmp, * pdes = (byte*)&i;
    pdes += 3;//����ָ�룬׼�����ҵ�ת
    for (short j = 3; j >= 0; --j)
    {
        CopyMemory(pdes - j, psour + j, 1);
    }
    return;
}

//MD5ѭ�����㺯����label=�ڼ���ѭ����1<=label<=4����lGroup����=4�����Ӹ�����M=���ݣ�16��32λ��ָ�룩
void AccLoop(unsigned short label, unsigned int* lGroup, void* M)
{
    unsigned int* i1, * i2, * i3, * i4, TAcc, tmpi = 0; //����:4��ָ�룻 T���ۼ����� �ֲ�����
    typedef unsigned int(*clac)(unsigned int X, unsigned int Y, unsigned int Z); //���庯������

    //ѭ������-λ����
    const unsigned int rolarray[4][4] = {
        { 7, 12, 17, 22 },
        { 5, 9, 14, 20 },
        { 4, 11, 16, 23 },
        { 6, 10, 15, 21 }
    };
    //���������
    const unsigned short mN[4][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 },
        { 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 },
        { 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 }
    };
    const unsigned int* pM = static_cast<unsigned int*>(M);//ת������Ϊ32λ��Uint
    TAcc = ((label - 1) * 16) + 1;
    clac clacArr[4] = { F, G, H, I }; //���岢��ʼ�����㺯��ָ������

    //һ��ѭ����ʼ��16��->16�Σ�
    for (short i = 0; i < 16; ++i)
    {
        /*����ָ���Ա任*/
        i1 = lGroup + ((0 + i) % 4);
        i2 = lGroup + ((1 + i) % 4);
        i3 = lGroup + ((2 + i) % 4);
        i4 = lGroup + ((3 + i) % 4);

        if (0 == i % 2)
        {
            //���㿪ʼ: A+F(B,C,D)+M[i]+T[i+1]
            tmpi = (*i1 + clacArr[label - 1](*i2, *i3, *i4) + pM[(mN[label - 1][i])] + (unsigned int)(0x100000000UL * abs(sin((double)(TAcc + i)))));
            //ѭ������
            ROL(tmpi, rolarray[label - 1][i % 4]);
            //��Ӳ���ֵ����һ������
            *i1 = *i2 + tmpi;
        }
        else
        {
            tmpi = (*i3 + clacArr[label - 1](*i4, *i1, *i2) + pM[(mN[label - 1][i])] + (unsigned int)(0x100000000UL * abs(sin((double)(TAcc + i)))));
            //ѭ������
            ROL(tmpi, rolarray[label - 1][i % 4]);
            //��Ӳ���ֵ����һ������
            *i3 = *i4 + tmpi;
        }
    }
    return;
}

//���ܺ���
unsigned int* MD5(const char* mStr)
{
    //���㻺�������ȣ��������������
    unsigned int mLen = strlen(mStr);
    if (mLen <= 0)
    {
        return 0;
    }
    unsigned int FillSize = 448 - ((mLen * 8) % 512); //����������bit��
    unsigned int FSbyte = FillSize / 8;               //���ֽڱ�ʾ�������
    //Ԥ��512-448=64bit,���ԭ��Ϣ�ĳ���
    unsigned int BuffLen = mLen + 8 + FSbyte;         //����������
    unsigned char* md5Buff = new unsigned char[BuffLen]; //���仺����
    CopyMemory(md5Buff, mStr, mLen); //�����ַ�����������

    //�������
    md5Buff[mLen] = 0x80; //��һ��bit���1
    ZeroMemory(&md5Buff[mLen + 1], FSbyte - 1); //����bit���0
    unsigned long long lenBit = mLen * 8ULL; //�����ַ������ȣ�׼������64bit
    CopyMemory(&md5Buff[mLen + FSbyte], &lenBit, 8);

    //��������
    unsigned int LoopNumber = BuffLen / 64; //��16����Ϊһ���飬�����������
    unsigned int A = 0x67452301, B = 0xEFCDAB89, C = 0x98BADCFE, D = 0x10325476;//��ʼ4�����ӣ�С������
    unsigned int* lGroup = new unsigned int[4];
    lGroup[0] = A;
    lGroup[1] = B;
    lGroup[2] = C;
    lGroup[3] = D;

    for (unsigned int Bcount = 0; Bcount < LoopNumber; ++Bcount) //�����ѭ����ʼ
    {
        //����4�μ����Сѭ������4*16��
        for (unsigned short Lcount = 0; Lcount < 4;)
        {
            AccLoop(++Lcount, lGroup, &md5Buff[Bcount * 64]);
        }
        //���������Ϊ��һ�ֵ����ӻ����������
        A = (lGroup[0] += A);
        B = (lGroup[1] += B);
        C = (lGroup[2] += C);
        D = (lGroup[3] += D);

    }
    //ת���ڴ��еĲ��ֺ����������ʾ
    ltob(lGroup[0]);
    ltob(lGroup[1]);
    ltob(lGroup[2]);
    ltob(lGroup[3]);
    delete[] md5Buff;
    return lGroup;
}

/*
MD5ѭ�����㺯����label=�ڼ���ѭ����1<=label<=4����lGroup����=4�����Ӹ�����M=���ݣ�16��32λ��ָ�룩
�����������з�ʽ: --A--D--C--B--���� lGroup[0]=A; lGroup[1]=D; lGroup[2]=C; lGroup[3]=B;
*/
void AccLoop_2(unsigned short label, unsigned int* lGroup, void* M)
{
    unsigned int* i1, * i2, * i3, * i4, TAcc, tmpi = 0; //����:4��ָ�룻 T���ۼ����� �ֲ�����
    typedef unsigned int(*clac)(unsigned int X, unsigned int Y, unsigned int Z); //���庯������
    const unsigned int rolarray[4][4] = {
        { 7, 12, 17, 22 },
        { 5, 9, 14, 20 },
        { 4, 11, 16, 23 },
        { 6, 10, 15, 21 }
    };//ѭ������-λ����
    const unsigned short mN[4][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 },
        { 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 },
        { 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 }
    };//���������
    const unsigned int* pM = static_cast<unsigned int*>(M);//ת������Ϊ32λ��Uint
    TAcc = ((label - 1) * 16) + 1; //���ݵڼ���ѭ����ʼ��T���ۼ���
    clac clacArr[4] = { F, G, H, I }; //���岢��ʼ�����㺯��ָ������

    /*һ��ѭ����ʼ��16��->16�Σ�*/
    for (short i = 0; i < 16; ++i)
    {
        /*����ָ���Ա任*/
        i1 = lGroup + ((0 + i) % 4);
        i2 = lGroup + ((3 + i) % 4);
        i3 = lGroup + ((2 + i) % 4);
        i4 = lGroup + ((1 + i) % 4);

        /*��һ�����㿪ʼ: A+F(B,C,D)+M[i]+T[i+1] ע:��һ����ֱ�Ӽ���T��*/
        tmpi = (*i1 + clacArr[label - 1](*i2, *i3, *i4) + pM[(mN[label - 1][i])] + (unsigned int)(0x100000000UL * abs(sin((double)(TAcc + i)))));
        ROL(tmpi, rolarray[label - 1][i % 4]);//�ڶ���:ѭ������
        *i1 = *i2 + tmpi;//������:��Ӳ���ֵ������
    }
    return;
}

