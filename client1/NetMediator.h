#pragma once
#include<QObject>
#include"MyDefine.h"
class NET;

class NetMediator:public QObject
{
    Q_OBJECT
public:
	//客户端的NetMediator 负责调用底层，同时对底层传来的数据进行拆分
    //对上层 kernel的信息进行加工  （例如加上协议头）
    NetMediator();
    ~NetMediator();
    bool Net_Init(int PORT, const char* IP);
    bool Net_Close();
    bool Net_Connect();
    bool Send();
    bool Recv();
    int DealRecvMsg(long ISendIp, char* buf, int nLen);
    NET* net;
	int type;
signals:
    void SIG_ReadyData(long ISendIp, char* buf, int nLen);
};

