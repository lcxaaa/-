#pragma once
#include"MyDefine.h"
#include"CMySql.h"
//经验：头文件里面最好不要放其他头文件,除了必要的参数设定和必须实现的父类的.h
//如果需要其他类，就只class，在.cpp里面才使用头文件
class ckernel;

typedef void (ckernel::* PFUN)(SOCKET ISendIp, char*, int);       //类函数指针

class ckernel {
public:
	ckernel();
	~ckernel();
<<<<<<< HEAD
	PFUN m_netProtocolMap[_DEF_PROTOCOL_COUNT];//类函数指针 数组
	void PFUNinit();//函数初始化
=======
	PFUN m_netProtocolMap[_DEF_PROTOCOL_COUNT];
	void PFUNinit();
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
	void Deal(SOCKET ISendIp,int t,char* buf,int len);
	void Del_Online(SOCKET ISendIp,char* buf, int len);
	void Del_Offline(SOCKET ISendIp,char* buf, int len);
	void Del_Register(SOCKET ISendIp,char* buf, int len);
	void Del_Friend_Info(SOCKET ISendIp, char* buf, int len);
	void Del_Friend(SOCKET ISendIp, char* buf, int len);
	void Del_Chat(SOCKET ISendIp, char* buf, int len);
	void Del_Add_Friend(SOCKET ISendIp, char* buf, int len);
	void Del_Join(SOCKET ISendIp, char* buf, int len);
	void Del_Create(SOCKET ISendIp, char* buf, int len);
	void Del_Leave(SOCKET ISendIp, char* buf, int len);
	void Del_Play(SOCKET ISendIp, char* buf, int len);
	void Del_Login(SOCKET ISendIp, char* buf, int len);
	void Del_Add_RS(SOCKET ISendIp, char* buf, int len);
	void Del_House_Info(SOCKET ISendIp, char* buf, int len);
	void Del_House_Reflush(SOCKET ISendIp, char* buf, int len);
	void Del_Playing(SOCKET ISendIp, char* buf, int len);
	void Del_PlayCheak(SOCKET ISendIp, char* buf, int len);
	void Deal_Failed(SOCKET ISendIp, char* buf, int len);
	void Deal_Alive_Test(SOCKET ISendIp, char* buf, int len);
	void Deal_Delete_Reflush(SOCKET ISendIp, char* buf, int len);
	void Deal_HouseNumber(SOCKET ISendIp, char* buf, int len);
	void Deal_HostAsk(SOCKET ISendIp, char* buf, int len);
	void DeleteFriend(SOCKET ISendIp, char* buf, int len);
	void Del_WaitOk(SOCKET ISendIp, char* buf, int len);
	void Del_vsHistory(SOCKET ISendIp, char* buf, int len);
	void Del_VsAnswerGame(SOCKET ISendIp, char* buf, int len);
public:
<<<<<<< HEAD
	//用户id和套接字的对应关系
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
	map<string, SOCKET> m_mapUseridToSocket;
	CMySql sql;
};
