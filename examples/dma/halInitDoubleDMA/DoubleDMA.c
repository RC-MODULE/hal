#include <stdio.h>
#include <hal.h>
#include "nmpp.h"

//this example shows how to use DoubleDma
#define SIZE 100

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
	nmppsMallocSetRoute16(0xF0101);
	
	int* src0 = nmppsMalloc_32s(size0);
	int* dst0 = nmppsMalloc_32s(size0);
	int* src1 = nmppsMalloc_32s(size1);
	int* dst1 = nmppsMalloc_32s(size1);
	
	if (src0==0 || src1==0 || dst0==0 || dst1==0 )
		return -1;
	
	for(int i = 0; i < size0; i++){
		src0[i] = i;
		dst0[i] = 0;
	}	
	for(int i = 0; i < size1; i++){
		src1[i] = 0x10000+i;
		dst1[i] = 0;
	}	
	
	halOpenDMA();//this function writes the interruption vector into interruption controller and initialise some variables needed to provide a functionality of dma on both core 
	
	halSetCallbackDMA((DmaCallback)user_callback);//set callback function to be called after dma is done. 
	
	//Pay attention that once set up call back will call after dma has finished forever until it flashed or changed 
	// to flash the last callback function call halSetCallBack(0);

	ret = halCheckParamsDoubleDMA(src0,src1,dst0,dst1,size0,size1);
	
	//load the parameters into DMA
	if (ret){
		printf("ERORR: wrong parametrs in DMA initialisation exit code is %d\n",ret);
		return ret;
	}
	//if (ret= halInitDoubleDMA((void*)(4*(int)src0),(void*)(4*(int)src1),(void*)(4*(int)dst0),(void*)(4*(int)dst1),size0,size1))
	halInitDoubleDMA(src0,src1,dst0,dst1,size0,size1);

	
	//an approach to get known dma had finished
	//halStatusDMA return 0 in case dma had finished
	while(halStatusDMA()){

	}
	
	printf("DMA had finished relays on halStausDMA\n");
	printf("\n");
	for(int i = 0; i < 16; i++){
		printf("src0[%d] = %x \t dst0[%d] = %x\n",i,src0[i],i,dst0[i]);
	}
	for(int i = 0; i < 16; i++){
		printf("src1[%d] = %x \t dst1[%d] = %x\n",i,src1[i],i, dst1[i]);
	}

	for(int i = 0; i < size0; i++){
		src0[i] = i;
		dst0[i] = 0;
	}	
	for(int i = 0; i < size1; i++){
		src1[i] = 0x10000+i;
		dst1[i] = 0;
	}	
	

	// next one way to get know dma had finished
	//use the callback and variable to set it up in callback
	halSetCallbackDMA((DmaCallback)user_callback_to_stop);
	halInitDoubleDMA(src0,src1,dst0,dst1,size0,size1);
	
	while(done){

	}
	printf("\n");
	printf("DMA had finished relays on CallBack function\n");
	printf("\n");
	for(int i = 0; i < 16; i++){
		printf("src0[%d] = %x \t dst0[%d] = %x\n",i,src0[i],i,dst0[i]);
	}
	for(int i = 0; i < 16; i++){
		printf("src1[%d] = %x \t dst1[%d] = %x\n",i,src1[i],i, dst1[i]);
	}

	
	return 0;
	
}