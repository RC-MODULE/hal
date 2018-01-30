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
#define LENGTH     2000
#pragma data_section ".data_imu1"
	int arr2read_0[32000];
#pragma data_section ".data_imu2"
	int arr2read_1[32000];
#pragma data_section ".data_shared"
	int dummu[10];
	int buff_0[SIZE_MEM+16]; 	
	int buff_1[SIZE_MEM+16]; 	
 	int buff_ref_0[SIZE_MEM+16];
 	int buff_ref_1[SIZE_MEM+16];
 	int status;
 	
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
 		status = 1;
 		return 0;
 } 


int main(){
	//set up a mask of interraption from MDMA enable
	*((int*)IMRH) = 0x1;
	int i;
	clock_t t0,t1;
	int index =0;
	int src_arr[5];
	int dst_arr[5];
	int dst_arr_ref[5];
	int size_arr[6];
	for(i=0;i<10;i++){
		dummu[i] = i;
	}


	src_arr[0] = (int)arr2read_0 + MEM_OFFSET;
	src_arr[1] = (int)arr2read_0 + LENGTH + MEM_OFFSET;
	src_arr[2] = (int)arr2read_1 + MEM_OFFSET;
	src_arr[3] = (int)arr2read_1 + LENGTH + MEM_OFFSET;
	src_arr[4] = (int)arr2read_1 + 2*LENGTH + MEM_OFFSET;

	dst_arr[0] = (int)buff_0;
	dst_arr[1] = (int)buff_0 + LENGTH;
	dst_arr[2] = (int)buff_1;
	dst_arr[3] = (int)buff_1 + LENGTH;
	dst_arr[4] = (int)buff_1 + 2*LENGTH;

	dst_arr_ref[0] = (int)buff_ref_0;
	dst_arr_ref[1] = (int)buff_ref_0 + LENGTH;
	dst_arr_ref[2] = (int)buff_ref_1;
	dst_arr_ref[3] = (int)buff_ref_1 + LENGTH;
	dst_arr_ref[4] = (int)buff_ref_1 + 2*LENGTH;

	size_arr[0] = 2000;
	size_arr[1] = 2000;
	size_arr[2] = 2000;
	size_arr[3] = 2000;
	size_arr[4] = 2000;
	size_arr[5] = 0;

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
	halInitDMA();
	halSetCallbackDMA((DmaCallback)user_callback);

	for(i=0;i<5;i++){
		printf("[%d] SRC_0: %x DST_0: %x\n",i,src_arr[i],dst_arr[i]);
	}
	printf("Case unaligned \n");
	

	status = 0;
	t0=clock();
	halInitPacketDMA((void**)src_arr,(void**)dst_arr,(int*)size_arr);
	while(1){
		if(status){
			t1 = clock();
			break;
		}
	}
	ref_pack_DMA((void**)src_arr,(void**)dst_arr_ref,(int*)size_arr);
	for(i=0;i<5;i++){
	 	index += cmp(LENGTH,(int*)dst_arr[i],(int*)dst_arr_ref[i]);
	}
	if(index){
		printf("ERROR: mismatch btw ref function and true DMA at %d\n",index);
	}else{
		printf("DMA has finished correctly\n");
		printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",5*LENGTH,(t1-t0));
	}
	
	
	
	int temp = (int)buff_0;
	temp = (temp+15) >> 4;
	temp = temp << 4;
	dst_arr[0] = (int)temp;
	dst_arr[1] = (int)temp + LENGTH;
	temp = (int)buff_1;
	temp = (int)(buff_1+15) >> 4;
	temp = temp << 4;
	dst_arr[2] = (int)temp;
	dst_arr[3] = (int)temp + LENGTH;
	dst_arr[4] = (int)temp + 2*LENGTH;


	for(i=0;i<5;i++){
		printf("[%d] SRC_0: %x DST_0: %x\n",i,src_arr[i],dst_arr[i]);
	}
	printf("Case aligned \n");
	status = 0;
	t0=clock();
	halInitPacketDMA((void**)src_arr,(void**)dst_arr,(int*)size_arr);
	while(1){
		if(status){
			t1 = clock();
			break;
		}
	}
	ref_pack_DMA((void**)src_arr,(void**)dst_arr_ref,(int*)size_arr);
	for(i=0;i<5;i++){
	 	index += cmp(LENGTH,(int*)dst_arr[i],(int*)dst_arr_ref[i]);
	}
	if(index){
		printf("ERROR: mismatch btw ref function and true DMA at %d\n",index);
	}else{
		printf("DMA has finished correctly\n");
		printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",5*LENGTH,(t1-t0));
	}

	return 0;
}