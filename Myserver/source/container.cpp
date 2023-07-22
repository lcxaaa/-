#include"../include/thread_pool.h"

extern int head;
extern int container[80];
extern int rear;


void InitContainer(){
	for(int i=0;i<80;i++)
	container[i]=-1;
	head=0;
	rear=0;
}

int GetContainer(){
	if(head<rear){

		int ans = container[head++%80];
		container[(head-1)%80] =-1;
		cout<<"ans is"<<ans<<endl;
		return ans;
	}else{
		return -1;	
	}	
}

void AddContainer(int a){
	if(container[rear%80]==-1){
		cout<<"add cur"<<endl;
	container[rear++%80] = a;
	}
}
