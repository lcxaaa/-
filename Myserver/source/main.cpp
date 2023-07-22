#include"../include/ckernel.h"
#include"../include/thread_pool.h"
ckernel* kernel = new ckernel;

int head;
int container[80];
int rear;

pthread_mutex_t mutex;

pthread_cond_t MA;
pthread_cond_t CR;
pthread_cond_t PR;
pool_t* pool;
int epfd;
int serverfd;
struct epoll_event epollevent[30];

int main()
{
   //6*2 -生产者 main keepalive  manager 后为8
 
	Thread_Pool_Create(8, 4);
	
	free(kernel);
	kernel =nullptr;
	
}

//CMySql sql;
//if (!sql.ConnectMySql(MSYIP, MSYUSER, MSYPASSWORD, DP)) {
//	cout << "连接错误" << endl;
//}
//list<string> listRes;
//char sqlBuf[1024] = "";
//sprintf(sqlBuf, "select password,userid from t_user where userid ='%s';", "123");
//cout << sql.SelectMySql(sqlBuf, 2, listRes);
//string password = listRes.front();	//得到数据库中的密码
//listRes.pop_front();
//
//string userid = listRes.front().c_str();//得到数据库中的id
//listRes.pop_front();
//cout << password << userid << endl;
