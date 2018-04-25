#include "chain7707.h"
#ifndef xdmac7707tools
#define xdmac7707tools
extern SpecDMA7707 chain_7707;
extern int chainBuf[];
extern "C"
{
  typedef int(*DmaCallback)();
  int xdmacChain( int* TBL );
  int xdmacChainN( int* TBL, int channel );
  int xdmacinitn( int dst, int src, int size32, int channel);	
  int xdmacinit( int dst, int src, int size32);
  int xdmac0();
}

int add2Chain(int dst,int src,int size);
SpecDMA7707* halInitDMA7707 (void** src, void** dst,  int* size32, void* specBuffer)
{   int *q,n,*asrc,*adst,*sz;
      openDma(specBuffer);
      asrc=(int*)src;  adst=(int*)dst;	sz=size32;
  for (;;) 
  {  n=*sz++;
     if (n) add2Chain((*adst++)<<2,(*asrc++)<<2,n<<2); else break;
  }
   q=chain_7707.root;
   return (SpecDMA7707*)chain_7707.root;
}

int halInitSingleDMA(void*  src,  void*  dst,  int  size32,  DmaCallback* func, int channel=0)
{  
	return xdmacinitn(((int)dst)<<2, ((int)src)<<2, size32<<2,channel);
} 
int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1)
{ int *q,k; 
   add2Chain(((int)dst0)<<2,((int)src0)<<2,intSize0<<2);
   add2Chain(((int)dst1)<<2,((int)src1)<<2, intSize1<<2);
   q=chain_7707.root;
   k=xdmacChain( q );
   return k;
}
int  halInitMatrixDMA(void*  src,  int  width,  int  height, int srcStride32,  void* dst, int dstStride32, void *DmaCallback, int channel)
{ int *q,k,i,srcrow,dstrow,asrc,adst,sz;
   srcrow=srcStride32<<2;   // байтовый размер
   dstrow=dstStride32<<2; // байтовый размер	
   sz=width<<2; // байтовый размер
   asrc=(int)src;  adst=(int)dst;	
  for (i=0; i<height;i++) 
  {  add2Chain(adst,asrc,sz);
      asrc+=srcrow;  adst+=dstrow;
  }
   q=chain_7707.root;
   k=xdmacChainN( q, channel);
   return k;
}	
int  halInitPacketDMA(void** src,  void** dst,  int* size32)
{ int *q,k,n,*asrc,*adst,*sz;
   q=(int*) halInitDMA7707 (src, dst, size32,  chainBuf);
   asrc=(int*)src;  adst=(int*)dst;	sz=size32;
  for (;;) 
  {  n=*sz++;
     if (n) add2Chain((*adst++)<<2,(*asrc++)<<2,n<<2); else break;
  }
   q=chain_7707.root;
   k=xdmacChain( q );
   return k;
}
int halStatusDma()
{  return xdmac0();
}
#endif