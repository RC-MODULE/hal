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
	int arr2read[SIZE_MEM];
#pragma data_section ".data_shared"
	int buff[SIZE_MEM+16]; 	
 	int buff_ref[SIZE_MEM+16];
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
	//set up a mask of interraption from MDMA enable
	int i;
	clock_t t0,t1;
	for(i=0;i<SIZE_MEM;i++){
		arr2read[i] = i;
		buff[i] = 0;
		buff_ref[i] = 0;
	}
	for(i=SIZE_MEM;i<SIZE_MEM+16;i++){
		buff[i] = 0;
		buff_ref[i] = 0;	
	}
	printf("SRC: %x DST: %x\n",arr2read,buff);
	printf("Case unaligned \n");

	status = 0;
	t0=clock();
	
	halInitMatrixDMA((arr2read + MEM_OFFSET),WIDTH,HEIGHT,STRIDE,buff,STRIDE);
	
	while(1){
		if(status){
			t1 = clock();
			break;
		}
	}
	
	printf("DMA has finished correctly\n");
	printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",WIDTH*HEIGHT,(t1-t0));
	
	int* pntr2mem;
	int temp = (int)(buff+15)>>4;
	pntr2mem = (int*)(temp<<=4);

	printf("\n");
	printf("SRC: %x DST: %x\n",arr2read,pntr2mem);
	printf("Case aligned \n");
	status = 0;
	
	t0=clock();
	halInitMatrixDMA((arr2read + MEM_OFFSET),WIDTH,HEIGHT,STRIDE,pntr2mem,STRIDE);
	
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