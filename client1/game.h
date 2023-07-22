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
    void draw23(int change);
    int x;
    int y;
    Cless* cless;
    string Housename;
    string againestName;
    QPixmap Pix;
    QPixmap clearPix;
    bool AiMode;
    int Dchange;
     bool conPlay;
     bool VSmode;
    QMessageBox msgBox;
    void SetName(string name);
    bool first;
    void AIgame();
    void AttackAnalyse();
    void DefendAnalyse();
    bool AI_ClessDown();
    void AIandPlayDraw(int a);
signals:
    void  on_sendCless_msg(int x,int y);
    void  on_sendask_Cheak(string s);
    void  on_sendFaile();
    void  on_sendGameChatMsg();
    void  on_EndVsgame(string winner,string againestName,string Do);
public slots:
    void on_Deal_AIgame();
private slots:
    void on_pushButton_2_clicked();
    void on_QuitPB_clicked();

    void on_GameChat_clicked();

private:
    Ui::Game *ui;
};

#endif // GAME_H
