	void halInitDoubleDMA(int*  src0, int*  src1, int* dst0,  int* dst1, int intSize0, int intSize1){
 		int i;
 		for(i=0;i<intSize0;i++){
 			dst0[i] = src0[i];
 		}
 		for(i=0;i<intSize1;i++){
 			dst1[i] = src1[i];
 		}
 	};

 	int  halInitMatrixDMA(int src,  int  width,int  height, int srcStride32,  int dst, int dstStride32){
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
		return 0;
 	};

 	void halInitPacketDMA(void** src_arr, void** dst_arr, int* size_arr){
		int i=0;
		int src,dst;
		int* ptr_src;
		int* ptr_dst;

		while(1){
			int j;
			//printf("index = %d size = %d\n",i,size_arr[i]);
			if(size_arr[i] == 0){
				break;
			}
			
			src = (int)*(src_arr + i); 
			dst = (int)*(dst_arr + i);
			ptr_src = (int*)src;
			ptr_dst = (int*)dst;
			for(j=0;j<size_arr[i];j++){
				ptr_dst[j] = ptr_src[j]; 
			}
			i++;
		}
	}

void halInitSingleDMA(int  src,  int  dst,  int  size32){
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

// 	void halSetCallbackDMA(DmaCallback user_callback){
//
//	};

	void halInitDMA(){};
	int  halStatusDMA(){return 0;};
	void halEnbExtInt(){};
	void halDisExtInt(){};
	void halMaskIntContMdma_mc12101(){};


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
};

int halTestParamPacketDMA(void** src,  void** dst,  int* size32){
	int pntr = 0;
	int error_code = 0;
	int test;
	while(size32[pntr]){
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
		pntr++;
	}
	if(error_code == 0){
		pntr = 0;
	}
	return error_code | (pntr << 4);
};




