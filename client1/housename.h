#ifndef HOUSENAME_H
#define HOUSENAME_H

#include <QWidget>
#include"./MyDefine.h"
namespace Ui {
class housename;
}

class housename : public QWidget
{
    Q_OBJECT

public:
    explicit housename(QWidget *parent = nullptr);
    ~housename();

private slots:
    void on_pushButton_clicked();//发送创建的房间的名字

    void on_pushButton_2_clicked();//清空文本

signals:
    void on_housename_send(string housename);
private:
    Ui::housename *ui;
};

#endif // HOUSENAME_H
