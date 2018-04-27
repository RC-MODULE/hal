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
	halLed(0xaa);
	return 0;
}


int main(){
	halOpenDMA();//this function writes the interruption vector into interruption controller and initialise some variables needed to provide a functionnality of dma on both core 

	halSetCallbackDMA((DmaCallback)user_callback);//set callback function to be called after dma is done. 
	//Pay attention that once set up call back will call after dma has finished forever untill it flashed or changhed 
	// to flash the last callback function call halSetCallBack(0);
	int* arr_src[4];
	int* arr_dst[4];
	int arr_size[4] = {SIZE,SIZE,SIZE,SIZE};
	//array of sourse addresses 
	arr_src[0] = src_1;
	arr_src[1] = src_2;
	arr_src[2] = (int*)malloc1(SIZE);
	arr_src[3] = (int*)malloc2(SIZE);
	//array of dst addresses 
	arr_dst[0] = dst_1;
	arr_dst[1] = dst_2;
	arr_dst[2] = (int*)dst_3;
	arr_dst[3] = (int*)malloc0(SIZE);


	for(int i = 0; i < SIZE; i++){
		src_1[i] = i;
	}
	for(int i = 0; i < SIZE; i++){
		src_2[i] = -i;
	}
	int* arr = arr_src[2];
	for(int i = 0; i < SIZE; i++){
		arr[i] = 123;
	}
	arr = arr_src[3];
	for(int i = 0; i < SIZE; i++){
		arr[i] = 23;
	}

	halInitPacketDMA((void**)arr_src,(void**)arr_dst,arr_size,4);

	int time = 0;
	while(done){
		time++;
		if(time > 1000000){
			printf("TIME IS OVER\n");
			return 3;
		}
	}

	printf("ARRAY 1\n");
	for(int i = 0; i < 10; i++){
		printf("[%d] = %d \n",i,*(arr_dst[0] + i));
	}
	printf("ARRAY 2\n");
	for(int i = 0; i < 10; i++){
		printf("[%d] = %d \n",i,*(arr_dst[1] + i));
	}
	printf("ARRAY 3\n");
	for(int i = 0; i < 10; i++){
		printf("[%d] = %d \n",i,*(arr_dst[2] + i));
	}
	printf("ARRAY 4\n");
	for(int i = 0; i < 10; i++){
		printf("[%d] = %d \n",i,*(arr_dst[3] + i));
	}
	return 0;
}
