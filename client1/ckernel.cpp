#include "ckernel.h"
#include"./NetMediator.h"
#include"NET.h"
#include<QTextCodec>
#include<QMessageBox>

extern     ckernel* kernel;
int OfflineAble =1;
ckernel::ckernel(QObject *parent) : QObject(parent)
{
<<<<<<< HEAD
    //创建对象，负责成员里面的指针
=======

>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    Mediator = new NetMediator;
    m_wnd = new MainWindow;
    m_Ground = new Ground;
    m_add = new addfriend;
    m_houname = new housename;
    m_fGround = new FriendGround;
    m_houseL = new houseList;
    HouseName ="";
    gameList = new Game;
//    m_Finfo.push_back(new friendinfo);
//    m_Finfo.push_back(new friendinfo);
//    m_Finfo.push_back(new friendinfo);
//    m_Finfo.push_back(new friendinfo);
//    m_Finfo.push_back(new friendinfo);
//        m_fGround->addItem(m_Finfo[0]);
//        m_fGround->addItem(m_Finfo[1]);
//        m_fGround->addItem(m_Finfo[2]);
//        m_fGround->addItem(m_Finfo[3]);
//        m_fGround->addItem(m_Finfo[4]);
//        m_fGround->setLayout(m_fGround->layout);

    //连接槽函数和对应信号
    QObject::connect(m_wnd,SIGNAL(on_login_net()),this,SLOT(On_Deal_Login()));
    QObject::connect(m_wnd,SIGNAL(on_register_net()),this,SLOT(On_Deal_Register()));
    QObject::connect(m_Ground,SIGNAL(on_add_friend()),this,SLOT(On_Deal_ADD()));
    QObject::connect(m_add,SIGNAL(on_sendaddfriend()),this,SLOT(Send_Add_Friend()));
    QObject::connect(m_Ground,SIGNAL(on_friend_info()),this,SLOT(On_Deal_INFO()));
    connect(Mediator,SIGNAL(SIG_ReadyData(long,char*,int)),this,SLOT(Deal(long,char*,int)));
    QObject::connect(m_Ground,SIGNAL(on_close_windows()),this,SLOT(On_Deal_OFFLINE()));

    connect(m_Ground,SIGNAL(on_create_house()),this,SLOT(On_Deal_CREATE()));
     connect(m_houname,SIGNAL(on_housename_send(string)),this,SLOT(On_Deal_GetHouseName(string)));
     connect(m_Ground,SIGNAL(on_Reflush_house()),this,SLOT(GetUserAndHouse()));
     connect(m_houseL,SIGNAL(on_close_House(string)),this,SLOT(On_Deal_LEAVE(string)));
     connect(m_houseL,SIGNAL(on_flush_list(string)),this,SLOT(On_Deal_HouseFlush(string)));
     connect(m_houseL,SIGNAL(on_send_GameHouse(string)),this,SLOT(On_Deal_PLAY(string)));
     connect(gameList,SIGNAL(on_sendCless_msg(int,int)),this,SLOT(On_Deal_Plying(int,int)));
     connect(gameList,SIGNAL(on_sendask_Cheak(string)),this,SLOT(On_Deal_PlayCheak(string)));
     connect(gameList,SIGNAL(on_sendFaile()),this,SLOT(On_Deal_Failed()));
     connect(m_Ground,SIGNAL(on_Ai_Game()),gameList,SLOT(on_Deal_AIgame()));
     connect(m_Ground,SIGNAL(on_ShowSetting()),this,SLOT(On_Deal_ShowSetting()));
     connect(m_wnd,SIGNAL(on_closeN()),this,SLOT(On_Deal_CloseNow()));
     connect(gameList,SIGNAL(on_EndVsgame(string,string,string)),this,SLOT(On_Deal_SendVsMsg(string,string,string)));
     connect(m_Ground,SIGNAL(on_GetVshistory()),this,SLOT(On_Deal_GetVsHistory()));
<<<<<<< HEAD
     //绑定类函数指针和类中的功能函数
     PFUNinit();
     //网络初始化
=======
     PFUNinit();
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
       Mediator->Net_Init(port,ip);
       Mediator->Net_Connect();
       //登陆界面
    m_wnd->show();

}
ckernel::~ckernel()
{
    //释放资源
    if(OfflineAble)
    On_Deal_OFFLINE();

     Mediator->Net_Close();
    if(m_wnd){
        m_wnd->hide();
        delete m_wnd;
        m_wnd=nullptr;
    }
    if(m_houname){
        m_houname->hide();
        delete m_houname;
        m_houname=nullptr;
    }
    if(m_Ground){
        m_Ground->hide();
        delete m_Ground;
        m_Ground=nullptr;
    }
    if(m_Finfo.size()>0)
    for(list<friendinfo*>::iterator ite = m_Finfo.begin();ite!=m_Finfo.end();){
        if(*ite!=nullptr)
        {
            (*ite)->close();
            delete *ite;

            *ite=nullptr;
        }
        ite=m_Finfo.erase(ite);
    }

    if(m_house.size()>0)
    for(list<House*>::iterator ite = m_house.begin();ite!=m_house.end();){
        if(*ite!=nullptr)
        {
            (*ite)->close();
            delete *ite;

            *ite=nullptr;
        }
        ite=m_house.erase(ite);
    }

    if(m_add){
        m_add->hide();
        delete m_add;
        m_add=nullptr;
    }

    if(m_fGround){
        m_fGround->hide();
        delete m_fGround;
        m_fGround=nullptr;
    }
    if(m_houseL){
        m_houseL->hide();
        delete m_houseL;
        m_houseL=nullptr;
    }
    if(gameList){
        gameList->hide();
        delete gameList;
        gameList= NULL;
    }
   cout<<"close now\n";
   //关闭接收数据包的线程
   this->Mediator->net->m_isStop = true;
    if(Mediator)
    delete Mediator;
    exit(-1);//退出进程
}

