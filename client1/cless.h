#ifndef CLESS_H
#define CLESS_H

#include"mainwindow.h"

class Cless
{
public:
    Cless();
    ~Cless();
    int color;
    bool CheakCless();
    int AValues;//进攻权重
    int DValues;//防御权重
    int change;//当前棋子回合
    int CloseAble;
    bool CheakWin(int C);//判断胜利
    void ClearAll();

};

#endif // CLESS_H
