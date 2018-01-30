#include <stdio.h>
#include <hal.h>
#include <sleep.h>
#include "time.h"
#define SIZE_MEM   32000
#define MEM_OFFSET 0x40000
#define IASH       0x40000458
#define IASH_CLR   0x4000045C
#define IRRH       0x40000440
#define IMRH       0x40000448
#define WIDTH 		512
#define HEIGHT 		60
#define STRIDE 		WIDTH

#pragma data_section ".data_imu1"
	int arr2read_0[32000];
#pragma data_section ".data_imu2"
	int arr2read_1[32000];
#pragma data_section ".data_shared"
	int buff_0[SIZE_MEM+16]; 	
	int buff_1[SIZE_MEM+16]; 	
 	int buff_ref_0[SIZE_MEM+16];
 	int buff_ref_1[SIZE_MEM+16];
 	int status;
 	
 void ref_double_DMA(int*  src0, int*  src1, int* dst0,  int* dst1, int intSize0, int intSize1){
 		int i;
 		for(i=0;i<intSize0;i++){
 			dst0[i] = src0[i];
 		}
 		for(i=0;i<intSize1;i++){
 			dst1[i] = src1[i];
 		}
 	}
 	int cmp(int amm, int* arr_1, int* arr_2){
 		int i;
 		for(i=0;i<amm;i++){
 			if(arr_1[i] != arr_2[i]){
 				return i + 1;
 			}
 		}
 		return 0;
 	}


 int user_callback(){
 		status = 1;
 		return 0;
 } 


int main(){
	//set up a mask of interraption from MDMA enable
	*((int*)IMRH) = 0x1;
	int i;
	clock_t t0,t1;

	for(i=0;i<SIZE_MEM;i++){
		arr2read_0[i] = i;
		arr2read_1[i] = i;
		buff_0[i] = 0;
		buff_1[i] = 0;
		buff_ref_0[i] = 0;
		buff_ref_1[i] = 0;
	}
	for(i=SIZE_MEM;i<SIZE_MEM+16;i++){
		buff_0[i] = 0;
		buff_1[i] = 0;
		buff_ref_0[i] = 0;
		buff_ref_1[i] = 0;
	}
	halInitDMA();
	halSetCallbackDMA((DmaCallback)user_callback);

	printf("SRC_0: %x DST_0: %x\n",arr2read_0,buff_0);
	printf("SRC_1: %x DST_1: %x\n",arr2read_1,buff_1);
	printf("Case unaligned \n");
	

	status = 0;
	t0=clock();
	halInitDoubleDMA(arr2read_0 + MEM_OFFSET, arr2read_1 + MEM_OFFSET, buff_0, buff_1,32000,32000);
	while(1){
		if(status){
			t1 = clock();
			break;
		}
	}
	ref_double_DMA(arr2read_0 + MEM_OFFSET, arr2read_1 + MEM_OFFSET,buff_ref_0,buff_ref_1,32000,32000);
	int index = cmp(SIZE_MEM,buff_0,buff_ref_0);
	index = cmp(SIZE_MEM,buff_1,buff_ref_1);
	if(index){
		printf("ERROR: mismatch btw ref function and true DMA at %d\n",index);
	}else{
		printf("DMA has finished correctly\n");
		printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",WIDTH*HEIGHT,(t1-t0));
	}
	
	
	int temp = int(buff_0 + 15)>>4;
	temp<<=4;
	int* ptr2buff_0 = (int*)temp;
	temp = (int)(buff_1 + 15)>>4;
	temp<<=4;
	int* ptr2buff_1 = (int*)temp;	

	
	printf("\n");
	printf("Case aligned \n");
	printf("SRC_0: %x DST_0: %x\n",arr2read_0,ptr2buff_0);
	printf("SRC_1: %x DST_1: %x\n",arr2read_1,ptr2buff_1);
	

	status = 0;
	t0=clock();
	halInitDoubleDMA(arr2read_0 + MEM_OFFSET,arr2read_1 + MEM_OFFSET,ptr2buff_0,ptr2buff_1,32000,32000);
	while(1){
		if(status){
			t1 = clock();
			break;
		}
	}
	ref_double_DMA(arr2read_0 + MEM_OFFSET,arr2read_1 + MEM_OFFSET,ptr2buff_0,ptr2buff_1,32000,32000);
	index = cmp(SIZE_MEM,ptr2buff_0,buff_ref_0);
	index = cmp(SIZE_MEM,ptr2buff_1,buff_ref_1);
	if(index){
		printf("ERROR: mismatch btw ref function and true DMA at %d\n",index);
	}else{
		printf("DMA has finished correctly\n");
		printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",WIDTH*HEIGHT,(t1-t0));
	}

	return 0;
}