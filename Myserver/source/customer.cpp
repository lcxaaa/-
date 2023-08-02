#include"../include/thread_pool.h"
#include"../include/ckernel.h"
#include<sys/mman.h>
#include<fcntl.h>
extern ckernel* kernel;

extern pthread_mutex_t mutex;
extern pthread_cond_t MA;
extern pthread_cond_t CR;
extern pthread_cond_t PR;
extern pool_t* pool;
extern int epfd;
extern int serverfd;


void* customerWork(void*val){
	//printf("thread %d  id:%lu is working\n",(int)val,pthread_self());
	
	while(pool->able){
		pthread_mutex_lock(&mutex);
		//等待任务队列
		while(pool->cur==0&&!pool->dead)
			pthread_cond_wait(&CR,&mutex);	

		if(!pool->dead&&pool->cur!=0){
			pool->cur --;
			pool->busy--;
		
			pthread_mutex_unlock(&mutex);
		//得到队列里面的套接字
			int clientfd = GetContainer();
			if(clientfd==-1){
				printf("customer error\n");
			}
			char buf[1024];
<<<<<<< HEAD
			//接受信息
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
			int size = recv(clientfd,buf,2048,0);
			int temp=0,ans=0;
			cout<<"size == "<<size<<endl;
			if(size ==0){
<<<<<<< HEAD
				//size为0表示对端断开连接
=======
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
				epoll_ctl(epfd,EPOLL_CTL_DEL,clientfd,NULL);
				pool->busy++;
				continue;
			}

			while(size>0){
				if(ans+4<=2048){
					//一个协议包 前4个为 type而 前4-8为 size 大小
					temp =*(int*)(buf+ans+4);
					cout<<"temp size:"<<temp<<endl;
				}else{
					break;	
				}

				int type = *(int*)(buf+ans);
				if(temp==0&&type==0) break;
				//进入核心类进行处理
				kernel->Deal(clientfd,type,buf+ans,temp);
				ans+=temp;
				size-=temp;
			}
				
			pool->busy++;
		}else{
			pool->dead --;
			pthread_mutex_unlock(&mutex);
			printf("dead \n");
			break;
		}
		//pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_lock(&mutex);
	pool->busy--;
	pool->work--;
	pthread_mutex_unlock(&mutex);
}
