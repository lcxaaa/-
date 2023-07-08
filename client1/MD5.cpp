#include <math.h>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

void ROL(unsigned int& s, unsigned short cx); //32位数循环左移实现函数
void ltob(unsigned int& i); //B\L互转，接受UINT类型
unsigned int* MD5(const char* mStr); //MD5加密函数，并执行数据填充

//4组计算函数
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

//32位数循环左移(或称右移)实现函数
void ROL(unsigned int& s, unsigned short cx)
{
    if (cx > 32)cx %= 32;
    s = (s << cx) | (s >> (32 - cx));
    return;
}

//B\L互转，接收UINT类型
void ltob(unsigned int& i)
{
    unsigned int tmp = i;//保存副本
    byte* psour = (byte*)&tmp, * pdes = (byte*)&i;
    pdes += 3;//调整指针，准备左右调转
    for (short j = 3; j >= 0; --j)
    {
        CopyMemory(pdes - j, psour + j, 1);
    }
    return;
}

//MD5循环计算函数，label=第几轮循环（1<=label<=4），lGroup数组=4个种子副本，M=数据（16组32位数指针）
void AccLoop(unsigned short label, unsigned int* lGroup, void* M)
{
    unsigned int* i1, * i2, * i3, * i4, TAcc, tmpi = 0; //定义:4个指针； T表累加器； 局部变量
    typedef unsigned int(*clac)(unsigned int X, unsigned int Y, unsigned int Z); //定义函数类型

    //循环左移-位数表
    const unsigned int rolarray[4][4] = {
        { 7, 12, 17, 22 },
        { 5, 9, 14, 20 },
        { 4, 11, 16, 23 },
        { 6, 10, 15, 21 }
    };
    //数据坐标表
    const unsigned short mN[4][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 },
        { 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 },
        { 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 }
    };
    const unsigned int* pM = static_cast<unsigned int*>(M);//转换类型为32位的Uint
    TAcc = ((label - 1) * 16) + 1;
    clac clacArr[4] = { F, G, H, I }; //定义并初始化计算函数指针数组

    //一轮循环开始（16组->16次）
    for (short i = 0; i < 16; ++i)
    {
        /*进行指针自变换*/
        i1 = lGroup + ((0 + i) % 4);
        i2 = lGroup + ((1 + i) % 4);
        i3 = lGroup + ((2 + i) % 4);
        i4 = lGroup + ((3 + i) % 4);

        if (0 == i % 2)
        {
            //计算开始: A+F(B,C,D)+M[i]+T[i+1]
            tmpi = (*i1 + clacArr[label - 1](*i2, *i3, *i4) + pM[(mN[label - 1][i])] + (unsigned int)(0x100000000UL * abs(sin((double)(TAcc + i)))));
            //循环左移
            ROL(tmpi, rolarray[label - 1][i % 4]);
            //相加并赋值到下一个种子
            *i1 = *i2 + tmpi;
        }
        else
        {
            tmpi = (*i3 + clacArr[label - 1](*i4, *i1, *i2) + pM[(mN[label - 1][i])] + (unsigned int)(0x100000000UL * abs(sin((double)(TAcc + i)))));
            //循环左移
            ROL(tmpi, rolarray[label - 1][i % 4]);
            //相加并赋值到下一个种子
            *i3 = *i4 + tmpi;
        }
    }
    return;
}

