#include "sleep.h"
#include "hal_target.h"
#include "led.h"
#include "hal.h"
#include "section-hal.h"

#ifdef __cplusplus
		extern "C" {
#endif


#ifdef __GNUC__
/**************************NMC-GCC************************/
__attribute__((section(".data_hal_syncro"))) SyncBuf halSyncro={-1,0,0,0,0,0,0,0,0,0};
//__attribute__((section(".data"))) int procNo=-1;

#else
/***************************NMCC*************************/
#pragma data_section ".data_hal_syncro"
  SyncBuf halSyncro={-1,0,0,0,0,0,0,0,0,0};
#pragma data_section ".data"

#endif
    

#include "section-hal.h"
	
INSECTION(".data_hal")	static int procNo=-1;
	
INSECTION(".text_hal")	void halSetProcessorNo(int number){
		procNo=number;
	};


INSECTION(".text_hal") int halSync(int val,int processor){

	if (procNo==-1)
		procNo=ncl_getProcessorNo();
	//printf ("HOST[%d]:Sync[%d] (%8xh)...",processor,pSyncBuf[processor].counter0,val);


	int sync;
	SyncBuf& syncro= halSyncro;
	if (procNo==0){
		while(syncro.writeCounter[0]>syncro.readCounter[0]){
			halSleep(1);
		}
		syncro.sync0=val;
		syncro.writeCounter[0]++;

		while(syncro.readCounter[1]==syncro.writeCounter[1]){
			halSleep(1);
		}
		sync=syncro.sync1;
		syncro.readCounter[1]++;
	}
	else if (procNo==1){

		while(syncro.writeCounter[1]>syncro.readCounter[1]){
			halSleep(1);
		}
		syncro.sync1=val;
		syncro.writeCounter[1]++;

		while(syncro.readCounter[0]==syncro.writeCounter[0]){
			halSleep(1);
		}
		sync=syncro.sync0;
		syncro.readCounter[0]++;

	}

	return sync;


}

INSECTION(".text_hal") void* halSyncAddr(void* addr,int processor){
	return (void*)halSync((int)addr,processor);
}

INSECTION(".text_hal") int halSyncArray(
					 int value,        // Sync value
					 void *outAddress, // Sended array address (can be NULL)
					 size_t outLen,    // Sended array length (can be 0)
					 void **inAddress, // Received array address pointer (can be NULL)
					 size_t *inLen,   // Received array size pointer (can be NULL)
					 int  procNo)
{
	int sync=halSync(value,procNo);
	if (inAddress)
		*inAddress=(void*)halSync((int)outAddress,procNo);
	else
		halSync((int)outAddress,procNo);
	if (inLen)
		*inLen=halSync(outLen,procNo);
	else
		halSync(outLen,procNo);
	return sync;
}

#ifdef __cplusplus
};
#endif

