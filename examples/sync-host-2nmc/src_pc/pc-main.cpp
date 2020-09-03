#include "hal_host.h"
#include "stdio.h"
const int SIZE=2048;
unsigned int srcHostArray[SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
unsigned int dstHostArray[SIZE];


int main(){
	//for(int i=0;i<SIZE;i++)	srcHostArray[i]=i;
	
	ARCH_ADDR buffAddr0,buffAddr1;
	unsigned sync,size=16;

	if (halOpen("main0.abs","main1.abs",NULL))	// Load executable files on two processsors, and connect to shared memory
		return -1;
	
	//------------- handshakes --------------------------------------------------
	sync        =halSync(size, 0);						// receive handshake (0x6406) / send size
	sync		=halSync(size, 0);						// receive handshake (0x6406) / send size
	sync		=halSync(size, 1);						// receive handshake (0x6406) / send size
	sync        =halSync(size, 1);						// receive handshake (0x6406) / send size


	//--------- processor #0 communication ----------------------------------------
	
	buffAddr0 =halSyncAddr((void*)102,0);						// receive address to write  on board   
	halWriteMemBlock(srcHostArray, buffAddr0, size, 0); // write data to board (nmpu-0)
	sync = halSync(0x600DB00F, 0);						// send end of write status

	//--------- processor #1 communication ----------------------------------------
	buffAddr1 = halSyncAddr((void*)103, 1);						// receive address to write  on board   
	halReadMemBlock(dstHostArray, buffAddr1, size, 1);	// read data from board (nmpu-1)
	sync = halSync(0x600DB00F, 1);						// send end of write status

	for(int i=0; i<size;i++)	
		printf("dstHostArray[%d]=%X\n",i,srcHostArray[i]);
	//---------- finish -----------------------------------------------------------
	halGetResult(&sync,0);								// get return value 
	printf("Processor[0] Return value:%X\n", sync);
	halGetResult(&sync,1);								// get return value
	printf("Processor[1] Return value:%X\n", sync);
	
	//------------------------------------------------------------------------------
	halClose();											// close board
	
	return sync;

}