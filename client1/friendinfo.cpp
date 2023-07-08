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

void friendinfo::closeEvent(QCloseEvent *e){
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确认退出?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        this->hide();
        e->accept();
    }else{
        e->ignore();
    }
}


void friendinfo::on_DelFriend_clicked()
{
    emit on_send_DelFriend(friendname);
}

