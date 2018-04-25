#include <stdio.h>
#include <malloc.h>
#include <hal.h>

#define SIZE 100

#pragma data_section ".data_imu0"
	int src_1[SIZE];
#pragma data_section ".data_imu1"
	int src_2[SIZE];
	int dst_1[SIZE];
#pragma data_section ".data_ext"	
	int dst_2[SIZE];
	int dst_3[SIZE];


int done = 1;
int user_callback(){
	done = 0;
	halled(0xaa);
}

//dummy function to initialise arrays
void InitChaine(void* arr_src, void* arr_dst, int arr_size){
	int ptr = 0;
	int* ptr_src;
	int counter = 1; 
	while(arr_size[ptr]){
		ptr_src = arr_src[ptr];
		for(int i = 0; i < arr_size[ptr]; i++){
			ptr_src[i] = counter;  
		}
		counter++;
		ptr++;
	}
}

int main(){
	// 3 functions below serve to initialise environment and variables for dma
	//pay attention that in case useing dma with interruptions thiese functions has to be called 
	halEnbExtInt();//this function enable extern interruptions for core (write into pswr register)
	halMaskIntContMdma_mc12101();//this function set mask into interruption controller unit to allow signals from dma
	halInitDMA();//this function writes the interruption vector into interruption controller and initialise some variables needed to provide a functionnality of dma on both core 

	halSetCallbackDMA((DmaCallback)user_callback);//set callback function to be called after dma is done. 
	//Pay attention that once set up call back will call after dma has finished forever untill it flashed or changhed 
	// to flash the last callback function call halSetCallBack(0);
	int* arr_src[4];
	int* arr_dst[4];
	int arr_size[5] = {SIZE,SIZE,SIZE,SIZE,0};
	//array of sourse addresses 
	arr_dst[0] = src_1;
	arr_dst[1] = src_2;
	arr_src[2] = malloc1(SIZE);
	arr_src[3] = malloc2(SIZE);
	//array of dst addresses 
	arr_dst[0] = dst_1;
	arr_dst[1] = dst_2;
	arr_dst[2] = malloc2(SIZE);
	arr_dst[3] = malloc0(SIZE);
	
	halInitPacketDMA(arr_src,arr_dst,arr_size);

	return 0;
}
