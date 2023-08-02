#include "friendinfo.h"
#include "ui_friendinfo.h"

friendinfo::friendinfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::friendinfo)
{
    ui->setupUi(this);
    m_talk = new TalkInfo;
}

friendinfo::~friendinfo()
{
    if(m_talk){
        m_talk->hide();
        delete m_talk;
        m_talk=nullptr;
    }
    delete ui;
}
void friendinfo::GetName(){
   ui->label->setText(QString::fromStdString(friendname));

}

void friendinfo::on_sendMsgTo_clicked()
{

    emit on_send_chatmsg(friendname);
}


void friendinfo::on_DelFriend_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确认删除?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
    emit on_send_DelFriend(friendname);
    }else{

    }
}

