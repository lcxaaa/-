#include "housename.h"
#include "ui_housename.h"

housename::housename(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::housename)
{
    ui->setupUi(this);
}

housename::~housename()
{
    delete ui;
}

void housename::on_pushButton_clicked()
{
    string housename = ui->lineEdit->text().toStdString();
    if(housename.size()>9){
         QMessageBox::about(this,"提示","房间名字过长");
         this->ui->lineEdit->clear();
    }else{
    emit on_housename_send(housename);
        this->hide();
        this->ui->lineEdit->clear();
    }
}


void housename::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
}

