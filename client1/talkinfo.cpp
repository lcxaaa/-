#include "talkinfo.h"
#include "ui_talkinfo.h"
#include"QTime"
TalkInfo::TalkInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TalkInfo)
{
    ui->setupUi(this);

}

TalkInfo::~TalkInfo()
{
    delete ui;
}
string TalkInfo::GetTalk(string name){
        setWindowTitle(QString("与%1的聊天窗口").arg(name.c_str()));
    QString content=ui->lineEdit->text();
    ui->lineEdit->clear();
       ui->TalkInfoMsg->append(QString("【我】 %1").arg(QTime::currentTime().toString("hh:mm:ss"))); //新起一行取追加内容
       ui->TalkInfoMsg->append(content);
       return content.toStdString();
}

void TalkInfo::SetTalk(string s,QString chat){

    ui->TalkInfoMsg->append(QString("【%1】  %2").arg(s.c_str()).arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->TalkInfoMsg->append(chat);
}

void TalkInfo::on_SendInfo_clicked()
{
    emit on_Send_chat(friendname,this);
}

void TalkInfo::SetEmplty(){
    this->ui->TalkInfoMsg->clear();
}

void TalkInfo::on_PlayWithFriend_clicked()
{
    emit on_PlayWithFriends(friendname);
}

void TalkInfo::On_CHAR_OPEN(string friends){
    //判断在不在先应该
    this->show();
    this->friendname = friends;
}
