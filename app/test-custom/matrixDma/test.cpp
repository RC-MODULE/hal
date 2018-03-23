#include "time.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"

#define Heap_size 10000

#pragma data_section ".ddr"
int arr_ref[Heap_size];

int index = 0;
int callback(){
	index++;
	halLed(index);
	return 0;
}

struct int_int
{
	int index;
	int flag;	
};

int_int find_mismatch(nm32s* ref, nm32s* arr, int amm){
	int_int out;
	out.flag = 0;
	for(int i = 0; i < amm; i++){
		if(ref[i] != arr[i]){
			out.flag = 1;
			out.index = i;
		}
	}
	return out;
}

nm32s* AlignAddr(nm32s* addr){
	int temp = (int)addr;
	temp+=15;
	temp>>=4;
	temp<<=4;
	addr = (nm32s*)temp;
	return addr;
}

nm32s* UnalignAddr(nm32s* addr){
	int temp = (int)AlignAddr(addr);
	temp+=2;
	addr = (nm32s*)temp;
	return addr;
}

void InitArr(nm32s* arr, int amm){
	for(int i =0; i<amm; i++){
		arr[i] = i;
	}
};
void SetArr(nm32s* arr,int amm,int const2wrt){
	for(int i =0; i<amm; i++){
		arr[i] = const2wrt;
	}
};

int  ref_matrix_dma(int src,  int  width_,int  height, int srcStride32_,  int dst, int dstStride32_){
		int i,j;
		//align
		int width = width_ >> 1;
		int srcStride32 = srcStride32_ >> 1;
		int dstStride32 = dstStride32_ >> 1;
		src >>= 1;
		dst >>= 1;
		src <<= 1;
		dst <<= 1;
		long long* ptr2src = (long long*)src;
		long long* ptr2dst = (long long*)dst;
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				ptr2dst[j] = ptr2src[j];
			}
			ptr2dst+=dstStride32;
			ptr2src+=srcStride32;
		}
 		return 0;
 	};

struct params_of_run_2save
{
	nm32s* src;
	nm32s* dst;
	int width;
	int height;		
};

int main(){ 
	int call_counter = 0;
	halEnbExtInt();
	halInitDMA();
	halMaskIntContMdma_mc12101();
	halSetCallbackDMA((DmaCallback)callback);
	int count = 0;
	params_of_run_2save output;	
	for (int srcBankIndx = 0; srcBankIndx < 4; srcBankIndx++) {
		for (int dstBankIndx = 0; dstBankIndx < 4; dstBankIndx++) {
			printf("STEP %d\n",count++);
			nmppsMallocSetRoute16((0xF00) | (dstBankIndx << 4) | srcBankIndx);
			nm32s* src = nmppsMalloc_32s(Heap_size);
			nm32s* dst = nmppsMalloc_32s(Heap_size);
			printf("src = %x dst = %x\n",src,dst);
			if (src == 0 || dst == 0){
				printf("ERROR : heap had not been created\n");
				return -1;
			}
			InitArr(src,Heap_size);
			//halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
			for(int offset = 0; offset < 32; offset+=2){
				printf("Current offset %d\n",offset);
				for(int height = 0; height < 100; height += 20){
					for(int width = 0; width < 100; width += 2){
						SetArr(dst,Heap_size,0xcccccccc);
						SetArr(arr_ref,Heap_size,0xcccccccc);
						int test_param_flag = halCheckParamMatrixDMA(src + offset,width,height,width,dst,width);
						if(test_param_flag == 0){
							halInitMatrixDMA(src + offset,width,height,width,dst,width);
							call_counter++;
						}else{
							printf("wrong parametrs were detected \n src = 0x%x width = %d height = %d \n src_stride32 = %d dst = 0x%x dst_stride32 = %d\n",(src + offset),width,height,width,dst,width);
						}
						int count = 0;
						while(halStatusDMA()){
							if(count++ > 2000){
								printf("ERROR : halStatusDMA does not work or time of coping more then expected\n");
								printf("case srs = 0x%x dst = 0x%x width = %d height = %d\n",src,dst,width,height);	
								return 3;
							}
						};
						if(test_param_flag == 0){
							ref_matrix_dma((int)src + offset,width,height,width,(int)arr_ref,width);
						}
						unsigned Crc_dst  	 = nmppsCrc_32s(dst, Heap_size);
						unsigned Crc_arr_ref = nmppsCrc_32s(arr_ref,Heap_size);
						if(Crc_dst != Crc_arr_ref){
							printf("ERROR: crc mismatch \n");
							printf("case srs = %x dst = %x width = %d height = %d\n",src,dst,width,height);	
							halLed(9);
							output.src    = src;
							output.dst    = dst;
							output.width  = width;
							output.height = height;
							goto PRINT;
							//return 9;
						}
					}
				}
			}	

			nmppsFree(src);
			nmppsFree(dst);
		}
	}
	if(call_counter != index){
		printf("ERROR : callback had not been called\n");
	}
	halLed(0xaa);
	return 777;		
PRINT:
	int_int to_print = find_mismatch(arr_ref,output.dst,output.width*output.height);	
	printf("mismatch at [%d]\n",to_print.index);
	
	printf("\n");
	printf("src");
	for(int i = 0,stride = 0; i < output.height; i++,stride+=output.width){
		printf("\n");
		for(int j = 0,pntr = stride; j < output.width; j++,pntr++){
			printf("%d ",output.src[pntr]);
		}
	}
	printf("\n");
	printf("dst mine");
	for(int i = 0,stride = 0; i < output.height; i++,stride+=output.width){
		printf("\n");
		for(int j = 0,pntr = stride; j < output.width; j++,pntr++){
			printf("%d ",output.dst[pntr]);
		}
	}
	printf("\n");
	printf("dst ref");
	for(int i = 0,stride = 0; i < output.height; i++,stride+=output.width){
		printf("\n");
		for(int j = 0,pntr = stride; j < output.width; j++,pntr++){
			printf("%d ",arr_ref[pntr]);
		}
	}
	halLed(0x9);
	return 9;
}
