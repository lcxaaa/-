#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
     setWindowTitle("登陆界面");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_clicked()
{
    //登陆按钮事件
    if(ui->password->text().size() ==0) strcpy(password,"");
     if(ui->username->text().size() ==0) strcpy(userName,"");

        strcpy(password,ui->password->text().toStdString().c_str());
        strcpy(userName,ui->username->text().toStdString().c_str());
        emit on_login_net();
}


void MainWindow::on_register_2_clicked()
{
    if(ui->password->text().size() ==0) strcpy(password,"");
     if(ui->username->text().size() ==0) strcpy(userName,"");

    bool b = ui->password->text().contains(QRegExp("[\\x4e00-\\x9fa5]+"));
    if(b)
    {
        QMessageBox::about(this,"提示","密码不合规，不可以含中文");
        return;
    }


    b = ui->username->text().contains(QRegExp("[\\x4e00-\\x9fa5]+"));
    if(b)
    {
        QMessageBox::about(this,"提示","名字不合规，不可以含中文");
        return;
    }

    //注册按钮事件
    strcpy(password,ui->password->text().toStdString().c_str());
    strcpy(userName,ui->username->text().toStdString().c_str());
     emit on_register_net();

}

void MainWindow::closeEvent(QCloseEvent *e){
    //窗口关闭时弹出的提示窗口
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确认退出?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        emit on_closeN();
        e->accept();
    }else{
        e->ignore();
    }
}
