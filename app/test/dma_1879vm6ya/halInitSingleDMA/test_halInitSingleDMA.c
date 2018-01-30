#include <stdio.h>
#include <hal.h>
#include <sleep.h>
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
#pragma data_section ".data_shared"
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
	halInitDMA();
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
	int buff_loc = (int)buff>>4;
	buff_loc<<=4;
	int* pntr2mem = (int*)buff_loc;
	printf("buff = %x\n",buff);	
	printf("buff_loc = %x\n",buff_loc);	
	count=0;
	status =0;	
	halSetCallbackDMA((DmaCallback)call_back);
	/*halInitSingleDMA((arr2read + MEM_OFFSET),(buff),32000);
	int temp =0;
	while(halStatusDMA()){
		temp++;
	}
	printf("temp = %d\n",temp);
	for(i=0;i<100;i++){
		printf("out[%d] = %d \n",i,buff[i]);
	}
	return 100;*/


	for(i=0;i<32000;i++){
		status = 0;
		count++;
		halInitSingleDMA((arr2read + MEM_OFFSET),(pntr2mem),i);
		while(1){
			//printf("status dma = %x\n",halStatusDMA());
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

	for(i=0;i<32000;i++){
		arr2read[i] = i;
	}
	for(i=0;i<20;i++){
		align[i] = 0;
	}

	for (int i = 0; i < SIZE_DDR;i++){
		buff[i] = 0;
		buff_ref[i] = 0;
	}

	printf("SATAGE 2 \n");
	printf("address = %x\n",buff);
	for(i=0;i<1000;i++){
		status = 0;
		count++;
		halSetCallbackDMA((DmaCallback)call_back);
		halInitSingleDMA((arr2read + MEM_OFFSET),(buff),i);
		//printf("i = %d\n",i);
		halSleep(0);
		while(1){
			if(status){
				//printf("status dma = %x\n",halStatusDMA());
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
	}

goto PASS;
	MIS_CMP:
		printf("MIS_CMP at [%d]\n",res);

	MIS_CALL:
		printf("MIS_CALL at [%d]\n",count);
	
PASS:

	if(cmp(SIZE_DDR,buff,buff_ref)){
		printf("ERROR!!!!\n");
		return 100;
	}else{
		printf("TEST has been passed\n");
	}

	return 1;
	return 1;
}