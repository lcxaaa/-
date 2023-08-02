#ifndef FRIENDINFO_H
#define FRIENDINFO_H

#include <QWidget>
#include"MyDefine.h"
#include"./talkinfo.h"
namespace Ui {
class friendinfo;
}

class friendinfo : public QWidget
{
    Q_OBJECT

public:
    explicit friendinfo(QWidget *parent = nullptr);
    ~friendinfo();
    string friendname;//好友名字
    void GetName();//得到ui控件里面的字
        TalkInfo * m_talk;
public slots:
    void on_sendMsgTo_clicked();
signals:
    //控件产生的信号
    void on_send_chatmsg(string friends);//聊天信号
    void on_send_DelFriend(string friends);//删除好友信号
private slots:
    void on_DelFriend_clicked();

private:
    Ui::friendinfo *ui;
};

#endif // FRIENDINFO_H
