#include "NET.h"

NET::NET(NetMediator* pMendiator)
{
    Mediator = pMendiator;
	memset(&saddr, 0, sizeof(saddr));
	memset(&WsaData, 0, sizeof(WsaData));
	memset(&RecvBuf, 0, BufSize);
	memset(&SendBuf, 0, BufSize);
	serversock = 0;
	able = true;
    m_isStop =false;
}

NET::~NET()
{

    m_isStop=true;
	closesocket(serversock);
	WSACleanup();

    if (m_ThreadHadle) //m_ThreadHadle默认值为0   不为0则是已经有了线程
    {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_ThreadHadle, 100)) {
            cout << "退出线程" << endl;
            TerminateThread(m_ThreadHadle, -1);

        }
        CloseHandle(m_ThreadHadle);
        m_ThreadHadle = NULL;
    }
	cout << "disconnect" << endl;
}

bool NET::Net_Init(int PORT,const char* IP)
{
	if (WSAStartup(MAKEWORD(2,2),&WsaData) != 0) {
		cout<<"error: "<<WSAGetLastError()<<endl;
		return false;
	}
	serversock = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

	if (serversock == INVALID_SOCKET) {
		cout << "socket error: " << WSAGetLastError() << endl;
		return false;
	}
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT);
	saddr.sin_addr.S_un.S_addr = inet_addr(IP);
}

bool NET::Net_Connect()
{
	if (connect(serversock, (sockaddr*)&saddr, sizeof(saddr))!=0) {
		cout << "connect error: " << WSAGetLastError() << endl;
		return false;
	}
     m_ThreadHadle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, NULL);
	return true;
}

bool NET::Net_Close()
{
	cout << "Close now" << endl;
	return false;
}

bool NET::Send(char *buf,int size)
{
    if (send(serversock, buf, size, 0) == SOCKET_ERROR) {
		cout << "send error: " << WSAGetLastError() << endl;
		return SOCKET_ERROR;
	}

	return true;;
}

bool NET::Recv()
{
    while(!m_isStop){
        //int bufSize=0;
    // recv(serversock,(char*)&bufSize,sizeof(int),0);
	int size = recv(serversock, RecvBuf, BufSize, 0);

	if (size == SOCKET_ERROR) {
		cout << "recv error: " << WSAGetLastError() << endl;
		return SOCKET_ERROR;

    }
    int ans =0,temp=0;
         while(size>0){
                 //通过得到的buf 的type  得到它的size1  size -size1 即可

             if(ans+4<1024){
             temp=*(int*)(RecvBuf+4+ans);
             }else break;

             cout<<"size: "<<size<<"temp: "<<temp<<endl;
              this->Mediator->DealRecvMsg(serversock, RecvBuf+ans, temp);
             ans+=temp;
             size-=temp;
           }
    }
    ClearBuf();
	return true;
}

unsigned int _stdcall NET::RecvThread(void* IpVoid)
{

    NET* IpThis = (NET*)IpVoid;
    IpThis->Recv();
    cout<<"thread out"<<endl;
    return 0;
}

void NET::ReadBuf(char* buf)
{
	cout << "content: " << buf << endl;
	if (strstr(buf, "stop")) able = false;

}

void NET::ClearBuf()
{
	memset(SendBuf, 0, sizeof(BufSize));
	memset(RecvBuf, 0, sizeof(BufSize));
}
