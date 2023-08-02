#include "addfriend.h"
#include "ui_addfriend.h"

addfriend::addfriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addfriend)
{
    setWindowTitle("添加好友界面");
    ui->setupUi(this);
}

addfriend::~addfriend()
{
    delete ui;
}

void addfriend::on_addfriend_2_clicked()
{
    strcpy(userName,ui->friendid->text().toStdString().c_str());
    emit on_sendaddfriend();
}

