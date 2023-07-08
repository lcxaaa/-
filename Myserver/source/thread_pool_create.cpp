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

void Thread_Pool_Create(int max,int min){
	//线程池的初始化
	InitContainer();
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&CR,NULL);
	pthread_cond_init(&PR,NULL);
	pthread_cond_init(&MA,NULL);
	InitNet();
	InitEpoll();

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
	int count = 0;
	while (pool->able) {
		sleep(30);//30s 进行一次心跳
		//tcp自带的心跳太慢了 2hour一次
		if (kernel->m_mapUseridToSocket.size() > 0) {
			
			//问题  当客户端多的时候，一个alive线程不够
			if (!pool->able)break;
			STRU_AliveTest rq;
			auto m = kernel->m_mapUseridToSocket.begin();
			while (m != kernel->m_mapUseridToSocket.end()) {
				if (m_AliveLoseCount[m->second] <0) {
					m = kernel->m_mapUseridToSocket.erase(m);
					continue;
				}
				while (count++ <= 3) {
					strcpy(rq.quest, "Test");
					strcpy(rq.username, m->first.c_str());
					send(m->second, (char*)&rq, rq.size, 0);
					cout << "Send Alive Quest" << endl;
					m_AliveLoseCount[m->second];//创建表项
					sleep(1);
					cout<<"socket :  " << m_AliveLoseCount[m->second] << endl;
				}

				if (m_AliveLoseCount[m->second] == 0) {
					close(m->second);
					cout << "user should offline" << endl;
					
					char sqlBuf[1024] = "";
					list<string> listRes;
					sprintf(sqlBuf, "update t_user set status =0 where userid ='%s';", m->first.c_str());

					if (!kernel->sql.UpdateMySql(sqlBuf)) {
						cout << "sqlBuf:" << sqlBuf << endl;
						cout << "userName 更新数据库错误" << endl;
					}

					cout << "OFFline success" << endl;
					m = kernel->m_mapUseridToSocket.erase(m);
			
					if (m == kernel->m_mapUseridToSocket.end()) break;

				}
				else {
					m_AliveLoseCount[m->second] = 0;
				}
				count = 0;
				m++;
			}

			// 每进行三次进行判断，一次都没有回应的，即 m_AliveLoseCount[]  为0 的，close;

			//可能在数到3时，刚好有一个进行连接
			count = 0;
		}
	}
}
