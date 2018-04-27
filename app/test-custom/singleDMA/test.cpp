#include "time.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"
#include <string.h>

#define Heap_size 4000
#define DDR_add_beging 0x20000000;
volatile int index = 0;
volatile int call_counter = 0;
volatile int status = 0;
int callback(){
	index++;
	halLed(index);
	status = 0;
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

/*nm32s* CheckIsExtMem(nm32s* addr, int min, int max){
	if((int)addr > min && (int)addr < max){
		return (nm32s*)((int)addr + MIRROR);
	}else{
		return addr; 
	}
}*/
int main(){ 
	printf("Were mirror check : Mirror at 0x%x\n",halWereMirror());	
	printf("Core ID : %d\n",halGetCoreId());
	//halEnbExtInt();
	//halMaskIntContMdma_mc12101();
	halOpenDMA();
	printf("Value of var coreID %d\n",halReadCoreID()); 
	printf("DMA status is %d \n",halIsBusyDMA());
	halSetCallbackDMA((DmaCallback)callback);
	halLed(0xaa);
	//error code check
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
			for(int j=0;j<Heap_size;j+=700)
			for(int i=0; i<j+100; i+=2){
				InitArr(src,i);
				call_counter++;
				//halLed(call_counter);
				//printf("Value of var coreID %d\n",halReadCoreID()); 
				//printf("DMA status is %d \n",halIsBusyDMA());
				halInitSingleDMA(src,dst,i);
				//printf("after Value of var coreID %d\n",halReadCoreID()); 
				//printf("after DMA status is %d \n",halIsBusyDMA());
				int time = 0;
				while(1){
					if(halStatusDMA() == 0){
						break;
					}
					//printf("Status = 0x%x\n",halStatusDMA());
					time++;
					if(time > (i<<10) + 100){
						printf("ERROR time is over. Used loops = %d\n",time);
						printf("DMA size %d\n",i);
						halLed(3);
						return 3;
					}
					halSleep(1);
				}
				nmppsCrcAcc_32s(dst, Heap_size + 20, &crcDst);//compute crc code of destination
				nmppsCrcAcc_32s(src, Heap_size + 20, &crcSrc);//compute crc code of source
				if(crcDst != crcSrc){
					printf("ERROR mismatch btw crc of src and dst\n");
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
			for(int j=0;j<Heap_size;j+=700)
			for(int i=0; i<j+100; i+=2){
				InitArr(src,i);
				call_counter++;
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
					halSleep(1);
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
	if(call_counter != index){
		printf("ERROR : call_counter is unequled to index \n"); 
	}
	halLed(0xaa);
	return 777;		
}
