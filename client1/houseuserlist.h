#ifndef HOUSEUSERLIST_H
#define HOUSEUSERLIST_H

#include <QWidget>
#include"./MyDefine.h"
namespace Ui {
class HouseuserList;
}

class HouseuserList : public QWidget
{
    Q_OBJECT

public:
    explicit HouseuserList(QWidget *parent = nullptr);
    ~HouseuserList();
void SetText(QString name);//设置玩家的名字
QString DelText();//删除玩家的名字 退出时使用
string GetName();//得到玩家的名字
private:
    Ui::HouseuserList *ui;
};

#endif // HOUSEUSERLIST_H
