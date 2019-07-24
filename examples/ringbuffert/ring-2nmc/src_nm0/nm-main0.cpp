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
#include "ringbuffert.h"


#include "dma.h"




extern "C" {
	void*  copyDMA   (const void* src,  void* dst,  unsigned size32){
		printf("[%x] => [%x]\n",src, dst);
		halDmaStartC(src, dst, size32);		// Запускаем DMA с критической секцией
		while (!halDmaIsCompleted()){		// Ждем окончания DMA
			printf(".");
			halSleep(10);
		};
		printf("\n");
		return dst;
	}
};

long align64bit=0;	// Выравниваем массивы A,B по гранияце 64 бита
int A[100]={0,1,2,3,4,5,6,7,8,9};
int B[100]={0,0,0,0,0,0,0,0,0,0};


int main()
{
	halLed(0);											// reset all leds
	halSetProcessorNo(0);								// Set processor number 
	halHandshake(1);
	halLedOn(0);
	
	HalRingBufferData<int,128> ringBufferData0to1;		// Создаем в стеке контейнер кольцевого буфера для пересылки с ядра-0 на ядро-1
	HalRingBufferData<int,128> ringBufferData1to0;		// Создаем в стеке контейнер кольцевого буфера для пересылки с ядра-1 на ядро-0
	halSync((int)halMapAddr(&ringBufferData0to1),1);	// Передаем адрес контейнера, смапированный в глобальное адресное пространство
	halSync((int)halMapAddr(&ringBufferData1to0),1);	// Передаем адрес контейнера, смапированный в глобальное адресное пространство
	printf("ringBuffer0to1:[%x] \n",&ringBufferData0to1);
	printf("ringBuffer1to0:[%x] \n",&ringBufferData1to0);
	
	//------------ Example of ring buffers based on RISC memcopy function -----------------------------------------
	halLedOn(2);
	printf("test ringbuffer based on RISC(nmpu0) & RISC(nmpu1):\n");
	memset(B,0,10);
	HalRingBufferConnector<int,128> ringBuffer0to1_RISC(&ringBufferData0to1,halCopyRISC,halCopyRISC);	// Создаем коннектор к кольцевому буферу с функциями чтения и записи через RISC
	HalRingBufferConnector<int,128> ringBuffer1to0_RISC(&ringBufferData1to0);							// Создаем коннектор к кольцевому буферу с функциями чтения и записи через RISC
	ringBuffer0to1_RISC.push(A,5);		// передаем 5 слов из A в кольцевой буффер
	ringBuffer0to1_RISC.push(A+5,5);	// передаем 5 слов из A в кольцевой буффер
	ringBuffer1to0_RISC.pop (B,10);		// принимаем 10 слов из кольцевого буффер в B
	
	for(int i=0;i<10; i++){
		printf("Send: %x  Recv: %x\n",A[i],B[i]);
	}
	
	//------------ Example of ring buffers based on DMA memcopy function -----------------------------------------
	halLedOn(4);
	halOpenDMA();	// Инициализируем DMA
	memset(B,0,10);
	printf("Test ringbuffer based on DMA(nmpu0) & DMA(nmpu1):\n");
	HalRingBufferConnector<int,128> ringBuffer0to1_DMA(&ringBufferData0to1,copyDMA,copyDMA);		// Создаем коннектор к кольцевому буферу с функциями чтения и записи через DMA
	HalRingBufferConnector<int,128> ringBuffer1to0_DMA(&ringBufferData1to0,copyDMA,copyDMA);		// Создаем коннектор к кольцевому буферу с функциями чтения и записи через DMA
	
	ringBuffer0to1_DMA.push(A,4);		// передаем 4 слова из A в кольцевой буффер
	ringBuffer0to1_DMA.push(A+4,6);     // передаем 6 слов из A в кольцевой буффер
	ringBuffer1to0_DMA.pop (B,10);      // принимаем 10 слов из кольцевого буффер в B
	
	for(int i=0;i<10; i++){
		printf("Send: %x  Recv: %x\n",A[i],B[i]);
	}
	halLedOn(6);
	return 111;
}
