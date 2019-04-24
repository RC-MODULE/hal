#include "section-hal.h"

#ifdef __cplusplus
		extern "C" {
#endif


INSECTION(".text_hal") int halCheckParamsSingleDMA(void* src, void* dst, int size){
	int temp = (int)src;
	if(temp << 31){
		return 1;
	}
	temp = (int)dst;
	if(temp << 31){
		return 2;
	}
	if(size << 31){
		return 3;
	}
	return 0;
};

INSECTION(".text_hal") int halCheckParamsDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1){
	int temp = (int)src0;
	if(temp << 31){
		return 1;
	}
	temp = (int)src1;
	if(temp << 31){
		return 2;
	}
	temp = (int)dst0;
	if(temp << 31){
		return 3;
	}
	temp = (int)dst1;
	if(temp << 31){
		return 4;
	}
	if(intSize0 << 31){
		return 5;
	}
	if(intSize1 << 31){
		return 6;
	}
	return 0;
};

INSECTION(".text_hal") int halCheckParamsMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32){
	int temp = (int)src;
	if(temp << 31){
		return 1;
	}
	temp = (int)dst;
	if(temp << 31){
		return 4;
	}
	if(width << 31){
		return 2;
	}
	if(srcStride32 << 31){
		return 3;
	}
	if(dstStride32 << 31){
		return 5;
	}
	if(width > srcStride32){
		return 6;
	}
	if(width > dstStride32){
		return 7;
	}
	return 0;
}

INSECTION(".text_hal") int halCheckParamsStausMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32){
	int temp = (int)src << 28;
	if(temp){
		return 1;
	}
	temp = (int)width << 28;
	if(temp){
		return 2;
	}
	temp = (int)srcStride32 << 28;
	if(temp){
		return 3;
	}
	temp = (int)dst << 28;
	if(temp){
		return 4;
	}
	temp = (int)dstStride32 << 28;
	if(temp){
		return 5;
	}
	if(width > srcStride32){
		return 6;
	}
	if(width > dstStride32){
		return 7;
	}
	return 0;
}

INSECTION(".text_hal") int halCheckParamsPacketDMA(void** src,  void** dst,  int* size32, int amm){
	int pntr = 0;
	int error_code = 0;
	int test;
	for(pntr = 0; pntr < amm; pntr++){
		test = (int)src[pntr] << 31;//test src is even or not
		if(test){
			error_code = 1;
			break;
		}
		test = (int)dst[pntr] << 31;//test dst is even or not
		if(test){
			error_code = 2;
			break;
		}
		test = (int)size32[pntr] << 31;//test size32 is even or not
		if(test){
			error_code = 4;
			break;
		}
	}
	if(error_code == 0){
		pntr = 0;
	}
	return error_code | (pntr << 16);
}

#ifdef __cplusplus
		};
#endif
