#include "friendground.h"
#include "ui_friendground.h"
#include"qscrollarea.h"
FriendGround::FriendGround(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendGround)
{
    ui->setupUi(this);
    widget = new QWidget(ui->scrollArea);
    widget->setFixedSize(1000, 1000);
      widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
      layout = new QVBoxLayout(widget);
      ui->scrollArea->setWidget(widget);
         ui->scrollArea->setWidgetResizable(true);
         ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
          ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 水平滚动条不需显示
      setWindowTitle("好友界面");
}

FriendGround::~FriendGround()
{
    if(layout){
        delete layout;
        layout =nullptr;
    }
    if(widget){
        delete widget;
        widget =nullptr;
    }
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


