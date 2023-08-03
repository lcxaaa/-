#pragma once
#ifndef _Thread_Pool_question_

#define _Thread_Pool_question_ 1
#include"MyDefine.h"
#include<openssl/md5.h>
typedef struct thread_pool{
	int max;
	int min;
	int busy;
	int work;
	pthread_t Mt;
	pthread_t Pt;
	pthread_t*ct;
	pthread_t Al;
	int able;
	int cur;
	int dead;
}pool_t;

void *customerWork(void* val);
void *producerWork(void* val);
void *manageWork(void* val);
void *aliveTest(void* val);
void Thread_Pool_Create(int max,int min);
void thread_pool_destroy();
void InitContainer();
int GetContainer();
void AddContainer(int a);
void InitNet();
void InitEpoll();
#endif
