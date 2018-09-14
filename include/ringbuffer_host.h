#ifndef HAL_HOST_RING_INCLUDED
#define HAL_HOST_RING_INCLUDED
#include <string.h>
#include <time.h>



#define MIN(a,b) ((a) > (b) ? (b) : (a))

#define RING_BUFFER_DECLARED	0xDEC1A8ED
#define RING_BUFFER_ALLOCATED	0xA10CA7ED
#define RING_BUFFER_MEM_ERROR	0xDEADF00D
#define RING_BUFFER_ERROR		0xBEDABEDA
#define RING_BUFFER_OK			0x0
#define RING_BUFFER_INCORRECT_COUNT -1
#define RING_BUFFER_SLEEP 100
#define EXIT_ON_TIMEOUT       1
#define EXIT_ON_COMPLETED     0
#define EXIT_ON_FULL_BUFFER   2
#define EXIT_ON_EMPTY_BUFFER  2

#ifdef __NM__
#define bytesizeof(T) (sizeof(T)<<2)
#else
#define bytesizeof(T) sizeof(T)
#endif 

#ifdef __NM__
#define sizeof32(t) sizeof(t)
#else
#define sizeof32(t) (sizeof(t)/4)
#endif


#include "dma.h"
#include "sleep.h"
#include "stddef.h"
#include "ringbuffer.h"


struct HalHostRingBuffer{
	unsigned 	data;			///<  физический адрес кольцевого буфера входных данных в памяти NM
	unsigned 	maxCount;		///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	maxCountMinus1;	///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	size;			///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	size_t 	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]

	unsigned remoteHeadAddr;	///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned remoteTailAddr;	//<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	int processor;
	unsigned sleep;

	FuncSingleCopy singleCopy;
	FuncDoubleCopy doubleCopy;

};

	
int 	halHostRingBufferInit	(HalHostRingBuffer* ringBuffer, unsigned remoteRingBufferAddress, int core=0);
void 	halHostRingBufferPush	(HalHostRingBuffer* ringBuffer, void* src, size_t count);
void 	halHostRingBufferPop	(HalHostRingBuffer* ringBuffer, void* dst, size_t count);
int		halHostRingBufferIsFull (HalHostRingBuffer* ringBuffer);
int		halHostRingBufferIsEmpty(HalHostRingBuffer* ringBuffer);


#endif
