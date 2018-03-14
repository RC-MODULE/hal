#include <stdio.h>
#include <time.h>
#include <led.h>
#include <sleep.h>
#include "hal.h"
#define SIZE_DDR 700000
#define IASH  0x40000458
#define IASH_CLR  0x4000045C
#define IRRH  0x40000440
#define IMRH  0x40000448
#define MEM_OFFSET 0x40000

#pragma data_section ".data_imu1"
	int arr2read[32000];
#pragma data_section ".data_imu2"
	int arr2wrt[32000];
#pragma data_section ".data_shared"
	int alight[16];
	int buff[SIZE_DDR];
	int buff_ref[SIZE_DDR];
	int status;
	int index;
	
	/*int  ref_matrix_dma(int src,  int  width,int  height, int srcStride32,  int dst, int dstStride32){
		int i,j;
		int ind2read = 0;
		int ind2wrt = 0;
		int* pntr2read = (int*)src;
		int* pntr2wrt  = (int*)dst;
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				pntr2wrt[ind2wrt] = pntr2read[ind2read];
				pntr2read++;
				pntr2wrt++;
			}
			pntr2read+=srcStride32;
			pntr2wrt+=dstStride32;
		}
	
	}*/

	int  ref_matrix_dma(int src,  int  width,int  height, int srcStride32,  int dst, int dstStride32){
		int i,j;
		int* ptr2src = (int*)src;
		int* ptr2dst = (int*)dst;
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				ptr2dst[j] = ptr2src[j];
			}
			ptr2dst+=dstStride32;
			ptr2src+=srcStride32;
		}
 	}
	int cmp(int amm, int* arr1, int* arr2){
		int i;
		for(i=0;i<amm;i++){
			if(arr1[i] != arr2[i]){
				return i+1;
			}
		}
		return 0;
	}

  
  int user_callback(){
 		status = 1;
 		index++;
 		halLed(index);
 		return 0;
 	} 


int main(){
	int* pntr2ic = (int*)IASH;
	printf("IASH = %x\n",*pntr2ic);
	pntr2ic = (int*)IMRH;
	*pntr2ic = 0x1;
	printf("IMRH = %x\n",*pntr2ic);
	int i,j;
	halInitDMA();
	halSetCallbackDMA(user_callback);
////////////////////////////init arrays
	for(i=0;i<32000;i++){
		arr2read[i] = i;
		arr2wrt[i] = 0;
	}
	for(i=0;i<16;i++){
		alight[i] = 0;
	}
	for(i=0;i<SIZE_DDR;i++){
		buff[i] = 0;
		buff_ref[i] = 0;
	}
	int buff_loc = (int)buff >> 4;
	buff_loc<<= 4;
	int* pntr2mem = (int*)buff_loc;
	printf("pntr2mem = %x\n",pntr2mem);
	printf("buff = %x\n",buff);
	int coutn =0;
	index =0;
	status = 0;
	printf("statge 1 \n");
	for(j=0;j<100;j++){
		for(i=0;i<100;i+=4){
			coutn++;
			status =0;
			printf("st1 i = %d j = %d\n",i,j);
			halInitMatrixDMA((arr2read + MEM_OFFSET),i,j, i,buff,i);
			while(1){
				if(status){
					ref_matrix_dma(int(arr2read + MEM_OFFSET),i,j, i,(int)buff_ref,i);
					int res = cmp(SIZE_DDR,buff,buff_ref);
					if(res){
						printf("Mismatch at %d \n",res);
						goto END;
					}
					if(coutn != index){
						printf("call has not been called\n");
						goto END;
					}
					break;
				}
			}
		}
	}
	coutn  = 0;
	index  = 0;
	status = 0;
	printf("statge 1 with dma status\n");
	for(j=0;j<100;j++){
		for(i=0;i<100;i+=4){
			coutn++;
			status =0;
			printf("st1 i = %d j = %d\n",i,j);
			halInitMatrixDMA((arr2read + MEM_OFFSET),i,j, i,buff,i);
			while(1){
				if(halStatusDMA() == 0){
					ref_matrix_dma(int(arr2read + MEM_OFFSET),i,j, i,(int)buff_ref,i);
					int res = cmp(SIZE_DDR,buff,buff_ref);
					if(res){
						printf("Mismatch at %d \n",res);
						goto END;
					}
					if(coutn != index){
						printf("call has not been called\n");
						goto END;
					}
					break;
				}
			}
		}
	}
	printf("satage 2\n");

	for(j=0;j<100;j++){
		for(i=0;i<100;i+=4){
			coutn++;
			status =0;
			printf("st2 i = %d j = %d\n",i,j);
			halInitMatrixDMA((arr2read + MEM_OFFSET),i,j, i,pntr2mem,i);
			while(1){
				if(status){
					ref_matrix_dma(int(arr2read + MEM_OFFSET),i,j, i,(int)buff_ref,i);
					int res = cmp(SIZE_DDR,pntr2mem,buff_ref);
					if(res){
						printf("Mismatch at %d \n",res);
						goto END;
					}
					if(coutn != index){
						printf("call has not been called\n");
						goto END;
					}
					break;
				}
			}
		}
	}
	printf("TEST has secsessfuly passed\n");
END:
	return 10;
}
