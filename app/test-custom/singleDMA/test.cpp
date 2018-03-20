#include "time.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"
#include <string.h>

#define Heap_size 8000
#define DDR_add_beging 0x20000000;
int index = 0;
int callback(){
	index++;
	halLed(index);
	return 0;
}

nm32s* AlignAddr(nm32s* addr){
	int temp = (int)addr;
	temp+=15;
	temp>>=4;
	temp<<=4;
	addr = (nm32s*)temp;
	return addr;
}

nm32s* UnalignAddr(nm32s* addr){
	int temp = (int)AlignAddr(addr);
	temp+=2;
	addr = (nm32s*)temp;
	return addr;
}

void InitArr(nm32s* arr, int amm){
	for(int i =0; i<amm; i++){
		arr[i] = i;
	}
};
void SetArr(nm32s* arr,int amm,int const2wrt){
	for(int i =0; i<amm; i++){
		arr[i] = const2wrt;
	}
};

int main(){ 
	
	halEnbExtInt();
	halInitDMA();
	//halSetCallbackDMA((DmaCallback)callback);
	//error code check
	int error_code = halInitSingleDMA((int*)3,(int*)3,3);
	if(error_code != 1){
		printf("ERROR: wrong error code case src is not even\n");
		printf("code is %d\n",error_code);
		return 10;
	}
	error_code = halInitSingleDMA((int*)210,(int*)3,3);
	if(error_code != 2){
		printf("ERROR: wrong error code case dst is not even\n");
		printf("code is %d\n",error_code);
		return 10;
	}
	error_code = halInitSingleDMA((int*)210,(int*)210,11);
	if(error_code != 4){
		printf("ERROR: wrong error code case size32 is not even\n");
		printf("code is %d\n",error_code);
		return 10;
	}
	
	clock_t t0,t1;
	int count = 0;
	for (int srcBankIndx = 0; srcBankIndx < 4; srcBankIndx++) {
		for (int dstBankIndx = 0; dstBankIndx < 4; dstBankIndx++) {
			printf("STEP %d\n",count++);
			nmppsMallocSetRoute16((0xF00) | (dstBankIndx << 4) | srcBankIndx);
			nm32s* src = nmppsMalloc_32s(Heap_size + 20);
			nm32s* dst = nmppsMalloc_32s(Heap_size + 20);
			printf("src = %x dst = %x\n",src,dst);
			if (src == 0 || dst == 0){
				printf("ERROR : heap had not been created\n");
				return -1;
			}
			//allign address
			src = AlignAddr(src);
			dst = AlignAddr(dst);
			printf("aligned addresses src = %x dst = %x\n",src,dst);
			SetArr(src,Heap_size + 20,0xCCCCCCCC);
			SetArr(dst,Heap_size + 20,0xCCCCCCCC);
			unsigned int crcDst = 0;
			unsigned int crcSrc = 0;
			for(int j=0;j<Heap_size;j+=500)
			for(int i=0; i<j+100; i+=2){
				InitArr(src,i);
				halLed(i);
				halInitSingleDMA(src,dst,i);
				int time = 0;
				while(1){
					if(halStatusDMA() == 0){
						break;
					}
					time++;
					if(time > (i<<1)){
						printf("ERROR time is over\n");
						printf("DMA size %d\n",i);
						halLed(3);
						return 3;
					}
				}
				nmppsCrcAcc_32s(dst, Heap_size + 20, &crcDst);//compute crc code of destination
				nmppsCrcAcc_32s(src, Heap_size + 20, &crcSrc);//compute crc code of source
				if(crcDst != crcSrc){
					printf("ERROR mismatch btw crc of src and dst\n");
					printf("srcBankIndx = %d dstBankIndx = %d index = %d\n",srcBankIndx,dstBankIndx,i);
					halLed(9);
					return 9;
				}

			}
			src = UnalignAddr(src);
			dst = UnalignAddr(dst);
			printf("unalighed address src = %x dst = %x\n",src,dst);
			SetArr(src,Heap_size + 18,0xCCCCCCCC);
			SetArr(dst,Heap_size + 18,0xCCCCCCCC);
			crcDst = 0;
			crcSrc = 0;
			for(int i=0; i<Heap_size; i+=2){
				InitArr(src,i);
				halLed(i);
				halInitSingleDMA(src,dst,i);
				int time = 0;
				while(1){
					halSleep(1);
					if(halStatusDMA() == 0){
						break;
					}
					time++;
					if(time > (i<<1)){
						printf("ERROR time is over\n");
						printf("DMA size %d\n",i);
						halLed(3);
						return 3;
					}
				}
				nmppsCrcAcc_32s(dst, Heap_size + 18, &crcDst);//compute crc code of destination
				nmppsCrcAcc_32s(src, Heap_size + 18, &crcSrc);//compute crc code of source
				if(crcDst != crcSrc){
					printf("ERROR mismatch btw crc of src and dst\n");
					printf("srcBankIndx = %d dstBankIndx = %d index = %d\n",srcBankIndx,dstBankIndx,i);
					halLed(9);
					return 9;
				}

			}
			nmppsFree(src);
			nmppsFree(dst);
			
		}
	}
	halLed(0xaa);
	return 777;		
}
