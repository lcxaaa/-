#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include"./MyDefine.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *e);//关闭窗口事件
private slots:

    void on_Login_clicked();//登陆按键

    void on_register_2_clicked();//注册按键

private:
    Ui::MainWindow *ui;
signals:
    void on_login_net();
    void on_register_net();
    void on_closeN();//关闭窗口时产生的事件
public:
    char userName[_DEF_NAME_SIZE_];//用户名字
    char password[_DEF_PASSWORD_SIZE];
    char content[_DEF_CONTENT_SIZE];
    char HouseName[_DEF_HOUSE_ID_SIZE];
    int GameX;
    int GameY;
};
#endif // MAINWINDOW_H
