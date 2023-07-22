#include "houselist.h"
#include "ui_houselist.h"

houseList::houseList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::houseList)
{
    ui->setupUi(this);
    layout=new QVBoxLayout();
      layout->setDirection(QBoxLayout::TopToBottom);
    this->setLayout(layout);
}

houseList::~houseList()
{
    for(auto ite = Plain.begin();ite!=Plain.end();){
        if(*ite!=nullptr){
            delete *ite;
             *ite=nullptr;
        }
            ite=Plain.erase(ite);
    }

    delete layout;
    delete ui;
}
int k=0;


void houseList:: AddItem(string name){

     bool tru=false;
     for(auto ite = Plain.begin();ite!=Plain.end();ite++){
         if((*ite)->GetName()==name){
             tru=true;
             break;
         }
     }

     if(!tru){
    Plain.push_back(new HouseuserList);
    layout->addWidget(Plain.back());
    Plain.back()->SetText((QString::fromStdString(name)));
     }
}
void houseList::clearAll(){
    for(auto ite = Plain.begin();ite!=Plain.end();){
        if(*ite!=nullptr){
            delete *ite;
             *ite=nullptr;
        }
            ite=Plain.erase(ite);
    }
}

void houseList:: deleteItem(string name){
    for(auto ite = Plain.begin();ite!=Plain.end();ite++){
        if(strcmp((*ite)->DelText().toStdString().c_str(),name.c_str())==0){
            (*ite)->close();
            layout->removeWidget((*ite));
            delete (*ite);
            ite=Plain.erase(ite);
            break;
        }
    }

}
void houseList::closeEvent(QCloseEvent *e){
    //窗口关闭时弹出的提示窗口
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确认退出?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){

        emit on_close_House(HosueName);
        e->accept();
    }else{
        e->ignore();
    }
}

void houseList::startPb(){
    ui->pushButton->show();
}

void houseList::Unstart(){
    ui->pushButton->hide();
}

void houseList::on_pushButton_2_clicked()
{
    emit on_flush_list(HosueName);
}


void houseList::on_pushButton_clicked()
{
    //打开游戏
       emit on_send_GameHouse(HosueName);
       cout<<"HOUSElITS nAME:"<<HosueName<<endl;
    //房主默认白棋
}

