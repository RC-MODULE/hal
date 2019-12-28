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
		printf("[nmc0] copyDMA: [%x] => [%x]\n",(int)src,(int) dst);
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
int A[100]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
int B[100]={0,0,0,0,0,0,0,0,0,0};

long align64bit2=0;	// Выравниваем массивы A,B по гранияце 64 бита
	
typedef HalRingBufferData<int, 128> RingBuffer;
RingBuffer* ringBufferData0to1;		// Создаем контейнер кольцевого буфера для пересылки с ядра-0 на ядро-1
RingBuffer* ringBufferData1to0;		// Создаем контейнер кольцевого буфера для пересылки с ядра-1 на ядро-0

#define HAL_MALLOC_OBJECT(T) (T*)halMalloc32(sizeof(T))
template<class T> T* halMallocT(T* type){
	return (T*)halMalloc32(sizeof32(T));
}
	
int main()
{
	printf("connector=%d\n",sizeof(HalRingBufferConnector<int,128>));
	
	printf("[nmc0]:Start\n");
	halLed(0);											// reset all leds
	halSetProcessorNo(0);								// Set processor number 
	halHandshake(1);									// Initial barrier sync 
	
	ringBufferData0to1 = HAL_MALLOC_OBJECT(RingBuffer);	// allocate ring buffer
	ringBufferData1to0 = HAL_MALLOC_OBJECT(RingBuffer);	// allocate ring buffer
	if (ringBufferData0to1 ==0 || ringBufferData0to1 ==0){
		printf("[nmc0] malloc error\n");
		return -1;
	}
	ringBufferData0to1->init();							// reset head and tail
	ringBufferData1to0->init();							// reset head and tail
	
	printf("[nmc0]: ringBufferData0to1.data = %x \n",(int)ringBufferData0to1->data);
	printf("[nmc0]: ringBufferData0to1.head = %x \n",(int)ringBufferData0to1->head);
	printf("[nmc0]: ringBufferData0to1.tail = %x \n",(int)ringBufferData0to1->tail);
	
	
	halSyncAddr(ringBufferData0to1, 1);	// Передаем адрес контейнера, смапированный в глобальное адресное пространство
	halSyncAddr(ringBufferData1to0, 1);	// Передаем адрес контейнера, смапированный в глобальное адресное пространство

	
	//------------ Example of ring buffers based on RISC memcopy function -----------------------------------------
	halLedOn(1);
	printf("test ringbuffer based on RISC(nmpu0) & RISC(nmpu1):\n");
	memset(B,0,10);
	HalRingBufferConnector<int,128> ringBuffer0to1_RISC(ringBufferData0to1,halCopyRISC,halCopyRISC);	// Создаем коннектор к кольцевому буферу с функциями чтения и записи через RISC
	HalRingBufferConnector<int,128> ringBuffer1to0_RISC(ringBufferData1to0); // use halCopyRISC as default copying function							// Создаем коннектор к кольцевому буферу с функциями чтения и записи через RISC
	
	ringBuffer0to1_RISC.push(A,5);		// передаем 5 слов из [A] в кольцевой буффер [0to1]
	ringBuffer0to1_RISC.push(A+5,5);	// передаем 5 слов из [A] в кольцевой буффер [0to1]
	ringBuffer1to0_RISC.pop (B,10);		// принимаем 10 слов из кольцевого буффера [1to0] в [B]
	for(int i=0;i<10; i++){
		printf("[nmc0] Send: %x  Recv: %x\n",A[i],B[i]);
	}
	
	//------------ Example of ring buffers based on DMA memcopy function with status checking -----------------------------------------
	halLedOn(2);
	halDmaInitC();	// Инициализируем DMA с криическими секциями

	HalRingBufferConnector<int,128> ringBuffer0to1_DMA(ringBufferData0to1,copyDMA,copyDMA);		// Создаем и подключаем коннектор к кольцевому буферу с функциями чтения и записи через DMA
	HalRingBufferConnector<int,128> ringBuffer1to0_DMA(ringBufferData1to0,copyDMA,copyDMA);		// Создаем и подключаем коннектор к кольцевому буферу с функциями чтения и записи через DMA
	
	
	ringBuffer0to1_DMA.push(A+10,4);	// передаем 4 слова из [A] в кольцевой буффер [0to1]
	ringBuffer0to1_DMA.push(A+14,6);    // передаем 6 слова из [A] в кольцевой буффер [0to1]
	ringBuffer1to0_DMA.pop (B,10);  	// принимаем 10 слов из кольцевого буффера [1to0] в [B]

	
	for(int i=0;i<10; i++){
		printf("[nmc0] DMA Send: %x  DMA Recv: %x\n",A[10+i],B[i]);
	}
	
	halLedOn(3);
	halSync(1,0); // final sync, which prevents premature free of  ringBufferData0to1
	halFree(ringBufferData0to1);
	halFree(ringBufferData1to0);
	return 111;
}
