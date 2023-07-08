#ifndef SYCHATDIALOG_H
#define SYCHATDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include<useritem.h>
#include<QMenu>
#include<QMessageBox>
#include<QCloseEvent>

namespace Ui {
class sychatdialog;
}

class sychatdialog : public QDialog
{
    Q_OBJECT

public:
    explicit sychatdialog(QWidget *parent = nullptr);
    ~sychatdialog();
    void addFriend(UserItem *item);
    void setInfo(QString name,QString feeling , int iconid);
    void closeEvent(QCloseEvent* event);

signals:
    void SIG_Addfriend();
    void SIG_Offline();
private slots:
    void SLOT_DealMenu(QAction* action);
    void on_pb_tool1_clicked();

private:
    QVBoxLayout * m_layout;//垂直布局的层
    Ui::sychatdialog *ui;
    QMenu * m_menu;


};

#endif // SYCHATDIALOG_H
