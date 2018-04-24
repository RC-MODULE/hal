#include "hal.h"
#include "led.h"
extern "C"{
static void** pSrc; 
static void** pDst; 
static int*   pSize;
static DmaCallback userCallback;
//static int count_ = 0;
	DmaCallback readCallback();
static int ownCallback(){
	//halLedOn(6);
	//count_++;
	//halLedOn(count_);
	pSrc++;
	pDst++;
	pSize++;
  if(*(int*)(pSize+1) == 0){
		//halLedOn(7);
		halSetCallbackDMA(userCallback);	
		SetFlagDMA(0x0);
	}
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	//halLedOff(count_);
	return 0;
}


int halInitPacketDMA(void** psrc,  void** pdst,  int* psize32){
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
