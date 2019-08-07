#include "dma.h"
#include "chain7707.h"
#ifndef xdmac7707tools
#define xdmac7707tools
#include "section-hal.h"
extern SpecDMA7707 chain_7707;

#ifdef __cplusplus
		extern "C" {
#endif


	extern int chainBuf[];
	typedef int(*DmaCallback)();
	int xdmacChain( int* TBL );
	int xdmacChainN( int* TBL, int channel );
	int xdmacinitn( int dst, int src, int size32, int channel);	
	int xdmacinit( int dst, int src, int size32);
	int xdmac0();


	INSECTION(".text_hal") int halOpenDMA(){
		return 0;
	}

	INSECTION(".text_hal") void halSetCallbackDMA(DmaCallback user_callback){
	
	}
	
	
//int add2Chain(int dst,int src,int size);
INSECTION(".text_hal") SpecDMA7707* halInitDMA7707 (void** src, void** dst,  int* size32, void* specBuffer)
{   
	int *q,n,*asrc,*adst,*sz;
	openDma(specBuffer);
	asrc=(int*)src;  adst=(int*)dst;	sz=size32;
	for (;;) 
	{  
		n=*sz++;
		if (n) 
			add2Chain((*adst++)<<2,(*asrc++)<<2,n<<2); 
		else break;
	}
	q=chain_7707.root;
	return (SpecDMA7707*)chain_7707.root;
}

/*
int halInitSingleDMA(void*  src,  void*  dst,  int  size32,  DmaCallback* func, int channel=0)
{  
	return xdmacinitn(((int)dst)<<2, ((int)src)<<2, size32<<2,channel);
} 
*/
INSECTION(".text_hal") int halInitSingleDMA(const void*  src,  void*  dst,  int  size32)
{  
	int channel=0;
	xdmacinitn(((int)dst)<<2, ((int)src)<<2, size32<<2,channel);
	return 0; 
} 

INSECTION(".text_hal") int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1)
{ 
	int *q,k; 
	add2Chain(((int)dst0)<<2,((int)src0)<<2,intSize0<<2);
	add2Chain(((int)dst1)<<2,((int)src1)<<2, intSize1<<2);
	q=chain_7707.root;
	k=xdmacChain( q );
	return 0;
}

INSECTION(".text_hal") int  halInitMatrixDMA(void*  src,  int  width,  int  height, int srcStride32,  void* dst, int dstStride32 )
{ 
	int channel = 0;
	int *q,k,i,srcrow,dstrow,asrc,adst,sz;
	srcrow=srcStride32<<2;   // байтовый размер
	dstrow=dstStride32<<2; // байтовый размер	
	sz=width<<2; // байтовый размер
	asrc=(int)src;  adst=(int)dst;	
	if (height>MAX_CHAIN_BUF_SIZE)
	for (i=0; i<height;i++) 
	{  
		add2Chain(adst,asrc,sz);
		asrc+=srcrow;  adst+=dstrow;
	}
	q=chain_7707.root;
	k=xdmacChainN( q, channel);
	return 0;
}	

INSECTION(".text_hal") int  halInitPacketDMA(void** src,  void** dst,  int* size32, int packets)
{ 
	int *q,k,n,*asrc,*adst,*sz;
	q=(int*) halInitDMA7707 (src, dst, size32,  chainBuf);
	asrc=(int*)src;  adst=(int*)dst;	sz=size32;
	for (int i=0; i<packets; i++)
	{  
		n=*sz++;
		//if (n) 
			add2Chain((*adst++)<<2,(*asrc++)<<2,n<<2); 
	//	else 
		//	break;
	}
	q=chain_7707.root;
	k=xdmacChain( q );
	return 0;
}

INSECTION(".text_hal") int halStatusDMA()
{  
	return xdmac0();
}

#ifdef __cplusplus
		};
#endif


#endif