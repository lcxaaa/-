#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_clicked()
{
    //登陆按钮事件
        strcpy(password,ui->password->text().toStdString().c_str());
        strcpy(userName,ui->username->text().toStdString().c_str());
        emit on_login_net();
}


void MainWindow::on_register_2_clicked()
{
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
