#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include<iostream>
#include <QPainter>
#include"./cless.h"
#include <synchapi.h>
#include<deque>
using namespace std;

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent * a);
    //绘制五子棋的函数
    void draw23(int change);
    //五子棋的x y位置，不是界面的 xy坐标
    int x;
    int y;
    Cless* cless;
    string Housename;//房间名字
    string againestName;//对方的名字
    QPixmap Pix;//保存上次绘制的图片
    QPixmap clearPix;//覆盖图，更新时使用
    bool AiMode;//ai模式
    int Dchange;//本机颜色
     bool conPlay;//一人对战
     bool VSmode;//好友对战
    QMessageBox msgBox;
    void SetName(string name);//设置控件名字
    bool first;
    void AIgame();//与AI进行游戏
    void AttackAnalyse();//AI的判断权限计算
    void DefendAnalyse();
    bool AI_ClessDown();
    //AI的棋子的绘制
    void AIandPlayDraw(int a);
signals:
    void  on_sendCless_msg(int x,int y);//棋子位置消息信号
    void  on_sendask_Cheak(string s);//结束后再来一次的信号
    void  on_sendFaile();//失败的信号
    void  on_sendGameChatMsg();
    void  on_EndVsgame(string winner,string againestName,string Do);//结束对局信号
public slots:
    void on_Deal_AIgame();
private slots:
    void on_pushButton_2_clicked();//认输槽函数
    void on_QuitPB_clicked();//退出按键槽函数

private:
    Ui::Game *ui;
};

#endif // GAME_H
