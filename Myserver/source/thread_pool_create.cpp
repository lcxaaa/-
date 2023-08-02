#include"../include/thread_pool.h"
#include"../include/ckernel.h"
extern ckernel* kernel;
extern pthread_mutex_t mutex;
extern pthread_cond_t MA;
extern pthread_cond_t CR;
extern pthread_cond_t PR;
extern pool_t* pool;
extern int epfd;
extern int serverfd;
pthread_mutex_t mapmutex;

void Thread_Pool_Create(int max,int min){
	//线程池的初始化
	InitContainer();
	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&mapmutex,NULL);
	pthread_cond_init(&CR,NULL);
	pthread_cond_init(&PR,NULL);
	pthread_cond_init(&MA,NULL);
	InitNet();
	InitEpoll();
	//线程池参数初始化
	pool = (pool_t*)malloc(sizeof(pool_t));
	pool->max= max;
	pool->min = min;
	pool->busy = min+(max-min)/2;
	pool->work = 0;
	pool->able = 1;
	pool->cur =0;
	pool->dead =0;
	pthread_create(&pool->Mt,NULL,manageWork,NULL);
	sleep(0);
	pthread_create(&pool->Pt,NULL,producerWork,NULL);
	pthread_create(&pool->Al,NULL,aliveTest,NULL);
	printf("create success\n");
	//线程池状态输出
	while(1){
		printf("thread pool work....\n");
		pthread_mutex_lock(&mutex);
		if(!pool->able){
			pthread_mutex_unlock(&mutex);
			break;
		}
		printf("cur:%d   busy:%d   work:%d\n",pool->cur,pool->busy,pool->work);

		pthread_mutex_unlock(&mutex);
		sleep(1);
	}

	printf("destory pool\n");
	//线程池的销毁
	thread_pool_destroy();
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&CR);
	pthread_cond_destroy(&PR);
	pthread_cond_destroy(&MA);

}

map<SOCKET, int> m_AliveLoseCount;

void *aliveTest(void* val){
	//有可能出现进程崩溃，但是帮他进行四次挥手了,可以通过send得到
	int count = 0;
	bool cAble =false;
	while (pool->able) {
		sleep(20);//30s 进行一次心跳
		//tcp自带的心跳太慢了 2hour一次
		map<string, SOCKET> m_UseridToSocketTemp = kernel->m_mapUseridToSocket;
		/*
		   cout<<"find socket ================="<<endl;
		   for(auto&c:m_UseridToSocketTemp){
		   cout<<"userid"<<c.first<<"socket"<<c.second<<endl;
		   }*/
		if (m_UseridToSocketTemp.size() > 0) {

			//问题  当客户端多的时候，一个alive线程不够
			if (!pool->able)break;
			STRU_AliveTest rq;
			auto m = m_UseridToSocketTemp.begin();
			while (m != m_UseridToSocketTemp.end()) {
				//赋值时可能已经是-5了
				if (m_AliveLoseCount[m->second] <0) {
					m = m_UseridToSocketTemp.erase(m);
					continue;
				}

				while (count++ <= 3) {
					strcpy(rq.quest, "Test");
					strcpy(rq.username, m->first.c_str());
					if(send(m->second, (char*)&rq, rq.size, 0)<=0){
						
						//处理下线
						char sqlBuf[1024] = "";
						list<string> listRes;
<<<<<<< HEAD
						sprintf(sqlBuf, "update t_user set status =0 where userid ='%s';", m->first.c_str());
=======
						sprintf(sqlBuf, "update t_user set status =0 where userid ='%s';", rq.username);
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
						if (!kernel->sql.UpdateMySql(sqlBuf)) { 
							cout << "sqlBuf:" << sqlBuf << endl; 
							cout << "userName 更新数据库错误" << endl;
						}
						cout << "OFFline success" << endl;
						close(m->second);
						pthread_mutex_lock(&mapmutex);
						kernel->m_mapUseridToSocket.erase(rq.username);
						pthread_mutex_unlock(&mapmutex);
						m = m_UseridToSocketTemp.erase(m);
						cAble =true;
						break;
					}
					cout << "Send Alive Quest" << endl;
					m_AliveLoseCount[m->second];//创建表项
					sleep(1);
					cout<<m->first<<"socket :  " << m_AliveLoseCount[m->second] << endl;
				}
				if(cAble){
					cAble =false;
					continue;
				}
			//判断是否没有回复
				sleep(2);

				if (m_AliveLoseCount[m->second] == 0) {
					//没有恢复，强制下线
					close(m->second);
					cout<<"user should offline"<<endl;
					char sqlBuf[1024]="";
					list<string>listRes;
					sprintf(sqlBuf, "update t_user set status =0 where userid ='%s';", rq.username);
					if (!kernel->sql.UpdateMySql(sqlBuf)) {
						cout << "sqlBuf:" << sqlBuf << endl;
						cout << "userName 更新数据库错误" << endl;
					}
				cout<<"OFFLINE success"<<endl;
				 pthread_mutex_lock(&mapmutex);
				  kernel->m_mapUseridToSocket.erase(rq.username);
				   pthread_mutex_unlock(&mapmutex);

				m=m_UseridToSocketTemp.erase(m);
					if (m == m_UseridToSocketTemp.end()) break;
				}
				else {
					m_AliveLoseCount[m->second] = 0;
					m++;
				}
				count = 0;
			}

			// 每进行三次进行判断，一次都没有回应的，即 m_AliveLoseCount[]  为0 的，close;

			//可能在数到3时，刚好有一个进行连接
			count = 0;
		}
	}
}
