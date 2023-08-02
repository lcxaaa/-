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
	//释放核心功能类	
	free(kernel);
	kernel =nullptr;
	
}
