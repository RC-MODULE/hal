//------------------------------------------------------------------------
//
//
//  Copyright (c) RC Module Inc.
//
//!
//! \file   main.cpp
//! \author S. Mushkaev
//! \brief  Example of ring buffer usage on two-core  proccessor 
//!
//! \endif
//!
//------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "hal_target.h"
#include "sleep.h"
#include "hal.h"
#include "led.h"
#include "cache.h"
#include "ringbuffert.h"
#include "dma.h"



extern "C" {
	void*  copyDMA   (const void* src,  void* dst,  unsigned size32){
		printf("[nmc1] copyDMA: [%x] => [%x]\n",(int)src,(int) dst);
		halDmaStartC(src, dst, size32);		// Запускаем DMA с критической секцией
		while (!halDmaIsCompleted()){		// Ждем окончания DMA
			halSleep(10);
		};
		printf("\n");
		return dst;
	}
};

long align64bit=0;	// Выравниваем массивы A,B по гранияце 64 бита
int A[100]={0,0,0,0,0,0,0,0,0,0};
int B[100]={0,0,0,0,0,0,0,0,0,0};
int main()
{
	printf("[nmc1]:Start\n");
	
	halSetProcessorNo(1);							// Set processor number 


	halHandshake(0);								// Initial barrier sync 
	halLedOn(4);
	
	HalRingBufferData<int,128>* ringBufferData0to1=(HalRingBufferData<int,128>*)halSyncAddr(0,0);	// Принимаем адрес контейнера кольцевого буфера в глобальном адресном пространстве (зеркале);
	HalRingBufferData<int,128>* ringBufferData1to0=(HalRingBufferData<int,128>*)halSyncAddr(0,0);	// Принимаем адрес контейнера кольцевого буфера в глобальном адресном пространстве (зеркале);
	
	printf("[nmc1]: ringBufferData0to1.data = %x \n",(int)ringBufferData0to1->data);
	printf("[nmc1]: ringBufferData0to1.head = %x \n",(int)ringBufferData0to1->head);
	printf("[nmc1]: ringBufferData0to1.tail = %x \n",(int)ringBufferData0to1->tail);
	
	//------------ Example of ring buffers based on RISC memcopy function -----------------------------------------
	halLedOn(5);
	memset(A,0,10);
	HalRingBufferConnector<int,128> ringBuffer0to1_RISC(ringBufferData0to1,halCopyRISC,halCopyRISC);	// Создаем коннектор к кольцевому буферу с функциями чтения и записи через RISC
	HalRingBufferConnector<int,128> ringBuffer1to0_RISC(ringBufferData1to0);							// Создаем коннектор к кольцевому буферу с функциями чтения и записи через RISC
	
	ringBuffer0to1_RISC.pop(A,10);		// принимаем 10 слов из кольцевого буфера [0to1] в [A]
	ringBuffer1to0_RISC.push(A,10);		// передаем 10 слов из [A] в кольцевой буффер [1to0]
	
	for (int i=0; i<10; i++){
		printf("[nmc1] A[%d]=%d\n",i,A[i]);
	}
	
	//------------ Example of ring buffers based on DMA memcopy function with status checking -----------------------------------------
	halLedOn(6);
	halDmaInitC();	// Инициализируем DMA с криическими секциями

	HalRingBufferConnector<int,128> ringBuffer0to1_DMA(ringBufferData0to1,copyDMA,copyDMA);		// Создаем коннектор к кольцевому буферу с функциями чтения и записи через DMA
	HalRingBufferConnector<int,128> ringBuffer1to0_DMA(ringBufferData1to0,copyDMA,copyDMA);		// Создаем коннектор к кольцевому буферу с функциями чтения и записи через DMA
	
	
	ringBuffer0to1_DMA.pop (A,10); // принимаем 10 слов из кольцевого буфера [0to1] в [A]
	ringBuffer1to0_DMA.push(A,10); // передаем 10 слов из [A] в кольцевой буффер [1to0]
	
	for (int i=0; i<10; i++){
		printf("[nmc1] Recv: A[%d]=%d\n",i,A[i]);
	}
	

	halLedOn(7);
	
	halSync(0,0); //This is final sync, which prevents premature call of free HalRingBufferData 
	return 222;
}

