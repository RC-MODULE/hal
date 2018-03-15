#include <hal.h>
extern "C"{
static int pntr2src_2dma; 
static int pntr2dst_2dma; 
static int pntr2size_2dma;
static DmaCallback user_callback_loc;
static int flag_of_pack_DMA;

	DmaCallback readCallback();

static int ownCallback(){
	halSetCallbackDMA((DmaCallback)user_callback_loc);
	flag_of_pack_DMA = 0x0;
	halInitSingleDMA((void*)pntr2src_2dma,(void*)pntr2dst_2dma,pntr2size_2dma);
	return 0;
}

int halInitDoubleDMA(void*  src0, void*  src1, void* dst0, void* dst1, int intSize0, int intSize1){
	flag_of_pack_DMA = 0xffffffff;
	pntr2src_2dma  = (int)src1;	
	pntr2dst_2dma  = (int)dst1;	
	pntr2size_2dma = intSize1;
	user_callback_loc = readCallback();
	halSetCallbackDMA((DmaCallback) ownCallback);	
	halInitSingleDMA(src0,dst0,intSize0);
	return 0;
}

};	
