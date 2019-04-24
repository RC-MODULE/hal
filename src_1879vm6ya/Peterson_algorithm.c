#include <dma.h>
#include <hal.h>
#include "section-hal.h"

#ifdef __cplusplus
		extern "C" {
#endif


extern SyncBuf halSyncro;

INSECTION(".text_hal") void halEnterCriticalSection(){
	int id = halGetCoreId(); 
	int* flag = &(halSyncro.flag0);
	flag[id] = true;
	int opid = (id == 0)? 1 : 0;
	halSyncro.turn = opid;
	//halLedOn(flag[opid]);
	while( flag[opid] && (halSyncro.turn == opid));
}

INSECTION(".text_hal") void halExitCriticalSection(){
	int id = halGetCoreId();
	int* flag = &(halSyncro.flag0);
	int wrt_turn = (id == 0)? (1):(0);
	halSyncro.turn = wrt_turn;
	flag[id] = false;
}

#ifdef __cplusplus
};
#endif