//加密函数
unsigned int* MD5(const char* mStr)
{
    //计算缓冲区长度，并进行数据填充
    unsigned int mLen = strlen(mStr);
    if (mLen <= 0)
    {
        return 0;
    }
    unsigned int FillSize = 448 - ((mLen * 8) % 512); //计算需填充的bit数
    unsigned int FSbyte = FillSize / 8;               //以字节表示的填充数
    //预留512-448=64bit,填充原消息的长度
    unsigned int BuffLen = mLen + 8 + FSbyte;         //缓冲区长度
    unsigned char* md5Buff = new unsigned char[BuffLen]; //分配缓冲区
    CopyMemory(md5Buff, mStr, mLen); //复制字符串到缓冲区

    //数据填充
    md5Buff[mLen] = 0x80; //第一个bit填充1
    ZeroMemory(&md5Buff[mLen + 1], FSbyte - 1); //其它bit填充0
    unsigned long long lenBit = mLen * 8ULL; //计算字符串长度，准备填充后64bit
    CopyMemory(&md5Buff[mLen + FSbyte], &lenBit, 8);

    //数据运算
    unsigned int LoopNumber = BuffLen / 64; //以16个字为一分组，计算分组数量
    unsigned int A = 0x67452301, B = 0xEFCDAB89, C = 0x98BADCFE, D = 0x10325476;//初始4个种子，小端类型
    unsigned int* lGroup = new unsigned int[4];
    lGroup[0] = A;
    lGroup[1] = B;
    lGroup[2] = C;
    lGroup[3] = D;

    for (unsigned int Bcount = 0; Bcount < LoopNumber; ++Bcount) //分组大循环开始
    {
        //进入4次计算的小循环，共4*16次
        for (unsigned short Lcount = 0; Lcount < 4;)
        {
            AccLoop(++Lcount, lGroup, &md5Buff[Bcount * 64]);
        }
        //数据相加作为下一轮的种子或者最终输出
        A = (lGroup[0] += A);
        B = (lGroup[1] += B);
        C = (lGroup[2] += C);
        D = (lGroup[3] += D);

    }
    //转换内存中的布局后才能正常显示
    ltob(lGroup[0]);
    ltob(lGroup[1]);
    ltob(lGroup[2]);
    ltob(lGroup[3]);
    delete[] md5Buff;
    return lGroup;
}

/*
MD5循环计算函数，label=第几轮循环（1<=label<=4），lGroup数组=4个种子副本，M=数据（16组32位数指针）
种子数组排列方式: --A--D--C--B--，即 lGroup[0]=A; lGroup[1]=D; lGroup[2]=C; lGroup[3]=B;
*/
void AccLoop_2(unsigned short label, unsigned int* lGroup, void* M)
{
    unsigned int* i1, * i2, * i3, * i4, TAcc, tmpi = 0; //定义:4个指针； T表累加器； 局部变量
    typedef unsigned int(*clac)(unsigned int X, unsigned int Y, unsigned int Z); //定义函数类型
    const unsigned int rolarray[4][4] = {
        { 7, 12, 17, 22 },
        { 5, 9, 14, 20 },
        { 4, 11, 16, 23 },
        { 6, 10, 15, 21 }
    };//循环左移-位数表
    const unsigned short mN[4][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 },
        { 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 },
        { 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 }
    };//数据坐标表
    const unsigned int* pM = static_cast<unsigned int*>(M);//转换类型为32位的Uint
    TAcc = ((label - 1) * 16) + 1; //根据第几轮循环初始化T表累加器
    clac clacArr[4] = { F, G, H, I }; //定义并初始化计算函数指针数组

    /*一轮循环开始（16组->16次）*/
    for (short i = 0; i < 16; ++i)
    {
        /*进行指针自变换*/
        i1 = lGroup + ((0 + i) % 4);
        i2 = lGroup + ((3 + i) % 4);
        i3 = lGroup + ((2 + i) % 4);
        i4 = lGroup + ((1 + i) % 4);

        /*第一步计算开始: A+F(B,C,D)+M[i]+T[i+1] 注:第一步中直接计算T表*/
        tmpi = (*i1 + clacArr[label - 1](*i2, *i3, *i4) + pM[(mN[label - 1][i])] + (unsigned int)(0x100000000UL * abs(sin((double)(TAcc + i)))));
        ROL(tmpi, rolarray[label - 1][i % 4]);//第二步:循环左移
        *i1 = *i2 + tmpi;//第三步:相加并赋值到种子
    }
    return;
}

