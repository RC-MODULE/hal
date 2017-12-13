#include "hal_host.h"
#include "stdio.h"
const int SIZE=2048;
unsigned int srcHostArray[SIZE];
unsigned int dstHostArray[SIZE];


int main(){
	//for(int i=0;i<SIZE;i++)	srcHostArray[i]=i;
	
	unsigned srcBrdAddr0,srcBrdAddr1;
	unsigned dstBrdAddr0,dstBrdAddr1;
	unsigned sync,size;
	if (halOpen("main0.abs","main1.abs",NULL))	// Load executable files on two processsors, and connect to shared memory
		return -1;
	
	//------------- handshakes --------------------------------------------------
	sync        =halSync(0x8086,0);			// receive handshake (0x6406)
	sync        =halSync(0x8086,1);			// receive handshake (0x6406)

	sync        =halSync(0x8086,0);			// receive repost proc1-proc0 sync // 101
	sync        =halSync(0x8086,1);			// receive repost proc0-proc1 sync // 100
	//--------- processor #0 communication ----------------------------------------
	
	size		=halSync(101,0);			// receive address to write  on board  
	srcBrdAddr0 =halSync(102,0);			// receive address to write  on board   
	dstBrdAddr0 =halSync(103,0);			// receive address to read from board   

	halReadMemBlock (srcHostArray,srcBrdAddr0,size,0); // read data from board
	sync        =halSync(0x600DB00F,0);		// send end of write status
	for(int i=0; i<size;i++) printf("src[%i]=%X\n",i,srcHostArray[i]);

	halWriteMemBlock(srcHostArray,dstBrdAddr0,size,0); // write data to board 
	sync        =halSync(0x6000DBA1,0); 	// wait ready to read status , send/receive last sync (G000dBuy)
	
	halGetResult(&sync,0);					// get return value
	
	printf("Processor[0] Return value:%X\n",sync);
	//--------- processor #1 communication ----------------------------------------
	
	size        =halSync(201,1);	// send shared addr of proc-0
	srcBrdAddr1 =halSync(202,1);				// get shared addr of proc-1
	dstBrdAddr1 =halSync(203,1);				// get shared addr of proc-1
	
	halReadMemBlock (srcHostArray,srcBrdAddr1,size,1); // read data from board 1
	sync		=halSync(123,1);			// receive address to read from board  , send increment value
	for(int i=0; i<size;i++)	printf("srcHostArray[%d]=%X\n",i,srcHostArray[i]);

	halWriteMemBlock(dstHostArray,dstBrdAddr1,size,1); // write data to board 
	sync        =halSync(0x600DB00F,1);		// send end of write status

	halGetResult(&sync,1);					// get return value
	printf("Processor[1] Return value:%X\n",sync);
	
	//------------------------------------------------------------------------------
	halClose();							// close board
	
	return sync;

}