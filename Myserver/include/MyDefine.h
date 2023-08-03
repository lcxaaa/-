#pragma once
#include <iostream>
#include<list>
#include<map>
#include<string.h>
#include<mysql/mysql.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<signal.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<openssl/md5.h>

typedef int SOCKET;


using namespace std;
#define	ip (INADDR_ANY)
#define Port (12345)
#define BufSize  (1024)
#define BackLog (128)

#define _DEF_NAME_SIZE_ (10) //名字长度
#define _DEF_CONTENT_SIZE (900)//聊天内容长度
#define _DEF_PROTOCOL_COUNT (29)//函数偏移
#define _DEF_PASSWORD_SIZE (40)//密码
#define _DEF_HOUSE_ID_SIZE (10) //房间名字长度
#define _DEF_PROTOCOL_BASE_ (900) //协议头开始

//-----------状态--------------
//注册
#define register_success  "success"
#define user_is_exist  "is_exist"
#define name_is_used   "is_used"

//登陆结果
#define login_success "success"
#define password_error "key error"
#define user_not_exist "not_exist"



//添加好友结果
#define add_success "success"
#define no_this_user "nouser"
#define friend_offline "useroff"
#define friend_refuse "refuse"

//聊天结果
#define send_success (0)
#define send_fail (1)

//状态
#define status_offline (0)
#define status_online (1)

//登陆
#define _DEF_PROTOCOL_Login_ (_DEF_PROTOCOL_BASE_ +1)
//上线
#define _DEF_PROTOCOL_ONLINE_ (_DEF_PROTOCOL_BASE_ +2)
//下线
#define _DEF_PROTOCOL_OFFLINE_ (_DEF_PROTOCOL_BASE_+3)
//注册
#define _def_PROTOCOL_register_  (_DEF_PROTOCOL_BASE_+4)
//好友信息
#define _def_PROTOCOL_friend_INFO (_DEF_PROTOCOL_BASE_+5)

//添加好友
#define _def_PROTOCOL_add_friend_  (_DEF_PROTOCOL_BASE_+6) 
//聊天
#define _def_PROTOCOL_chat_  (_DEF_PROTOCOL_BASE_+7)
//加入房间
#define _def_PROTOCOL_Join_  (_DEF_PROTOCOL_BASE_+8)
//创建房间
#define _def_PROTOCOL_Create_  (_DEF_PROTOCOL_BASE_+9)
//离开房间
#define _def_PROTOCOL_Leave_  (_DEF_PROTOCOL_BASE_+10)
//游戏过程
#define _def_PROTOCOL_Play_  (_DEF_PROTOCOL_BASE_+11)
//好友回复包
#define _def_PROTOCOL_add_friend_rs1 (_DEF_PROTOCOL_BASE_+12)
//好友回复包
#define _def_PROTOCOL_add_friend_rs2 (_DEF_PROTOCOL_BASE_+13)
//创建回复报文
#define _def_PROTOCOL_Create_Rs (_DEF_PROTOCOL_BASE_+14)
//房间内部信息
#define _def_PROTOCOL_House_List (_DEF_PROTOCOL_BASE_+15)
#define _def_PROTOCOL_reflush (_DEF_PROTOCOL_BASE_+16)
//好友信息回复
#define _def_PROTOCOL_friend_INFO_RS (_DEF_PROTOCOL_BASE_+17)

#define _def_PROTOCOL_STRU_PLAY_Process (_DEF_PROTOCOL_BASE_+18)

#define _def_PROTOCOL_STRU_PLAY_Cheak (_DEF_PROTOCOL_BASE_+19)

#define _def_PROTOCOL_STRU_MyFailed (_DEF_PROTOCOL_BASE_ + 20)

#define _def_PROTOCOL_AliveTest (_DEF_PROTOCOL_BASE_ + 21)

#define _def_PROTOCOL_DeleteHouseReflush (_DEF_PROTOCOL_BASE_ + 22)

#define _def_PROTOCOL_HouseNumberReflush (_DEF_PROTOCOL_BASE_ + 23)

#define _def_PROTOCOL_AskHostJoin (_DEF_PROTOCOL_BASE_ + 24)

#define _def_PROTOCOL_DelFriend (_DEF_PROTOCOL_BASE_ + 25)

#define _def_PROTOCOL_WaitOk (_DEF_PROTOCOL_BASE_ + 26)

//对局结果
#define _def_PROTOCOL_VSanwser (_DEF_PROTOCOL_BASE_ + 27)
//对局记录
#define _def_PROTOCOL_History (_DEF_PROTOCOL_BASE_ + 28)

