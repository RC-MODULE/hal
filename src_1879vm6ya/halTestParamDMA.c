extern "C"{


int halTestParamSingleDMA(void* src, void* dst, int size){
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

int halTestParamDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1){
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

int halTestParamMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32){
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
	return 0;
}

int halTestParamPacketDMA(** src,  void** dst,  int* size32){
	
}

};