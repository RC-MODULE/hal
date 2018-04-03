#include "dma5103.h"
//typedef int(*cnt_func)();
typedef void(*bell_func)();
//extern SpecDMA5103 chain_5103;
extern int * chainBuf;
extern "C"
{
  //typedef int(*DmaCallback)();
  void SetCallBack( int vectoraddr, int ProcAddress);
  //int halDmaStart( int *q );
}
extern int ofdmainit;
int zzz[6];
extern int matrixcnt;
void BellFromDmaMatrix()// Îáğàáîò÷èê DMA ïğåğûâàíèÿ (âåêòîğû 58h,60h)
{  if (zzz[3])
   { zzz[3]--;
     zzz[1]+=zzz[4];
     zzz[2]+=zzz[5];
     halDmaStart( zzz);
   }
}  // BellFromDmaMatrix

int  halInitMatrixDMA(void*  src,  int  width,  int  height, int srcStride32,
     void* dst, int dstStride32, bell_func DmaCallback, int channel)
{ int i,srcrow,dstrow,asrc,adst,sz, *q;
    q=(int*)0x10000050;
    i=channel; // only to avoid warning
    if(matrixcnt==0)
    {  SetCallBack(0x58, (int)DmaCallback);
       SetCallBack(0x60, (int)DmaCallback);
       matrixcnt=1;
    }

   srcrow=srcStride32;   // ÀŞÈĞÍÁØÈ ÏŞÃËÅÏ
   dstrow=dstStride32; // ÀŞÈĞÍÁØÈ ÏŞÃËÅÏ	
   sz=width; // ÀŞÈĞÍÁØÈ ÏŞÃËÅÏ
   asrc=(int)src;  adst=(int)dst;
   zzz[3]=height;
   zzz[2]=(int)dst;
   zzz[1]=(int)src;
   zzz[0]=(int)width;
   zzz[4]=srcStride32;
   zzz[5]=dstStride32;
   statusFunc=halDmaStart( zzz);
   return 0;
} //  halInitMatrixDMA

