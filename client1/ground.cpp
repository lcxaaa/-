#include "ground.h"
#include "ui_ground.h"

Ground::Ground(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ground)
{

    ui->setupUi(this);
     widget = new QWidget(ui->scrollArea);
     widget->setFixedSize(1000, 1000);
       widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
       layout = new QVBoxLayout(widget);
      firstOpen =true;
       ui->scrollArea->setWidget(widget);
          ui->scrollArea->setWidgetResizable(true);
          ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
           ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 水平滚动条不需显示
           setWindowTitle("大厅界面");
}

Ground::~Ground()
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

void Ground::on_pushButton_3_clicked(){

    emit  on_add_friend();
}
void Ground::AddItem(House* m_house){

    layout->addWidget(m_house);
}
void Ground::DelItem(House* m_house){
    layout->removeWidget(m_house);
}

void Ground::on_friendinfo_clicked()
{
    emit on_friend_info();
}
void Ground::closeEvent(QCloseEvent *e){
    //窗口关闭时弹出的提示窗口
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确认退出?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        emit on_close_windows();
        e->accept();
    }else{
        e->ignore();
    }
}

void Ground::on_create_house_clicked()
{
     emit on_create_house();
}


void Ground::on_pushButton_clicked()
{
    emit on_Reflush_house();
}


void Ground::on_AiGamePb_clicked()
{
    QMessageBox::about(this,"提示","和AI对战不记入战绩");
    emit on_Ai_Game();
}



void Ground::on_pushButton_2_clicked()
{
    emit on_GetVshistory();
}

