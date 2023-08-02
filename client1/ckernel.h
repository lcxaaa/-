#ifndef CKERNEL_H
#define CKERNEL_H
#include"./mainwindow.h"
#include"./ground.h"
#include"MyDefine.h"
#include <QObject>
#include"./addfriend.h"
#include"./friendinfo.h"

#include"./friendground.h"
#include"./housename.h"
#include"./house.h"
#include"./houselist.h"
#include<QPushButton>
class NetMediator;
//经验：头文件里面最好不要放其他头文件,除了必要的参数设定和必须实现的父类的.h
//如果需要其他类，就只class，在.cpp里面才使用头文件
class ckernel;

typedef void (ckernel::* PFUN)(long , char* , int );       //类函数指针
class ckernel : public QObject
{
    Q_OBJECT
public:
     ~ckernel();
    explicit ckernel(QObject *parent = nullptr);
    PFUN m_netProtocolMap[_DEF_PROTOCOL_COUNT];//类函数指针，利用类函数指针 直到type后就可以一个语句执行多个函数
        void PFUNinit();//类函数指针初始化
        void Del_Online(long ISendIp, char* buf, int nLen);
        void Del_Playing(long ISendIp, char* buf, int nLen);
        void Del_Offline(long ISendIp, char* buf, int nLen);
        void Del_Register(long ISendIp, char* buf, int nLen);
        void Del_Friend(long ISendIp, char* buf, int nLen);
        void Del_Chat(long ISendIp, char* buf, int nLen);
        void Del_Add_Friend(long ISendIp, char* buf, int nLen);
        void Del_Join(long ISendIp, char* buf, int nLen);
        void Del_Create(long ISendIp, char* buf, int nLen);
        void Del_Leave(long ISendIp, char* buf, int nLen);
        void Del_Play(long ISendIp, char* buf, int nLen);
        void Del_Login(long ISendIp, char* buf, int nLen);
        void Del_fin_friend(long ISendIp, char* buf, int nLen);
        void Del_Friend_Info(long ISendIp, char* buf, int nLen);
        void Del_Create_rs(long ISendIp, char* buf, int nLen);
        static void Utf8TOGb2312(char* gbBuf,int nLen,QString utf8);
        static QString Gb2312TOUtf8(char* qbBuf);
        void Del_House_Info(long ISendIp, char* buf, int nLen);
        void Del_House_Reflush(long ISendIp, char* buf, int nLen);
        void Del_PlayCheak(long ISendIp, char* buf, int nLen);
        void Del_Failed(long ISendIp, char* buf, int nLen);
        void Del_AliveTest(long ISendIp, char* buf, int nLen);
        void Del_DeleteReflush(long ISendIp, char* buf, int nLen);
        void Del_HouseNumber(long ISendIp, char* buf, int nLen);
        void Del_HostAsk(long ISendIp, char* buf, int nLen);
        void Del_WaitOk(long ISendIp, char* buf, int nLen);
        void Del_Vshistory(long ISendIp, char* buf, int nLen);
        string HouseName;//所在房间的名字

        void deleteItem(string s);//删除用户控件
    private:

        NetMediator* Mediator;
        MainWindow* m_wnd;
        Ground* m_Ground;
        addfriend* m_add;
        list<friendinfo*> m_Finfo;
        FriendGround* m_fGround;
        housename* m_houname;
        list<House*>m_house;
        houseList* m_houseL;
        Game *gameList;
public slots:
        //接收对应信号，执行功能的槽函数 一把为发送报文
    void Deal(long ISendIp, char* buf, int nLen);
    void On_Deal_Login();
    void On_Deal_Register();
    void On_Deal_ONLINE();
    void On_Deal_ADD();
    void On_Deal_OFFLINE();
    void On_Deal_CHAR(string friends,TalkInfo* m);
    void On_Deal_INFO();
    void On_Deal_JOIN(string housename);
    void On_Deal_CREATE();
    void On_Deal_LEAVE(string name);
    void On_Deal_PLAY(string hostName);
    void Send_Add_Friend();
    void On_Deal_GetHouseName(string name);
    void GetUserAndHouse();
    void On_Deal_Plying(int x,int y);
    void On_Deal_PlayCheak(string s);
    void On_Deal_Failed();
    void On_Deal_FriendVS(string s);
    void On_Deal_HouseFlush(string s);
    void On_Deal_DelFrriend(string friendName);
    void On_Deal_CloseNow();
    void On_Deal_SendVsMsg(string winner,string againestName,string DO);
    void On_Deal_GetVsHistory();
};

#endif // CKERNEL_H
