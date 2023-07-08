#include "cless.h"
extern Cless clessArray[15][15];
Cless::Cless()
{
    AValues = 0;
    DValues = 0;
    color = 0;//为 0为空位，1为白，2为黑
    change =1; //为1 为白棋
    CloseAble =false;

}

Cless::~Cless()
{

}

bool Cless::CheakCless()
{
    if (color == 0)return true;
    return false;
}

bool Cless::CheakWin(int C)
{	//a[i][j]i为列  j为行

    int count = 0;
    // 横着判断
    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 15; i++) {
            if (clessArray[i][j].color == C) {
                count++;
            }else{
                count=0;
            }
            if (count == 5) {

                return true;
            }
        }
        count = 0;
    }
    //竖直判断
    count = 0;
    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 15; i++) {
            if (clessArray[j][i].color == C) {
                count++;
            }else{
                count=0;
            }
            if (count == 5) {

                // x1,y1  为新下的子的坐标
                return true;
            }
        }
        count = 0;
    }
    //左下斜线判断  \类型
    count = 0;
    for (int i = 0; i < 15; i++) {
        for (int j = 0, temp = i; temp < 15; j += 1, temp += 1) {
            if (clessArray[j][temp].color == C) {
                count++;
            }else{
                count=0;
            }
            if (count == 5) {

                return true;
            }
        }
        count=0;
        for (int j = 0, temp = i; temp < 15; j += 1, temp += 1) {
            if (clessArray[temp][j].color == C) {
                count++;
            }else{
                count=0;
            }
            if (count == 5) {

                return true;
            }
        }
        count = 0;
    }
    count = 0;
    //右上斜线分析   / 类型
    for (int i = 14; i >= 0; i -= 1) {
        for (int j = 0, temp = i; j< 15; temp--, j++) {
            if (clessArray[temp][j].color == C) {
                count++;
            }
            else {
                count = 0;
            }
            if (count == 5) {
                //TEMP --  所以 为 +4  +5

                return true;
            }
        }
        count = 0;
        for (int j = 14, temp = i; temp<15; temp++, j--) {
            if (clessArray[temp][j].color == C) {
                count++;
            }
            else {
                count = 0;
            }
            if (count == 5) {

                return true;
            }
        }
        count = 0;
    }
    return false;
}

void Cless::ClearAll(){
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
           clessArray[i][j].color =0;
        }
    }
    this->CloseAble=true;
}
