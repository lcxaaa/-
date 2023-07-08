#include"../include/thread_pool.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t MA;
extern pthread_cond_t CR;
extern pthread_cond_t PR;
extern pool_t* pool;
extern int epfd;
extern int serverfd;


void InitNet(){
	serverfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(12345);
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	socklen_t serverlen = sizeof(struct sockaddr_in);
	int res = bind(serverfd,(struct sockaddr*)&serveraddr,serverlen);
	if(res==-1) perror("bind error:");
	res = listen(serverfd,5);
	if(res==-1) perror("listen error:");
}

void InitEpoll(){
	epfd = epoll_create(1024);
	struct epoll_event epollnode;
	epollnode.data.fd = serverfd;
	epollnode.events = EPOLLIN;//默认水平触发
	epoll_ctl(epfd,EPOLL_CTL_ADD,serverfd,&epollnode);
}
