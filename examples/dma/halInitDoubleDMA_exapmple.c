#include <stdio.h>
#include <hal.h>

//this example shows how to use DoubleDma
#define SIZE 100
#pragma data_section ".data_imu0"
	int src_arr_1[SIZE];
	int src_arr_2[SIZE];
#pragma data_section ".data_shared"	
	int dst_arr_1[SIZE];
	int dst_arr_2[SIZE]; 

int user_callback(){
	halLed(0xaa);
}
int done = 1;
int user_callback_to_stop(){
	halLed(0xbb);
	done = 0;
}

int main(){
	for(int i = 0; i < SIZE; i++){
		src_arr_1[i] = i;
		src_arr_2[i] = -i;
	}	
	// 3 functions below serve to initialise environment and variables for dma
	//pay attention that in case useing dma with interruptions thiese functions has to be called 
	halEnbExtInt();//this function enable extern interruptions for core (write into pswr register)
	halMaskIntContMdma_mc12101();//this function set mask into interruption controller unit to allow signals from dma
	halInitDMA();//this function writes the interruption vector into interruption controller and initialise some variables needed to provide a functionnality of dma on both core 

	halSetCallbackDMA((DmaCallback)user_callback);//set callback function to be called after dma is done. 
	//Pay attention that once set up call back will call after dma has finished forever untill it flashed or changhed 
	// to flash the last callback function call halSetCallBack(0);

	
	//load the paramentrs into DMA
	halInitDoubleDMA(src_arr_1,src_arr_2,dst_arr_1,dst_arr_2,10,10);

	//an aproach to get known dma had finished
	//halStatusDMA return 0 in case dma had finished
	while(halStatusDMA()){

	}
	
	printf("DMA had finished relays on halStausDMA\n");
	for(int i = 0; i < 10; i++){
		printf("dst_arr_1[%d] = %d\n",i,dst_arr_1[i]);
	}
	for(int i = 0; i < 10; i++){
		printf("dst_arr_2[%d] = %d\n",i,dst_arr_2[i]);
	}



	// next one way to get know dma had finished
	//use the callback and varable to set it up in callback
	halSetCallbackDMA((DmaCallback)user_callback_to_stop);
	halInitDoubleDMA(src_arr_1+10,src_arr_2+10,dst_arr_1,dst_arr_2,10,10);
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
}