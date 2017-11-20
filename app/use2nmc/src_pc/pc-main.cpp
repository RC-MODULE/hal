#include "ubc_host.h"
#include "stdio.h"
const int SIZE=2048;
unsigned int srcHostArray[SIZE];
unsigned int dstHostArray[SIZE];


int main(){
	//for(int i=0;i<SIZE;i++)	srcHostArray[i]=i;
	
	unsigned srcBrdAddr0,srcBrdAddr1;
	unsigned dstBrdAddr0,dstBrdAddr1;
	unsigned sync,size;
	if (ubcOpen("main0.abs","main1.abs",NULL))	// Load executable files on two processsors, and connect to shared memory
		return -1;
	
	
	//--------- processor #0 communication ----------------------------------------
	sync        =ubcSync(0x8086,0);			// receive handshake (0x6406)
	sync        =ubcSync(0x8086,1);			// receive handshake (0x6406)
	sync        =ubcSync(0x8086,0);			// receive proc1-proc0 sync // 101
	sync        =ubcSync(0x8086,1);			// receive proc0-proc1 sync // 100

	size		=ubcSync(101,0);			// receive address to write  on board  
	srcBrdAddr0 =ubcSync(102,0);			// receive address to write  on board   
	dstBrdAddr0 =ubcSync(103,0);			// receive address to read from board   

	ubcReadMemBlock (srcHostArray,srcBrdAddr0,size,0); // read data from board
	sync        =ubcSync(0x600DB00F,0);		// send end of write status
	
	ubcWriteMemBlock(srcHostArray,dstBrdAddr0,size,0); // write data to board 
	sync        =ubcSync(0x6000DBA1,0); 	// wait ready to read status , send/receive last sync (G000dBuy)
	
	ubcGetResult(&sync,0);					// get return value
	for(int i=0; i<128;i++) printf("src[%i]=%d\n",i,srcHostArray[i]);
	printf("Processor[0] Return value:%X\n",sync);
	//--------- processor #1 communication ----------------------------------------
	
	size        =ubcSync(srcBrdAddr0,1);	// send shared addr of proc-0
	size        =ubcSync(dstBrdAddr0,1);	// send shared addr of proc-0
	srcBrdAddr1 =ubcSync(0,1);				// get shared addr of proc-1
	dstBrdAddr1 =ubcSync(0,1);				// get shared addr of proc-1
	
	ubcReadMemBlock (dstHostArray,srcBrdAddr1,size-100,1); // read data from board 1
	sync		=ubcSync(123,1);			// receive address to read from board  , send increment value

	ubcWriteMemBlock(dstHostArray,dstBrdAddr1,size,1); // write data to board 
	sync        =ubcSync(0x600DB00F,1);		// send end of write status

	ubcGetResult(&sync,1);					// get return value
	printf("Processor[1] Return value:%X\n",sync);
	for(int i=0; i<SIZE;i++) printf("%d\n",dstHostArray[i]);
	//------------------------------------------------------------------------------
	ubcClose();							// close board
	
	return sync;

}