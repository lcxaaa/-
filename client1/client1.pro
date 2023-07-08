QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    #../3_07_network/sychatdialog.cpp \
    MD5.cpp \
    NET.cpp \
    NetMediator.cpp \
    addfriend.cpp \
    ckernel.cpp \
    cless.cpp \
    friendground.cpp \
    friendinfo.cpp \
    game.cpp \
    ground.cpp \
    house.cpp \
    houselist.cpp \
    housename.cpp \
    houseuserlist.cpp \
    main.cpp \
    mainwindow.cpp \
    talkinfo.cpp

HEADERS += \
   # ../3_07_network/sychatdialog.h \
    MyDefine.h \
    NET.h \
    NetMediator.h \
    addfriend.h \
    ckernel.h \
    cless.h \
    friendground.h \
    friendinfo.h \
    game.h \
    ground.h \
    house.h \
    houselist.h \
    housename.h \
    houseuserlist.h \
    mainwindow.h \
    talkinfo.h

FORMS += \
   # ../3_07_network/sychatdialog.ui \
    addfriend.ui \
    friendground.ui \
    friendinfo.ui \
    game.ui \
    ground.ui \
    house.ui \
    houselist.ui \
    housename.ui \
    houseuserlist.ui \
    mainwindow.ui\
    talkinfo.ui

LIBS += -lws2_32
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    NET1.sln \
    NET1.vcxproj \
    NET1.vcxproj.filters \
    NET1.vcxproj.user
