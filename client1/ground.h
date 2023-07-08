#ifndef GROUND_H
#define GROUND_H

#include <QWidget>
#include"./MyDefine.h"
#include"./house.h"
#include<QVBoxLayout>

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
    void AddItem(House* m_house);
    QVBoxLayout*layout;
    void DelItem(House* m_house);

    bool firstOpen;
private slots:
    void on_pushButton_3_clicked();
    void on_friendinfo_clicked();

    void on_create_house_clicked();

    void on_pushButton_clicked();

    void on_AiGamePb_clicked();

    void on_pushButton_2_clicked();

signals:
    void on_add_friend();
    void on_friend_info();
        void on_close_windows();
        void on_create_house();
        void on_Reflush_house();
        void on_Ai_Game();
        void on_ShowSetting();
private:
    Ui::Ground *ui;
};

#endif // GROUND_H
