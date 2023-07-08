#include "ckernel.h"
#include"./NetMediator.h"
#include"NET.h"
#include<QTextCodec>
#include<QMessageBox>

extern     ckernel* kernel;
int OfflineAble =1;
ckernel::ckernel(QObject *parent) : QObject(parent)
{
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
       Mediator->Net_Init(port,ip);
       Mediator->Net_Connect();
    m_wnd->show();

}
ckernel::~ckernel()
{
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
   this->Mediator->net->m_isStop = true;
    if(Mediator)
    delete Mediator;
    exit(-1);
}

void ckernel::Deal(long ISendIp, char* buf, int nLen)
{
    //根据协议，划分功能
    //函数里面与数据库交互
    int type = *(int*)buf;
    cout<<"type:  "<<type<<"size: "<<nLen<<endl;
    switch (type) {
    case _DEF_PROTOCOL_Login_:
        m_netProtocolMap[_DEF_PROTOCOL_Login_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Login;
        (this->*m_netProtocolMap[_DEF_PROTOCOL_Login_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
        break;
    case _DEF_PROTOCOL_ONLINE_:
        m_netProtocolMap[_DEF_PROTOCOL_ONLINE_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Online;
    (this->*m_netProtocolMap[_DEF_PROTOCOL_ONLINE_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case _DEF_PROTOCOL_OFFLINE_:
        m_netProtocolMap[_DEF_PROTOCOL_OFFLINE_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Offline;
        (this->*m_netProtocolMap[_DEF_PROTOCOL_OFFLINE_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case _def_PROTOCOL_register_:
        m_netProtocolMap[_def_PROTOCOL_register_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Register;
        (this->*m_netProtocolMap[_def_PROTOCOL_register_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case _def_PROTOCOL_friend_INFO_RS:
        m_netProtocolMap[_def_PROTOCOL_friend_INFO_RS - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Friend_Info;
        (this->*m_netProtocolMap[_def_PROTOCOL_friend_INFO_RS - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case _def_PROTOCOL_add_friend_:
        m_netProtocolMap[_def_PROTOCOL_add_friend_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Add_Friend;
        (this->*m_netProtocolMap[_def_PROTOCOL_add_friend_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case  _def_PROTOCOL_chat_:
        m_netProtocolMap[_def_PROTOCOL_chat_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Chat;
        (this->*m_netProtocolMap[_def_PROTOCOL_chat_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case  _def_PROTOCOL_Join_:
        m_netProtocolMap[_def_PROTOCOL_Join_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Join;
        (this->*m_netProtocolMap[_def_PROTOCOL_Join_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case  _def_PROTOCOL_Create_:
        m_netProtocolMap[_def_PROTOCOL_Create_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Create;
        (this->*m_netProtocolMap[_def_PROTOCOL_Create_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case  _def_PROTOCOL_Leave_:
        m_netProtocolMap[_def_PROTOCOL_Leave_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Leave;
        (this->*m_netProtocolMap[_def_PROTOCOL_Leave_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;
    case  _def_PROTOCOL_Play_:
        m_netProtocolMap[_def_PROTOCOL_Play_ - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_Play;
        (this->*m_netProtocolMap[_def_PROTOCOL_Play_ - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
        break;
    case _def_PROTOCOL_add_friend_rs1:
            m_netProtocolMap[_def_PROTOCOL_add_friend_rs1 - _DEF_PROTOCOL_BASE_] =
                &ckernel::Del_Friend;
            (this->*m_netProtocolMap[_def_PROTOCOL_add_friend_rs1 - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
            break;
    case _def_PROTOCOL_add_friend_rs2:
            m_netProtocolMap[_def_PROTOCOL_add_friend_rs2 - _DEF_PROTOCOL_BASE_] =
                &ckernel::Del_fin_friend;
            (this->*m_netProtocolMap[_def_PROTOCOL_add_friend_rs2 - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
            break;

       case     _def_PROTOCOL_Create_Rs:
    m_netProtocolMap[_def_PROTOCOL_Create_Rs - _DEF_PROTOCOL_BASE_] =
        &ckernel::Del_Create_rs;
    (this->*m_netProtocolMap[_def_PROTOCOL_Create_Rs - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
    break;

    case _def_PROTOCOL_House_List:
        m_netProtocolMap[_def_PROTOCOL_House_List - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_House_Info;
        (this->*m_netProtocolMap[_def_PROTOCOL_House_List - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
        break;
    case _def_PROTOCOL_reflush:
             m_netProtocolMap[_def_PROTOCOL_reflush - _DEF_PROTOCOL_BASE_] =&ckernel::Del_House_Reflush;
             (this->*m_netProtocolMap[_def_PROTOCOL_reflush - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
             break;
    case _def_PROTOCOL_STRU_PLAY_Process:
             m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Process - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Playing;
             (this->*m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Process - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
             break;
    case _def_PROTOCOL_STRU_PLAY_Cheak:
            m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Cheak - _DEF_PROTOCOL_BASE_] =
                &ckernel::Del_PlayCheak;
            (this->*m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Cheak - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
            break;
    case _def_PROTOCOL_STRU_MyFailed:
            m_netProtocolMap[_def_PROTOCOL_STRU_MyFailed - _DEF_PROTOCOL_BASE_] =
                &ckernel::Del_Failed;
            (this->*m_netProtocolMap[_def_PROTOCOL_STRU_MyFailed - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
            break;
    case _def_PROTOCOL_AliveTest:
            m_netProtocolMap[_def_PROTOCOL_AliveTest - _DEF_PROTOCOL_BASE_] =
                &ckernel::Del_AliveTest;
            (this->*m_netProtocolMap[_def_PROTOCOL_AliveTest - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
            break;
    case _def_PROTOCOL_DeleteHouseReflush:
        m_netProtocolMap[_def_PROTOCOL_DeleteHouseReflush - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_DeleteReflush;
        (this->*m_netProtocolMap[_def_PROTOCOL_DeleteHouseReflush - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
        break;
    case _def_PROTOCOL_HouseNumberReflush:
        m_netProtocolMap[_def_PROTOCOL_HouseNumberReflush - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_HouseNumber;
        (this->*m_netProtocolMap[_def_PROTOCOL_HouseNumberReflush - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
        break;
    case _def_PROTOCOL_AskHostJoin:
        m_netProtocolMap[_def_PROTOCOL_AskHostJoin - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_HostAsk;
        (this->*m_netProtocolMap[_def_PROTOCOL_AskHostJoin - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
        break;
    case _def_PROTOCOL_WaitOk:
        m_netProtocolMap[_def_PROTOCOL_WaitOk - _DEF_PROTOCOL_BASE_] =
            &ckernel::Del_WaitOk;
        (this->*m_netProtocolMap[_def_PROTOCOL_WaitOk - _DEF_PROTOCOL_BASE_])(ISendIp,buf, nLen);
        break;

    default:
        cout << "unkonw connect!  error void ckernel::Deal" << endl;
    }

}

void  ckernel::Del_Login(long ISendIp, char* buf, int nLen) {
    cout << "get Del_Login" << endl;
    STRU_LOGIN l = (*(STRU_LOGIN*)buf);
    cout<<l.nType<<"  "<<l.password<<"  "<<l.szName<<endl;
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

    On_Deal_ONLINE();
}
// 处理数据
void ckernel::Del_Online(long ISendIp, char* buf, int nLen)
{
    //这个Mediator 不是 main里面的Mediator
    //这个Mediator 的buf里面没有东西，所以需要形参传入的缓冲区

}

void ckernel::Del_Offline(long ISendIp, char* buf, int nLen)
{
    cout << "get Del_Offline" << endl;

}

void ckernel::Del_Register(long ISendIp, char* buf, int nLen)
{
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
        QMessageBox::about(m_wnd,"提示","名字不可以存在空格");
    }
}
int i=0;
void ckernel::Del_Friend_Info(long ISendIp, char* buf, int nLen){
    //添加控件
    STRU_INFO_RS rs = *(STRU_INFO_RS*)buf;

    if(strcmp(rs.userName,"No")==0){
        QMessageBox::about(m_Ground,"提示","目前没有好友，去添加一个吧");
        return;
    }

    if(strcmp(rs.userName,"error")!=0){
    bool tre =true;

    cout<<rs.state<<"  "<<rs.userName<<endl;
    for(list<friendinfo*>::iterator ite = m_Finfo.begin();ite!=m_Finfo.end();ite++){
        cout<<(*ite)->friendname<<endl;
        if( (*ite)!=nullptr&&(*ite)->friendname==rs.userName){
               tre=false;
        }
    }

    if(rs.state==1&&tre){
        m_Finfo.push_back(new friendinfo);
        m_fGround->addItem(m_Finfo.back());
        m_Finfo.back()->friendname=rs.userName;
        m_Finfo.back()->GetName();

        connect( m_Finfo.back(),SIGNAL(on_send_chatmsg(string)),m_Finfo.back()->m_talk,SLOT(On_CHAR_OPEN(string)));
        connect( m_Finfo.back(),SIGNAL(on_send_DelFriend(string)),this,SLOT(On_Deal_DelFrriend(string)));
        connect(m_Finfo.back()->m_talk,SIGNAL(on_Send_chat(string,TalkInfo*)),this,SLOT(On_Deal_CHAR(string,TalkInfo*)));
        connect(m_Finfo.back()->m_talk,SIGNAL(on_PlayWithFriends(string)),this,SLOT(On_Deal_FriendVS(string)));

        if(m_fGround->ShouldAnsName==m_Finfo.back()->friendname){
            m_Finfo.back()->on_send_chatmsg(m_fGround->ShouldAnsName);
        }
    }
    }
    cout<<"m_Ground->firstOpen"<<m_Ground->firstOpen<<endl;
            if(! m_Ground->firstOpen){
                m_fGround->show();

            }
            m_Ground->firstOpen =false;
}

void ckernel::Del_Friend(long ISendIp, char* buf, int nLen)
{
    STRU_ADD_RS *rq = (STRU_ADD_RS *)buf;
    STRU_ADD_RS2 rs;
    QString str = QString("[%1] 请求添加你为好友").arg(rq->userName);
    if(QMessageBox::Yes==QMessageBox::question(m_Ground,"提示",str)){
        rs.result=1;
    }else{
        rs.result=0;
    }
    strcpy(rs.userName,rq->userName);
    strcpy(rs.friendName,rq->friendName);
    Mediator->net->Send((char*)&rs,rs.size);
}
void ckernel::Del_fin_friend(long ISendIp, char* buf, int nLen){
     STRU_ADD_RS2*rs=(STRU_ADD_RS2*)buf;
    if(rs->result==1){
         QMessageBox::about(m_wnd,"提示","用户同意添加");
    }else{
         QMessageBox::about(m_wnd,"提示","用户不同意添加");
    }
}
void ckernel::Del_Chat(long ISendIp, char* buf, int nLen)
{
    STRU_CHAR*rq=(STRU_CHAR*)buf;
    if(strcmp(rq->content,"offline")==0){
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("好友不在线，无法收到消息");
        msgBox.exec();
        return;
    }

    TalkInfo* m_talk =nullptr;
    for(auto c:m_Finfo){
        if(strcmp(c->friendname.c_str(),rq->szName)==0){
            m_talk =c->m_talk;
        }
    }
    if(m_talk ==nullptr){
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
    //contents 加密？
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

    if (msgBox.clickedButton() == Button0) {
        m_fGround->ShouldAnsName.clear();
        m_fGround->ShouldAnsName =rq->szName;
       On_Deal_INFO();

     } else if (msgBox.clickedButton() == Button1) {

     }else if(msgBox.clickedButton() ==Button2){
        m_fGround->m_FriendIgnored[rq->friendName] =true;
    }
}
}

void ckernel::Del_Add_Friend(long ISendIp, char* buf, int nLen)
{
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
    cout << "get Del_Join" << endl;
    STRU_JOIN rq = *(STRU_JOIN*)buf;

    if(strcmp(rq.HouseName,"-1")==0){
        QMessageBox::about(m_Ground,"提示","加入失败");
        return ;
    }else if(strcmp(rq.username,"-2")==0){
        QMessageBox::about(m_Ground,"提示","房主已经退出");

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
    m_houseL->show();
    m_houseL->Unstart();
    STRU_HOUSE_INFO rs;
    m_houseL->HosueName = rq.HouseName;
    strcpy(rs.HouseName,rq.HouseName);
    strcpy(rs.username,m_wnd->userName);

    m_houseL->AddItem(m_wnd->userName);
    Mediator->net->Send((char*)&rs,rs.size);
}

void ckernel::Del_Create(long ISendIp, char* buf, int nLen)
{
    cout<<"Del_Create"<<endl;
  STRU_CREATE rq =*(STRU_CREATE*)buf;
    QMessageBox::about(m_Ground,"提示","已创建房间");
    m_house.push_back(new House);
    auto ite = m_house.rbegin();
    (*ite)->SetHouseName(rq.HouseName);
    m_Ground->AddItem( (*ite));
    connect( (*ite),SIGNAL(on_join_house(string)),this,SLOT(On_Deal_JOIN(string)));
    m_houname->hide();
}

void ckernel::Del_Create_rs(long ISendIp, char* buf, int nLen){
    STRU_CREATE_RS rs=*(STRU_CREATE_RS*)buf;
      if(strcmp(rs.HouseName, "isnull")==0){
           QMessageBox::about(m_Ground,"提示","房间名字为空");
           return;
      }
      if(strcmp(rs.HouseName,"-1")==0){
             QMessageBox::about(m_Ground,"提示","出现同名房间或者已经创建房间");
             return ;
      }
      m_houseL->HosueName = rs.HouseName;
      m_houseL->show();
      m_houseL->AddItem(rs.hostName);

      m_house.push_back(new House);
      auto ite = m_house.rbegin();
      (*ite)->SetHouseName(m_houseL->HosueName);
      m_Ground->AddItem( (*ite));
      connect( (*ite),SIGNAL(on_join_house(string)),this,SLOT(On_Deal_JOIN(string)));
      m_houname->hide();
}

void ckernel::Del_House_Info(long ISendIp, char* buf, int nLen){
    cout<<"Del_House_Info"<<endl;

    STRU_HOUSE_INFO rs = *(STRU_HOUSE_INFO*)buf;

    m_houseL->AddItem(rs.username);
}
void ckernel::Del_Leave(long ISendIp, char* buf, int nLen)
{
    cout << "get Del_Leave" << endl;
    STRU_LEAVE rs =*(STRU_LEAVE*)buf;

    if(strcmp(rs.HouseName,"hostleave")==0){
        m_houseL->clearAll();
        QMessageBox::about(m_wnd,"提示","房主退出");
        m_houseL->hide();
    }
    m_houseL->deleteItem(rs.UserName);
}
void ckernel::Del_Playing(long ISendIp, char* buf, int nLen){
    cout<<"get msg\n";
    STRU_PLAY_Process rs = *(STRU_PLAY_Process*)buf;
    cout<<rs.x<<"  "<<rs.y<<endl;
    gameList->x = rs.x *40;
    gameList->y =rs.y *40;
    gameList->draw23(rs.change);

}
void ckernel::Del_Play(long ISendIp, char* buf, int nLen)
{

    STRU_PLAY rs = *(STRU_PLAY*)buf;
    if(strcmp(rs.HouseName,"VSmode")==0&&!m_fGround->m_FriendIgnored[rs.username]){
        gameList->VSmode =true;
        QMessageBox msgBox;
        msgBox.setText("提示");
          QPushButton *Button0 = msgBox.addButton(tr("开始对局"), QMessageBox::ActionRole);
          QPushButton *Button1 = msgBox.addButton(tr("稍后对局"),QMessageBox::ActionRole);
          QPushButton *Button2 = msgBox.addButton(tr("忽略"),QMessageBox::ActionRole);
          msgBox.setInformativeText(QString("您的好友用户【%1】邀请你和他进行对局\n如果本次登陆不在想显示该用户的弹窗提示，请点击忽略").arg(rs.username));
          msgBox.exec();
          if (msgBox.clickedButton() == Button0) {

              STRU_PLAY_Cheak rq;
              strcpy(rq.quest,"yes");
              strcpy(rq.username,m_wnd->userName);
              strcpy(rq.HouseName,"VSmode");
              strcpy(rq.username1,rs.username);

              Mediator->net->Send((char*)&rq,rq.size);

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
              gameList->show();

           } else if (msgBox.clickedButton() == Button1) {
              STRU_PLAY_Cheak rq;
              strcpy(rq.quest,"no");
              strcpy(rq.username,m_wnd->userName);
              strcpy(rq.HouseName,"VSmode");
              strcpy(rq.username1,rs.username);
              Mediator->net->Send((char*)&rq,rq.size);
              gameList->close();

           }else if(msgBox.clickedButton() ==Button2){
              //可以添加删除好友的功能
              m_fGround->m_FriendIgnored[rs.username] =true;
              STRU_PLAY_Cheak rq;
              strcpy(rq.quest,"no");
              strcpy(rq.username,m_wnd->userName);
              strcpy(rq.HouseName,"VSmode");
              strcpy(rq.username1,rs.username);
              Mediator->net->Send((char*)&rq,rq.size);
              gameList->close();
          }
        return;
    }

    cout<<"  "<<rs.username<<"  "<<rs.username1<<endl;
    if(rs.count ==2){

    if(strcmp(rs.hostname,m_wnd->userName)==0){
    gameList->cless->change =1; //房主为1 为白棋
    gameList->Dchange =1;
    QMessageBox::about(gameList,"提示","你是白棋");

    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("开局对战确认");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

        STRU_WaitOk rq;
        strcpy(rq.username,m_wnd->userName);
        strcpy(rq.friendname,rs.username1);
        Mediator->net->Send((char*)&rq,rq.size);

    Sleep(500);
    gameList->show();

    }else{
        cout<<"其他人为0 为黑棋"<<endl;
    gameList->cless->change =0; //其他人为0 为黑棋
    gameList->Dchange =1;
    QMessageBox::about(gameList,"提示","你是黑棋");
    }
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
        gameList->AiMode =true;
        gameList->first =true;
        gameList->AIandPlayDraw(1);
        QMessageBox::about(gameList,"提示","你是白棋");
        cout<<"Aimode"<<endl;
        gameList->show();
    }

}

void ckernel::Del_WaitOk(long ISendIp, char* buf, int nLen){
     STRU_WaitOk rq = *(STRU_WaitOk*) buf;
     gameList->show();
}

void ckernel::Del_PlayCheak(long ISendIp, char* buf, int nLen){
    cout<<"Del_PlayCheak"<<endl;
    STRU_PLAY_Cheak rq = *(STRU_PLAY_Cheak*)buf;
    if(strcmp(rq.quest,"Quit")==0){
        QMessageBox::about(gameList,"提示","对端退出棋盘");

        gameList->first=true;
        gameList->x=-1000;
        gameList->y=-1000;
        gameList->Dchange=1;
        gameList->cless->ClearAll();
        gameList->draw23(gameList->cless->change);
        gameList->close();

    }else if(strcmp(rq.quest,"Ok")==0){

        gameList->Pix=gameList->clearPix;
        gameList->Pix.fill(Qt::white);

        gameList->first =true;
        gameList->x=-1000;
        gameList->y=-1000;
        gameList->Dchange=1;
        gameList->cless->ClearAll();
        gameList->draw23(gameList->cless->change);
        gameList->msgBox.close();

        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("对端请求再来一把");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();

        if(ret == QMessageBox::Cancel){

            //告诉对面我已经退出
             emit gameList->on_sendask_Cheak("no");
            gameList->close();

        }else  if(ret == QMessageBox::Ok){


            gameList->draw23(gameList->cless->change);
            //告诉对面可以
             emit gameList->on_sendask_Cheak("yes");
            cout<<33333333<<endl;
        }
    }else if(strcmp(rq.quest,"no")==0){
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("对端拒绝");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
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
        if(strcmp(rq.HouseName,"VSmode")!=0){
        gameList->Pix=QPixmap(600, 600);
        gameList->Pix.fill(Qt::white);
        gameList->first =true;
        gameList->x=-1000;
        gameList->y=-1000;
        gameList->Dchange=1;
        gameList->cless->ClearAll();
        gameList->draw23(gameList->cless->change);
        }
        //告诉对面可以
        QMessageBox msgBox;
        msgBox.setText("提示");
        msgBox.setInformativeText("对端同意");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

void ckernel::GetUserAndHouse(){
    //刷新房间数量的函数
//    for(int i=m_Finfo.size()-1;i>=0;i--){
//        if(m_Finfo[i]!=nullptr)
//        delete m_Finfo[i];

//        m_Finfo[i] =nullptr;
//        m_Finfo[i]->close();
//        m_Finfo.pop_back();
//    }


//下面是检测有没有新的房间
    STRU_HOUSE_flush rs;
    strcpy(rs.username,m_wnd->userName);
    Mediator->net->Send((char*)&rs,rs.size);


    for(auto ite = m_house.begin();ite!=m_house.end();ite++){
            STRU_DeleteReflush rd;
        strcpy(rd.HouseName,(*ite)->housename.c_str());
        strcpy(rd.username,m_wnd->userName);
        rd.able=true;
        cout<<rd.nType<<"  12323424"<<endl;
        Mediator->net->Send((char*)&rd,rd.size);
    }

}

void ckernel::Del_House_Reflush(long ISendIp, char* buf, int nLen){
    //添加房间

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
        //删除已经退出的房间  可以发送界面上面的所有m_house[i]
}
void ckernel::Del_DeleteReflush(long ISendIp, char* buf, int nLen){
    cout<<"Del_DeleteReflush"<<endl;
    STRU_DeleteReflush rs = *(STRU_DeleteReflush*)buf;
    if(!rs.able){
        cout<<"should delete "<<rs.HouseName<<endl;
       if(m_house.size()>0)
       for(auto ite = m_house.begin();ite!=m_house.end();ite++){
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
    STRU_PLAY_Cheak rs = *(STRU_PLAY_Cheak*)buf;
    if(strcmp(rs.quest,"fail")==0){
        QMessageBox::about(gameList,"提示","对方认输");
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
    STRU_AliveTest rq =*(STRU_AliveTest*)buf;
    cout<<"Get Alive "<<endl;
    if(strcmp(rq.quest, "Test")==0){
        memset(rq.quest,0,sizeof(rq.quest));
        strcpy(rq.quest,"test");
        Mediator->net->Send((char*)&rq,rq.size);
    }
}
void ckernel::Del_HouseNumber(long ISendIp, char* buf, int nLen){
        STRU_HouseNumReflush rq = *(STRU_HouseNumReflush*)buf;
        if(strcmp(rq.hostname, "DEL")!=0&&strcmp(rq.hostname, "NoDel")!=0){
            m_houseL->AddItem(rq.hostname);

        }else{
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
    STRU_AskHostJoin rq = *(STRU_AskHostJoin*)buf;
    cout<<"my name:"<<m_wnd->userName<<"Del_HostAsk  "<<rq.hostname<<endl;
    if(strcmp(m_wnd->userName,rq.hostname)==0){

    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText(QString("用户【%1】请求加入房间").arg(rq.username));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();

    if(ret == QMessageBox::Ok){
        strcpy(rq.username1,"yes");
        cout<<rq.HouseName<<"  "<<rq.nType<<"  "<<rq.username<<endl;
        Mediator->net->Send((char*)&rq,rq.size);
        gameList->hide();

    }else{
        strcpy(rq.username1,"no");
        Mediator->net->Send((char*)&rq,rq.size);
        }

    }else{

        if(strcmp(rq.username1,"no")==0){
             QMessageBox::about(m_Ground,"提示","对端不允许");

        }else{
            QMessageBox::about(m_Ground,"提示","对端允许");
            On_Deal_JOIN(rq.HouseName);
        }
    }
}

//发出数据------------------------------------------------------------------------------------------------------------------------
unsigned int* MD5(const char* mStr); //MD5加密函数，并执行数据填充
void  ckernel::On_Deal_Login(){
     STRU_LOGIN SL;
    SL.nType =_DEF_PROTOCOL_Login_;
    if(strcmp(m_wnd->password,"")==0&&strcmp(m_wnd->userName,"")==0){
         QMessageBox::about(m_Ground,"提示","密码或用户不可以为空");
         return;
    }
    for(int i=0;i<_DEF_PASSWORD_SIZE;i++){
        if(m_wnd->userName[i]==' '){
            QMessageBox::about(m_Ground,"提示","名字不可以有空格");
            return;
        }
    }

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
    Mediator->net->Send((char*)&SL,SL.size);

}
void  ckernel::On_Deal_Register(){
    STRU_REGISTER SR;
   SR.nType =_def_PROTOCOL_register_;
   if(strcmp(m_wnd->password,"")==0&&strcmp(m_wnd->userName,"")==0){
        QMessageBox::about(m_Ground,"提示","密码不可以为空");
        return;
   }
   for(int i=0;i<_DEF_PASSWORD_SIZE;i++){
       if(m_wnd->userName[i]==' '){
           QMessageBox::about(m_Ground,"提示","名字不可以有空格");
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

    unsigned int* tmpGroup = MD5(tmpstr);

    snprintf(buf1, 9, "%8X", tmpGroup[0]);  //A
    snprintf(buf1 + 8, 9, "%8X", tmpGroup[1]);  //B
    snprintf(buf1 + 16, 9, "%8X", tmpGroup[2]);  //C
    snprintf(buf1 + 24, 9, "%8X", tmpGroup[3]);  //D
    cout << buf1 << endl;
    delete[] tmpGroup;
   strcpy(SR.password,buf1);
   strcpy(SR.userName,m_wnd->userName);
   Mediator->net->Send((char*)&SR,SR.size);
}
void  ckernel:: On_Deal_ONLINE(){
    STRU_ONLINE rq;
    strcpy(rq.szName,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);
}
void   ckernel::On_Deal_ADD(){
    m_add->show();
}
void ckernel::Send_Add_Friend(){
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
    cout<<1111<<endl;
    closesocket(this->Mediator->net->serversock);
    this->Mediator->net->m_isStop = true;
    m_wnd->close();
    OfflineAble =0;
    delete kernel;
}
void  ckernel::On_Deal_OFFLINE(){
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

    STRU_CHAR rq;
    strcpy(rq.content,m->GetTalk(friends).c_str());
    strcpy(rq.szName,m_wnd->userName);
    strcpy(rq.friendName,friends.c_str());
     Mediator->net->Send((char*)&rq,rq.size);
}
void ckernel:: On_Deal_INFO(){
    STRU_INFO rq;
    strcpy(rq.userName,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);//再发一次报文
}
void ckernel::On_Deal_JOIN(string housename){
    STRU_JOIN rq;
    m_houseL->HosueName = housename;
    HouseName = housename;
    strcpy(rq.HouseName,housename.c_str());
    strcpy(rq.username,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);
}
void  ckernel::On_Deal_CREATE(){
    cout<<"On_Deal_CREATE"<<endl;

    m_houname->show();
}

void  ckernel::On_Deal_GetHouseName(string name){
    HouseName = name;
    //创建房间
    STRU_CREATE rq;
    strcpy(rq.HouseName,HouseName.c_str());
    strcpy(rq.hostName,m_wnd->userName);
    Mediator->net->Send((char*)&rq,rq.size);
}
void  ckernel::On_Deal_LEAVE(string name){
    cout<<"On_Deal_LEAVE"<<endl;
    m_houseL->clearAll();
    STRU_LEAVE rq;
    strcpy(rq.UserName,m_wnd->userName);
    strcpy(rq.HouseName,name.c_str());
    Mediator->net->Send((char*)&rq,rq.size);
    m_houseL->hide();
}
void  ckernel::On_Deal_PLAY(string Housename){
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
}
void ckernel::On_Deal_Plying(int x,int y){
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
    STRU_PLAY_Cheak rq;
    strcpy(rq.quest,s.c_str());
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.HouseName,gameList->Housename.c_str());
    strcpy(rq.username1,gameList->againestName.c_str());
    cout<<"sned cheak"<<endl;
    Mediator->net->Send((char*)&rq,rq.size);

}
void ckernel::On_Deal_Failed(){
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
}

void ckernel::On_Deal_HouseFlush(string s){
    STRU_HouseNumReflush rq;
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.HouseName,m_houseL->HosueName.c_str());
    Mediator->net->Send((char*)&rq,rq.size);
    strcpy(rq.hostname,"DEL");

    for(auto ite =m_houseL->Plain.begin();ite!=m_houseL->Plain.end();ite++){
        strcpy(rq.username1,(*ite)->GetName().c_str());
        Mediator->net->Send((char*)&rq,rq.size);
        memset(rq.username1,0,sizeof(rq.username1));
    }
}

void ckernel::deleteItem(string s){
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
    cout<<"On_Deal_DelFrriend"<<friendName<<endl;
    STRU_DelFriend rq;
    strcpy(rq.username,m_wnd->userName);
    strcpy(rq.username1,friendName.c_str());
    Mediator->net->Send((char*)&rq,rq.size);
    deleteItem(friendName);
     //QMessageBox::about(gameList,"提示","本次登陆被删除好友还在列表上");
}
