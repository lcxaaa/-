#include"../include/thread_pool.h"

extern int head;
extern int container[80];
extern int rear;


void InitContainer(){
	//任务队列初始化
	for(int i=0;i<80;i++)
	container[i]=-1;
	head=0;
	rear=0;
}

int GetContainer(){
	if(head<rear){
		//如果还有任务，得到任务，得到后赋值为-1
		int ans = container[head++%80];
		container[(head-1)%80] =-1;
		cout<<"ans is"<<ans<<endl;
		return ans;
	}else{
		return -1;	
	}	
}

void AddContainer(int a){
	//如果队列满了，不加入新的任务
	if(container[rear%80]==-1){
		cout<<"add cur"<<endl;
	container[rear++%80] = a;
	}
}
