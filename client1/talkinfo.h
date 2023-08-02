#ifndef TALKINFO_H
#define TALKINFO_H

#include <QWidget>
#include"MyDefine.h"
namespace Ui {
class TalkInfo;
}

class TalkInfo : public QWidget
{
    Q_OBJECT

public:
    explicit TalkInfo(QWidget *parent = nullptr);
    ~TalkInfo();
    string GetTalk(string name);//得到聊天的文本
    void SetTalk(string s,QString chat);//设置文本
    string friendname;//好友名字，即对方名字
    void SetEmplty();//清空输入框
public slots:
    void on_SendInfo_clicked();
    void on_PlayWithFriend_clicked();
    void On_CHAR_OPEN(string friends);
signals:
    void on_Send_chat(string s,TalkInfo* m);
    void on_PlayWithFriends(string s);
private:
    Ui::TalkInfo *ui;
};

#endif // TALKINFO_H
