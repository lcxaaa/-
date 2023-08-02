#include"../include/thread_pool.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t MA;
extern pthread_cond_t CR;
extern pthread_cond_t PR;
extern pool_t* pool;
extern int epfd;
extern int serverfd;
extern struct epoll_event epollevent[30];
//生产线程，为了避免epoll的惊群问题，值使用一个线程访问epoll
void* producerWork(void*val){
	while(1){
	int num = epoll_wait(epfd,epollevent,30,-1);
		if(num<0){
			perror("epoll wair error");
			return NULL;
		}
		for(int i=0;i<num;i++){
			if(epollevent[i].data.fd==serverfd){
				//新的连接
				printf("connect success\n");
				struct sockaddr_in clientaddr;
				socklen_t len = sizeof(struct sockaddr_in);
				int clientfd=accept(serverfd,(struct sockaddr*)&clientaddr,&len);
				struct epoll_event epollnode;
				epollnode.data.fd = clientfd;
				epollnode.events = EPOLLIN;
				epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&epollnode);
			}else{
				//产生事件，加入任务队列
				printf("having events\n");
				cout<<"clientfd is :"<<epollevent[i].data.fd<<endl;
				AddContainer(epollevent[i].data.fd);
				pool->cur++;
			}
		}
		pthread_mutex_lock(&mutex);

		printf("=================\n");
		printf("cur=%d\n",pool->cur);
		printf("=================\n");

		if(pool->cur>0){
<<<<<<< HEAD
			//当连接多的时候，可能导致事件的丢失，后期优化
			//线程池任务大于0,让管理者唤醒任务线程
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
			printf("producer wait\n");
			pthread_cond_signal(&MA);
			pthread_cond_wait(&PR,&mutex);
			printf("producer wake up\n");
		}
		pthread_mutex_unlock(&mutex);
	}
	//线程池关闭
	printf("time to end\n");
	pthread_mutex_lock(&mutex);
	pool->able =0;
	pthread_mutex_unlock(&mutex);
}
