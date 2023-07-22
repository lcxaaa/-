#include"../include/thread_pool.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t MA;
extern pthread_cond_t CR;
extern pthread_cond_t PR;
extern pool_t* pool;
extern int epfd;
extern int serverfd;


int FindAlive(pthread_t tid){
	pthread_kill(tid,0);
	if(errno==ESRCH){
		return 0;	
	}
	return 1;
}
//不应该出现pool 的cur 的 -- 行为 或者 ++ 行为  用来判断还可以
void * manageWork(void* val){
	int able=1,size=0 ;
	//创建工作者线程，未被使用的tid为 -1
	pthread_mutex_lock(&mutex);
	pool->ct = (pthread_t*)malloc(sizeof(pthread_t)*pool->max);
	for(int i=0;i<pool->max;i++){
		pool->ct[i]=-1;	
	}
	pthread_mutex_unlock(&mutex);

	pthread_mutex_lock(&mutex);
	for(int i=0;i<pool->busy;i++){
		pool->work++;
		pthread_create(&pool->ct[i],NULL,&customerWork,NULL);
	}
	pthread_mutex_unlock(&mutex);
	//able 为线程池是否在工作状态 在就设置为1  每层while都要同步一次able的值
	while(able){		
		pthread_mutex_lock(&mutex);

		//线程池没有任务了，让生产者去创建
		if(pool->cur==0){
			printf("manager wait\n");
			//唤醒生产者
			pthread_cond_signal(&PR);
			pthread_cond_wait(&MA,&mutex);
			//被唤醒后看看线程池有没有被关闭
			if(!pool->able){
				pthread_mutex_unlock(&mutex);
				printf("return now\n");
				return NULL ;
			}
			printf("manager wake up\n");
		}
		pthread_mutex_unlock(&mutex);

	
		pthread_mutex_lock(&mutex);
		//唤醒工作者线程，让他干活
		//cur在工作者里面会--
		for(int i=0,j=0;i<pool->max;i+=1){
			if(pool->ct[i]!=-1){
				pthread_cond_signal(&CR);
				j++;
			}
			//因为不确定是任务多还是线程多 所以写成这样
			if(j==pool->busy&&j==pool->cur) break;
		}
		pthread_mutex_unlock(&mutex);
		sleep(0);
		
		pthread_mutex_lock(&mutex);
		//工作完后，回收tid
		for(int i=0;i<pool->max;i+=1){
			if(pool->ct[i]!=-1&&FindAlive(pool->ct[i])==0){
				pthread_join(pool->ct[i],NULL);
				pool->ct[i]=-1;
			}
		}


		//工作完后判断 cur 和busy的关系
		if((size=pool->cur-pool->busy)>0){	
			//线程少了
			printf("create tid\n");
			for(int i=0;size!=0&&i<pool->max;i+=1){
				if(pool->work>=pool->max) break;
				if(pool->ct[i] ==-1){
					size--;
					pool->busy++;
					pool->work++;
					pthread_create(&pool->ct[i],NULL,customerWork,NULL);
				}
			}
		}else if(size<=0&&pool->busy>pool->min+(pool->max-pool->min)/2){
			//线程多了
			printf("--------------------\n");
			printf("dead nums :%d\n",pool->dead);
			printf("--------------------\n");
			pthread_cond_signal(&CR);
			pool->dead=pool->busy - pool->min+(pool->max-pool->min)/2;
			
		}else if(pool->busy<pool->min){
			int nums = pool->min+(pool->max-pool->min)/2- pool->busy;
			for(int i=0,j=0;i<pool->max;i++){
				if(pool->ct[i]==-1){
					pthread_create(&pool->ct[i],NULL,customerWork,NULL);
					pool->busy++;
					pool->work++;
					j++;
				}
				if(j>=nums) break;
			}
		}
		pthread_mutex_unlock(&mutex);
		
	
		pthread_mutex_lock(&mutex);

		//回收上面导致线程自杀的线程tid
		for(int i=0;i<pool->max;i+=1){
			if(pool->ct[i]!=-1&&FindAlive(pool->ct[i])==0){
				pthread_join(pool->ct[i],NULL);
				pool->ct[i]=-1;
			}
		}

		able = pool->able;
		
		pthread_mutex_unlock(&mutex);
	}

}
