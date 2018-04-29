#include "dma.h"
#include "stdio.h"

char num; 

int call_back(){
	printf("TEST CALLBACK\n");
	return 0;
}

#define SIZE 100

int src0_arr[SIZE];
int src1_arr[SIZE];
int src2_arr[SIZE];
int src3_arr[SIZE];

int dst0_arr[SIZE];
int dst1_arr[SIZE];
int dst2_arr[SIZE];
int dst3_arr[SIZE];

int main(){
	for(int i = 0 ; i < SIZE; i++){
		src0_arr[i] = 1;
		src1_arr[i] = i;
		src2_arr[i] = 3;
		src3_arr[i] = -i;
		dst0_arr[i] = 0;
		dst1_arr[i] = 0;
		dst2_arr[i] = 0;
		dst3_arr[i] = 0;
	}
	
	halOpenDMA();
	halSetCallbackDMA(call_back);
	num = 1;
	halInitSingleDMA(src0_arr, dst0_arr, SIZE);
	while(halStatusDMA()){

	}
	num = 2;
	halInitMatrixDMA(src1_arr,4,4,12,dst1_arr,4);
	while(halStatusDMA()){

	}
	num = 3;
	halInitDoubleDMA(src0_arr,src1_arr,dst0_arr,dst1_arr,SIZE,SIZE);
	while(halStatusDMA()){

	}
	int* arr_src[4] = {src0_arr,src1_arr,src2_arr,src3_arr};
	int* arr_dst[4] = {dst0_arr,dst1_arr,dst2_arr,dst3_arr};
	int arr_size[4] = {SIZE,SIZE,SIZE,SIZE};
	num = 4;
	halInitPacketDMA(arr_src,arr_dst,arr_size,4);
	while(halStatusDMA()){

	}
	halSetCallbackDMA(0);
	num = 5;
	halInitSingleDMA(src0_arr, dst0_arr, SIZE);
	while(halStatusDMA()){

	}

}