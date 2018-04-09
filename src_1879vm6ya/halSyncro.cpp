
#include "sleep.h"
#include "hal_target.h"
#include "led.h"
#pragma data_section ".data_hal_syncro"
struct SyncBuf {
	int		readCounter[2];
	int		writeCounter[2];
	int		sync0;
	int		sync1;
	int 	stateDMA;	// -1  free	
						// 0 used by core 0
						// 1 used by core 1

} halSyncro={0,0,0,0,0,0,-1};

#pragma data_section ".data"
extern "C"{
	int procNo=-1;
	void halSetProcessorNo(int number){
		procNo=number;
	}; 


	
int halSync(int val,int processor){
	
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
void* halSyncAddr(void* addr,int processor){
	return (void*)halSync((int)addr,processor);
}
int halSyncArray(
					 int value,        // Sync value
					 void *outAddress, // Sended array address (can be NULL)
					 size_t outLen,    // Sended array length (can be 0)
					 void **inAddress, // Received array address pointer (can be NULL)
					 size_t *inLen,   // Received array size pointer (can be NULL)
					 int  procNo)
{
	int sync=halSync(value,procNo);
	if (inAddress)
		*inAddress=(void*)halSync((int)outAddress);
	else 
		halSync((int)outAddress);
	if (inLen)
		*inLen=halSync(outLen);
	else 
		halSync(outLen);
	return sync;
}


};