#include "chain7707.h"

extern unsigned int first7707;
extern chain7707 chain;
int add2Chain(int dst,int src,int size);

extern "C" {

    int xdmacChain( int* TBL );

    int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1)
    {
         int *q,k; 
        add2Chain(((int)dst0)<<2,((int)src0)<<2,intSize0<<2);
        add2Chain(((int)dst1)<<2,((int)src1)<<2, intSize1<<2);
        q=chain.root;
        k=xdmacChain( q );
        return k;
    }

    int  halInitMatrixDMA(void*  src,  int  width,  int  height, int srcStride32,  void* dst, int dstStride32, void *DmaCallback, int channel)
    {
        int *q,k,i,srcrow,dstrow,asrc,adst,sz;
        srcrow=srcStride32<<2;   // байтовый размер
        dstrow=dstStride32<<2; // байтовый размер	
        sz=width<<2; // байтовый размер
        asrc=(int)src;  adst=(int)dst;	
         for (i=0; i<height;i++) 
        {  
            add2Chain(adst,asrc,sz);
            asrc+=srcrow;  adst+=dstrow;
        }
        q=chain.root;
        k=xdmacChain( q );
        return k;
    }	

    int  halInitPacketDMA(void** src,  void** dst,  int* size32)
        { 
            int *q,k,n,*asrc,*adst,*sz;
            asrc=(int*)src;  adst=(int*)dst;	sz=size32;
            for (;;) {  
                n=*sz++;
                if (n) add2Chain((*adst++)<<2,(*asrc++)<<2,n<<2); else break;
            }
            q=chain.root;
            k=xdmacChain( q );
           return k;
    }	

};