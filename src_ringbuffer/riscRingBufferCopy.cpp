#include "string.h"

extern "C"{
	int riscSingleCopy(void* src, void* dst, int size){
#ifdef __NM__
		memcpy(dst,src,size);
#else 
		memcpy(dst, src, size*4);
#endif

		return 0;
	}

int riscDoubleCopy(void* src0,void* src1, void* dst0, void* dst1, int size0,int size1){
#ifdef __NM__
		memcpy(dst0,src0,size0);
		memcpy(dst1,src1,size1);
#else 
		memcpy(dst0, src0, size0*4);
		memcpy(dst1, src1, size1*4);
#endif
		return 0;
	}
};