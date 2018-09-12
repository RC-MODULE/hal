typedef int(*DmaCallback)();

int dummy_function(){
	return 0;
}
static DmaCallback user_callback = dummy_function;

int halStatusDMA(){
	return 0;
}

void halOpenDMA(){

}

void halOpenStatusDMA(){
	
}

void halSetCallbackDMA(DmaCallback funct){
	if(funct == 0){
		user_callback = dummy_function;
	}else{
		user_callback = funct;
	}
}

	void halInitDoubleDMA(void*  src0, void*  src1, void* dst0,  void* dst1, int intSize0, int intSize1){
 		int i;
 		int* ptr_src0 = (int*)src0;
 		int* ptr_src1 = (int*)src1;
 		int* ptr_dst0 = (int*)dst0;
 		int* ptr_dst1 = (int*)dst1;
 		for(i=0;i<intSize0;i++){
 			ptr_dst0[i] = ptr_src0[i];
 		}
 		for(i=0;i<intSize1;i++){
 			ptr_dst1[i] = ptr_src1[i];
 		}
 		user_callback();
 	};

 	int  halInitMatrixDMA(void* src,  int  width,int  height, int srcStride32, void* dst, int dstStride32){
		int i,j;
		int* ptr2src = (int*)src;
		int* ptr2dst = (int*)dst;
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				ptr2dst[j] = ptr2src[j];
			}
			ptr2dst+=dstStride32;
			ptr2src+=srcStride32;
		}
		user_callback();
		return 0;
 	};

 	void halInitPacketDMA(void** src_arr, void** dst_arr, int* size_arr, int amm){
		int i=0;
		int src,dst;
		int* ptr_src;
		int* ptr_dst;
	
		for(i = 0; i < amm; i++){
			int j;
			src = (int)*(src_arr + i); 
			dst = (int)*(dst_arr + i);
			ptr_src = (int*)src;
			ptr_dst = (int*)dst;
			for(j=0;j<size_arr[i];j++){
				ptr_dst[j] = ptr_src[j]; 
			}
		}
		user_callback();
	}

void halInitSingleDMA(void* src, void* dst,int size32){
 		int i;
		int* pntr_src;
		int* pntr_dst;

 		int amm = size32>>1;
 		amm<<=1;
 		pntr_src  = (int*)src;
 		pntr_dst  = (int*)dst;
 		for(i=0;i<amm;i++){
 			pntr_dst[i] = pntr_src[i];
 		}
 		user_callback();
 	}

void halInitStatusSingleDMA(void* src,void* dst,int size32){
 		int i;
		int* pntr_src;
		int* pntr_dst;

 		int amm = size32>>1;
 		amm<<=1;
 		pntr_src  = (int*)src;
 		pntr_dst  = (int*)dst;
 		for(i=0;i<amm;i++){
 			pntr_dst[i] = pntr_src[i];
 		}
 	}

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

int halTestParamDoubleDMA(void*  src0, void* src1, void* dst0, void* dst1, int intSize0, int intSize1){
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

int halTestParamMatrixDMA(void* src, int width, int height, int srcStride32, void* dst, int dstStride32){
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
};

int halTestParamPacketDMA(void** src, void** dst, int* size32, int amm){
	int error_code = 0;
	int test;
	int ptr;
	for(ptr = 0; ptr < amm; ptr++){
		test = (int)src[ptr] << 31;//test src is even or not
		if(test){
			error_code = 1;
			break;
		}
		test = (int)dst[ptr] << 31;//test dst is even or not
		if(test){
			error_code = 2;
			break;
		}
		test = (int)size32[ptr] << 31;//test size32 is even or not
		if(test){
			error_code = 4;
			break;
		}
	}
	if(error_code == 0){
		ptr = 0;
	}
	return error_code | (ptr << 4);
};
