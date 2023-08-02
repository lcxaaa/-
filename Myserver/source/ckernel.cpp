#include"../include/ckernel.h"

using namespace std;
extern int serverfd;//服务器套接字
extern map<SOCKET,int> m_AliveLoseCount;//心跳报文的丢失计数
extern pthread_mutex_t mapmutex;//保护map的锁，map为套接字和用户对

ckernel::ckernel()
{
	//服务器连接初始化
	char MSYUSER[] = "root";
	char DP[] = "work";
	char MSYIP[] = "127.0.0.1";
	char MSYPASSWORD[] = "1147385086";
	if (!sql.ConnectMySql(MSYIP, MSYUSER, MSYPASSWORD, DP)) {
		cout << "连接错误" << endl;
	}
	cout<<"连接成功"<<endl;
	//服务器上线，用户在线状态为0
	char mysql[] = "update t_user set status = 0;\0";
	if (!sql.UpdateMySql(mysql)){
		cout << "sqlBuf:" << mysql << endl;
		cout << "userName 更新数据库错误" << endl;
		return;
	}
	PFUNinit();//类函数和类的函数指针初始化
	//Epoll 头文件使用了网络初始化 这里不用
}

ckernel::~ckernel()
{
	sql.DisConnect();
	//关闭网络
	//关闭套接字
	close(serverfd);
	//关闭套结字
	for(auto&a:m_mapUseridToSocket){
		close(a.second);	
	}
}

