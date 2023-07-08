 #include "sychatdialog.h"
#include "ui_sychatdialog.h"

sychatdialog::sychatdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sychatdialog)
{
    ui->setupUi(this);

    setWindowTitle("通讯软件v1.0");
    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);//包含的控件和被包含的控件的边框间距
    m_layout->setSpacing(3);//每个控件的距离

    //让wdg_list的分布变成m_layout内储存的内容    m_layout为垂直分布类型   上面调过它的间隔
    ui->wdg_list->setLayout(m_layout);

    m_menu = new QMenu ;

    m_menu ->addAction("添加好友");
    m_menu-> addAction("系统设置");
    connect(m_menu,SIGNAL(triggered(QAction*)),this,SLOT(SLOT_DealMenu(QAction*)));
}

sychatdialog::~sychatdialog()
{
    delete ui;
}

void sychatdialog:: addFriend(UserItem *item){

    m_layout->addWidget(item);

}

void sychatdialog:: setInfo(QString name,QString feeling , int iconid){

    ui->lb_name->setText(name);
    ui->le_feeling->setText(feeling);
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(iconid)));
}

void  sychatdialog:: SLOT_DealMenu(QAction* action){

    if("添加好友"==action->text()){

        Q_EMIT SIG_Addfriend();
    }else if("系统设置"==action->text()){

    }

}

void sychatdialog::on_pb_tool1_clicked()
{
    QPoint p =QCursor::pos();//得到鼠标当前的坐标
    QSize size = m_menu->sizeHint();//菜单栏的绝对大小
    //相对大小就会导致点按钮中，鼠标箭头点哪里，菜单会生成到鼠标箭头的坐标处，而不是固定的在一个地方  因为下面一步要用到菜单栏高度
    m_menu->exec(QPoint(p.x(),p.y() - size.height()));//在当前位置生成菜单栏
}

void sychatdialog:: closeEvent(QCloseEvent* event){
        event->ignore();

        if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否确认关闭")){

            Q_EMIT SIG_Offline();
        }

}