void ckernel::PFUNinit(){
    //绑定函数和 类中的功能函数
    m_netProtocolMap[_DEF_PROTOCOL_Login_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Login;
    m_netProtocolMap[_DEF_PROTOCOL_ONLINE_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Online;
    m_netProtocolMap[_DEF_PROTOCOL_OFFLINE_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Offline;
    m_netProtocolMap[_def_PROTOCOL_register_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Register;
    m_netProtocolMap[_def_PROTOCOL_friend_INFO_RS - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Friend_Info;
    m_netProtocolMap[_def_PROTOCOL_add_friend_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Add_Friend;
    m_netProtocolMap[_def_PROTOCOL_chat_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Chat;
    m_netProtocolMap[_def_PROTOCOL_Join_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Join;
    m_netProtocolMap[_def_PROTOCOL_Create_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Create;
    m_netProtocolMap[_def_PROTOCOL_Leave_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Leave;
    m_netProtocolMap[_def_PROTOCOL_Play_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Play;
    m_netProtocolMap[_def_PROTOCOL_add_friend_rs1 - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Friend;
    m_netProtocolMap[_def_PROTOCOL_add_friend_rs2 - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_fin_friend;
    m_netProtocolMap[_def_PROTOCOL_Create_Rs - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Create_rs;
    m_netProtocolMap[_def_PROTOCOL_House_List - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_House_Info;
      m_netProtocolMap[_def_PROTOCOL_reflush - _DEF_PROTOCOL_BASE_] =&ckernel::Del_House_Reflush;
       m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Process - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Playing;
       m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Cheak - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_PlayCheak;
       m_netProtocolMap[_def_PROTOCOL_STRU_MyFailed - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_Failed;
       m_netProtocolMap[_def_PROTOCOL_AliveTest - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_AliveTest;
       m_netProtocolMap[_def_PROTOCOL_DeleteHouseReflush - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_DeleteReflush;
       m_netProtocolMap[_def_PROTOCOL_HouseNumberReflush - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_HouseNumber;
       m_netProtocolMap[_def_PROTOCOL_AskHostJoin - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_HostAsk;
       m_netProtocolMap[_def_PROTOCOL_WaitOk - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_WaitOk;
       m_netProtocolMap[_def_PROTOCOL_History - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_Vshistory;

}

<<<<<<< HEAD
=======
void ckernel::PFUNinit(){
    m_netProtocolMap[_DEF_PROTOCOL_Login_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Login;
    m_netProtocolMap[_DEF_PROTOCOL_ONLINE_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Online;
    m_netProtocolMap[_DEF_PROTOCOL_OFFLINE_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Offline;
    m_netProtocolMap[_def_PROTOCOL_register_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Register;
    m_netProtocolMap[_def_PROTOCOL_friend_INFO_RS - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Friend_Info;
    m_netProtocolMap[_def_PROTOCOL_add_friend_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Add_Friend;
    m_netProtocolMap[_def_PROTOCOL_chat_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Chat;
    m_netProtocolMap[_def_PROTOCOL_Join_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Join;
    m_netProtocolMap[_def_PROTOCOL_Create_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Create;
    m_netProtocolMap[_def_PROTOCOL_Leave_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Leave;
    m_netProtocolMap[_def_PROTOCOL_Play_ - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Play;
    m_netProtocolMap[_def_PROTOCOL_add_friend_rs1 - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Friend;
    m_netProtocolMap[_def_PROTOCOL_add_friend_rs2 - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_fin_friend;
    m_netProtocolMap[_def_PROTOCOL_Create_Rs - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Create_rs;
    m_netProtocolMap[_def_PROTOCOL_House_List - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_House_Info;
      m_netProtocolMap[_def_PROTOCOL_reflush - _DEF_PROTOCOL_BASE_] =&ckernel::Del_House_Reflush;
       m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Process - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Playing;
       m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Cheak - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_PlayCheak;
       m_netProtocolMap[_def_PROTOCOL_STRU_MyFailed - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_Failed;
       m_netProtocolMap[_def_PROTOCOL_AliveTest - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_AliveTest;
       m_netProtocolMap[_def_PROTOCOL_DeleteHouseReflush - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_DeleteReflush;
       m_netProtocolMap[_def_PROTOCOL_HouseNumberReflush - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_HouseNumber;
       m_netProtocolMap[_def_PROTOCOL_AskHostJoin - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_HostAsk;
       m_netProtocolMap[_def_PROTOCOL_WaitOk - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_WaitOk;
       m_netProtocolMap[_def_PROTOCOL_History - _DEF_PROTOCOL_BASE_] =
           &ckernel::Del_Vshistory;

}

>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d

void ckernel::Deal(long ISendIp, char* buf, int nLen)
{
    //根据协议，划分功能
    //函数里面与数据库交互
    int type = *(int*)buf;//得到包内类型
    cout<<"type:  "<<type<<"size: "<<nLen<<endl;
    if(type>_DEF_PROTOCOL_BASE_&&type<_DEF_PROTOCOL_BASE_+_DEF_PROTOCOL_COUNT&&m_netProtocolMap[type-_DEF_PROTOCOL_BASE_]!=nullptr){
<<<<<<< HEAD
        //if里面为类型发范围判断和是否超过绑定
        (this->*m_netProtocolMap[type-_DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);//执行函数
=======
        (this->*m_netProtocolMap[type-_DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    }

}

void  ckernel::Del_Login(long ISendIp, char* buf, int nLen) {
    //处理登陆
    cout << "get Del_Login" << endl;
    STRU_LOGIN l = (*(STRU_LOGIN*)buf);
    cout<<l.nType<<"  "<<l.password<<"  "<<l.szName<<endl;
    //得到服务器的判断条件
    if(strcmp(l.szName,login_success)==0){
        QMessageBox::about(m_wnd,"提示","登陆成功");
        m_wnd->hide();
        m_Ground->show();
        On_Deal_INFO();
    }else if(strcmp(l.szName,password_error)==0){
        QMessageBox::about(m_wnd,"提示","密码错误");
        return;
    }else if(strcmp(l.szName,user_not_exist)==0){
        QMessageBox::about(m_wnd,"提示","用户不存在");
        return;
    }else if(strcmp(l.szName,"havethis")==0){
        QMessageBox::about(m_wnd,"提示","用户已登陆");
        return;
    }

    On_Deal_ONLINE();//登陆成功，执行上限报文的发送
}
// 处理数据
void ckernel::Del_Online(long ISendIp, char* buf, int nLen)
{
    //可以加入收到其他玩家上限的消息，但是以及在好友中实现
}

void ckernel::Del_Offline(long ISendIp, char* buf, int nLen)
{
    cout << "get Del_Offline" << endl;
    //离线接收，以及在好友中实现
}

void ckernel::Del_Register(long ISendIp, char* buf, int nLen)
{
    //处理服务器发来的 注册结果
       STRU_REGISTER l = (*(STRU_REGISTER*)buf);
    cout << "get Del_Register" << endl;
    if(strcmp(l.userName,register_success)==0){
        QMessageBox::about(m_wnd,"提示","注册成功");
    }else if(strcmp(l.userName,user_is_exist)==0){
        QMessageBox::about(m_wnd,"提示","用户存在");
    }else if(strcmp(l.userName,name_is_used)==0){
        QMessageBox::about(m_wnd,"提示","名字已存在");
    }else if(strcmp(l.userName,"error")==0){
        QMessageBox::about(m_wnd,"提示","密码不合规");
    }else if(strcmp(l.userName,"cantbe")==0){
        QMessageBox::about(m_wnd,"提示","名字不合规");
    }
}

void ckernel::Del_Friend_Info(long ISendIp, char* buf, int nLen){
    //添加控件，显示好友列表
    STRU_INFO_RS rs = *(STRU_INFO_RS*)buf;

    if(strcmp(rs.userName,"No")==0){
        QMessageBox::about(m_Ground,"提示","目前没有好友，去添加一个吧");
        return;
    }

    if(strcmp(rs.userName,"error")!=0){
        //如果查表成功，找到列表上面没有出现的名字
    bool tre =true;

    cout<<rs.state<<"  "<<rs.userName<<endl;
    for(list<friendinfo*>::iterator ite = m_Finfo.begin();ite!=m_Finfo.end();ite++){
        cout<<(*ite)->friendname<<endl;
        if( (*ite)!=nullptr&&(*ite)->friendname==rs.userName){
               tre=false;
        }
    }

    if(rs.state==1&&tre){
        //追加新的好友控件
        m_Finfo.push_back(new friendinfo);
        m_fGround->addItem(m_Finfo.back());
        m_Finfo.back()->friendname=rs.userName;
        m_Finfo.back()->GetName();
        //连接新的好友控件和其槽函数
        connect( m_Finfo.back(),SIGNAL(on_send_chatmsg(string)),m_Finfo.back()->m_talk,SLOT(On_CHAR_OPEN(string)));
        connect( m_Finfo.back(),SIGNAL(on_send_DelFriend(string)),this,SLOT(On_Deal_DelFrriend(string)));
        connect(m_Finfo.back()->m_talk,SIGNAL(on_Send_chat(string,TalkInfo*)),this,SLOT(On_Deal_CHAR(string,TalkInfo*)));
        connect(m_Finfo.back()->m_talk,SIGNAL(on_PlayWithFriends(string)),this,SLOT(On_Deal_FriendVS(string)));

        if(m_fGround->ShouldAnsName==m_Finfo.back()->friendname){
            //有人发消息
            m_Finfo.back()->on_send_chatmsg(m_fGround->ShouldAnsName);
        }
    }
   }
    cout<<"m_Ground->firstOpen"<<m_Ground->firstOpen<<endl;
            if(! m_Ground->firstOpen){
                //好友列表第一次打开
                m_fGround->show();
            }
            m_Ground->firstOpen =false;
}

void ckernel::Del_Friend(long ISendIp, char* buf, int nLen)
{
    //被添加好友 是否同意的过程
    STRU_ADD_RS *rq = (STRU_ADD_RS *)buf;
    STRU_ADD_RS2 rs;
    QString str = QString("[%1] 请求添加你为好友").arg(rq->userName);
    if(QMessageBox::Yes==QMessageBox::question(m_Ground,"提示",str)){
        rs.result=1;
    }else{
        rs.result=0;
    }
    //发送结果
    strcpy(rs.userName,rq->userName);
    strcpy(rs.friendName,rq->friendName);
    Mediator->net->Send((char*)&rs,rs.size);
}
void ckernel::Del_fin_friend(long ISendIp, char* buf, int nLen){
    //返还给客户端 最后结果，即对端的结果
     STRU_ADD_RS2*rs=(STRU_ADD_RS2*)buf;
    if(rs->result==1){
         QMessageBox::about(m_wnd,"提示","用户同意添加");
    }else{
         QMessageBox::about(m_wnd,"提示","用户不同意添加");
    }
}
void ckernel::Del_Chat(long ISendIp, char* buf, int nLen)
{
    //处理聊天过程
    STRU_CHAR*rq=(STRU_CHAR*)buf;
    if(strcmp(rq->content,"offline")==0){
        //离线
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("好友不在线，无法收到消息");
        msgBox.exec();
        return;
    }
    //看看是否已经存在控件
    TalkInfo* m_talk =nullptr;
    for(auto c:m_Finfo){
        if(strcmp(c->friendname.c_str(),rq->szName)==0){
            m_talk =c->m_talk;
        }
    }
    if(m_talk ==nullptr){
        //添加新的控件和其槽函数
       cout<<"error"<<endl;
       m_Finfo.push_back(new friendinfo);
       m_fGround->addItem(m_Finfo.back());
       m_Finfo.back()->friendname=rq->szName;
       m_Finfo.back()->GetName();
       m_talk = m_Finfo.back()->m_talk;
       connect( m_Finfo.back(),SIGNAL(on_send_chatmsg(string)),m_Finfo.back()->m_talk,SLOT(On_CHAR_OPEN(string)));
       connect( m_Finfo.back(),SIGNAL(on_send_DelFriend(string)),this,SLOT(On_Deal_DelFrriend(string)));
       connect(m_Finfo.back()->m_talk,SIGNAL(on_Send_chat(string,TalkInfo*)),this,SLOT(On_Deal_CHAR(string,TalkInfo*)));
       connect(m_Finfo.back()->m_talk,SIGNAL(on_PlayWithFriends(string)),this,SLOT(On_Deal_FriendVS(string)));

    }

    m_talk->SetTalk(rq->szName,QString::fromStdString(rq->content));
    //设置提示
    if(!m_fGround->m_FriendIgnored[rq->friendName]&&m_talk->friendname!=rq->szName){
    QMessageBox msgBox;
    msgBox.setText("提示");
      QPushButton *Button0 = msgBox.addButton(tr("转到界面"), QMessageBox::ActionRole);
      QPushButton *Button1 = msgBox.addButton(tr("稍后处理"),QMessageBox::ActionRole);
      QPushButton *Button2 = msgBox.addButton(tr("忽略"),QMessageBox::ActionRole);

    msgBox.setInformativeText(QString("用户【%1】给您发送了一条信息\n如果本次登陆不在想显示该用户的弹窗提示，请点击忽略").arg(rq->szName));
    msgBox.exec();
    //上面的点击判断
    if (msgBox.clickedButton() == Button0) {
        m_fGround->ShouldAnsName.clear();
        m_fGround->ShouldAnsName =rq->szName;
       On_Deal_INFO();

     } else if (msgBox.clickedButton() == Button1) {

     }else if(msgBox.clickedButton() ==Button2){
        //本次登陆的忽略玩家的列表
        m_fGround->m_FriendIgnored[rq->friendName] =true;
    }
}
}

void ckernel::Del_Add_Friend(long ISendIp, char* buf, int nLen)
{
    //第一次添加对端，即 添加好友的回复
    STRU_ADD l = (*(STRU_ADD*)buf);
    if(strcmp(l.friendName,register_success)==0){
        QMessageBox::about(m_wnd,"提示","已经发送请求");
    }else if(strcmp(l.friendName,no_this_user)==0){
        QMessageBox::about(m_wnd,"提示","无此用户");
    }else if(strcmp(l.friendName,friend_offline)==0){
        QMessageBox::about(m_wnd,"提示","用户离线");
    }
}

void ckernel::Del_Join(long ISendIp, char* buf, int nLen)
{
    //加入房间处理
    cout << "get Del_Join" << endl;
    STRU_JOIN rq = *(STRU_JOIN*)buf;

    if(strcmp(rq.HouseName,"-1")==0){
        QMessageBox::about(m_Ground,"提示","加入失败");
        return ;
    }else if(strcmp(rq.username,"-2")==0){
        QMessageBox::about(m_Ground,"提示","房主已经退出");
        //不存在次房间，点击后删除
        for(auto ite = m_house.begin();ite!=m_house.end();ite++){
            if(strcmp((*ite)->housename.c_str(),rq.HouseName)==0){
                (*ite)->close();
                m_Ground->DelItem((*ite));
                delete (*ite);
                m_house.erase(ite);
                break;
            }
        }
        return ;
    }else if(strcmp(rq.HouseName,"ison")==0){
         QMessageBox::about(m_Ground,"提示","已在房间");
    }else if(strcmp(rq.HouseName,"-3")==0){
        QMessageBox::about(m_Ground,"提示","房间人数已满");
        return ;
   }
    //正常加入房间
    m_houseL->show();
    m_houseL->Unstart();
    STRU_HOUSE_INFO rs;
    m_houseL->HosueName = rq.HouseName;
    strcpy(rs.HouseName,rq.HouseName);
    strcpy(rs.username,m_wnd->userName);
   //发送房间消息报文
    m_houseL->AddItem(m_wnd->userName);
    Mediator->net->Send((char*)&rs,rs.size);
}

void ckernel::Del_Create(long ISendIp, char* buf, int nLen)
{
    //其他用户接收到房间的消息时

    cout<<"Del_Create"<<endl;
  STRU_CREATE rq =*(STRU_CREATE*)buf;
  bool tru =false;//表示是否存在同名控件
  for(auto ite = m_house.begin();ite!=m_house.end();ite++){
        if((*ite)->housename == rq.HouseName){
            tru =true;
        }
  }
   QMessageBox::about(m_Ground,"提示",QString("已经创建房间[%1],房主为[%2]").arg(rq.HouseName).arg(rq.hostName));
    if(!tru){
        //添加房间控件
    m_house.push_back(new House);
    auto ite = m_house.rbegin();
    (*ite)->SetHouseName(rq.HouseName);
    m_Ground->AddItem( (*ite));
    connect( (*ite),SIGNAL(on_join_house(string)),this,SLOT(On_Deal_JOIN(string)));
    }
        m_houname->hide();
}

void ckernel::Del_Create_rs(long ISendIp, char* buf, int nLen){
    //房主添加房间后行为
    STRU_CREATE_RS rs=*(STRU_CREATE_RS*)buf;
      if(strcmp(rs.HouseName, "isnull")==0){
           QMessageBox::about(m_Ground,"提示","房间名字为空");
           return;
      }
      if(strcmp(rs.HouseName,"-1")==0){
             QMessageBox::about(m_Ground,"提示","出现同名房间\n[暂时不支持中文]");
             return ;
      }
    //显示房间界面
      m_houseL->HosueName = rs.HouseName;
      m_houseL->show();
      m_houseL->AddItem(rs.hostName);
    m_houseL->startPb();
<<<<<<< HEAD

     bool tru =false;//表示是否存在同名控件
    for(auto ite = m_house.begin();ite!=m_house.end();ite++){
          if((*ite)->housename == rs.HouseName){
              tru =true;
          }
    }
    if(!tru){
        //添加玩家 控件显示
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
      m_house.push_back(new House);
      auto ite = m_house.rbegin();
      (*ite)->SetHouseName(m_houseL->HosueName);
      m_Ground->AddItem( (*ite));
      connect( (*ite),SIGNAL(on_join_house(string)),this,SLOT(On_Deal_JOIN(string)));
    }
    //创建房间界面隐藏
      m_houname->hide();

}

void ckernel::Del_House_Info(long ISendIp, char* buf, int nLen){
    cout<<"Del_House_Info"<<endl;
    //房间消息，服务端进行筛选后返还
    STRU_HOUSE_INFO rs = *(STRU_HOUSE_INFO*)buf;

    m_houseL->AddItem(rs.username);
}
void ckernel::Del_Leave(long ISendIp, char* buf, int nLen)
{
    //退出房间
    cout << "get Del_Leave" << endl;
    STRU_LEAVE rs =*(STRU_LEAVE*)buf;

    if(strcmp(rs.HouseName,"hostleave")==0){
        //房主退出，删除其所有控件
        m_houseL->clearAll();
        QMessageBox::about(m_wnd,"提示","房主退出");
        m_houseL->hide();
    }
    //删除退出的用户空间
    m_houseL->deleteItem(rs.UserName);

}
void ckernel::Del_Playing(long ISendIp, char* buf, int nLen){
    //得到对端五子棋的位置
    cout<<"get msg\n";
    STRU_PLAY_Process rs = *(STRU_PLAY_Process*)buf;
    cout<<rs.x<<"  "<<rs.y<<endl;
    gameList->x = rs.x *40;
    gameList->y =rs.y *40;
    gameList->draw23(rs.change);//界面绘制

}
void ckernel::Del_Play(long ISendIp, char* buf, int nLen)
{
    //游戏开始，分为 房间开始  vsmode为好友开始  和aimode 为人机
    STRU_PLAY rs = *(STRU_PLAY*)buf;
    if(strcmp(rs.HouseName,"VSmode")==0&&!m_fGround->m_FriendIgnored[rs.username]){
        //可以进行 好友对局，对端发送的邀请显示在本端
        gameList->VSmode =true;
        QMessageBox msgBox;
        //判断是否同意
        msgBox.setText("提示");
          QPushButton *Button0 = msgBox.addButton(tr("开始对局"), QMessageBox::ActionRole);
          QPushButton *Button1 = msgBox.addButton(tr("稍后对局"),QMessageBox::ActionRole);
          QPushButton *Button2 = msgBox.addButton(tr("忽略"),QMessageBox::ActionRole);
          msgBox.setInformativeText(QString("您的好友用户【%1】邀请你和他进行对局\n如果本次登陆不在想显示该用户的弹窗提示，请点击忽略").arg(rs.username));
          msgBox.exec();
          if (msgBox.clickedButton() == Button0) {
                //同意开始
              STRU_PLAY_Cheak rq;
              strcpy(rq.quest,"yes");
              strcpy(rq.username,m_wnd->userName);
              strcpy(rq.HouseName,"VSmode");
              strcpy(rq.username1,rs.username);
                //发送同意报文
              Mediator->net->Send((char*)&rq,rq.size);
<<<<<<< HEAD
              //界面初始化
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
              gameList->show();
              gameList->Pix=QPixmap(600, 600);
              gameList->Pix.fill(Qt::white);
              gameList->first =true;
              gameList->x=-1000;
              gameList->y=-1000;
              gameList->Dchange=1;
              gameList->cless->change =0;
              gameList->cless->ClearAll();
              gameList->draw23(gameList->cless->change);
              gameList->SetName(rs.username);
              gameList->againestName = rs.username;
              QMessageBox::about(m_wnd,"提示","发起者为白棋，您是黑棋");



           } else if (msgBox.clickedButton() == Button1) {
              //不同意
              STRU_PLAY_Cheak rq;
              strcpy(rq.quest,"no");
              strcpy(rq.username,m_wnd->userName);
              strcpy(rq.HouseName,"VSmode");
              strcpy(rq.username1,rs.username);
              Mediator->net->Send((char*)&rq,rq.size);//发送不同意报文
              gameList->close();

           }else if(msgBox.clickedButton() ==Button2){
              //屏蔽对端，加入屏蔽的map
              //可以同时提供删除好友的功能
              m_fGround->m_FriendIgnored[rs.username] =true;
              STRU_PLAY_Cheak rq;
              strcpy(rq.quest,"no");
              strcpy(rq.username,m_wnd->userName);
              strcpy(rq.HouseName,"VSmode");
              strcpy(rq.username1,rs.username);
              Mediator->net->Send((char*)&rq,rq.size);//发送拒绝
              gameList->close();
          }
        return;
    }

    cout<<"  "<<rs.username<<"  "<<rs.username1<<endl;
    if(rs.count ==2){
    //房间存在两人
    if(strcmp(rs.hostname,m_wnd->userName)==0){
    gameList->cless->change =1; //房主为1 为白棋
    gameList->Dchange =1;
    QMessageBox::about(gameList,"提示","你是白棋");
    //同步确认
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("开局对战确认");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

        STRU_WaitOk rq;
        strcpy(rq.username,m_wnd->userName);
        strcpy(rq.friendname,rs.username1);
        Mediator->net->Send((char*)&rq,rq.size);//让黑方也确认一下

<<<<<<< HEAD
    Sleep(500);//等待网络发送报文的过程
        On_Deal_SendVsMsg("No",rs.username1,"a");//记录开始的对局
=======
    Sleep(500);
        On_Deal_SendVsMsg("No",rs.username1,"a");
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    gameList->show();

    }else{
        //接收方，机除房主外的用户
        cout<<"其他人为0 为黑棋"<<endl;
    gameList->cless->change =0; //其他人为0 为黑棋
    gameList->Dchange =1;
    QMessageBox::about(gameList,"提示","你是黑棋");
    }
    //棋盘初始化
    gameList->Pix=QPixmap(600, 600);
    gameList->Pix.fill(Qt::white);
    gameList->SetName(rs.username1);
    gameList->againestName = rs.username1;
    gameList->x=-1000;
    gameList->y=-1000;
    gameList->first =true;
    gameList->cless->ClearAll();
    gameList->draw23( gameList->cless->change);

    }else if(rs.count ==1){
<<<<<<< HEAD
        //房间只有一个人
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("是否先和人机对战（后续有玩家可以中途加入）");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
<<<<<<< HEAD
        //是否进入人机模式
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
        if(ret==QMessageBox::Ok){
        gameList->AiMode =true;
        gameList->first =true;
        gameList->AIandPlayDraw(1);
        QMessageBox::about(gameList,"提示","你是白棋");
        cout<<"Aimode"<<endl;
        gameList->show();
        }else{
            return;
        }
    }

}

void ckernel::Del_WaitOk(long ISendIp, char* buf, int nLen){
     STRU_WaitOk rq = *(STRU_WaitOk*) buf;//黑色方收到同步报文
     gameList->show();
}

void ckernel::Del_PlayCheak(long ISendIp, char* buf, int nLen){
    //游戏结束时的确认机制
    cout<<"Del_PlayCheak"<<endl;
    STRU_PLAY_Cheak rq = *(STRU_PLAY_Cheak*)buf;
    if(strcmp(rq.quest,"Quit")==0){
        //对端确定退出
        QMessageBox::about(gameList,"提示","对端退出棋盘");
    //初始化界面
        gameList->first=true;
        gameList->x=-1000;
        gameList->y=-1000;
        gameList->Dchange=1;
        gameList->cless->ClearAll();
        gameList->draw23(gameList->cless->change);
        gameList->close();

    }else if(strcmp(rq.quest,"Ok")==0){
        //对端发送再来一次请求
        //界面初始化
        gameList->Pix=gameList->clearPix;
        gameList->Pix.fill(Qt::white);

        gameList->first =true;
        gameList->x=-1000;
        gameList->y=-1000;
        gameList->Dchange=1;
        gameList->cless->ClearAll();
        gameList->draw23(gameList->cless->change);
        gameList->msgBox.close();
        //弹出窗口
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("对端请求再来一把");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();

        if(ret == QMessageBox::Cancel){

            //告诉对面我已经退出，不愿来
             emit gameList->on_sendask_Cheak("no");
            gameList->close();

        }else  if(ret == QMessageBox::Ok){


            gameList->draw23(gameList->cless->change);
            //告诉对面可以

             emit gameList->on_sendask_Cheak("yes");
            cout<<33333333<<endl;
        }
    }else if(strcmp(rq.quest,"no")==0){
        //发送请求后被拒绝
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("对端拒绝");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        //初始化界面
         gameList->Pix=QPixmap(600, 600);
         gameList->Pix.fill(Qt::white);
         gameList->first =true;
         gameList->x=-1000;
         gameList->y=-1000;
         gameList->Dchange=1;
         gameList->cless->ClearAll();
         gameList->draw23(gameList->cless->change);
         //告诉对面我已经退出
         gameList->close();

    }else if(strcmp(rq.quest,"yes")==0){
        //对端同意再来一次
        if(strcmp(rq.HouseName,"VSmode")!=0){
            //不是好友对局就初始化
        gameList->Pix=QPixmap(600, 600);
        gameList->Pix.fill(Qt::white);
        gameList->first =true;
        gameList->x=-1000;
        gameList->y=-1000;
        gameList->Dchange=1;//先请求者为白棋
        gameList->cless->ClearAll();
        gameList->draw23(gameList->cless->change);
        }
        //告诉对面可以
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("对端同意");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        On_Deal_SendVsMsg("No",gameList->againestName,"a");
    }
}

void ckernel::GetUserAndHouse(){
<<<<<<< HEAD
    //刷新房间控件
=======

>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    STRU_HOUSE_flush rs;
    strcpy(rs.username,m_wnd->userName);
    Mediator->net->Send((char*)&rs,rs.size);
    //判断当前存在的控件要不要被删除
    for(auto ite = m_house.begin();ite!=m_house.end();ite++){
            STRU_DeleteReflush rd;
        strcpy(rd.HouseName,(*ite)->housename.c_str());
        strcpy(rd.username,m_wnd->userName);
        rd.able=true;
        cout<<rd.nType<<"  12323424"<<endl;
        Mediator->net->Send((char*)&rd,rd.size);//发送给服务端处理
    }

}

void ckernel::Del_House_Reflush(long ISendIp, char* buf, int nLen){
    //刷新房间种的 添加房间

    cout<<"Del_House_Reflush"<<endl;
    STRU_HOUSE_flush rq = *(STRU_HOUSE_flush*)buf;
    bool tre =false;
        for(auto ite = m_house.begin();ite!=m_house.end();ite++){
            if((*ite)->housename==rq.HouseName){
                   tre=true;//看看在不在
            }
        }

        if(!tre){
            //有新的房间
         m_house.push_back(new House);
         auto ite = m_house.rbegin();//指向末尾  rend 是指向第一个元素的前一个位置
         m_Ground->AddItem(*ite);
         (*ite)->SetHouseName(rq.HouseName);
         connect((*ite),SIGNAL(on_join_house(string)),this,SLOT(On_Deal_JOIN(string)));
        }

}
void ckernel::Del_DeleteReflush(long ISendIp, char* buf, int nLen){
    //接收服务端的判断，看看服务端认为需不需要删除此控件
    cout<<"Del_DeleteReflush"<<endl;
    STRU_DeleteReflush rs = *(STRU_DeleteReflush*)buf;
    if(!rs.able){
        cout<<"should delete "<<rs.HouseName<<endl;
       if(m_house.size()>0)
       for(auto ite = m_house.begin();ite!=m_house.end();ite++){
           //查看有没有需要被删除的控件
            if(strcmp((*ite)->housename.c_str(),rs.HouseName)==0){
                (*ite)->close();
                m_Ground->DelItem((*ite));
                delete (*ite);
                ite=m_house.erase(ite);
            }
        }
    }
}
void ckernel::Del_Failed(long ISendIp, char* buf, int nLen){
    //对端失败的处理
    STRU_PLAY_Cheak rs = *(STRU_PLAY_Cheak*)buf;
    if(strcmp(rs.quest,"fail")==0){
        QMessageBox::about(gameList,"提示","对方认输");
        if(gameList->cless->change==1){
<<<<<<< HEAD
            //房主发送结束对局的的报文  如果房主认输，失败那里已经发送过一次了
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
            emit gameList->on_EndVsgame(gameList->againestName,gameList->againestName,"b");
        }
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("是否退出?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();

        if(ret == QMessageBox::Ok){
            //退出
            gameList->Pix=QPixmap(600, 600);
            gameList->Pix.fill(Qt::white);
            gameList->first =true;
            gameList->x=-1000;
            gameList->y=-1000;
            gameList->Dchange=1;
            gameList->cless->ClearAll();
            gameList->draw23(gameList->cless->change);
            gameList->close();
            //告诉对面我已经退出
             emit gameList->on_sendask_Cheak("Quit");
        }else{
            //再来一次
            gameList->Pix=QPixmap(600, 600);
            gameList->Pix.fill(Qt::white);
            gameList->first =true;
            gameList->x=-1000;
            gameList->y=-1000;
            gameList->Dchange=1;
            gameList->cless->ClearAll();
            gameList->draw23(gameList->cless->change);
            //看看对面有没有退出
            QMessageBox::about(gameList,"提示","已经发送邀请，等待对端回应");
            emit gameList->on_sendask_Cheak("Ok");
        }
    }
}
void ckernel::Del_AliveTest(long ISendIp, char* buf, int nLen){
    //心跳报的测试，如果结束到心跳包，需要返还给服务器alive报文
    STRU_AliveTest rq =*(STRU_AliveTest*)buf;
    cout<<"Get Alive "<<endl;
    if(strcmp(rq.quest, "Test")==0){
        //如果  心跳报文没有出问题
        memset(rq.quest,0,sizeof(rq.quest));
        strcpy(rq.quest,"test");
        Mediator->net->Send((char*)&rq,rq.size);
    }
}
void ckernel::Del_HouseNumber(long ISendIp, char* buf, int nLen){
    //房间成员刷新
        STRU_HouseNumReflush rq = *(STRU_HouseNumReflush*)buf;
        if(strcmp(rq.hostname, "DEL")!=0&&strcmp(rq.hostname, "NoDel")!=0){
            //表示一个增加新的控件
            m_houseL->AddItem(rq.hostname);
        }else{
            //对已经判断的控件进行判断时，才是 DEL 或者 NoDel
            if(strcmp(rq.hostname, "DEL")==0){
            //删除username1
                cout<<"should DEL"<<endl;
                m_houseL->deleteItem(rq.username1);
            }else{
                //不用删除
            }
        }
}

void ckernel::Del_HostAsk(long ISendIp, char* buf, int nLen){
    //中途加入请求
    STRU_AskHostJoin rq = *(STRU_AskHostJoin*)buf;
    cout<<"my name:"<<m_wnd->userName<<"Del_HostAsk  "<<rq.hostname<<endl;
    if(strcmp(m_wnd->userName,rq.hostname)==0){
    //让房主判断是否加入
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText(QString("用户【%1】请求加入房间").arg(rq.username));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();

    if(ret == QMessageBox::Ok){
        //允许加入
        strcpy(rq.username1,"yes");
        cout<<rq.HouseName<<"  "<<rq.nType<<"  "<<rq.username<<endl;
        Mediator->net->Send((char*)&rq,rq.size);
        gameList->hide();

    }else{
        //不允许加入
        strcpy(rq.username1,"no");
        Mediator->net->Send((char*)&rq,rq.size);
        }

    }else{
        //用户加入的部分，即用户收到后使用此部分
        if(strcmp(rq.username1,"no")==0){
             QMessageBox::about(m_Ground,"提示","对端不允许");

        }else{
            QMessageBox::about(m_Ground,"提示","对端允许");
            On_Deal_JOIN(rq.HouseName);
        }
    }
}

void ckernel::Del_Vshistory(long ISendIp, char* buf, int nLen){
<<<<<<< HEAD
    //得到对战的历史记录，此时是接收到服务器的数据
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    STRU_VsHistory rs=*(STRU_VsHistory*) buf;
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText(QString("总场数【%1】，胜利场数【%2】").arg(rs.SumVs).arg(rs.win));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
<<<<<<< HEAD
=======

>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    int ret = msgBox.exec();
}

//发出数据------------------------------------------------------------------------------------------------------------------------
unsigned int* MD5(const char* mStr); //MD5加密函数，并执行数据填充
void  ckernel::On_Deal_Login(){
     STRU_LOGIN SL;
    SL.nType =_DEF_PROTOCOL_Login_;
    //判断密码和名字
    if(strcmp(m_wnd->password,"")==0){
         QMessageBox::about(m_Ground,"提示","密码不可以为空");
         return;
    }
    if(strcmp(m_wnd->userName,"")==0){
        QMessageBox::about(m_Ground,"提示","名字不可以为空");
        return;
    }
    for(int i=0;i<strlen(m_wnd->userName)+1;i++){
        if(m_wnd->userName[i]==' '){
            QMessageBox::about(m_Ground,"提示","名字不可以有空格");
            return;
        }
    }
    //得到密码加密过后的md5值
    char tmpstr[256];
    strcpy(tmpstr,m_wnd->password);
    char buf1[40];

    unsigned int* tmpGroup = MD5(tmpstr);

    snprintf(buf1, 9, "%8X", tmpGroup[0]);  //A
    snprintf(buf1 + 8, 9, "%8X", tmpGroup[1]);  //B
    snprintf(buf1 + 16, 9, "%8X", tmpGroup[2]);  //C
    snprintf(buf1 + 24, 9, "%8X", tmpGroup[3]);  //D
    cout << buf1 << endl;
    delete[] tmpGroup;

    strcpy(SL.password,buf1);
    strcpy(SL.szName,m_wnd->userName);
    Mediator->net->Send((char*)&SL,SL.size);//发送MD5值
    //实际上可以添加一个字符串，实现加盐

}
void  ckernel::On_Deal_Register(){
<<<<<<< HEAD
    //发送注册报文
=======
    //正则分析是否为空
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    STRU_REGISTER SR;
   SR.nType =_def_PROTOCOL_register_;
   //判断密码和名字
   if(strcmp(m_wnd->password,"")==0){
        QMessageBox::about(m_Ground,"提示","密码不可以为空");
        return;
   }
   if(strcmp(m_wnd->userName,"")==0){
       QMessageBox::about(m_Ground,"提示","名字不可以为空");
       return;
   }
   for(int i=0;i<strlen(m_wnd->userName)+1;i++){
       if(m_wnd->userName[i]==' '){
           QMessageBox::about(m_Ground,"提示","名字不合规，不可以含空格");
           return;
       }
   }

    if(strlen(m_wnd->password)>=15){
        QMessageBox::about(m_Ground,"提示","密码不可以超过15个字符");
        return;
    }
    if(strlen(m_wnd->userName)>=_DEF_NAME_SIZE_){
        QMessageBox::about(m_Ground,"提示","密码不可以超过15个字符");
        return;
    }
    char tmpstr[256];
    strcpy(tmpstr,m_wnd->password);
    char buf1[40];
    //MD5处理
    unsigned int* tmpGroup = MD5(tmpstr);

    snprintf(buf1, 9, "%8X", tmpGroup[0]);  //A
    snprintf(buf1 + 8, 9, "%8X", tmpGroup[1]);  //B
    snprintf(buf1 + 16, 9, "%8X", tmpGroup[2]);  //C
    snprintf(buf1 + 24, 9, "%8X", tmpGroup[3]);  //D
    cout << buf1 << endl;
    delete[] tmpGroup;
   strcpy(SR.password,buf1);
   strcpy(SR.userName,m_wnd->userName);
   Mediator->net->Send((char*)&SR,SR.size);//发送报文，此时密码为MD5
}
void  ckernel:: On_Deal_ONLINE(){
    //上线处理
    STRU_ONLINE rq;
    strcpy(rq.szName,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);//发送上线报文，让服务器修改数据
}
void   ckernel::On_Deal_ADD(){
    m_add->show();//添加好友界面显示
}
void ckernel::Send_Add_Friend(){
    //发送添加好友请求
    STRU_ADD ad;
    strcpy(ad.friendName,m_add->userName);
    strcpy(ad.userName,m_wnd->userName);
    if(strcmp(m_add->userName,m_wnd->userName)==0){
        QMessageBox::about(m_Ground,"提示","不可以添加自己");
        return;
    }
    Mediator->net->Send((char*)&ad,ad.size);
//应该在好友信息信息添加，当打开好友信息的时候，recv一下，看看有没有Add_Friend类型的请求，有就弹窗
//将弹窗结果发给服务器
}

void ckernel::On_Deal_CloseNow(){
    //退出应用处理
    cout<<1111<<endl;
    closesocket(this->Mediator->net->serversock);
    this->Mediator->net->m_isStop = true;//关闭接收线程
    m_wnd->close();
    OfflineAble =0;
    delete kernel;
}
void  ckernel::On_Deal_OFFLINE(){
    //下线处理
    STRU_OFFLINE rq;
    strcpy(rq.userName,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);
    //退出程序
    closesocket(this->Mediator->net->serversock);
    this->Mediator->net->m_isStop = true;

    m_Ground->close();
    m_add->close();
    m_houname->close();
    m_fGround->close();
    m_houseL->close();

    gameList->close();

    m_wnd->close();
    //析构与关闭
    OfflineAble =0;
    delete kernel;
}

void  ckernel::On_Deal_CHAR(string friends,TalkInfo* m){
    //聊天处理
    STRU_CHAR rq;
    strcpy(rq.content,m->GetTalk(friends).c_str());
    strcpy(rq.szName,m_wnd->userName);
    strcpy(rq.friendName,friends.c_str());
     Mediator->net->Send((char*)&rq,rq.size);
}
void ckernel:: On_Deal_INFO(){
    //查询好友消息
    STRU_INFO rq;
    strcpy(rq.userName,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);//再发一次报文
}
void ckernel::On_Deal_JOIN(string housename){
    //加入房间发送报文
    STRU_JOIN rq;
    m_houseL->HosueName = housename;
    HouseName = housename;
    strcpy(rq.HouseName,housename.c_str());
    strcpy(rq.username,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);
}
void  ckernel::On_Deal_CREATE(){
    cout<<"On_Deal_CREATE"<<endl;
    m_houname->show();//显示创建房间的列表
}

void  ckernel::On_Deal_GetHouseName(string name){
   //发送 创建房间报文
    HouseName = name;
    //创建房间
    STRU_CREATE rq;
    strcpy(rq.HouseName,HouseName.c_str());
    strcpy(rq.hostName,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);
}
void  ckernel::On_Deal_LEAVE(string name){
    //发送离开房间的报文
    cout<<"On_Deal_LEAVE"<<endl;
    m_houseL->clearAll();
    STRU_LEAVE rq;
    strcpy(rq.UserName,m_wnd->userName);
    strcpy(rq.HouseName,name.c_str());
    Mediator->net->Send((char*)&rq,rq.size);
    m_houseL->hide();
}
void  ckernel::On_Deal_PLAY(string Housename){
    //发送开始游戏报文
    STRU_PLAY rq;
    gameList->AiMode =false;
    memset(rq.username,0,sizeof(rq.username));
    memset(rq.HouseName,0,sizeof(rq.HouseName));

    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.HouseName,Housename.c_str());
    memset(rq.hostname,0,sizeof(rq.hostname));
    rq.count =0;
    cout<<"HouseNmae "<<Housename<<" userName "<<rq.username<<endl;
    Mediator->net->Send((char*)&rq,rq.size);
<<<<<<< HEAD
    //表示游戏开始，发送游戏记录的报文
=======

>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    On_Deal_SendVsMsg("No",gameList->againestName,"a");
}
void ckernel::On_Deal_Plying(int x,int y){
    //游戏过程棋子的报文
    STRU_PLAY_Process rq;
    rq.x =x;
    rq.y =y;
    gameList->AiMode =false;
    rq.change = gameList->cless->change;
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.HouseName,gameList->Housename.c_str());
    strcpy(rq.username1,gameList->againestName.c_str());
    Mediator->net->Send((char*)&rq,rq.size);
}
void ckernel::On_Deal_PlayCheak(string s){
<<<<<<< HEAD
    //游戏结束后再来一把的报文发送
    if(gameList->cless->change==1){
        emit gameList->on_EndVsgame("MyName",gameList->againestName,"b");
        //此时游戏结束，发送记录结束的报文
=======
    if(gameList->cless->change==1){
        emit gameList->on_EndVsgame("MyName",gameList->againestName,"b");
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    }
    STRU_PLAY_Cheak rq;
    strcpy(rq.quest,s.c_str());
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.HouseName,gameList->Housename.c_str());
    strcpy(rq.username1,gameList->againestName.c_str());
    cout<<"send cheak"<<endl;
    Mediator->net->Send((char*)&rq,rq.size);

}
void ckernel::On_Deal_Failed(){
    //认输报文
    STRU_MyFailedk rq;
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.HouseName,gameList->Housename.c_str());
    strcpy(rq.username1,gameList->againestName.c_str());
    Mediator->net->Send((char*)&rq,rq.size);
}

void ckernel:: Utf8TOGb2312(char* gbBuf,int nLen,QString utf8){
    QTextCodec* gb2312code = QTextCodec::codecForName("gb2312");
    QByteArray ba = gb2312code->fromUnicode(utf8);
    strcpy_s(gbBuf,nLen,ba.data());
}

QString ckernel::Gb2312TOUtf8(char* qbBuf){
    QTextCodec * gb2312code=QTextCodec::codecForName("gb2312");
    return gb2312code->toUnicode(qbBuf);

}

void ckernel::On_Deal_FriendVS(string s){
    //好友对战发送报文
    STRU_PLAY rq;
    gameList->show();
    gameList->AiMode =false;
    gameList->Dchange=1;
    gameList->cless->change =1;
    gameList->draw23(gameList->cless->change);
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.hostname,s.c_str());
    strcpy(rq.HouseName,"VSmode");
    gameList->VSmode =true;
    rq.count =0;
    gameList->againestName = rq.hostname;
    gameList->SetName(gameList->againestName);
    Mediator->net->Send((char*)&rq,rq.size);
    //发送对局记录
<<<<<<< HEAD
    //好友对战不算在战绩，于是不加入
=======

>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
}

void ckernel::On_Deal_HouseFlush(string s){
    STRU_HouseNumReflush rq;
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.HouseName,m_houseL->HosueName.c_str());
    Mediator->net->Send((char*)&rq,rq.size);
    //上面是判断要不要添加，下面赋值为del后，就判断 m_house里面保存的用户在不在玩家里面了
    //如果不在 服务器判断回来 host还是del
    strcpy(rq.hostname,"DEL");

    for(auto ite =m_houseL->Plain.begin();ite!=m_houseL->Plain.end();ite++){
        strcpy(rq.username1,(*ite)->GetName().c_str());
        Mediator->net->Send((char*)&rq,rq.size);
        memset(rq.username1,0,sizeof(rq.username1));
    }
}

void ckernel::deleteItem(string s){
    //删除好友的控件函数
    for(list<friendinfo*>::iterator ite = m_Finfo.begin();ite!=m_Finfo.end();ite++){
        if((*ite)->friendname==s){
            (*ite)->close();
            delete  (*ite);
            m_fGround->deleteItem((*ite));
            ite = m_Finfo.erase(ite);
            (*ite)=nullptr;
        }
    }
}

void ckernel::On_Deal_DelFrriend(string friendName){
    //处理删除好友，此时发送要删除的好友给服务器
    cout<<"On_Deal_DelFrriend"<<friendName<<endl;
    STRU_DelFriend rq;
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.username1,friendName.c_str());
    Mediator->net->Send((char*)&rq,rq.size);
    deleteItem(friendName);
     //QMessageBox::about(gameList,"提示","本次登陆被删除好友还在列表上");
}

void ckernel::On_Deal_SendVsMsg(string winner,string againestname,string Do){
    cout<<"============================"<<endl;
<<<<<<< HEAD
    //发送对局记录 D为a为开始  为b为结束
    STRU_VsAnswer rq;
    //报文名字处理
=======
    STRU_VsAnswer rq;
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    if(winner == "MyName"){
        strcpy(rq.Winname,m_wnd->userName);
    }else{
        strcpy(rq.Winname,winner.c_str());
    }
    if(againestname.size()==0)return ;

    if(againestname =="Myname"){
        strcpy(rq.friendname,m_wnd->userName);
        strcpy(rq.username,gameList->againestName.c_str());
    }else{
            strcpy(rq.friendname,againestname.c_str());
            strcpy(rq.username,m_wnd->userName);
    }

    rq.shouldDo = Do[0];
    cout<<"my name is:"<<rq.username<<"againest with:"<<rq.friendname<<"winner is:"<<rq.Winname<<endl;
     Mediator->net->Send((char*)&rq,rq.size);
}
void ckernel::On_Deal_GetVsHistory(){
<<<<<<< HEAD
    //查询对局记录
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
    STRU_VsHistory rq;
    strcpy(rq.username,m_wnd->userName);
    //数据库负责汇总表记录  返回给用户
    Mediator->net->Send((char*)&rq,rq.size);
}