void ckernel::PFUNinit(){
	//绑定 函数指针和目标函数
	m_netProtocolMap[_DEF_PROTOCOL_Login_ - _DEF_PROTOCOL_BASE_]=&ckernel::Del_Login;
	m_netProtocolMap[_DEF_PROTOCOL_ONLINE_ - _DEF_PROTOCOL_BASE_] = &ckernel::Del_Online;
	m_netProtocolMap[_DEF_PROTOCOL_OFFLINE_ - _DEF_PROTOCOL_BASE_] = &ckernel::Del_Offline;
	m_netProtocolMap[_def_PROTOCOL_register_ - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Register;
	m_netProtocolMap[_def_PROTOCOL_friend_INFO - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Friend_Info;
	m_netProtocolMap[_def_PROTOCOL_add_friend_ - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Add_Friend;
	m_netProtocolMap[_def_PROTOCOL_chat_ - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Chat;
	m_netProtocolMap[_def_PROTOCOL_Join_ - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Join;
	m_netProtocolMap[_def_PROTOCOL_Create_ - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Create;
	m_netProtocolMap[_def_PROTOCOL_Leave_ - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Leave;
	m_netProtocolMap[_def_PROTOCOL_Play_ - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Play;
	m_netProtocolMap[_def_PROTOCOL_add_friend_rs2 - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Add_RS;
	m_netProtocolMap[_def_PROTOCOL_House_List - _DEF_PROTOCOL_BASE_] =&ckernel::Del_House_Info;
	m_netProtocolMap[_def_PROTOCOL_reflush - _DEF_PROTOCOL_BASE_] =&ckernel::Del_House_Reflush;
	m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Process - _DEF_PROTOCOL_BASE_] =&ckernel::Del_Playing;
	m_netProtocolMap[_def_PROTOCOL_STRU_PLAY_Cheak - _DEF_PROTOCOL_BASE_] =&ckernel::Del_PlayCheak;
	m_netProtocolMap[_def_PROTOCOL_STRU_MyFailed - _DEF_PROTOCOL_BASE_] =&ckernel::Deal_Failed;
	m_netProtocolMap[_def_PROTOCOL_AliveTest - _DEF_PROTOCOL_BASE_] =&ckernel::Deal_Alive_Test;
	m_netProtocolMap[_def_PROTOCOL_DeleteHouseReflush - _DEF_PROTOCOL_BASE_] =&ckernel::Deal_Delete_Reflush;
	m_netProtocolMap[_def_PROTOCOL_HouseNumberReflush - _DEF_PROTOCOL_BASE_] =&ckernel::Deal_HouseNumber;
	m_netProtocolMap[_def_PROTOCOL_AskHostJoin - _DEF_PROTOCOL_BASE_] =&ckernel::Deal_HostAsk;
	m_netProtocolMap[_def_PROTOCOL_DelFriend - _DEF_PROTOCOL_BASE_] =&ckernel::DeleteFriend;
	m_netProtocolMap[_def_PROTOCOL_WaitOk - _DEF_PROTOCOL_BASE_] =&ckernel::Del_WaitOk;
	m_netProtocolMap[_def_PROTOCOL_VSanwser - _DEF_PROTOCOL_BASE_] =&ckernel::Del_VsAnswerGame; 
	m_netProtocolMap[_def_PROTOCOL_History - _DEF_PROTOCOL_BASE_] = &ckernel::Del_vsHistory;
}


void ckernel::Deal(SOCKET ISendIp,int t,char*buf,int len)
{
	//根据协议，划分功能
	//函数里面与数据库交互
	cout <<"type: "<< t << endl;
	if(t>_DEF_PROTOCOL_BASE_&&t<_DEF_PROTOCOL_BASE_+_DEF_PROTOCOL_COUNT&&m_netProtocolMap[t-_DEF_PROTOCOL_BASE_]!=nullptr){
		(this->*m_netProtocolMap[t-_DEF_PROTOCOL_BASE_])(ISendIp,buf, len);
	}
}

void  ckernel::Del_Login(SOCKET ISendIp, char* buf, int len) {
	STRU_LOGIN* rq = (STRU_LOGIN*) buf;
	STRU_LOGIN rs;
	//登录处理
	char sqlBuf[1024] = "";
	list<string> listRes;
	sprintf(sqlBuf, "select status from t_user where userid ='%s';", rq->szName);
	//查询数据库
	int err = sql.SelectMySql(sqlBuf, 1, listRes);
	if (!err) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}

	if(listRes.size()==0){
		//没有结果
		strcpy(rs.szName,user_not_exist);//如果没有就代表user不存在
		cout << "user_not_exist" << endl;
		send(ISendIp, (char*)&rs, rs.size, 0);
		return;
	}

	if (listRes.front()[0]-'0'==1) {
		//查看状态
		cout << "已经上线" << endl;
		strcpy(rs.szName, "havethis");
		send(ISendIp, (char*)&rs, rs.size, 0);
		return;
	}

	memset(sqlBuf, 0, sizeof(sqlBuf));
	listRes.pop_back();
	sprintf(sqlBuf, "select userid,password from t_user where userid ='%s';", rq->szName);
	//如果只用rq->tel查询password,userid   就可以不用那么多  pop_front();去清除无用信息了
	err= sql.SelectMySql(sqlBuf, 2, listRes);
	if (!err) {

		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}

	if (0 == listRes.size()) {
		strcpy(rs.szName,user_not_exist);//如果没有就代表user不存在
		cout << "user_not_exist" << endl;
		send(ISendIp, (char*)&rs, rs.size, 0);
		return;
	}
	else {
		string userid = listRes.front();	//得到数据库中的密码
		listRes.pop_front();

		string password = listRes.front();//得到数据库中的id
		listRes.pop_front();

		if (0 != strcmp(password.c_str(), rq->password)) {//数据库和rq发来的password进行比较  strcmp为0则代表两个字符串相同
			strcpy(rs.szName, password_error);
			cout << "password_error";
		}
		else {
			m_mapUseridToSocket[rq->szName] = ISendIp;
			strcpy(rs.szName, login_success);
		}
	}
	cout << "登陆密码：" << rs.password << "    登陆用户名字：  " << rs.szName <<"Len :"<<len<< endl;
	send(ISendIp, (char*)&rs, rs.size, 0);//发送会结果
	cout << "登陆密码：" << rs.password << "    登陆用户名字：  " << rs.szName << endl;
	return;
}

void ckernel::Del_Friend_Info(SOCKET ISendIp, char* buf, int len) {
	//查询好友信息
	STRU_INFO_RS* rq = (STRU_INFO_RS*)buf;
	string name = rq->userName;
	char sqlBuf[1024] = "";
	list<string> listRes;
	//查询好友关系
	sprintf(sqlBuf, "select t_friend.idB,t_user.status from t_friend inner join t_user on t_friend.idB =t_user.userid where t_user.userid !='%s';", rq->userName);
	int err = sql.SelectMySql(sqlBuf, 2, listRes);

	if (listRes.size() == 0&&err ==1) {
		cout << "Del_Friend_Info size ==0" << endl;
		STRU_INFO_RS rs;
		//无好友
		strcpy(rs.userName,"No");
		send(ISendIp, (char*)&rs, rs.size, 0);
		return;
	}

	if (!err) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		//查表错误
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "查询数据库错误" << endl;
		STRU_INFO_RS rs;
		strcpy(rs.userName, "error");
		send(ISendIp, (char*)&rs, rs.size, 0);
		return;
	}
	cout << "sqlBuf:" << sqlBuf << endl;
	while (listRes.size() > 0) {
		//发送好友信息
		STRU_INFO_RS rs;
		rs.nType = _def_PROTOCOL_friend_INFO_RS;

		strcpy(rs.userName, listRes.front().c_str());
		listRes.pop_front();

		rs.state = atoi(listRes.front().c_str());
		listRes.pop_front();

		cout << rs.userName << rs.state << endl;
		send(m_mapUseridToSocket[name], (char*)&rs, rs.size,0);//向发送申请的sock发送
	}
}

void ckernel::Del_Add_RS(SOCKET ISendIp, char* buf, int len)
{
	STRU_ADD_RS2* rq = (STRU_ADD_RS2*)buf;
	cout << "------------" << endl;
	if (1 == rq->result) {
		//被添加端同意，此时修改数据库
		char sqlBuf[1024] = "";
		sprintf(sqlBuf, "insert into t_friend value('%s','%s');", rq->userName, rq->friendName);

		if (!sql.UpdateMySql(sqlBuf)) {
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "userName 更新数据库错误" << endl;

			return;
		}
		//双端关系都要添加
		memset(sqlBuf,0,sizeof(sqlBuf));
		sprintf(sqlBuf, "insert into t_friend value('%s','%s');", rq->friendName, rq->userName);
		if (!sql.UpdateMySql(sqlBuf)) {
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "userName 更新数据库错误" << endl;

			return;
		}

		// rq->userName
		cout << "my id" << rq->userName <<"friend id is"<<rq->friendName<< endl;
		//添加好友信息
		send(m_mapUseridToSocket[rq->userName], rq, rq->size,0);
	}
	else {
		//发送不成功的通知
		send(m_mapUseridToSocket[rq->userName], rq, rq->size, 0);
	}

}

void ckernel::Del_Register(SOCKET ISendIp, char* buf, int len)
{
	STRU_REGISTER* rq = (STRU_REGISTER*)buf;
	STRU_REGISTER rs ;
	//判断是否为空
	if (!rq->password||!rq->userName)
	{
		cout << "电话号，密码不可以为空";
		strcpy(rs.userName, "error");

		send(ISendIp, (char*)&rs,rs.size, 0);
		return;

	}
	//判断上线
	if ( strlen(rq->password) > _DEF_PASSWORD_SIZE) {
		cout << "电话号，密码不可以超过15位，电话号输入错误" << endl;
		strcpy(rs.userName, "error");

		send(ISendIp, (char*)&rs, rs.size, 0);
		return;

	}
	cout << "注册密码：" << rq->password << "  注册用户名： " << rq->userName << endl;
	list<string> listRes;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select userid from t_user where userid = '%s';", rq->userName);
	//查询是否已经有此用户
	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "userName 更新数据库错误" << endl;
		return;
	}
	if (listRes.size() > 0) {
		//存在此用户
		strcpy(rs.userName,name_is_used);

	}else {
		//没有就向数据库里面插入
		sprintf(sqlBuf, "insert into t_user (userid,password) values('%s', '%s');",rq->userName,rq->password);

		if (!sql.UpdateMySql(sqlBuf)) {
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << " UpdateMySql 数据库更新错误";

			return;
		}
		bzero(sqlBuf,sizeof(sqlBuf));
		//创建对应的战绩表
		sprintf(sqlBuf,"insert into t_score (userid) values('%s');",rq->userName);
		if (!sql.UpdateMySql(sqlBuf)) { 
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << " UpdateMySql 数据库更新错误";

			return;
		}

		strcpy(rs.userName, register_success);
	}
	//发送数据
	send(ISendIp, (char*)&rs, rs.size,0);
	bzero(sqlBuf,sizeof(sqlBuf));
	
}

void ckernel::Del_Online(SOCKET ISendIp, char* buf, int len)
{
	//上线处理
	STRU_ONLINE* rq = (STRU_ONLINE*)buf;
	char sqlBuf[1024] = "";
	list<string> listRes;
	//修改数据库
	sprintf(sqlBuf, "update t_user set status =1 where userid ='%s';", rq->szName);
	if (!sql.UpdateMySql(sqlBuf)) {
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "userName 更新数据库错误" << endl;
		return;
	}
	cout << "Online success" << endl;
}

void ckernel::Del_Offline(SOCKET ISendIp, char* buf, int len)
{
	//下线处理
	STRU_OFFLINE* rq = (STRU_OFFLINE*)buf;
	char sqlBuf[1024] = "";
	list<string> listRes;
	//修改数据库在线列 status
	sprintf(sqlBuf, "update t_user set status =0 where userid ='%s';", rq->userName);
	if (!sql.UpdateMySql(sqlBuf)) {
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "userName 更新数据库错误" << endl;
		return;
	}
	cout << "OFFline success" << endl;
	//关闭套接字
	close(m_mapUseridToSocket[rq->userName]);
	//防止alive 的map 得到即将删除前的 map表，于是加锁
	pthread_mutex_lock(&mapmutex);
	m_AliveLoseCount[ISendIp] = -10;//即使alive会++ 但是最多5次 并且看到负数就直接删除
	m_mapUseridToSocket.erase(rq->userName);
	pthread_mutex_unlock(&mapmutex);
}

void ckernel::Del_Friend(SOCKET ISendIp, char* buf, int len)
{
	cout << "get Del_Friend" << endl;
}

void ckernel::Del_Chat(SOCKET ISendIp, char* buf, int len)
{
	cout << "get Del_Chat" << endl;
	//发送文本，后期可以加密
	STRU_CHAR rq = *(STRU_CHAR*)buf;
	if(m_mapUseridToSocket.find(rq.friendName)!= m_mapUseridToSocket.end())
		send(m_mapUseridToSocket[rq.friendName],(char*)&rq, rq.size,0);
	else{
		//好友不在线
		cout<<"======friend"<<endl;
		memset(rq.content,0,sizeof(rq.content));
		strcpy(rq.content,"offline");
		send(ISendIp,(char*)&rq,rq.size,0);		
	}
}

void ckernel::Del_Add_Friend(SOCKET ISendIp, char* buf, int len)
{
	//好友添加 第一次 即发送方刚刚发送请求
	cout << "addfriend_RQ" << endl;
	STRU_ADD* rq = (STRU_ADD*)buf;

	cout << "要添加的好友名称：" << rq->friendName << "  我的名字：  " << rq->userName << endl;

	list<string> listRes;

	STRU_ADD rs;
	
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select userid,status from  t_user where userid ='%s';", rq->friendName);
	//如果只用rq->tel查询password,userid   就可以不用那么多  pop_front();去清除无用信息了

	if (!sql.SelectMySql(sqlBuf, 2, listRes)) {

		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  

		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}
	//没有这个对象
	if (listRes.size() <= 0) {
		strcpy(rs.friendName, no_this_user);
		cout << "no_this_user" << endl;
		send(ISendIp, (char*)&rs, rs.size,0);
	}
	else {
		//查看是否下线
		strcpy(rs.friendName, listRes.front().c_str());

		listRes.pop_front();
		if (strcmp(listRes.front().c_str(), "0") == 0) {
			strcpy(rs.friendName, friend_offline);
			send(ISendIp, (char*)&rs, rs.size,0);
			return;
		}

		strcpy(rs.userName, rq->userName);
		
		cout << rs.friendName << "  " << rs.userName << endl;
		//仅仅转发消息给请求方
		cout << "已发送好友请求" << endl;
		SOCKET sock = m_mapUseridToSocket[rs.friendName];
		strcpy(rs.friendName, add_success);
		STRU_ADD_RS rs2;
		strcpy( rs2.userName, rq->userName);
		strcpy(rs2.friendName, rq->friendName);

		send(ISendIp, (char*)&rs, rs.size,0);

		send(sock, (char*)&rs2, rs2.size,0);//把登陆请求返还给qt  让qt处理rq  这里只是核对有没有这个人  因为还有个是否同意
		//这里只是发信息成功
		return;


	}

}

void ckernel::Del_Join(SOCKET ISendIp, char* buf, int len)
{
	//加入房间判断
	STRU_JOIN rq = *(STRU_JOIN*)buf;
	list<string> listRes;
	char sqlBuf[1024] = "";
	//查询房间人数
	sprintf(sqlBuf, "select count(distinct username) from t_house where housename ='%s';", rq.HouseName);
	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		strcpy(rq.HouseName, "-1");
		send(ISendIp, (char*)&rq, rq.size,0);
		return;
	}
	string nums = listRes.front();
	if (strcmp(nums.c_str(), "2") == 0) {
		//达到2人，此时返回不可以加入
		cout << nums << "answer\n";
		strcpy(rq.HouseName, "-3");
		send(ISendIp, (char*)&rq, rq.size,0);
		return;
	}
	
	while (listRes.size() > 0) listRes.pop_front();
	memset(sqlBuf, 0, sizeof(sqlBuf));
	//得到房主名字
	sprintf(sqlBuf, "select hostname from t_house where housename ='%s';", rq.HouseName);
	string hostname;
	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		strcpy(rq.HouseName, "-1");
		send(ISendIp, (char*)&rq, rq.size,0);
		return;
	}
	//没有得到结果
	if (listRes.size() <= 0) {
		//房主退出
		strcpy(rq.username, "-2");
		send(ISendIp, (char*)&rq, rq.size,0);
		return;
	}
	hostname = listRes.front();

	if (strcmp(nums.c_str(), "1")==0) {
		//查看得到的房间人数是否为1
		cout << "num is:" << nums << endl;
		//看看房主在不在AI模式，如果在AI模式，提示房主有人加入可不可以
		while (listRes.size() > 0) listRes.pop_front();

		memset(sqlBuf, 0, sizeof(sqlBuf));
		//查看是否为AI模式
		sprintf(sqlBuf, "select AImode from t_house where housename ='%s';", rq.HouseName);
		if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
			//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "更新数据库错误" << endl;
			strcpy(rq.HouseName, "-1");
			send(ISendIp, (char*)&rq, rq.size, 0);
			return;
		}
		string nums = listRes.front();
		//为1 表示在人机模式
		if (strcmp(nums.c_str(), "1") == 0) {
			cout << "AImode is open  " << hostname<<endl;
			//如果在人机模式,询问房主是否允许加入
			STRU_AskHostJoin rs;
			strcpy(rs.HouseName, rq.HouseName);
			strcpy(rs.username, rq.username);
			strcpy(rs.hostname, hostname.c_str());
			send(m_mapUseridToSocket[hostname], (char*)&rs,rs.size, 0);
			return;
		}
	}

	while (listRes.size() > 0) listRes.pop_front();
	memset(sqlBuf, 0, 1024);
	//查看房间内玩家
	sprintf(sqlBuf, "select username from t_house where housename ='%s';", rq.HouseName);
	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}
	while(listRes.size() > 0) {
		//查看是否已经加入
		if (listRes.front() == rq.username) {
			strcpy(rq.HouseName, "ison");
			send(ISendIp, (char*)&rq, rq.size,0);
			return;
		}
		listRes.pop_front();
	}
	memset(sqlBuf, 0, 1024);
	//数据库 显示玩家加入房间，即添加玩家在房间的记录
	sprintf(sqlBuf, "insert into t_house(housename,hostname,username) values('%s','%s','%s');",rq.HouseName,hostname.c_str(),rq.username);

	if (!sql.UpdateMySql(sqlBuf)) {
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << " UpdateMySql 数据库更新错误";
		strcpy(rq.HouseName, "-1");
		send(ISendIp,(char*)&rq, rq.size,0);
		return;
	};
	//发送给请求端，加入的结果
	send(ISendIp, (char*)&rq, rq.size,0);
	memset(sqlBuf, 0, 1024);

	sprintf(sqlBuf, "select username from t_house where housename = '%s';", rq.HouseName);
	//得到房间内部其他玩家

	STRU_HOUSE_INFO rs;
	strcpy(rs.username, rq.username);
	strcpy(rs.HouseName, rq.HouseName);

	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}

	while (listRes.size() > 0) {
		//给房间内部其他玩家加入结果
		if(listRes.front()!= rq.username)
			send(m_mapUseridToSocket[listRes.front()], (char*)&rs, rs.size,0);

		listRes.pop_front();
	}
}

