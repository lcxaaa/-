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
    int AValues;
    int DValues;
    int change;
    int CloseAble;
    bool CheakWin(int C);
    void ClearAll();

};

#endif // CLESS_H
