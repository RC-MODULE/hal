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
int zzzMatrix[6];
extern int matrixcnt;
void BellFromDmaMatrix()// ���������� DMA ���������� (������� 58h,60h)
{  if (zzzMatrix[3])
   { zzzMatrix[3]--;
     zzzMatrix[1]+=zzzMatrix[4];
     zzzMatrix[2]+=zzzMatrix[5];
     halDmaStart( zzzMatrix);
   }
}  // BellFromDmaMatrix

int  halInitMatrixDMA(void*  src,  int  width,  int  height, int srcStride32,
     void* dst, int dstStride32, bell_func DmaCallback, int channel)
{ int i,srcrow,dstrow,asrc,adst,sz, *q;
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