void ckernel::Del_Create(SOCKET ISendIp, char* buf, int len)
{
	//创建房间
	STRU_CREATE rq = *(STRU_CREATE*)buf;

	STRU_CREATE_RS rs;
	if (rq.HouseName[0] == '\0') {
		//房间名字为null
		strcpy(rs.HouseName, "isnull");

		send(ISendIp, (char*)&rs, rs.size,0);
		return;
	}

	list<string> listRes;
	char sqlBuf[1024] = "";
	//在数据库里面插入房间记录
	sprintf(sqlBuf, "insert into t_house(housename,hostname,username)  values('%s', '%s','%s');", rq.HouseName, rq.hostName,rq.hostName);
	if (!sql.UpdateMySql(sqlBuf)) {
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << " UpdateMySql 数据库更新错误";
		strcpy(rs.hostName, "-1");
		strcpy(rs.HouseName,"-1");
		send(ISendIp,(char*)&rs, rs.size,0);
		return;
	};
	//给在线玩家发送 有新的创建房间的消息
	for (auto ite = m_mapUseridToSocket.begin(); ite != m_mapUseridToSocket.end(); ite++) {
		if(ite->second!= ISendIp)
			send(ite->second, (char*)&rq, rq.size,0);
	}
	//发给房主结果
	strcpy(rs.hostName, rq.hostName);
	strcpy(rs.HouseName, rq.HouseName);
	send(ISendIp, (char*)&rs, rs.size,0);
	cout << "Del_Create success" << endl;
}

