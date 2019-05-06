#include "dma5103.h"
#include "section-hal.h"
//typedef int(*cnt_func)();

typedef void(*bell_func)();
//extern SpecDMA5103 chain_5103;
extern int * chainBuf;

#ifdef __cplusplus
		extern "C" {
#endif

  //typedef int(*DmaCallback)();
  void SetCallBack( int vectoraddr, int ProcAddress);
  //int halDmaStart( int *q );

extern int ofdmainit;
INSECTION(".data_hal") int zzzMatrix[6];
extern int matrixcnt;

INSECTION(".text_hal") void BellFromDmaMatrix()// Обработчик DMA прерывания (векторы 58h,60h)
{  
	if (zzzMatrix[3])
   { zzzMatrix[3]--;
     zzzMatrix[1]+=zzzMatrix[4];
     zzzMatrix[2]+=zzzMatrix[5];
     halDmaStart( zzzMatrix);
   }
}  // BellFromDmaMatrix

INSECTION(".text_hal") int  halInitMatrixDMA(void*  src,  int  width,  int  height, int srcStride32,
     void* dst, int dstStride32, bell_func DmaCallback, int channel)
{ 
	if (((int)src ^ (int)dst)<0) return -2;

	int i,srcrow,dstrow,asrc,adst,sz, *q;
    q=(int*)0x10000050;
    i=channel; // only to avoid warning
    if(matrixcnt==0)
    {  SetCallBack(0x58, (int)BellFromDmaMatrix);
       SetCallBack(0x60, (int)BellFromDmaMatrix);
       matrixcnt=1;
    }

   srcrow=srcStride32;    //
   dstrow=dstStride32;   // 
   sz=width;                    //
   asrc=(int)src;  adst=(int)dst;
   zzzMatrix[0]=(int)width;
   zzzMatrix[1]=(int)src;
   zzzMatrix[2]=(int)dst;
   zzzMatrix[3]=height;
   zzzMatrix[4]=srcStride32;
   zzzMatrix[5]=dstStride32;
   statusFunc=halDmaStart( zzzMatrix);
   return 0;
} //  halInitMatrixDMA

#ifdef __cplusplus
		};
#endif

