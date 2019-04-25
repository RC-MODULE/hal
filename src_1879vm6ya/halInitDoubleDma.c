#include <hal.h>
#include "section-hal.h"
#ifdef __cplusplus
		extern "C" {
#endif

	
	INSECTION(".data_hal") static int pntr2src_2dma; 
	INSECTION(".data_hal") static int pntr2dst_2dma; 
	INSECTION(".data_hal") static int size1_dma;
	INSECTION(".data_hal") static DmaCallback user_callback_loc;


	DmaCallback readCallback();
	INSECTION(".text_hal")  static int ownCallback(){
		halSetCallbackDMA((DmaCallback)user_callback_loc);
		SetFlagDMA(0x0);
		halInitSingleDMA((void*)pntr2src_2dma,(void*)pntr2dst_2dma,size1_dma);
		return 0;
	}

	INSECTION(".text_hal") int halInitDoubleDMA(void*  src0, void*  src1, void* dst0, void* dst1, int intSize0, int intSize1){
		SetFlagDMA(0xffffffff);
		pntr2src_2dma  = (int)src1;	
		pntr2dst_2dma  = (int)dst1;	
		size1_dma = intSize1;
		user_callback_loc = readCallback();
		halSetCallbackDMA((DmaCallback) ownCallback);	
		halInitSingleDMA(src0,dst0,intSize0);
		return 0;
	}

#ifdef __cplusplus
};
#endif

