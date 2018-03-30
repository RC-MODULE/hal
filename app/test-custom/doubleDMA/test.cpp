#include "time.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"
#include <string.h>

#define Heap_size 8000
#define IMRH       0x40000448

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

nm32s* CheckIsExtMem(nm32s* addr, int min, int max){
	if((int)addr > min && (int)addr < max){
		return (nm32s*)((int)addr + MIRROR);
	}else{
		return addr; 
	}
}

int main(){ 
	int call_counter = 0;
	halEnbExtInt();
	halMaskIntContMdma_mc12101();
	halInitDMA();
	halSetCallbackDMA((DmaCallback)callback);
	
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
			nm32s* src1;
			nm32s* dst1;
			SetArr(src,Heap_size + 18,0xCCCCCCCC);
			SetArr(dst,Heap_size + 18,0xCCCCCCCC);
			crcDst = 0;
			crcSrc = 0;
			for(int i=0; i<2000; i+=4){
				InitArr(src,i);
				call_counter++;
				nm32s* src_loc = CheckIsExtMem(src,0,0xA0000);
				nm32s* dst_loc = CheckIsExtMem(dst,0,0xA0000);
				halInitDoubleDMA(src_loc,(nm32s*)((int)src_loc+i/2),dst_loc,(nm32s*)((int)dst_loc+i/2),i/2,i/2);
				int time = 0;
				while(halStatusDMA()){
					/*halSleep(1);
					if(halStatusDMA() == 0){
						break;
					}*/
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
			src = UnalignAddr(src);
			dst = UnalignAddr(dst);
			printf("unalighed address src = %x dst = %x\n",src,dst);
			SetArr(src,Heap_size + 18,0xCCCCCCCC);
			SetArr(dst,Heap_size + 18,0xCCCCCCCC);
			crcDst = 0;
			crcSrc = 0;
			for(int i=0; i<2000; i+=4){
				InitArr(src,i);
				call_counter++;
				nm32s* src_loc = CheckIsExtMem(src,0,0xA0000);
				nm32s* dst_loc = CheckIsExtMem(dst,0,0xA0000);
				halInitDoubleDMA(src_loc,(nm32s*)((int)src_loc+i/2),dst_loc,(nm32s*)((int)dst_loc+i/2),i/2,i/2);
				int time = 0;
				while(halStatusDMA()){
					time++;
					if(time > (i<<1)){
						printf("ERROR time is over\n");
						printf("DMA size %d\n",i);
						halLed(3);
						return 3;
					}
				}
				nmppsCrcAcc_32s(dst-2, Heap_size + 18, &crcDst);//compute crc code of destination
				nmppsCrcAcc_32s(src-2, Heap_size + 18, &crcSrc);//compute crc code of source
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
		printf("ERROR : callback had not been called\n");
	}
	halLed(0xaa);
	return 777;		
}