void ckernel::Del_Leave(SOCKET ISendIp, char* buf, int len)
{
	//离开房间功能
	cout << "get Del_Leave" << endl;
	STRU_LEAVE rs = *(STRU_LEAVE*)buf;
	list<string> listRes;

	string name = rs.userName;
	char sqlBuf[1024] = "";
	//找到房间内的其他玩家
	sprintf(sqlBuf, "select username,hostname from t_house where username!='%s';",name.c_str());
	cout << sqlBuf << endl;
	if (!sql.SelectMySql(sqlBuf, 2, listRes)) {

		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  

		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}
	bool tru = false;
	while (listRes.size() > 0) {
		//给其他人发送 该端已经退出

		//得到用户名字
		string user = listRes.front();
		STRU_LEAVE rq;
		::strcpy(rq.userName, name.c_str());
		listRes.pop_front();

		if (listRes.front() == name) {
			//房主离开
			string hostname = listRes.front();
			::strcpy(rq.HouseName, "hostleave");
			listRes.pop_front();
			if (listRes.size() == 0) {
				//给房间内剩下的用户发送房主离开的信息
				send(m_mapUseridToSocket[user], (char*)&rq, rq.size,0);
			}
			while (listRes.size() > 0) {
				//发送房主离开的信息
				send(m_mapUseridToSocket[listRes.front()], (char*)&rq, rq.size,0);
				listRes.pop_front();
				listRes.pop_front();
			}
			tru = true;//说明离开的是房主
			break;

		}

		listRes.pop_front();
		//告诉房间的人，发出请求的用户离开
		send(m_mapUseridToSocket[user], (char*)&rq, rq.size,0);
		cout <<"---------------------"<< endl<< rq.userName << endl;
	}
	memset(sqlBuf, 0, 1024);
	//删除该玩家在房间的记录
	sprintf(sqlBuf, "delete from t_house where username='%s';", rs.userName);
	cout << sqlBuf << endl;
	if (!sql.UpdateMySql(sqlBuf)) {
		return;
	}
	memset(sqlBuf, 0, 1024);
	if (tru) {
		//房主离开，删除房间
		sprintf(sqlBuf, "delete from t_house where hostname='%s';", name.c_str());
		if (!sql.UpdateMySql(sqlBuf)) {
			return;
		}
	}
}

