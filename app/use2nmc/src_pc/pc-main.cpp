#include "ubc_host.h"
#include "stdio.h"
const int SIZE=128;
unsigned int srcHostArray[SIZE];
unsigned int dstHostArray[SIZE];


int main(){
	for(int i=0;i<SIZE;i++)	srcHostArray[i]=i;
	
	unsigned srcBoardAddr;
	unsigned dstBoardAddr;
	unsigned sync;
	if (ubcOpen("main0.abs","main1.abs",NULL))	// Load executable files on two processsors, and connect to shared memory
		return -1;
	
	
	//--------- processor #0 communication ----------------------------------------
	sync        =ubcSync(0x8086,0);			// receive handshake (0x6406)
	srcBoardAddr=ubcSync(SIZE,0);			// receive address to write  on board  , send size of array
	dstBoardAddr=ubcSync(123,0);			// receive address to read from board  , send increment value
	ubcWriteMemBlock(srcHostArray,srcBoardAddr,SIZE,0); // write data to board 
	sync        =ubcSync(0x600DB00F,0);		// send end of write status
	sync        =ubcSync(0x6000DBA1,0); 	// wait ready to read status , send/receive last sync (G000dBuy)
	ubcReadMemBlock (dstHostArray,dstBoardAddr,SIZE,0); // read data from board
	ubcGetResult(&sync,0);					// get return value
	for(int i=0; i<SIZE;i++) printf("%d\n",dstHostArray[i]);
	printf("Processor[0] Return value:%X\n",sync);
	//--------- processor #1 communication ----------------------------------------
	sync        =ubcSync(0x8086,1);			// receive handshake (0x6406)
	srcBoardAddr=ubcSync(SIZE,1);			// receive address to write  on board  , send size of array
	dstBoardAddr=ubcSync(123,1);			// receive address to read from board  , send increment value
	ubcWriteMemBlock(srcHostArray,srcBoardAddr,SIZE,1); // write data to board 
	sync        =ubcSync(0x600DB00F,1);		// send end of write status
	sync        =ubcSync(0x6000DBA1,1); 	// wait ready to read status , send/receive last sync (G000dBuy)
	ubcReadMemBlock (dstHostArray,dstBoardAddr,SIZE,1); // read data from board
	ubcGetResult(&sync,1);					// get return value
	printf("Processor[1] Return value:%X\n",sync);
	for(int i=0; i<SIZE;i++) printf("%d\n",dstHostArray[i]);
	//------------------------------------------------------------------------------
	ubcClose();							// close board
	
	return sync;

}