//登陆协议包
struct STRU_LOGIN {
	STRU_LOGIN() :nType(_DEF_PROTOCOL_Login_),size(sizeof(STRU_LOGIN)) {
		memset(szName, 0, _DEF_NAME_SIZE_);
		memset(password, 0, _DEF_PASSWORD_SIZE);
	}
	int nType;
	int size;
	char szName[_DEF_NAME_SIZE_];
	char password[_DEF_PASSWORD_SIZE];
};
//上线协议包
struct STRU_ONLINE {
	STRU_ONLINE() :nType(_DEF_PROTOCOL_ONLINE_),size(sizeof(STRU_ONLINE)) {
		memset(szName, 0, _DEF_NAME_SIZE_);
	}
	int nType;
	int size;
	char szName[_DEF_NAME_SIZE_];
};

//添加好友
struct STRU_ADD {
	STRU_ADD() :nType(_def_PROTOCOL_add_friend_) ,size(sizeof(STRU_ADD)){
		memset(userName, 0, _DEF_NAME_SIZE_);
		memset(friendName, 0, _DEF_NAME_SIZE_);
	}
	int nType;
	int size;
	char userName[_DEF_NAME_SIZE_];//用户名字
	char friendName[_DEF_NAME_SIZE_];
};
//最后添加好友的 确认报文
struct STRU_ADD_RS2 {
	STRU_ADD_RS2() :nType(_def_PROTOCOL_add_friend_rs2),size(sizeof(STRU_ADD_RS2)) {
		memset(userName, 0, _DEF_NAME_SIZE_);
		memset(friendName, 0, _DEF_NAME_SIZE_);
	}
	int nType;
	int size;
	int result;
	char userName[_DEF_NAME_SIZE_];//用户名字
	char friendName[_DEF_NAME_SIZE_];
};
struct STRU_ADD_RS {
	STRU_ADD_RS() :nType(_def_PROTOCOL_add_friend_rs1),size(sizeof(STRU_ADD_RS)) {
		memset(userName, 0, _DEF_NAME_SIZE_);
		memset(friendName, 0, _DEF_NAME_SIZE_);
	}
	int nType;
	int size;
	char userName[_DEF_NAME_SIZE_];//用户名字
	char friendName[_DEF_NAME_SIZE_];
};
//下线协议包 
struct STRU_OFFLINE {
	STRU_OFFLINE() :nType(_DEF_PROTOCOL_OFFLINE_),size(sizeof(STRU_OFFLINE)) {
		memset(userName, 0, _DEF_NAME_SIZE_);
	}
	int nType;
	int size;
	char userName[_DEF_NAME_SIZE_];//用户名字
};

//注册协议包 
struct STRU_REGISTER {
	STRU_REGISTER() :nType(_def_PROTOCOL_register_),size(sizeof(STRU_REGISTER)) {
		memset(password, 0, _DEF_PASSWORD_SIZE);
		memset(userName, 0, _DEF_NAME_SIZE_);
	}
	int nType;
	int size;
	char userName[_DEF_NAME_SIZE_];//用户名字
	char password[_DEF_PASSWORD_SIZE];//用户密码
};

//聊天协议包 
struct STRU_CHAR {
	STRU_CHAR() :nType(_def_PROTOCOL_chat_),size(sizeof(STRU_CHAR)){
		memset(szName, 0, _DEF_NAME_SIZE_);
		memset(content, 0, _DEF_CONTENT_SIZE);
		memset(friendName, 0, sizeof(friendName));
	}
	int nType;
	int size;
	char szName[_DEF_NAME_SIZE_];
	char friendName[_DEF_NAME_SIZE_];
	char content[_DEF_CONTENT_SIZE];
	char MD5[40];
};

//好友信息协议包 
struct STRU_INFO {
	STRU_INFO() :nType(_def_PROTOCOL_friend_INFO),size(sizeof(STRU_INFO)) {
		memset(userName, 0, _DEF_NAME_SIZE_);
	}
	int nType;
	int size;
	char userName[_DEF_NAME_SIZE_];//用户名字
	int state;
};

//好友信息协议包
struct STRU_INFO_RS {
	STRU_INFO_RS() :nType(_def_PROTOCOL_friend_INFO_RS),size(sizeof(STRU_INFO_RS)) {
		memset(userName, 0, _DEF_NAME_SIZE_);
	}
	int nType;
	int size;
	char userName[_DEF_NAME_SIZE_];//用户名字
	int state;
};
//加入房间协议包 
struct STRU_JOIN {
	STRU_JOIN() :nType(_def_PROTOCOL_Join_),size(sizeof(STRU_JOIN)) {
		memset(HouseName, 0, _DEF_HOUSE_ID_SIZE);
		memset(username, 0, sizeof(username));
	}
	int nType;
	int size;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char username[_DEF_NAME_SIZE_];
};

