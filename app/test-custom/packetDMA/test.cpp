#include "time.h"
#include "chain7707.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"
#include <string.h>

void initChain7707(int *buf);

#define MAX_NUM_BUFFERS 16 // Maximum buffers in packet
#define MAX_BUFFER_SIZE 32	// Maximum size of each buffer 
#define ALIGN( addr, numInts) ((((unsigned)addr)+numInts-1)%numInts*numInts) // align "addr" address to boundary of numInts 32-bit words

int chainBuf[MAX_NUM_BUFFERS*16];
void InitArr(nm32s* arr, int amm){
	for(int i=0; i<amm; i++){
		arr[i] = i;
	}
}

int main(){ 
	clock_t t0,t1;
	
	halInitDMA();
	halEnbExtInt();	

	for (int srcBankIndx = 0; srcBankIndx < 4; srcBankIndx++) {
		for (int dstBankIndx = 0; dstBankIndx < 4; dstBankIndx++) {
			nmppsMallocSetRoute16((0xF00) | (dstBankIndx << 4) | srcBankIndx);
			nm32s* src = nmppsMalloc_32s(MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			nm32s* dst = nmppsMalloc_32s(MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			printf("src: %x dst:%x \n", src, dst);
			//nmppsRandUniform_32s(src, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			//nmppsSet_32s(dst, 0xcccccccc, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			memset(src, 0xcccccccc, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			InitArr(src,MAX_NUM_BUFFERS*MAX_BUFFER_SIZE);
			//memset(src, 0xcccccccc, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			memset(dst, 0xcccccccc, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);

			if (src == 0 || dst == 0)
				return -1;

			nm32s* srcAddrList[MAX_NUM_BUFFERS];
			nm32s* dstAddrList[MAX_NUM_BUFFERS];
			int    bufSizeList[MAX_NUM_BUFFERS + 1];


			unsigned crcSrc = 0;
			int bufSize = MAX_BUFFER_SIZE;
			for (int i = 0; i < MAX_NUM_BUFFERS; i++) {
				//srcAddrList[i] = nmppsAddr_32s((nm32s*)ALIGN(src, 16), i*bufSize * 2);
				//dstAddrList[i] = nmppsAddr_32s((nm32s*)ALIGN(dst, 16), i*bufSize * 2);
				srcAddrList[i] = nmppsAddr_32s((nm32s*)src, i*bufSize );
				dstAddrList[i] = nmppsAddr_32s((nm32s*)dst, i*bufSize );

				bufSizeList[i] = bufSize ;

				nmppsCrcAcc_32s(srcAddrList[i], bufSize, &crcSrc);
			}
			bufSizeList[MAX_NUM_BUFFERS] = 0;

			t0 = clock();
			int err = halInitPacketDMA((void**)srcAddrList, (void**)dstAddrList, (int*)bufSizeList);

			if (err)	return -2;

			int status=1;
			unsigned time = 0;
			
			while(status) {
				halSleep(1);
				time++;
				printf("time %d\n", time);
				if (time > 2000) {
					printf("time =%d\n", time);
					return 333;
				}
				status = halStatusDMA();
			}; 
			t1 = clock();

			unsigned crcDst = 0;
			for (int i = 0; i < MAX_NUM_BUFFERS; i++) {
				nmppsCrcAcc_32s(dstAddrList[i], bufSize, &crcDst);
			}
			printf("%x %x dt=%d\n", crcSrc, crcDst, t1-t0 );

//		crcDst = 0;
//		crcSrc = 0;
		crcSrc = nmppsCrc_32s(src, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
		crcDst = nmppsCrc_32s(dst, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
//
			if (crcSrc - crcDst)
				return 9;
			
			nmppsFree(src);
			nmppsFree(dst);
			
		}
	}
	
	
	//return t1-t0;		
	
	return 777;		

}
