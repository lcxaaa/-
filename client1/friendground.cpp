#include "friendground.h"
#include "ui_friendground.h"
#include"qscrollarea.h"
FriendGround::FriendGround(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendGround)
{
    ui->setupUi(this);
    layout=new QVBoxLayout();
      layout->setDirection(QBoxLayout::TopToBottom);
    this->setLayout(layout);
}

FriendGround::~FriendGround()
{
    delete layout;
    delete ui;
}
void FriendGround::addItem(friendinfo* info){
    cout<<"addItem"<<endl;
    layout->addWidget(info);
}
void FriendGround::closeEvent(QCloseEvent *e){
    //窗口关闭时弹出的提示窗口
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确认退出?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        e->accept();
    }else{
        e->ignore();
    }
}
void FriendGround:: deleteItem(friendinfo* info){
    layout->removeWidget(info);
}


