#include <stdio.h>
#include <hal.h>
#include "nmpp.h"

//this example shows how to use DoubleDma
#define SIZE 100
#pragma data_section ".data_imu0"
	int src_arr_1[SIZE];
	int dst_arr_2[SIZE]; 
#pragma data_section ".data_shared"	
	int src_arr_2[SIZE];
	int dst_arr_1[SIZE];

int user_callback(){
	halLed(0xaa);
	return 0;
}
int done = 1;
int user_callback_to_stop(){
	halLed(0xbb);
	done = 0;
	return 0;
}

int main(){
	int ret;
	int size0 = 128;
	int size1 = 16;
	
	int* src0 = nmppsMalloc_32s(size0);
	int* dst0 = nmppsMalloc_32s(size0);
	int* src1 = nmppsMalloc_32s(size1);
	int* dst1 = nmppsMalloc_32s(size1);
	
	return 777;
	for(int i = 0; i < size0; i++){
		src0[i] = i;
		dst0[i] = 0;
	}	
	for(int i = 0; i < size1; i++){
		src1[i] = 0x10000+i;
		dst1[i] = 0;
	}	
	
	halOpenDMA();//this function writes the interruption vector into interruption controller and initialise some variables needed to provide a functionnality of dma on both core 

	halSetCallbackDMA((DmaCallback)user_callback);//set callback function to be called after dma is done. 
	//Pay attention that once set up call back will call after dma has finished forever untill it flashed or changhed 
	// to flash the last callback function call halSetCallBack(0);

	
	
	//load the paramentrs into DMA
	//if (ret= halInitDoubleDMA((void*)(4*(int)src0),src1,dst0,dst1,size0,size1))
	if (ret= halInitDoubleDMA((void*)(4*(int)src0),(void*)(4*(int)src1),(void*)(4*(int)dst0),(void*)(4*(int)dst1),size0,size1))
		return ret;

	//an aproach to get known dma had finished
	//halStatusDMA return 0 in case dma had finished
	while(halStatusDMA()){

	}
	
	
	
	printf("DMA had finished relays on halStausDMA\n");
	for(int i = 0; i < 10; i++){
		printf("src0[%d] = %d dst0[%d] = %d\n",i, src0[i], dst0[i]);
	}
	for(int i = 0; i < 10; i++){
		printf("src1[%d] = %d dst1[%d] = %d\n",i, src1[i], dst1[i]);
	}

	//return 1;

/*
	// next one way to get know dma had finished
	//use the callback and varable to set it up in callback
	halSetCallbackDMA((DmaCallback)user_callback_to_stop);
	halInitDoubleDMA(src0,src1,dst0,dst1,size0,size1);
	
	while(done){

	}

	printf("DMA had finished relays on CallBack function\n");
	for(int i = 0; i < 10; i++){
			printf("dst_arr_1[%d] = %d\n",i,dst_arr_1[i]);
	}
	for(int i = 0; i < 10; i++){
		printf("dst_arr_2[%d] = %d\n",i,dst_arr_2[i]);
	}
	return 0;
	*/
}