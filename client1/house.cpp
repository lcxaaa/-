#include "house.h"
#include "ui_house.h"

House::House(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::House)
{
    ui->setupUi(this);
}

House::~House()
{
    delete ui;
}
void House::SetHouseName(string housename){
    this->housename = housename;
    ui->textBrowser->append(QString::fromStdString(housename));
}

void House::on_pushButton_clicked()
{
   emit  on_join_house(housename);
}


