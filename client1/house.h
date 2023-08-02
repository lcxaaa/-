#ifndef HOUSE_H
#define HOUSE_H

#include <QWidget>
#include"MyDefine.h"
namespace Ui {
class House;
}

class House : public QWidget
{
    Q_OBJECT

public:
    explicit House(QWidget *parent = nullptr);
    ~House();

    void SetHouseName(string housename);//设置ui 里面的房间名字
        string housename;//房间名字
private slots:
    void on_pushButton_clicked();//加入房间的槽函数

signals:
    void on_join_house(string housename);//发给ckernel的加入房间的信号
private:
    Ui::House *ui;
};

#endif // HOUSE_H
