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
	
	char filename[100];
	sprintf(filename,"./temp/Recv%lu",pthread_self());
	int MMapFd =open(filename,O_RDWR|O_CREAT,0664);
	ftruncate(MMapFd,2048);
	lseek(MMapFd,0,SEEK_SET);
	char* buf = (char*)mmap(NULL,2048,PROT_READ|PROT_WRITE,MAP_SHARED,MMapFd,0);
	char * bufSave =buf;

	while(pool->able){
		pthread_mutex_lock(&mutex);
		while(pool->cur==0&&!pool->dead)
			pthread_cond_wait(&CR,&mutex);	

		if(!pool->dead&&pool->cur!=0){
			pool->cur --;
			pool->busy--;
		
			pthread_mutex_unlock(&mutex);

			int clientfd = GetContainer();
			//char buf[1024];
			int size = recv(clientfd,buf,2048,0);
			int temp=0,ans=0;

			if(size ==0){
				printf("client close fd\n");
				close(clientfd);
				epoll_ctl(epfd,EPOLL_CTL_DEL,clientfd,NULL);
				pool->busy++;

				lseek(MMapFd,0,SEEK_SET);
				bzero(buf,2048);

				continue;
			}

			while(size>0){
				if(ans+4<=2048){
					temp =*(int*)(buf+ans+4);
					cout<<"temp size:"<<temp<<endl;
				}else{
					break;	
				}

				int type = *(int*)(buf+ans);
				if(temp==0&&type==0) break;
				kernel->Deal(clientfd,type,buf+ans,temp);
				ans+=temp;
				size-=temp;
			}

			lseek(MMapFd,0,SEEK_SET);
			buf = bufSave;
			bzero(buf,2048);
						
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

	munmap(bufSave,2048);
	close(MMapFd);
	remove(filename);


}
