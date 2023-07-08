#ifndef HOUSEUSERLIST_H
#define HOUSEUSERLIST_H

#include <QWidget>
#include"./MyDefine.h"
namespace Ui {
class HouseuserList;
}

class HouseuserList : public QWidget
{
    Q_OBJECT

public:
    explicit HouseuserList(QWidget *parent = nullptr);
    ~HouseuserList();
void SetText(QString name);
QString DelText();
string GetName();
private:
    Ui::HouseuserList *ui;
};

#endif // HOUSEUSERLIST_H
