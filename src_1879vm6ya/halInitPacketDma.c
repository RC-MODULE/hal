#include "hal.h"
#include "led.h"
extern "C"{
static void** pSrc; 
static void** pDst; 
static int*   pSize;
static int    Amm;
static DmaCallback userCallback;
	DmaCallback readCallback();
	
#ifdef __GNUC__
__attribute__((section(".text_hal")))
#else
#pragma code_section ".text_hal"
#endif
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

#ifdef __GNUC__
__attribute__((section(".text_hal")))
#else
#pragma code_section ".text_hal"
#endif
int halInitPacketDMA(void** psrc,  void** pdst,  int* psize32, int amm){
	SetFlagDMA(0xffffffff);
	if(amm == 0){
		SetFlagDMA(0x0);
		halInitSingleDMA(*psrc,*pdst,0);
		return 0;
	}
	if(amm == 1){
		SetFlagDMA(0x0);
		halInitSingleDMA(*psrc,*pdst,*psize32);
		return 0;
	}
	pSrc  = psrc;
	pDst  = pdst;
	pSize = psize32;
	Amm = amm;
	userCallback = readCallback();
	halSetCallbackDMA((DmaCallback)ownCallback);
	halInitSingleDMA(*pSrc,*pDst,*pSize);
	Amm--;
	return 0;
}

};
