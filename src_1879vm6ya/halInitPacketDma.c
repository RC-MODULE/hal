#include "hal.h"
#include "led.h"
extern "C"{
#ifdef __GNUC__
__attribute__((section(".data_hal"))) static void** pSrc; 
__attribute__((section(".data_hal"))) static void** pDst; 
__attribute__((section(".data_hal"))) static int*   pSize;
__attribute__((section(".data_hal"))) static int    Amm;
__attribute__((section(".data_hal"))) static DmaCallback userCallback;
#else
#pragma code_section ".data_hal"
	static void** pSrc; 
	static void** pDst; 
	static int*   pSize;
	static int    Amm;
	static DmaCallback userCallback;
#pragma data_section ".data"
#endif


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
