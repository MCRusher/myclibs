#include "../code/myclibs/newstd.h"

mutex_t mtx;

int threaded fun(int* code){
    string s = string_Create("reattempting lock");
    while(!mutex_timedlock(&mtx,500))
		println("reattempting lock");
    println("finished waiting");
    string_Delete(&s);
    mutex_unlock(&mtx);
    return *code;
}

int main(void){
	mtx = mutex_create();
	int val = 0, arg = 1337;
	mutex_lock(&mtx);
	println("waiting");
	thread_t tfun = thread_start(&fun,&arg);
	sleep_for(3000);
	mutex_unlock(&mtx);
	thread_join(tfun,&val);
	printf("val: %d\n",val);
	mutex_delete(&mtx);
	return 0;
}