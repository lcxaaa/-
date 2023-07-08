#include "mainwindow.h"
#include"./ckernel.h"
#include"./MyDefine.h"
#include"NetMediator.h"
#include <QApplication>
#include"NET.h"
    ckernel* kernel;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    kernel =new ckernel;
    return a.exec();
}
