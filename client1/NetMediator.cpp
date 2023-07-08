#include "NetMediator.h"
#include"ckernel.h"
#include"NET.h"

NetMediator::NetMediator(){
    net = new NET(this);
}
NetMediator::~NetMediator(){
        delete net;
}
bool NetMediator::Net_Init(int PORT, const char* IP)
{
    return net->Net_Init(PORT, IP);
}

bool NetMediator::Net_Close()
{
    return net->Net_Close();
}

bool NetMediator::Net_Connect()
{
    return net->Net_Connect();
}


bool NetMediator::Recv()
{
    return net->Recv();
}

int NetMediator::DealRecvMsg(long ISendIp, char* buf, int nLen)
{
	//处理 接收到的信息，提取协议头
	//处理 接收到的信息，提取协议头

     Q_EMIT SIG_ReadyData(ISendIp,buf,nLen);
    return 1;
}



