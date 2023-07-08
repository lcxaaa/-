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
    string friendname;
    void GetName();
        void closeEvent(QCloseEvent *);
        TalkInfo * m_talk;
public slots:
    void on_sendMsgTo_clicked();
signals:
    void on_send_chatmsg(string friends);
    void on_send_DelFriend(string friends);
private slots:
    void on_DelFriend_clicked();

private:
    Ui::friendinfo *ui;
};

#endif // FRIENDINFO_H
