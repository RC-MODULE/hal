#include <dma.h>
#include <hal.h>

extern "C"{

extern SyncBuf halSyncro;

#ifdef __GNUC__
__attribute__((section(".text_hal")))
#else
#pragma code_section ".text_hal"
#endif
void halEnterCriticalSection(){
	int id = halGetCoreId(); 
	int* flag = &(halSyncro.flag0);
	flag[id] = true;
	int opid = (id == 0)? 1 : 0;
	halSyncro.turn = opid;
	//halLedOn(flag[opid]);
	while( flag[opid] && (halSyncro.turn == opid));
}

#ifdef __GNUC__
__attribute__((section(".text_hal")))
#else
#pragma code_section ".text_hal"
#endif
void halExitCriticalSection(){
	int id = halGetCoreId();
	int* flag = &(halSyncro.flag0);
	int wrt_turn = (id == 0)? (1):(0);
	halSyncro.turn = wrt_turn;
	flag[id] = false;
}

};

