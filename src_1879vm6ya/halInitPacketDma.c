#include "hal.h"
static void** pSrc; 
static void** pDst; 
static int*   pSize;
static DmaCallback userCallback;
extern int flag_of_pack_DMA;

DmaCallback readCallback();

static int ownCallback(){
	pSrc++;
	pDst++;
	pSize++;
  if(*(int*)(pSize+1) == 0){
		halSetCallbackDMA(userCallback);	
		flag_of_pack_DMA = 0xFFFF;
	}
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	return 0;
}


int halInitPacketDMA(void** psrc,  void** pdst,  int* psize32){
	flag_of_pack_DMA = 0;
	pSrc  = psrc;
	pDst  = pdst;
	pSize = psize32;
	userCallback = readCallback();
	halSetCallbackDMA(ownCallback);
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	return 0;
}

