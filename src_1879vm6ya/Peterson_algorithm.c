#include <dma.h>
#include <hal.h>

extern "C"{

extern SyncBuf halSyncro;

void halEnterCriticalSection(){
	int id = halGetCoreId(); 
	int* flag = &(halSyncro.flag0);
	flag[id] = true;
	int opid = (id == 0)? 1 : 0;
	halSyncro.turn = opid;
	//halLedOn(flag[opid]);
	while( flag[opid] && (halSyncro.turn == opid));
}


void halExitCriticalSection(){
	int id = halGetCoreId();
	int* flag = &(halSyncro.flag0);
	int wrt_turn = (id == 0)? (1):(0);
	halSyncro.turn = wrt_turn;
	flag[id] = false;
}

};

