#include "hal.h"
#include "led.h"
#include "section-hal.h"

#ifdef __cplusplus
		extern "C" {
#endif



INSECTION(".data_hal") static void** pSrc; 
INSECTION(".data_hal") static void** pDst; 
INSECTION(".data_hal") static int*   pSize;
INSECTION(".data_hal") static int    Amm;
INSECTION(".data_hal") static DmaCallback userCallback;


	DmaCallback readCallback();
	
INSECTION(".text_hal") static int ownCallback(){
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

INSECTION(".text_hal") int halInitPacketDMA(void** psrc,  void** pdst,  int* psize32, int amm){
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

#ifdef __cplusplus
};
#endif

