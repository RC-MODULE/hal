#include <string.h>
void dma_init(int N, size_t src, size_t dst)
{	
	memcpy((void*)dst,(void*)src,N*8);
	//for(i = 0; i < N; i++) {
	//	dst[i] = srs[i];
	//}
}

void dma_bias_init(int n, size_t src, size_t dstq, int row2read, int bias2read, int row2wrt, int bias2wrt)
{
	long long* pSrc=(long long*)src;
	long long* pDst=(long long*)dstq;
	while (n>=row2read) {
		memcpy(pDst,pSrc,row2read*8);
		pSrc+=bias2read/2;
		pDst+=row2read;
		n-=row2read;
	}
	memcpy(pDst,pSrc,n*8);
	
}

int dma_status() 
{
	return 2;
}