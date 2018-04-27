#include "hal.h"
#include "led.h"
extern "C"{
static void** pSrc; 
static void** pDst; 
static int*   pSize;
static int    Amm;
static DmaCallback userCallback;
	DmaCallback readCallback();
static int ownCallback(){
	pSrc++;
	pDst++;
	pSize++;
  if(Amm  == 1){
		halSetCallbackDMA(userCallback);	
		SetFlagDMA(0x0);
	}
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	Amm--;
	return 0;
}


int halInitPacketDMA(void** psrc,  void** pdst,  int* psize32, int amm){
	SetFlagDMA(0xffffffff);
	pSrc  = psrc;
	pDst  = pdst;
	pSize = psize32;
	Amm = amm;
	if(amm == 1){
		SetFlagDMA(0x0);
		halInitSingleDMA(*pSrc,*pDst,*pSize);
		return 0;
	}
	userCallback = readCallback();
	halSetCallbackDMA((DmaCallback)ownCallback);
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	Amm--;
	return 0;
}

};
