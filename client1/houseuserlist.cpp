#include "houseuserlist.h"
#include "ui_houseuserlist.h"

HouseuserList::HouseuserList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HouseuserList)
{
    ui->setupUi(this);
}

HouseuserList::~HouseuserList()
{
    delete ui;
}
string HouseuserList::GetName(){
    return ui->plainTextEdit->toPlainText().toStdString();
}
void HouseuserList::SetText(QString name){
    cout<<name.toStdString();
    ui->plainTextEdit->insertPlainText(name);
    ui->plainTextEdit->setReadOnly(true);
}

QString HouseuserList::DelText(){
    return ui->plainTextEdit->toPlainText();
}
