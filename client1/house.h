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

    void SetHouseName(string housename);
        string housename;
private slots:
    void on_pushButton_clicked();

signals:
    void on_join_house(string housename);
private:
    Ui::House *ui;
};

#endif // HOUSE_H
