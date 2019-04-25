#include "dma5103.h"
#include "section-hal.h"

#ifdef __cplusplus
		extern "C" {
#endif
    //cnt_func statusFunc;
 // typedef int(*DmaCallback)();
  void SetCallBack( int vectoraddr, int ProcAddress);
  //cnt_func halDmaStart( int *q );
  //int dmainit( int dmahigh, int dst, int src, int size32);

extern int ofdmainit;
extern int chain[8];
extern int nonfirst;


INSECTION(".text_hal") void doubleCBF()
{ if (nonfirst<2)
  { halDmaStart( chain+5 );
    nonfirst=2;
  }
}

INSECTION(".text_hal") int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1,
     int intSize0, int intSize1)
{ int k;

	if (((int)src0 ^ (int)dst0)<0) return -2;
	if (((int)src1 ^ (int)dst1)<0) return -3;
	
   if (nonfirst==0)
   {  SetCallBack(0x58, (int)doubleCBF);
      SetCallBack(0x60, (int)doubleCBF);
      chain[3]=0;      chain[8]=0;
   }
   nonfirst=1;
   k=(int)chain;
   chain[0]=intSize0;  chain[1]=(int)src0; chain[2]=(int)dst0;    chain[4]=k+5;
   chain[5]=intSize1;  chain[6]=(int)src1; chain[7]=(int)dst1;    chain[9]=0;
   statusFunc=halDmaStart( chain );
   return 0;
} // halInitDoubleDMA


#ifdef __cplusplus
		};
#endif