void ckernel::Del_WaitOk(SOCKET ISendIp, char* buf, int len){
	//同步五子棋，当黑方确定了在白方在下
	STRU_WaitOk*  rs = (STRU_WaitOk*)buf;
	cout<<"friendname is "<<rs->friendname<<endl;
	send(m_mapUseridToSocket[rs->friendname], (char*)rs,rs->size, 0);

}

void ckernel::Del_Play(SOCKET ISendIp, char* buf, int len)
{

	STRU_PLAY*  rs = (STRU_PLAY*)buf;

	list<string> listRes;
	char sqlBuf[1024] = "";
	cout << "   "<<rs->hostname << "  " << rs->HouseName << " " << rs->username << endl;
	//如果是 好友vs 模式 而非创建房间，就向对端发送消息
	if (strcmp(rs->HouseName, "VSmode") == 0) {
		send(m_mapUseridToSocket[rs->hostname], (char*)rs,rs->size, 0);
		//rs->hostname为好友的名字
		return;
	}
	//找到房间里面 房主的名字
	sprintf(sqlBuf, "select hostname from  t_house where housename ='%s';", rs->HouseName);

	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {

		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  

		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}

	string user = listRes.front();//

	while (listRes.size() > 0) {
		listRes.pop_front();
	}
	memset(sqlBuf, 0,1024);
	strcpy(rs->hostname, user.c_str());
	//找到房间里面另外一个人
	sprintf(sqlBuf, "select username from  t_house where username !='%s';", rs->username);

	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {

		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  

		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}

	if (listRes.size() != 0) {
		// username1为对手名字
		string user = listRes.front();
		strcpy(rs->username1, user.c_str());
	}
	else {
		memset(rs->username1, 0, sizeof(rs->username1));
	}

	while (listRes.size() > 0) {
		listRes.pop_front();
	}
	memset(sqlBuf, 0, 1024);
	//显示房间人数
	sprintf(sqlBuf, "select count(distinct username) from t_house where housename ='%s';", rs->HouseName);
	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}
	rs->count = listRes.front()[0]-'0';//得到房间人数的int
	cout << "play nums:" << rs->count << endl;

	rs->nType = _def_PROTOCOL_Play_;
	send(ISendIp, (char*)rs, rs->size,0);//发送给房主报文

	cout << rs->hostname << "  " << rs->username << "  " << rs->username1 << endl;

	if (listRes.front()[0] - '0' == 1) {
		//如果房间人数为1 开启则代表加入aimode
		memset(sqlBuf, 0, 1024);
		sprintf(sqlBuf, "update t_house set AImode = 1 where housename ='%s';", rs->HouseName);
		if (!sql.UpdateMySql(sqlBuf)) {
			//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "更新数据库错误" << endl;
			return;
		}
	}

	if (strcmp(rs->username, rs->hostname)==0 && rs->count == 2) {
		//房主打开游戏，且有两个人，则可以开始正常对局
		STRU_PLAY rq;
		string m = rs->username1;
		strcpy(rq.username1, rs->username);
		strcpy(rq.username, rs->username1);

		rq.count = 2;
		strcpy(rq.HouseName, rs->HouseName);
		cout << "host is open game" << m <<"type:"<<rs->nType<< endl;;
		send(m_mapUseridToSocket[m], (char*)&rq, rq.size,0);
	}
}
void ckernel::Del_Playing(SOCKET ISendIp, char* buf, int len) {
	STRU_PLAY_Process rs = *(STRU_PLAY_Process*)buf;
	//游戏过程中对端棋子的信息
	cout << rs.x << "  " << rs.y << "  " <<rs.change<< endl;
	send(m_mapUseridToSocket[rs.username1], (char*)&rs, rs.size,0);

}
void ckernel::Del_House_Info(SOCKET ISendIp, char* buf, int len) {
	//房间内的信息
	STRU_HOUSE_INFO rs=*(STRU_HOUSE_INFO*)buf;
	string userid = rs.username;
	list<string> listRes;
	char sqlBuf[1024] = "";
	//找到房间的房主名字
	sprintf(sqlBuf, "select username from t_house where housename ='%s';",rs.HouseName);
	//上面遍历显示的是hostname
	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}

	while(listRes.size() > 0) {
		//发个请求端房间内的人
		rs.nType = _def_PROTOCOL_House_List;
		if (listRes.front() == userid) {
			listRes.pop_front();
			listRes.pop_front();
			if (listRes.size() <= 0) break;
		}
		strcpy(rs.username, listRes.front().c_str());
		listRes.pop_front();

		strcpy(rs.hostname, listRes.front().c_str());
		listRes.pop_front();
		//发送给请求方
		send(ISendIp, (char*)&rs,rs.size,0);

		memset(&rs, 0, sizeof(STRU_HOUSE_INFO));
	}
}
void ckernel::Del_PlayCheak(SOCKET ISendIp, char* buf, int len) {
	//判断双方有没有确认 进行下一步
	cout << "Del_PlayCheak" << endl;
	STRU_PLAY_Cheak rs = *(STRU_PLAY_Cheak*)buf;
	cout <<"Myname:"<< rs.username << "  " << rs.HouseName << "  " << rs.username1 << endl;
	send(m_mapUseridToSocket[rs.username1], (char*)&rs, rs.size,0); //发送确认包给对方
}
void ckernel::Del_House_Reflush(SOCKET ISendIp, char* buf, int len)
{
	//处理房间刷新的函数
	cout << "Del_House_Reflush " << endl;
	STRU_HOUSE_flush rq = *(STRU_HOUSE_flush*)buf;
	list<string> listRes;
	char sqlBuf[1024] = "";
	//显示房间和其房主
	sprintf(sqlBuf, "select housename ,hostname from t_house;");
	if (!sql.SelectMySql(sqlBuf, 2, listRes)) {

		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  

		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}
	string name = rq.username;
	cout << sqlBuf << endl;
	while (listRes.size() > 0) {
		//发送给 刷新的用户
		STRU_HOUSE_flush rs;
		strcpy(rs.HouseName, listRes.front().c_str());
		listRes.pop_front();
		strcpy(rs.hostname, listRes.front().c_str());
		listRes.pop_front();
		strcpy(rs.username, name.c_str());
		cout << rs.HouseName << endl;
		send(ISendIp, (char*)&rs,rs.size,0);
	}

}
void ckernel::Deal_Failed(SOCKET ISendIp, char* buf, int len) {
	//用户点击认输，此时发给对端
	STRU_PLAY_Cheak rs = *(STRU_PLAY_Cheak*)buf;
	cout << "Deal_Failed" << endl;
	strcpy(rs.quest, "fail");
	send(m_mapUseridToSocket[rs.username1], (char*)&rs, rs.size,0);
}

