#include <stdio.h>
#include <time.h>
#include <led.h>
#include <sleep.h>
#include "hal.h"
#define IASH        0x40000458
#define IASH_CLR    0x4000045C
#define IRRH        0x40000440
#define IMRH        0x40000448
#define MEM_OFFSET  0x40000
#define SIZE_DDR    100000
#define CHAIN_LENGTH 3

#pragma data_section ".data_imu1"
	int arr2read[32000];
#pragma data_section ".data_imu2"
	int arr2wrt[32000];
#pragma data_section ".data_shared"
	int buff[SIZE_DDR];
	int buff_ref[SIZE_DDR];
	int srs_arr[CHAIN_LENGTH];
	int dst_arr[CHAIN_LENGTH];
	int size32_arr[CHAIN_LENGTH];

	void ref_pack_DMA(void** src_arr, void** dst_arr, int* size_arr){
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

int user_callback(){
	halLed(0xaa);

}

int main(){
	int* pntr2ic;
	pntr2ic = (int*)IMRH;
	*pntr2ic = 0x1;
	halInitDMA();
	halSetCallbackDMA(user_callback);
	int src_arr[CHAIN_LENGTH+10];
	int dst_arr[CHAIN_LENGTH+10];
	int size_arr[CHAIN_LENGTH+10];
	int i;
	int offset;
	printf("from main user call back =  %x\n",user_callback);

	for(i=0;i<32000;i++){
		arr2read[i] = i;
	}

	for(i=0;i<SIZE_DDR;i++){
		buff[i] = 0;
		buff_ref[i] = 0;
	}

	for(i=0,offset=0;i<CHAIN_LENGTH;i++){
		int temp = (int)arr2read + MEM_OFFSET + offset;
		src_arr[i] = temp;
		offset+=16;
		printf("address of arr2read[%d] = %x contents = %x \n",i,src_arr[i],&srs_arr[i]);
	}
	for(i=0,offset=0;i<CHAIN_LENGTH;i++){
		int temp = (int)buff + offset;
		dst_arr[i] = temp;
		offset+=16;
		printf("address of buff[%d] = %x contents = %x \n",i,dst_arr[i],&dst_arr[i]);
	}
	for(i=0;i<CHAIN_LENGTH;i++){
		size_arr[i] = 16;
	}
	size_arr[CHAIN_LENGTH] = 0; 
	src_arr[CHAIN_LENGTH]  = 0;
	dst_arr[CHAIN_LENGTH]  = 0;
	halInitPacketDMA((void**)src_arr,(void**)dst_arr,(int*)size_arr);
	//ref_pack_DMA((void**)src_arr,(void**)dst_arr,(int*)size_arr);
	halSleep(100);
	for(i=0;i<100;i++){
		printf("out[%d] = %d\n",i,buff[i]);
	}
	return 100;
}
