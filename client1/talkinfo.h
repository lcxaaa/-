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
    string GetTalk(string name);
    void SetTalk(string s,QString chat);
    string friendname;
    void SetEmplty();
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
