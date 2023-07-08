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
    void addItem(friendinfo* info);
    void deleteItem(friendinfo* info);
        QVBoxLayout *layout;
        void closeEvent(QCloseEvent *e);
        map<string,bool> m_FriendIgnored;
        string ShouldAnsName;
private:
    Ui::FriendGround *ui;

};

#endif // FRIENDGROUND_H