void ckernel::Deal_Alive_Test(SOCKET ISendIp, char* buf, int len) {
	//收到心跳报文的回复
	STRU_AliveTest rq = *(STRU_AliveTest*)buf;
	cout << "get Alive Quest" << endl;
	//把没有收到报文的，和报文错误的，全部close掉
	if (strcmp(rq.quest, "test") == 0) {
		m_AliveLoseCount[ISendIp]++;
	}
}

void ckernel::Deal_Delete_Reflush(SOCKET ISendIp, char* buf, int len)
{
	//房间是否应该被删除
	STRU_DeleteReflush rq = *(STRU_DeleteReflush*)buf;
	cout << "Deal_Delete_Reflush" << endl;

	list<string> listRes;
	char sqlBuf[1024] = "";
	//显示房间房主
	sprintf(sqlBuf, "select hostname from t_house where housename ='%s';",rq.HouseName);
	if (!sql.SelectMySql(sqlBuf, 1, listRes)) {

		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  

		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}
	if (listRes.size() == 0) {
		//没有结果，表示一个被删除
		rq.able = false;
		send(ISendIp, (char*)&rq,rq.size, 0);
	}
	else {
		//房间存在，不应该被删除
		rq.able = true;
		strcpy(rq.hostname,listRes.front().c_str());
		send(ISendIp, (char*)&rq,rq.size, 0);
	}
}