//创建房间协议包 
struct STRU_CREATE {
	STRU_CREATE() :nType(_def_PROTOCOL_Create_),size(sizeof(STRU_CREATE)) {
		memset(HouseName, 0, _DEF_HOUSE_ID_SIZE);
		memset(hostName, 0, _DEF_HOUSE_ID_SIZE);
	}
	int nType;
	int size;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char hostName[_DEF_NAME_SIZE_];
};
//创建房间回复
struct STRU_CREATE_RS {
	STRU_CREATE_RS() :nType(_def_PROTOCOL_Create_Rs),size(sizeof(STRU_CREATE_RS)) {
		memset(HouseName, 0, _DEF_HOUSE_ID_SIZE);
		memset(hostName, 0, _DEF_HOUSE_ID_SIZE);
	}
	int nType;
	int size;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char hostName[_DEF_NAME_SIZE_];
};
//离开房间协议包 
struct STRU_LEAVE {
	STRU_LEAVE() :nType(_def_PROTOCOL_Leave_),size(sizeof(STRU_LEAVE)) {
		memset(HouseName, 0, _DEF_HOUSE_ID_SIZE);
		memset(userName, 0, sizeof(userName));
	}
	int nType;
	int size;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char userName[_DEF_NAME_SIZE_];
};
//房间信息
struct STRU_HOUSE_INFO {
	STRU_HOUSE_INFO() :nType(_def_PROTOCOL_House_List),size(sizeof(STRU_HOUSE_INFO)) {
		memset(HouseName, 0, _DEF_HOUSE_ID_SIZE);
		memset(hostname, 0, sizeof(hostname));
		memset(username, 0, sizeof(username));
	}
	int nType;
	int size;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char hostname[_DEF_NAME_SIZE_];//房主名字
	char username[_DEF_NAME_SIZE_];//用户名字
};
//游戏过程协议包 
struct STRU_PLAY {
	STRU_PLAY() :nType(_def_PROTOCOL_Play_),count(0),size(sizeof(STRU_PLAY)){
		memset(HouseName, 0, sizeof(HouseName));
		memset(hostname, 0, sizeof(hostname));
		memset(username, 0, sizeof(username));
		memset(username1, 0, sizeof(username1));
	}
	int nType;
	int size;
	int count;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char hostname[_DEF_NAME_SIZE_];//房主名字
	char username[_DEF_NAME_SIZE_];//用户名字
	char username1[_DEF_NAME_SIZE_];//对方用户名字
};
//房间刷新
struct STRU_HOUSE_flush {
	STRU_HOUSE_flush() :nType(_def_PROTOCOL_reflush) ,size(sizeof(STRU_HOUSE_flush)){
		memset(HouseName, 0, _DEF_HOUSE_ID_SIZE);
		memset(username, 0, sizeof(username));

		memset(hostname, 0, sizeof(hostname));
	}
	int nType;
	int size;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char hostname[_DEF_NAME_SIZE_];//房主名字
	char username[_DEF_NAME_SIZE_];//用户名字
};
//传递自己下的子的坐标
struct STRU_PLAY_Process {
	STRU_PLAY_Process() :nType(_def_PROTOCOL_STRU_PLAY_Process),x(0),y(0),size(sizeof(STRU_PLAY_Process)){
		memset(username, 0, sizeof(username));
		memset(HouseName, 0, sizeof(HouseName));
		memset(username1, 0, sizeof(username1));
	}
	int nType;
	int size;
	int x;
	int y;
	int change;//为1 为白棋
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char username[_DEF_NAME_SIZE_];//用户名字
	char username1[_DEF_NAME_SIZE_];//对方用户名字
};
//结束是否继续的确认报文
struct STRU_PLAY_Cheak {
	STRU_PLAY_Cheak() :nType(_def_PROTOCOL_STRU_PLAY_Cheak),size(sizeof(STRU_PLAY_Cheak)){
		memset(username, 0, sizeof(username));
		memset(HouseName, 0, sizeof(HouseName));
		memset(username1, 0, sizeof(username1));
		memset(quest, 0, sizeof(quest));
	}
	int nType;
	int size;
	int x;
	int y;
	int change;//为1 为白棋
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char username[_DEF_NAME_SIZE_];//用户名字
	char username1[_DEF_NAME_SIZE_];//对方用户名字
	char quest[6];
};
//失败报文
struct STRU_MyFailedk {
	STRU_MyFailedk() :nType(_def_PROTOCOL_STRU_MyFailed),size(sizeof(STRU_MyFailedk)){
		memset(username, 0, sizeof(username));
		memset(HouseName, 0, sizeof(HouseName));
		memset(username1, 0, sizeof(username1));
		memset(quest, 0, sizeof(quest));
	}
	int nType;
	int size;
	int x;
	int y;
	int change;//为1 为白棋
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char username[_DEF_NAME_SIZE_];//用户名字
	char username1[_DEF_NAME_SIZE_];//对方用户名字
	char quest[6];
};
//心跳报文
struct STRU_AliveTest {
	STRU_AliveTest() :nType(_def_PROTOCOL_AliveTest) ,size(sizeof(STRU_AliveTest)){
		memset(quest, 0, sizeof(quest));
		memset(username, 0, sizeof(username));

	}
	int nType;
	int size;
	char username[_DEF_NAME_SIZE_];//用户名字
	char quest[6];//内容
};
//删除并且刷新的报文
struct STRU_DeleteReflush {
	STRU_DeleteReflush() :nType(_def_PROTOCOL_DeleteHouseReflush),size(sizeof(STRU_DeleteReflush)) {
		memset(HouseName, 0, sizeof(HouseName));
		memset(username, 0, sizeof(username));
		memset(hostname, 0, sizeof(hostname));
	}
	int nType;
	int size;
	bool able;//是否存在，存在为true
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char username[_DEF_NAME_SIZE_];//用户名字
	char hostname[_DEF_NAME_SIZE_];//房主名字
};
//房间内刷新报文
struct STRU_HouseNumReflush {
	STRU_HouseNumReflush() :nType(_def_PROTOCOL_HouseNumberReflush),size(sizeof(STRU_HouseNumReflush)) {
		memset(HouseName, 0, sizeof(HouseName));
		memset(username, 0, sizeof(username));
		memset(hostname, 0, sizeof(hostname));
		memset(username1, 0, sizeof(username1));
	}
	int nType;
	int size;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char username[_DEF_NAME_SIZE_];//用户名字
	char hostname[_DEF_NAME_SIZE_];//房主名字
	char username1[_DEF_NAME_SIZE_];//判断的名字
};
//中途加入报文
struct STRU_AskHostJoin {
	STRU_AskHostJoin() :nType(_def_PROTOCOL_AskHostJoin) ,size(sizeof(STRU_AskHostJoin)){
		memset(HouseName, 0, sizeof(HouseName));
		memset(username, 0, sizeof(username));
		memset(hostname, 0, sizeof(hostname));
		memset(username1, 0, sizeof(username1));
	}
	int nType;
	int size;
	char HouseName[_DEF_HOUSE_ID_SIZE];//房间名字
	char username[_DEF_NAME_SIZE_];//用户名字
	char hostname[_DEF_NAME_SIZE_];//房主名字
	char username1[_DEF_NAME_SIZE_];//房主名字
};
//删除好友报文
struct STRU_DelFriend {
	STRU_DelFriend() :nType(_def_PROTOCOL_DelFriend) ,size(sizeof(STRU_DelFriend)){
		memset(username, 0, sizeof(username));
		memset(username1, 0, sizeof(username1));
	}
	int nType;
	int size;
	char username[_DEF_NAME_SIZE_];//用户名字
	char username1[_DEF_NAME_SIZE_];//房主名字
};
//同步报文
struct STRU_WaitOk {
	STRU_WaitOk() :nType(_def_PROTOCOL_WaitOk) ,size(sizeof(STRU_WaitOk)),status(0){
		memset(username, 0, sizeof(username));
		memset(friendname, 0, sizeof(friendname));
	}
	int nType;
	int size;
	int status;
	char username[_DEF_NAME_SIZE_];//用户名字
	char friendname[_DEF_NAME_SIZE_];//房主名字
};
//好友对战结果报文
struct STRU_VsAnswer {
	STRU_VsAnswer() :nType(_def_PROTOCOL_VSanwser) ,size(sizeof(STRU_VsAnswer)){
		memset(username, 0, sizeof(username));
		memset(friendname, 0, sizeof(friendname));
		memset(Winname, 0, sizeof(Winname));
		shouldDo = 'a';//为a表示开始
	}
	int nType;
	int size;
	char shouldDo;
	char Winname[_DEF_NAME_SIZE_];//赢的人的名字
	char username[_DEF_NAME_SIZE_];//用户名字
	char friendname[_DEF_NAME_SIZE_];//对手
};
//战绩的报文
struct STRU_VsHistory {
	STRU_VsHistory() :nType(_def_PROTOCOL_History) ,size(sizeof(STRU_VsHistory)){
		memset(username, 0, sizeof(username));
		win =0,SumVs=0;
	}
	//s=时间数据库使用curadte()得到
	int nType;
	int size;
	char username[_DEF_NAME_SIZE_];//用户名字
	int win;
	int SumVs;
};
