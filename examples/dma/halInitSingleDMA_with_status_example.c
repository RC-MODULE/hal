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

#pragma data_section ".data_imu1"
	int arr2read[SIZE_MEM];
#pragma data_section ".data_shared"
	int buff[SIZE_MEM+16]; 	
 	int buff_ref[SIZE_MEM+16];
 	int status;
 	
 	void ref_DMA(int  src,  int  dst,  int  size32){
 		int i;
 		int amm = size32>>1;
 		amm<<=1;
 		int* pntr_src  = (int*)src;
 		int* pntr_dst  = (int*)dst;
 		for(i=0;i<amm;i++){
 			pntr_dst[i] = pntr_src[i];
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
 		halLed(0xaa);
 		return 0;
 } 


int main(){
	//set up a mask of interraption from MDMA enable
	*((int*)IMRH) = 0x1;
	int i;
	clock_t t0,t1;
	for(i=0;i<SIZE_MEM;i++){
		arr2read[i] = i;
		buff[i] = 0;
		buff_ref[i] = 0;
	}
	halInitDMA();
	halSetCallbackDMA((DmaCallback)user_callback);

	printf("SRC: %x DST: %x\n",arr2read,buff);
	status = 0;
	t0=clock();
	halInitSingleDMA((arr2read + MEM_OFFSET),buff,SIZE_MEM);

	int dummy =0;
	while(halStatusDMA()){
		dummy++;
		printf("dummy = %d\n",dummy);
	}

	ref_DMA((int)(arr2read + MEM_OFFSET),(int)buff_ref,SIZE_MEM);
	int index = cmp(SIZE_MEM,buff,buff_ref);
	if(index){
		printf("ERROR: mismatch btw ref function and true DMA at %d\n",index);
	}else{
		printf("DMA has finished correctly\n");
		printf("ammount of loops %d \n",dummy);
	}
	
	for(i=0;i<10;i++){
		printf("buff[%d] = %d\n",i,buff[i]);
	}

//case aligned address
	printf("CASE address ALIGNED as 4 the LSBs are 0\n");
	int* pntr2mem;
	int aligned = (int)(int(buff + 15) >> 4);
	aligned<<=4;
	pntr2mem = (int*)aligned; 	
	printf("SRC: %x DST: %x\n",arr2read,pntr2mem);
	status = 0;
	t0=clock();
	halInitSingleDMA((arr2read + MEM_OFFSET),pntr2mem,SIZE_MEM);

	dummy =0;
	while(halStatusDMA()){
		dummy++;
		printf("dummy = %d\n",dummy);
	}

	ref_DMA((int)(arr2read + MEM_OFFSET),(int)buff_ref,SIZE_MEM);
	index = cmp(SIZE_MEM,pntr2mem,buff_ref);
	if(index){
		printf("ERROR: mismatch btw ref function and true DMA at %d\n",index);
	}else{
		printf("DMA has finished correctly\n");
		printf("ammount of loops %d \n",dummy);
	}
	
	for(i=0;i<10;i++){
		printf("buff[%d] = %d\n",i,buff[i]);
	}
	return 0;
}