#ifndef GROUND_H
#define GROUND_H

#include <QWidget>
#include"./MyDefine.h"
#include"./house.h"
#include<QVBoxLayout>
#include<QScrollArea>
namespace Ui {
class Ground;
}

class Ground : public QWidget
{
    Q_OBJECT

public:
    explicit Ground(QWidget *parent = nullptr);
    ~Ground();
    void closeEvent(QCloseEvent *);
    string friendname;
    void AddItem(House* m_house);//添加房间
    QVBoxLayout*layout;
<<<<<<< HEAD
    void DelItem(House* m_house);//删除房间
    QWidget *widget;
    bool firstOpen;//是否第一次打开好友界面
=======
    void DelItem(House* m_house);
     QScrollArea *scrollarea;

    bool firstOpen;
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
private slots:
    void on_pushButton_3_clicked();//好友添加
    void on_friendinfo_clicked();//好友消息

    void on_create_house_clicked();

    void on_pushButton_clicked();//刷新

    void on_AiGamePb_clicked();

    void on_pushButton_2_clicked();//个人战绩

signals:
<<<<<<< HEAD
    void on_add_friend();//添加好友信号
    void on_friend_info();//好友消息信号
        void on_close_windows();//关闭窗口信号，即退出
        void on_create_house();//创建房间的信号
        void on_Reflush_house();//刷新房间的信号
        void on_Ai_Game();//ai对战的信号
        void on_GetVshistory();//得到历史战绩的信号
=======
    void on_add_friend();
    void on_friend_info();
        void on_close_windows();
        void on_create_house();
        void on_Reflush_house();
        void on_Ai_Game();
        void on_GetVshistory();
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
private:
    Ui::Ground *ui;
};

#endif // GROUND_H
