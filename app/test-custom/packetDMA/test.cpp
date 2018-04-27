#include "time.h"
#include "chain7707.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"
#include <string.h>
#include "cache.h"
void initChain7707(int *buf);

#define MAX_NUM_BUFFERS 16 // Maximum buffers in packet
#define MAX_BUFFER_SIZE 512	// Maximum size of each buffer 
#define ALIGN( addr, numInts) ((((unsigned)addr)+numInts-1)%numInts*numInts) // align "addr" address to boundary of numInts 32-bit words

int chainBuf[MAX_NUM_BUFFERS*16];
void InitArr(nm32s* arr, int amm, int start){
	for(int i=0; i<amm; i++){
		arr[i] = start++;
	}
}

void InitArrInChain(void** src_arr, int* size_buff){
	int i = 0;
	int start = 0;
	while(size_buff[i]){
		InitArr((nm32s*)src_arr[i],size_buff[i],start);
		start += size_buff[i];
		i++;
	}
}

void Memset(nm32s* arr, int amm, int const2wrt){
	for(int i=0; i<amm; i++){
		arr[i] = const2wrt;
	}
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

void print_arr(nm32s** srcAddrList, nm32s** dstAddrList, int* bufSizeList){
	int i = 0;
	while(bufSizeList[i]){
		printf("buff[%d] = %d\n",i,bufSizeList[i]);
		i++;
	}
	printf("\n");
	nm32s** pntr = srcAddrList;
	for(int j = 0; j < i; j++){
		printf("src[%d] = %x\n",j,pntr[j]);
	}
	printf("\n");
	pntr = dstAddrList;
	for(int j = 0; j < i; j++){
		printf("dst[%d] = %x\n",j,pntr[j]);
	}
	printf("\n");
}

struct ret{
	nm32s* src;
	nm32s* dst;	
};
int index = 0;
int callback(){
	index++;
	halLed(index);
	return 0;
}

void PrintChaine(void** srcAddrList, void** dstAddrList, int* bufSizeList){
	int* ptr_src  = (int*)srcAddrList;
	int* ptr_dst  = (int*)dstAddrList;
	int* ptr_size = bufSizeList; 
	int  index = 0;
	while(ptr_size[index]){
		printf("chaine[%d] src = 0x%x, dst = 0x%x, amm = %d\n",index,ptr_src[index],ptr_dst[index],ptr_size[index]);
		index++;
	}
		printf("chaine[%d] src = 0x%x, dst = 0x%x, amm = %d\n",index,ptr_src[index],ptr_dst[index],ptr_size[index]);
}

extern "C"{

extern SyncBuf halSyncro;

int main(){
	//halInstrCacheEnable();
	printf("turn = %d\n",halSyncro.turn);
	printf("flag 0 = %d\n",halSyncro.flag0); 
	printf("flag 1 = %d\n",halSyncro.flag1);
	*(int*)(40001000) = 0x10;
	printf("system integrator CSR = 0x%x\n",*(int*)(40001000)); 
	int call_counter = 0;
	//return 0;
	clock_t t0,t1;
	ret loop_out;
	nm32s* srcAddrList[MAX_NUM_BUFFERS];
	nm32s* dstAddrList[MAX_NUM_BUFFERS];
	int    bufSizeList[MAX_NUM_BUFFERS + 1];
	//halLed(0xaa);
	//halSleep(1000);	
	halEnbExtInt();
	halMaskIntContMdma_mc12101();
	halInitDMA();
	halSetCallbackDMA((DmaCallback)callback);
	int step_count = 0;
	for (int srcBankIndx = 0; srcBankIndx < 4; srcBankIndx++) {
		for (int dstBankIndx = 0; dstBankIndx < 4; dstBankIndx++) {
			nmppsMallocSetRoute16((0xF00) | (dstBankIndx << 4) | srcBankIndx);
			printf("STEP %d\n",step_count++);
			nm32s* src = nmppsMalloc_32s(MAX_NUM_BUFFERS*MAX_BUFFER_SIZE+20);
			nm32s* dst = nmppsMalloc_32s(MAX_NUM_BUFFERS*MAX_BUFFER_SIZE+20);
			printf("src: %x dst:%x \n", src, dst);
			if (src == 0 || dst == 0){
				printf("ERROR : one of mallocs did't creat heap\n");
				return -1;
			}
			Memset(src,MAX_NUM_BUFFERS*MAX_BUFFER_SIZE + 20,0xcccccccc);
			Memset(dst,MAX_NUM_BUFFERS*MAX_BUFFER_SIZE + 20,0xcccccccc);
			nm32s* src_loc = AlignAddr(src);
			nm32s* dst_loc = AlignAddr(dst);
			printf("Aligned address src = 0x%x dst = 0x%x \n",src_loc,dst_loc);
			bufSizeList[MAX_NUM_BUFFERS] = 0;
			/////////////////////////
			for(int j = 0, size = 0; size < MAX_BUFFER_SIZE; j++,size += 2){
				for(int i = 0, offset = 0; i < MAX_NUM_BUFFERS; i++, offset += size){
					srcAddrList[i] = (nm32s*)((int)src_loc + offset); 
					dstAddrList[i] = (nm32s*)((int)dst_loc + offset);
					bufSizeList[i] = size;
				}
				bufSizeList[MAX_NUM_BUFFERS] = 0;
				unsigned crcDst = 0;
				unsigned crcSrc = 0;
				call_counter++;
				//PrintChaine((void**)srcAddrList, (void**)dstAddrList, (int*)bufSizeList);
				InitArrInChain((void**)srcAddrList,(int*)bufSizeList);
				halInitPacketDMA((void**)srcAddrList, (void**)dstAddrList, (int*)bufSizeList ,MAX_NUM_BUFFERS);
				while(halStatusDMA()){
					int count = 0;
					halSleep(1);
					count++;
					if(count > 100){
						printf("ERROR : time of expectation of halStatusDMA is over\n"); 
						return 333;
					}
				}
				crcSrc = nmppsCrc_32s(src, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE+20);
				crcDst = nmppsCrc_32s(dst, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE+20);
				if(crcSrc - crcDst){
					printf("ERROR : mismatch btw crces of src and dst\n");
					loop_out.src = src;
					loop_out.dst = dst;
					goto PRINT;
				}
			}
			///////////////////////////
			
			/////////////////////////// unaligned address
			Memset(src,MAX_NUM_BUFFERS*MAX_BUFFER_SIZE+20,0xcccccccc);
			Memset(dst,MAX_NUM_BUFFERS*MAX_BUFFER_SIZE+20,0xcccccccc);
			src_loc = UnalignAddr(src);
			dst_loc = UnalignAddr(dst);
			printf("Unaligned address src = %x dst = %x\n",src_loc,dst_loc);
			//for(int j = 0, size = 400; size < MAX_BUFFER_SIZE; j++,size += 10){
			//	for(int i = 0, offset = 0; i < MAX_NUM_BUFFERS; i++, offset += size){
			
			for(int j = 0, size = 0; size < MAX_BUFFER_SIZE; j++,size += 2){
				for(int i = 0, offset = 0; i < MAX_NUM_BUFFERS; i++, offset += size){
					srcAddrList[i] = (nm32s*)((int)src_loc + offset); 
					dstAddrList[i] = (nm32s*)((int)dst_loc + offset);
					bufSizeList[i] = size;
				}
				unsigned crcDst = 0;
				unsigned crcSrc = 0;
				call_counter++;
				int err = halInitPacketDMA((void**)srcAddrList, (void**)dstAddrList, (int*)bufSizeList, MAX_NUM_BUFFERS);
				while(halStatusDMA()){
					int count = 0;
					halSleep(1);
					count++;
					if(count > 100){
						printf("ERROR : time of expectation of halStatusDMA is over\n"); 
						return 333;
					}
				}
				crcSrc = nmppsCrc_32s(src, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE+20);
				crcDst = nmppsCrc_32s(dst, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE+20);
				if(crcSrc - crcDst){
					printf("ERROR : mismatch btw crces of src and dst\n");
					return 9;
				}
			}
			nmppsFree(src);
			nmppsFree(dst);
		}
	}
	
	if(call_counter != index){
		printf("ERROR: callback had not been called\n");
	}
	//return t1-t0;		
	halLed(0xaa);
	return 777;		
PRINT:
	print_arr(srcAddrList,dstAddrList,bufSizeList);
	return 9;
}

};
