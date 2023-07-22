#ifndef HOUSELIST_H
#define HOUSELIST_H

#include <QWidget>
#include"MyDefine.h"
#include<QVBoxLayout>
#include<QPlainTextEdit>
#include"./game.h"
#include"./houseuserlist.h"
namespace Ui {
class houseList;
}

class houseList : public QWidget
{
    Q_OBJECT

public:
    explicit houseList(QWidget *parent = nullptr);
    ~houseList();

    void AddItem(string name);
    QVBoxLayout *layout;
    list<HouseuserList*> Plain;
    void deleteItem(string name);
    void closeEvent(QCloseEvent *e);
    void clearAll();
    void Unstart();
    void startPb();
    string HosueName;
signals:
void on_flush_list(string name);
signals:
     void on_send_GameHouse(string housename);
signals:
    void on_close_House(string name);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::houseList *ui;
};

#endif // HOUSELIST_H
