#include "hal.h"
#include "led.h"
extern "C"{
static void** pSrc; 
static void** pDst; 
static int*   pSize;
static DmaCallback userCallback;

	DmaCallback readCallback();

static int ownCallback(){
  halLed(0x3);
	pSrc++;
	pDst++;
	pSize++;
  if(*(int*)(pSize+1) == 0){
		halSetCallbackDMA(userCallback);	
		SetFlagDMA(0x0);
	}
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	return 0;
}


int halInitPacketDMA(void** psrc,  void** pdst,  int* psize32){
	halLockDMA();
	SetFlagDMA(0xffffffff);
	pSrc  = psrc;
	pDst  = pdst;
	pSize = psize32;
	if(*(int*)(psize32 + 1) == 0){
		SetFlagDMA(0x0);
		halInitSingleDMA(*pSrc,*pDst,*pSize);
		return 0;
	}
	userCallback = readCallback();
	halSetCallbackDMA((DmaCallback)ownCallback);
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	return 0;
}

};
