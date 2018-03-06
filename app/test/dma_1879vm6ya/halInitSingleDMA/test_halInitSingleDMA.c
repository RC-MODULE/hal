#include <stdio.h>
#include <hal.h>
#include <sleep.h>
#define LOOP_SIZE  32000
#define SIZE_DDR   160000
#define MEM_OFFSET 0x40000
#define IASH       0x40000458
#define IASH_CLR   0x4000045C
#define IRRH       0x40000440
#define IMRH       0x40000448

#pragma data_section ".data_imu1"
	int arr2read[32000];
#pragma data_section ".data_imu2"
	int arr2wrt[32000];
#pragma data_section ".my_ddr"
	int align[20];
	int buff[SIZE_DDR]; 	
 	int buff_ref[SIZE_DDR];
 	int index;
 	int status;
 	int count;
 	int res;

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


 void call_back(){
 		index++;
 		halLed(index);
 		status = 1;
 } 


int main(){
int i;
	index  = 0;
	status = 0;
	halEnbExtInt();
	printf("TEST halInitDMA %d \n",halInitDMA());
	for(i=0;i<32000;i++){
		arr2read[i] = i;
	}
	for(i=0;i<20;i++){
		align[i] = 0;
	}

	for (int i = 0; i < SIZE_DDR; i++)
	{
		buff[i] = 0;
		buff_ref[i] = 0;
	}

	int* pntr2ic = (int*)IMRH;// init mask
	*pntr2ic = 0x1;
	
	pntr2ic = (int*)IASH;
	printf("IASH = %x\n",*pntr2ic);


	int buff_loc = (int)buff>>4;
	buff_loc<<=4;
	int* pntr2mem = (int*)buff_loc;
	printf("buff = %x\n",buff);	
	printf("buff_loc = %x\n",buff_loc);	
	count=0;
	
	halSetCallbackDMA((DmaCallback)call_back);

	printf("SATAGE 1 \n");
	for(i=0;i<LOOP_SIZE;i+=2){
		status = 0;
		count++;
		printf("%d\n",i);
		halInitSingleDMA((arr2read + MEM_OFFSET),(pntr2mem),i);
		while(1){
			if(status){
				ref_DMA(int(arr2read + MEM_OFFSET),int(buff_ref),i);
				res = cmp(SIZE_DDR,buff_ref,pntr2mem);
				if(res){
					goto MIS_CMP;
				}
				if(count != index){
					goto MIS_CALL;
				}
				break;
			}
		}
	}
	printf("SATAGE 2 unaligned address\n");
	count = 0;
	for(i=0;i<LOOP_SIZE;i+=2){
		status = 0;
		count++;
		halInitSingleDMA((arr2read + MEM_OFFSET),(buff),i);
		while(halStatusDMA() == 0){
				ref_DMA(int(arr2read + MEM_OFFSET),int(buff_ref),i);
				res = cmp(SIZE_DDR,buff_ref,buff);
				if(res){
					goto MIS_CMP;
				}
				if(count != index){
					goto MIS_CALL;
				}
				break;
		}
	}
	printf("SATAGE 3 test halStatusDMA alligned adderss\n");
	count = 0;
	for(i=0;i<LOOP_SIZE;i+=2){
		status = 0;
		count++;
		halInitSingleDMA((arr2read + MEM_OFFSET),(pntr2mem),i);
		while(halStatusDMA() == 0){
				ref_DMA(int(arr2read + MEM_OFFSET),int(buff_ref),i);
				res = cmp(SIZE_DDR,buff_ref,pntr2mem);
				if(res){
					goto MIS_CMP;
				}
				if(count != index){
					goto MIS_CALL;
				}
				break;
		}
	}
	printf("SATAGE 4 test halStatusDMA unalligned adderss\n");
	count = 0;
	for(i=0;i<LOOP_SIZE;i+=2){
		status = 0;
		count++;
		halInitSingleDMA((arr2read + MEM_OFFSET),(buff),i);
		while(halStatusDMA() == 0){
				ref_DMA(int(arr2read + MEM_OFFSET),int(buff_ref),i);
				res = cmp(SIZE_DDR,buff_ref,buff);
				if(res){
					goto MIS_CMP;
				}
				if(count != index){
					goto MIS_CALL;
				}
				break;
		}
	}

goto PASS;
MIS_CMP:
		printf("MIS_CMP at [%d]\n",res);

MIS_CALL:
		printf("MIS_CALL at [%d]\n",count);
	
PASS:
		printf("TEST has been passed\n");

	return 1;
}