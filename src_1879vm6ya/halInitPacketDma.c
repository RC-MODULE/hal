#include "hal.h"
extern "C"{
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
		flag_of_pack_DMA = 0x0;
	}
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	return 0;
}


int halInitPacketDMA(void** psrc,  void** pdst,  int* psize32){
	flag_of_pack_DMA = 0xffffffff;
	pSrc  = psrc;
	pDst  = pdst;
	pSize = psize32;
	if(*(int*)(psize32 + 1) == 0){
		flag_of_pack_DMA = 0x0;
		halInitSingleDMA(*pSrc,*pDst,*pSize);
		return 0;
	}
	userCallback = readCallback();
	halSetCallbackDMA((DmaCallback)ownCallback);
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	return 0;
}

};
