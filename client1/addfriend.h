#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include"./MyDefine.h"
namespace Ui {
class addfriend;
}

class addfriend : public QWidget
{
    Q_OBJECT

public:
    explicit addfriend(QWidget *parent = nullptr);
    ~addfriend();

private slots:
    void on_addfriend_2_clicked();
signals:
    void on_sendaddfriend();
private:
    Ui::addfriend *ui;
    public:
    char userName[_DEF_NAME_SIZE_];//用户名字
};

#endif // ADDFRIEND_H
