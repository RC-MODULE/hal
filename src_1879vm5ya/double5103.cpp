#include "dma5103.h"
extern "C"
{
    //cnt_func statusFunc;
 // typedef int(*DmaCallback)();
  void SetCallBack( int vectoraddr, int ProcAddress);
  //cnt_func halDmaStart( int *q );
  //int dmainit( int dmahigh, int dst, int src, int size32);

extern int ofdmainit;
extern int chain[8];
extern int nonfirst;

void doubleCBF()
{ if (nonfirst<2)
  { halDmaStart( chain+5 );
    nonfirst=2;
  }
}
int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1,
     int intSize0, int intSize1)
{ int k;
   if (nonfirst==0)
   {  SetCallBack(0x58, (int)doubleCBF);
      SetCallBack(0x60, (int)doubleCBF);
   }
   nonfirst=1;
   k=(int)chain;
   chain[0]=intSize0;  chain[1]=(int)src0; chain[2]=(int)dst0; chain[3]=0;   chain[4]=k+5;
   chain[5]=intSize1;  chain[6]=(int)src1; chain[7]=(int)dst1;  chain[8]=0;  chain[9]=0;
   statusFunc=halDmaStart( chain );
   return 0;
} // halInitDoubleDMA


};