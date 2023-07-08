#pragma once
#include"MyDefine.h"
#include"NetMediator.h"
#include<winsock2.h>
using namespace std;

//底层封装
class NET {
public:
    NET(NetMediator* pMendiator);
    virtual ~NET();
	virtual bool Net_Init(int PORT, const char* IP);
	virtual bool Net_Close();
	virtual bool Net_Connect();
    virtual bool Send(char *buf,int size);
	virtual bool Recv();
	virtual void ReadBuf(char* buf);
	virtual void ClearBuf();
//下面除了WSAData，都要外界进行值传递
	SOCKET serversock;//使用  TCP ipv4的socket,指向服务器的socket
	sockaddr_in saddr; //服务器的信息
	WSAData WsaData;
	char RecvBuf[BufSize];
	char SendBuf[BufSize];
	bool able;
    NetMediator*Mediator;
    HANDLE m_ThreadHadle;
    bool m_isStop;
    static unsigned int _stdcall RecvThread(void* IpVoid);

};
