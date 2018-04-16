/*
#include "string.h"
#include "nmpps.h"

extern "C"{
	int vectSingleCopy(void* src, void* dst, int size){
		nmppsCopy_32s((int*)src,(int*)dst,size);
		return 0;
	}

	int vectDoubleCopy(void* src0,void* src1, void* dst0, void* dst1, int size0,int size1){
		nmppsCopy_32s((int*)src0,(int*)dst0,size);
		nmppsCopy_32s((int*)src1,(int*)dst1,size);
		memcpy(dst1,src1,size1);
		return 0;
	}
};*/