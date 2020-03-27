#include "hal_host.h"
#include "stdio.h"
#include "windows.h"
const int SIZE32=1024;
unsigned int srcHostArray[SIZE32];
unsigned int dstHostArray[SIZE32];

int main(){

	for(int i=0;i<SIZE32;i++)	
		srcHostArray[i]=i;
	
	unsigned srcBoardAddr;
	unsigned dstBoardAddr;
	unsigned sync;
	printf("Openning...\n");
	if (halOpen("main.abs",NULL)){			// Load executable file to board, connect to shared memory
		::Sleep(10000);
		return -1;
	}
	
	sync        =halSync(0x8086);			// Receive handshake (0x6406)
	srcBoardAddr=halSync(SIZE32);			// Receive array address to write  on board  , send size of array
	dstBoardAddr=halSync(123);				// Receive array address to read from board  , send increment value
	halWriteMemBlock(srcHostArray,srcBoardAddr,SIZE32); // Write data to board 
	sync        =halSync(0x600DB00F);		// Send end of write status
											// ..... NMC working ...
	sync        =halSync(0x600DBA1); 		// Wait ready to read status , send/receive last sync (G000dBuy)
	halReadMemBlock (dstHostArray,dstBoardAddr,SIZE32); // read data from board
	for(int i=0; i<SIZE32;i++) printf("%d\n",dstHostArray[i]);
	
	//sync        =halSync(0x6000DA1); 	// wait ready to read status , send/receive last sync (G000dBuy)
	dstBoardAddr  =halSync(0x1); 		// Receive second array address to read from board 
	halReadMemBlock (dstHostArray,dstBoardAddr,SIZE32); // read data from board
	
											
	halGetResult(&sync);					// get return value (0x600D)
	printf("Return value:%X\n",sync);
	halClose();								// close board, disconect from shared memory
	printf("Sleep 5 sec...\n",sync);
	::Sleep(5000);
	return sync;

}