void ckernel::Deal_HouseNumber(SOCKET ISendIp, char* buf, int len) {
	//房间内部 成员离开后的刷新
	cout << "Deal_HouseNumber" << endl;
	STRU_HouseNumReflush rq = *(STRU_HouseNumReflush*)buf;

	if (strcmp(rq.hostname, "DEL") != 0) {
		//如果为del  表示需要鉴定这些username要不要被删除
		//不为是判断要不要添加
		list<string> listRes;
		char sqlBuf[1024] = "";
		// 得到房间的其他用户
		sprintf(sqlBuf, "select username from t_house where housename ='%s' and username !='%s';", rq.HouseName, rq.username);
		if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
			//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "更新数据库错误" << endl;
			return;
		}
		while (listRes.size() > 0) {

			strcpy(rq.hostname, listRes.front().c_str()); //hostname 为空 username 为自身的名字，还需保留
			listRes.pop_front();
			send(ISendIp, (char*)&rq, rq.size, 0);//返回其他用户的结果
			memset(rq.hostname, 0, sizeof(rq.hostname));
		}
	}
	else {

		list<string> listRes;
		char sqlBuf[1024] = "";
		sprintf(sqlBuf, "select username from t_house where username ='%s' and housename ='%s' ;", rq.username1,rq.HouseName);
		if (!sql.SelectMySql(sqlBuf, 1, listRes)) {
			//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "更新数据库错误" << endl;
			return;
		}
		if (listRes.size() == 0) {
			//没有此用户，需要删除
			send(ISendIp, (char*)&rq,rq.size, 0);
		}
		else {
			memset(rq.hostname, 0, sizeof(rq.hostname));
			strcpy(rq.hostname, "NoDel");//还在房间，不应该被删除
			send(ISendIp, (char*)&rq,rq.size, 0);
		}
	}

}

