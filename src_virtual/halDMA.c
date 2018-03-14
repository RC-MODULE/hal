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
 	};

 	void halInitPacketDMA(void** src_arr, void** dst_arr, int* size_arr){
		int i=0;
		while(1){
			int j;
			//printf("index = %d size = %d\n",i,size_arr[i]);
			if(size_arr[i] == 0){
				break;
			}
			int src,dst;
			src = (int)*(src_arr + i); 
			dst = (int)*(dst_arr + i);
			int* ptr_src = (int*)src;
			int* ptr_dst = (int*)dst;
			for(j=0;j<size_arr[i];j++){
				ptr_dst[j] = ptr_src[j]; 
			}
			i++;
		}
	}

void halInitSingleDMA(int  src,  int  dst,  int  size32){
 		int i;
 		int amm = size32>>1;
 		amm<<=1;
 		int* pntr_src  = (int*)src;
 		int* pntr_dst  = (int*)dst;
 		for(i=0;i<amm;i++){
 			pntr_dst[i] = pntr_src[i];
 		}
 	}

// 	void halSetCallbackDMA(DmaCallback user_callback){
//
 //	};

	void halInitDMA(){

	};
	
	int  halStatusDMA(){
		return 0;
	};
