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
 	
int user_callback(){
 		status = 1;
 		halLed(0xaa);
 		return 0;
 } 

int main(){
	halEnbExtInt();
	halMaskIntContMdma_mc12101();
	halInitDMA();
	halSetCallbackDMA((DmaCallback)user_callback);
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
	printf("DMA has finished correctly\n");
	printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",WIDTH*HEIGHT,(t1-t0));
	
	//aligning addresses
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
	printf("DMA has finished correctly\n");
	printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",WIDTH*HEIGHT,(t1-t0));
	
	return 0;
}