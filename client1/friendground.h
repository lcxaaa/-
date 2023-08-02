#ifndef FRIENDGROUND_H
#define FRIENDGROUND_H

#include <QWidget>
#include"./friendinfo.h"
#include<QVBoxLayout>
#include<map>
namespace Ui {
class FriendGround;
}

class FriendGround : public QWidget
{
    Q_OBJECT

public:
    explicit FriendGround(QWidget *parent = nullptr);
    ~FriendGround();
    void addItem(friendinfo* info);//添加控件
    void deleteItem(friendinfo* info);//删除控件
    //实现滚轮
        QVBoxLayout *layout;
           QWidget *widget;
        void closeEvent(QCloseEvent *e);
        //忽略好友请求的容器
        map<string,bool> m_FriendIgnored;
        string ShouldAnsName;//最近一次向用户发出聊天的好友
private:
    Ui::FriendGround *ui;

};

#endif // FRIENDGROUND_H
