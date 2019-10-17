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
#include "hal_target.h"
#include "sleep.h"
#include "hal.h"
#include "cache.h"
#include "string.h"
//DISABLE_PRINTF();
#include "ringbuffert.h"
#include "qdma.h"





extern "C" {
	
	void*  copyDMA   (const void* src,  void* dst,  unsigned size32){
		halDmaStartC(src, dst, size32);		// Запускаем DMA с критической секцией
		int ret;
		while (!halDmaIsCompleted()){		// Ждем окончания DMA
			halSleep(10);
		};
		return dst;
	}
};
long align64bit=0;	// Выравниваем массивы A,B по гранияце 64 бита
int A[100]={0,1,2,3,4,5,6,7,8,9};
int B[100]={0,0,0,0,0,0,0,0,0,0};
int main()
{

	
	halSetProcessorNo(1);							// Set processor number 


	halHandshake(0);
	halLedOn(1);
	HalRingBufferData<int,128>* ringBufferData0to1=(HalRingBufferData<int,128>*)halSync((int)2,0);	// Принимаем адрес контейнера кольцевого буфера в глобальном адресном пространстве (зеркале);
	HalRingBufferData<int,128>* ringBufferData1to0=(HalRingBufferData<int,128>*)halSync((int)1,0);	// Принимаем адрес контейнера кольцевого буфера в глобальном адресном пространстве (зеркале);
	printf("hello form nmc1 \n");
	return (int)ringBufferData0to1->head;
	//------------ Example of ring buffers based on RISC memcopy function -----------------------------------------
	halLedOn(3);
	memset(A,0,10);
	HalRingBufferConnector<int,128> ringBuffer0to1_RISC(ringBufferData0to1,halCopyRISC,halCopyRISC);	// Создаем коннектор к кольцевому буферу с функциями чтения и записи через RISC
//	HalRingBufferConnector<int,128> ringBuffer1to0_RISC(ringBufferData1to0);							// Создаем коннектор к кольцевому буферу с функциями чтения и записи через RISC
	
	return ringBufferData0to1->head;
//	ringBuffer0to1_RISC.pop(A,10);
//	ringBuffer1to0_RISC.push(A,10);
//	//------------ Example of ring buffers based on DMA memcopy function -----------------------------------------
//	
//	halLedOn(5);
	
	//ringBuffer0to1_RISC.pop(A,10);
	//ringBuffer0to1_RISC.pop(A,10);
	//ringBuffer1to0_RISC.push(A,10);
	
	
	
	//halOpenDMA();	// Инициализируем DMA
	//memset(A,0,10);
	//HalRingBufferConnector<int,128> ringBuffer0to1_DMA(ringBufferData0to1,copyDMA,copyDMA);		// Создаем коннектор к кольцевому буферу с функциями чтения и записи через DMA
	//HalRingBufferConnector<int,128> ringBuffer1to0_DMA(ringBufferData1to0,copyDMA,copyDMA);		// Создаем коннектор к кольцевому буферу с функциями чтения и записи через DMA
	
	//ringBuffer0to1_DMA.pop (A,10);
	//ringBuffer1to0_DMA.push(A,10);

	halLedOn(7);
	return 222;
}

