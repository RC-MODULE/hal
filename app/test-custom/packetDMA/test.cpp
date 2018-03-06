#include "mb7707_load.h"
#include "time.h"
#include "chain7707.h"
#include "nmpp.h"
#include "dma.h"
//#include "stdio.h"
//#include "time.h"

void initChain7707(int *buf);
extern chain7707 chain;


#define NUM_BUFFERS 16
int chainBuf[NUM_BUFFERS*16];
int main()
{ 
	clock_t t0,t1;
	initChain7707((int*)chainBuf); // Создаём пустую таблицу в памяти массива chainBuf
	
	nm32s* src = nmppsMalloc_32s(NUM_BUFFERS*256);
	nm32s* dst = nmppsMalloc_32s(NUM_BUFFERS*256);

	if (src==0 || dst==0 )
		return -1;

	nm32s* srcAddrList[NUM_BUFFERS];
	nm32s* dstAddrList[NUM_BUFFERS];
	int    bufSizeList[NUM_BUFFERS+1];
	
	for (int i=0; i<NUM_BUFFERS; i++){
		//srcAddrList[i]= nmppsAddr_32s(src, i*2);
		//dstAddrList[i]= nmppsAddr_32s(dst, i*2);
		srcAddrList[i]= nmppsAddr_32s((nm32s*)0x00040000, i*128);
		dstAddrList[i]= nmppsAddr_32s((nm32s*)0x00041000, i*128);
		bufSizeList[i]= 128;
	}
	bufSizeList[NUM_BUFFERS]= 0;
	
	t0 = clock();
	int err=halInitPacketDMA((void**)srcAddrList,  (void**) dstAddrList,  (int*)bufSizeList);
	
	//printf("sad|");
	//if (err)
	//	return err;
	//k=halInitSingleDMA(src,   dstp,  (int*)szp);
	for (int k=0; k<7164384; k++) // цикл ожидания транзакции
    { 
		int i=xdmac0(); 
		if (i==0) {
			t1 = clock();
			return t1-t0;		
		}
    }
	
	return -1;		

}