void ckernel::Deal_HostAsk(SOCKET ISendIp, char* buf, int len) {
	//中途加入请求
	cout << "=========Deal_HostAsk=========" << endl;
	STRU_AskHostJoin rq = *(STRU_AskHostJoin*)buf;
	if (strcmp(rq.username1, "yes") == 0) {
		//房主同意，此时房间取消aimode
		list<string> listRes;
		char sqlBuf[1024] = "";
		sprintf(sqlBuf, "update t_house set AImode = 0 where housename ='%s';", rq.HouseName);

		if (!sql.UpdateMySql(sqlBuf)) {
			//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "更新数据库错误" << endl;

			return;
		}

	}
	cout << "rq.username" <<rq.username<<"rq.username1"<<rq.username1<< endl;
	rq.nType = _def_PROTOCOL_AskHostJoin;
	send(m_mapUseridToSocket[rq.username], (char*)&rq,rq.size, 0);//返还给房主结果
}

void ckernel::DeleteFriend(SOCKET ISendIp, char* buf, int len) {
	//删除好友功能
	cout << "DeleteFriend" << endl;
	STRU_DelFriend rq =*(STRU_DelFriend*)buf;
	list<string> listRes;
	char sqlBuf[1024] = "";

	sprintf(sqlBuf, "delete from t_friend where idA ='%s' and idB='%s';", rq.username,rq.username1);
	//修改数据库，删除 idA和idB的关系
	if (!sql.UpdateMySql(sqlBuf)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;

		return;
	}

	memset(sqlBuf, 0, sizeof(sqlBuf));
	sprintf(sqlBuf, "delete from t_friend where idA ='%s' and idB='%s';", rq.username1, rq.username);
	//因为关系是双向的，需要删除两次
	if (!sql.UpdateMySql(sqlBuf)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}

}

void ckernel::Del_VsAnswerGame(SOCKET ISendIp, char* buf, int len){
	//好友约战
	STRU_VsAnswer rq = *(STRU_VsAnswer*)buf;
	cout<<"======================================================="<<endl;
	cout<<"type is "<<rq.shouldDo<<endl;
	cout<<"winname is "<<rq.Winname<<endl;
	cout<<"username is "<<rq.username<<endl;
	cout<<"friendname is "<<rq.friendname<<endl;
	//friendname 会有为null的干扰
	if(rq.shouldDo=='a'){
		//好友同意，并且为a是开始此时加入对局记录
		char sqlBuf[1024] = "";
		sprintf(sqlBuf,"insert into t_game value('%s','%s','%s',now(),now());",rq.username,rq.friendname,rq.Winname);
		if (!sql.UpdateMySql(sqlBuf)) { 
			cout << "sqlBuf:" << sqlBuf << endl; 
			cout << "userName 更新数据库错误" << endl;
			return;
		}

	}else if(rq.shouldDo=='b'){
		cout<<"b is update "<<endl;
		//好友约战结束，为b是对局结束的标志
		char sqlBuf[1024] = "";
		sprintf(sqlBuf,"update t_game set EndTime =now() ,win ='%s' where Time =(select ub.Time from (select *from t_game where idA='%s' and idB= '%s' order by Time desc limit 0,1) ub);",rq.Winname,rq.username,rq.friendname);
		if (!sql.UpdateMySql(sqlBuf)) { 
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "userName 更新错误" << endl;
			return; 

		}
		bzero(sqlBuf,sizeof(sqlBuf));
		//删除两人对战 因为异常退出的多余标记
		sprintf(sqlBuf,"delete from t_game where win ='No' and idA='%s' and idB='%s';",rq.username,rq.friendname);
		if (!sql.UpdateMySql(sqlBuf)) {
			cout << "sqlBuf:" << sqlBuf << endl;
			cout << "t_score error" << endl;
			return;
		}



	}
}
void  ckernel::Del_vsHistory(SOCKET ISendIp, char* buf, int len){
	//显示战绩
	STRU_VsHistory rq = *(STRU_VsHistory*)buf;
	list<string> listRes;
	char sqlBuf[1024] = "";
	//更新对面的
	sprintf(sqlBuf,"update t_score set times = (select count('%s') from t_game where idA = '%s'or idB='%s' and win !='No') where userid = '%s' ;",rq.username,rq.username,rq.username,rq.username);

	if (!sql.UpdateMySql(sqlBuf)) {
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "t_score error" << endl;
		return;
	}
	bzero(sqlBuf,sizeof(sqlBuf));
	//更新胜利的局数
	sprintf(sqlBuf,"update t_score set win =(select count('%s') from t_game where win ='%s') where userid ='%s';",rq.username,rq.username,rq.username);

	if (!sql.UpdateMySql(sqlBuf)) {
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "t_score 2  error" << endl;
		return;
	}

	bzero(sqlBuf,sizeof(sqlBuf));
	//得到更新后的结果
	sprintf(sqlBuf,"select times,win from t_score where userid ='%s';",rq.username);
	if (!sql.SelectMySql(sqlBuf, 2, listRes)) {
		//让listRes  得到sqlBuf中的sql语句输入后的表格的前2列  
		cout << "sqlBuf:" << sqlBuf << endl;
		cout << "更新数据库错误" << endl;
		return;
	}
	//返还报文
	rq.SumVs= atoi(listRes.front().c_str());
	listRes.pop_front();
	rq.win= atoi(listRes.front().c_str());
	send(ISendIp, (char*)&rq,rq.size, 0);

}
