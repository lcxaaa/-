#include"../include/thread_pool.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t MA;
extern pthread_cond_t CR;
extern pthread_cond_t PR;
extern pool_t* pool;
extern int epfd;
extern int serverfd;
extern pthread_mutex_t mapmutex;

void thread_pool_destroy(){
	for(int i=0;i<pool->max;i++){
		pool->dead =100;
		pthread_cond_signal(&CR);
	}

	for(int i=0;i<pool->max;i++){
		if(pool->ct[i]!=-1)
		pthread_join(pool->ct[i],NULL);	
	}
	printf("destory M\n");
	pthread_cond_signal(&MA);
	pthread_join(pool->Mt,NULL);
	printf("destory P\n");
	pthread_join(pool->Pt,NULL);
	pthread_join(pool->Al,NULL);

	free(pool->ct);
	free(pool);
	pthread_mutex_destroy(&mapmutex);
}
