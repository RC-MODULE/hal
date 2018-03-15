#include <hal.h>
extern "C"{
static void* pntr2src_2dma[2]; 
static void* pntr2dst_2dma[2]; 
static int pntr2size_2dma[2];
static DmaCallback user_callback_loc;
static int flag_of_pack_DMA;

	DmaCallback readCallback();

static int ownCallback(){
	halSetCallbackDMA((DmaCallback)user_callback_loc);
	flag_of_pack_DMA = 0x0;
	halInitSingleDMA(pntr2src_2dma[1],pntr2dst_2dma[1],pntr2size_2dma[1]);
	return 0;
}

int halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1){
	flag_of_pack_DMA = 0xffff;
	pntr2src_2dma[0]  = src0;	
	pntr2src_2dma[1]  = src1;	
	pntr2dst_2dma[0]  = dst0;	
	pntr2dst_2dma[1]  = dst1;	
	pntr2size_2dma[0] = intSize0;
	pntr2size_2dma[1] = intSize1;
	user_callback_loc = readCallback();
	halSetCallbackDMA((DmaCallback) ownCallback);	
	halInitSingleDMA(pntr2src_2dma[0],pntr2dst_2dma[0],pntr2size_2dma[0]);
	return 0;
}

};	
