#include"../include/thread_pool.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t MA;
extern pthread_cond_t CR;
extern pthread_cond_t PR;
extern pool_t* pool;
extern int epfd;
extern int serverfd;
extern struct epoll_event epollevent[30];

void* producerWork(void*val){
	while(1){
	int num = epoll_wait(epfd,epollevent,30,-1);
		if(num<0){
			perror("epoll wair error");
			return NULL;
		}
		for(int i=0;i<num;i++){
			if(epollevent[i].data.fd==serverfd){
				printf("connect success\n");
				struct sockaddr_in clientaddr;
				socklen_t len = sizeof(struct sockaddr_in);
				int clientfd=accept(serverfd,(struct sockaddr*)&clientaddr,&len);
				struct epoll_event epollnode;
				epollnode.data.fd = clientfd;
				epollnode.events = EPOLLIN;
				epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&epollnode);

			}else{
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
			printf("producer wait\n");
			pthread_cond_signal(&MA);
			pthread_cond_wait(&PR,&mutex);
			printf("producer wake up\n");
		}
		pthread_mutex_unlock(&mutex);
	}
	printf("time to end\n");
	pthread_mutex_lock(&mutex);
	pool->able =0;
	pthread_mutex_unlock(&mutex);
}
