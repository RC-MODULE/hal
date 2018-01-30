#include <stdio.h>
#include <time.h>
#include "hal.h"
#include "sleep.h"
#define SIZE_DDR   160000
#define MEM_OFFSET 0x40000
#define IASH       0x40000458
#define IASH_CLR   0x4000045C
#define IRRH       0x40000440
#define IMRH       0x40000448

#pragma data_section ".data_imu1"
	int arr2read_0[32000];
#pragma data_section ".data_imu2"
	int arr2read_1[32000];
#pragma data_section ".data_shared"
	int buff_0[SIZE_DDR]; 	
	int buff_1[SIZE_DDR]; 	
 	int buff_ref_0[SIZE_DDR];
 	int buff_ref_1[SIZE_DDR];
 	int index;
 	int status;

 //	int halInitSingleDMA(int src, int dst, int size32, int DmaCallBack, int channel);
 	void ref_double_DMA(int*  src0, int*  src1, int* dst0,  int* dst1, int intSize0, int intSize1){
 		int i;
 		for(i=0;i<intSize0;i++){
 			dst0[i] = src0[i];
 		}
 		for(i=0;i<intSize1;i++){
 			dst1[i] = src1[i];
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


 int Callback_user(){
 		index++;
 		//halSleep(100);
		halLed(index); 
		status = 1;
		return 0;		
 }


int main(){
	int i;
	int count;
	status = 0;
	int* pntr2ic;
	pntr2ic = (int*)IMRH;
	*pntr2ic = 0x1;

	for(i=0;i<32000;i++){
		arr2read_0[i] = i;
		arr2read_1[i] = -i;
	}
	for(i=0;i<SIZE_DDR;i++){
		buff_0[i]     = 0;
		buff_1[i]     = 0;
		buff_ref_0[i] = 0;
		buff_ref_1[i] = 0;
	}
	
	halInitDMA();
	halSetCallbackDMA(Callback_user);
	int error_index;
	int temp = int(buff_0 + 15)>>4;
	temp<<=4;
	int* ptr2buff_0 = (int*)temp;
	temp = (int)(buff_1 + 15)>>4;
	temp<<=4;
	int* ptr2buff_1 = (int*)temp;

	printf("case aligned addresses\n");
	printf("buff_0 address = %x\n",ptr2buff_0);
	printf("buff_1 address = %x\n",ptr2buff_1);
	
	index =0;
	count =0;
	for(i=0;i<32000;i+=2){
		status = 0;
		count++;
		halInitDoubleDMA(arr2read_0 + MEM_OFFSET, arr2read_1 + MEM_OFFSET, ptr2buff_0, ptr2buff_1, i, i);
		while(1){
			if(status){
				ref_double_DMA(arr2read_0 + MEM_OFFSET, arr2read_1 + MEM_OFFSET, buff_ref_0, buff_ref_1, i, i);
				int res_0 = cmp(32000,ptr2buff_0,buff_ref_0);
				if(res_0){
					error_index = res_0;
					printf("Mismatch at 0 aray at %d\n",res_0-1);
					goto END_arr0;
				}
				int res_1 = cmp(32000,ptr2buff_1,buff_ref_1);
				if(res_1){
					error_index = res_1;
					printf("Mismatch at 1 aray at %d\n",res_1-1);
					goto END_arr1;
				}
				if(count != index){
					printf("Callback ERROR at %d\n",count);
					goto END_callback;
				}
				break;
			}
		}
	}
	
	printf("CASE UNALIGNED ADDRESSES\n");
	for(i=0;i<32000;i++){
		arr2read_0[i] = i;
		arr2read_1[i] = -i;
	}
	for(i=0;i<SIZE_DDR;i++){
		buff_0[i]     = 0;
		buff_1[i]     = 0;
		buff_ref_0[i] = 0;
		buff_ref_1[i] = 0;
	}
	printf("buff_0 address = %x\n",buff_0);
	printf("buff_1 address = %x\n",buff_1);
	for(i=0;i<32000;i+=2){
		status = 0;
		count++;
		halInitDoubleDMA(arr2read_0 + MEM_OFFSET, arr2read_1 + MEM_OFFSET, buff_0, buff_1, i, i);
		while(1){
			if(status){
				ref_double_DMA(arr2read_0 + MEM_OFFSET, arr2read_1 + MEM_OFFSET, buff_ref_0, buff_ref_1, i, i);
				int res_0 = cmp(32000,buff_0,buff_ref_0);
				if(res_0){
					error_index = res_0;
					printf("Mismatch at 0 aray at %d\n",res_0-1);
					goto END_arr0;
				}
				int res_1 = cmp(32000,buff_1,buff_ref_1);
				if(res_1){
					error_index = res_1;
					printf("Mismatch at 1 aray at %d\n",res_1-1);
					goto END_arr1;
				}
				if(count != index){
					printf("Callback ERROR at %d\n",count);
					goto END_callback;
				}
				break;
			}
		}
	}
	
goto END;	
END_arr0:
	for(i=error_index-2;i<error_index+2;i++){
		printf("arr0[%d] = %d\n",i,buff_0[i]);
	}
	printf("\n");
	for(i=error_index-2;i<error_index+2;i++){
		printf("arr0_ref[%d] = %d\n",i,buff_ref_0[i]);
	}
END_arr1:
	printf("\n");
	for(i=error_index-2;i<error_index+2;i++){
		printf("arr1[%d] = %d\n",i,buff_1[i]);
	}
	printf("\n");
	for(i=error_index-2;i<error_index+2;i++){
		printf("arr1_ref[%d] = %d\n",i,buff_ref_1[i]);
	}	
END_callback:
END:
	/*for(i=0;i<100;i++){
		printf("buff_0[%d] = %d\n",i,buff_0[i]);
	}
	printf("\n");
	for(i=0;i<100;i++){
		printf("buff_1[%d] = %d\n",i,buff_1[i]);
	}*/
	return 100;
}