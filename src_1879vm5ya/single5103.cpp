#include "stdio.h"
#ifndef xdmac5103tools
#define xdmac5103tools
#include "section-hal.h"

typedef int(*cnt_func)();

#ifdef __cplusplus
		extern "C" {
#endif

	//extern SpecDMA5103 chain_5103;
	extern void *chainBuf;
    cnt_func statusFunc;
	typedef int(*DmaCallback)();
	void SetCallBack( int vectoraddr, int ProcAddress);
	cnt_func halDmaStart( int *q );
	cnt_func dmainit( int dmahigh, int dst, int src, int size32);

	void openDma();
	void BellFromDma();// Обработчик DMA прерывания (векторы 58h,60h)

	INSECTION(".text_hal")	void halSetCallbackDMA(DmaCallback user_callback){
		SetCallBack(0x58, (int)user_callback);
		SetCallBack(0x60, (int)user_callback);	
	}
	
	INSECTION(".text_hal") 	int  halStatusDMA(){
		return statusFunc();
	}
	
	INSECTION(".text_hal")	int halInitSingleDMA(void*  src,  void*  dst,  int  size32 ) //,  DmaCallback* func, int channel)
	{ 
		int i;
		if (((int)src ^ (int)dst)<0) return -2;
	  	if (size32&1) return -1;
		statusFunc=dmainit(0, (int)dst, (int)src, size32);
		printf("statusfunc=%x\n",(int)statusFunc);
		
		return 0;
	}
	
	//int args_a[4],args_b[4];
	//int chain[8];
	
	// halInitDMA5103
	
#ifdef __cplusplus
		};
#endif

#endif
