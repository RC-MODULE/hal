#ifndef HAL_RING_INCLUDED
#define HAL_RING_INCLUDED

#include "dma.h"
#include "hal.h"


#define RING_BUFFER_DECLARED	0xDEC1A8ED
#define RING_BUFFER_ALLOCATED	0xA10CA7ED
#define RING_BUFFER_MEM_ERROR	0xDEADF00D
#define RING_BUFFER_ERROR		0xBEDABEDA
#define RING_BUFFER_OK			0x0
#define RING_BUFFER_INCORRECT_COUNT -1
#define RING_BUFFER_SLEEP 100

#ifdef __cplusplus
		extern "C" {
#endif


//typedef  void (*memcpy_ptr)(const void *, void *, size_t) ;
//typedef  void *(*t_bytecpy)(void *to, int toIndex, void const *from, int fromIndex, size_t size) ;

//extern int statusDMA;
struct HalRingBuffer{
	int* 	data;			///<  ���������� ����� ���������� ������ ������� ������ 
	size_t 	maxCount;		///<  ������ ���������� ������ ������� ������ (� ���������; ������������� ��� ��� ������� ������)
	size_t 	maxCountMinus1;	///<  ������ ���������� ������ ������� ������ (� ���������; ������������� ��� ��� ������� ������)
	size_t 	size;			///<  ������ ���������� ������ ������� ������ (� ���������; ������������� ��� ��� ������� ������)
	size_t 	head;			///<  ������� ��������� �� ������ ������ ��� MASTER ��� ������� �	����� ������� ������ [����������� MASTER]
	size_t 	tail;			///<  ������� ��������� �� ������ ������ ��� SLAVE  ��� �������� (���������) 			 [����������� SLAVE]
	size_t  pendingCount;

	FuncSingleCopy singleCopy;
	FuncDoubleCopy doubleCopy;
	FuncSetCallbackDMA setCallback;
};


	
int 	halRingBufferInit(HalRingBuffer* ringBuffer, void* buffer, size_t size, size_t count, FuncSingleCopy singleMemCopy, FuncDoubleCopy doubleMemCopy,  FuncSetCallbackDMA setCallback);
void 	halRingBufferPush(HalRingBuffer* ringBuffer, void* src, size_t count);
void 	halRingBufferPop(HalRingBuffer* ringBuffer, void* dst, size_t count);
void* 	halRingBufferHead(HalRingBuffer* ringBuffer);
void* 	halRingBufferTail(HalRingBuffer* ringBuffer);
int		halRingBufferCount(HalRingBuffer* ringBuffer);
int 	halRingBufferIsBusy(HalRingBuffer* ringBuffer);
int 	halRingBufferIsEmpty(HalRingBuffer* ringBuffer);
int 	halRingBufferIsFull(HalRingBuffer* ringBuffer);


#ifdef __cplusplus
		};
#endif


#endif
