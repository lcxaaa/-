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

    void AddItem(string name);//添加玩家控件
    QVBoxLayout *layout;
    list<HouseuserList*> Plain;
    void deleteItem(string name);//删除控件
    void closeEvent(QCloseEvent *e);
    void clearAll();
<<<<<<< HEAD
    void Unstart();//非房主不可以开启，这个函数关闭它们的开始键显示
    void startPb();//开启房主的 开始键显示
=======
    void Unstart();
    void startPb();
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
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
