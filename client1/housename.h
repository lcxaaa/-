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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void on_housename_send(string housename);
private:
    Ui::housename *ui;
};

#endif // HOUSENAME_H
