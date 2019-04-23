#ifndef HAL_RING_INCLUDED
#define HAL_RING_INCLUDED
#include <string.h>
#include <time.h>
#include "ringbuffer.h"
#include "section-hal.h"

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
#define sizeof32(t) (sizeof(t)*4)
#endif

#ifdef __cplusplus
extern "C"{
#endif


typedef  void (*memcpy_ptr)(const void *, void *, size_t) ;
typedef  void *(*t_bytecpy)(void *to, int toIndex, void const *from, int fromIndex, size_t size) ;

extern int statusDMA;
#include "dma.h"
#include "hal.h"
struct HalRingBuffer{
	int* 	data;			///<  физический адрес кольцевого буфера входных данных 
	size_t 	maxCount;		///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	maxCountMinus1;	///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	size;			///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	size_t 	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	size_t  pendingCount;

	FuncSingleCopy singleCopy;
	FuncDoubleCopy doubleCopy;
	FuncSetCallbackDMA setCallback;
};

static HalRingBuffer* activeRingBuffer=0;

INSECTION(".text_hal") int halRingBufferCount(HalRingBuffer* ringBuffer) {
	return ringBuffer->head - ringBuffer->tail;
}

INSECTION(".text_hal")	int halRingBufferInit(HalRingBuffer* ringBuffer, void* buffer, size_t size, size_t count, FuncSingleCopy singleMemCopy, FuncDoubleCopy doubleMemCopy,  FuncSetCallbackDMA setCallback)
{
	if (buffer==0) return RING_BUFFER_MEM_ERROR;
	if ((count&(count-1))!=0) return RING_BUFFER_INCORRECT_COUNT;
	ringBuffer->data = (int*)buffer;
	ringBuffer->maxCount = count;
	ringBuffer->maxCountMinus1=count-1;
	ringBuffer->size = size;
	ringBuffer->head=0;
	ringBuffer->tail=0;
	ringBuffer->pendingCount=0;
	ringBuffer->singleCopy=singleMemCopy;
	ringBuffer->doubleCopy=doubleMemCopy;
	ringBuffer->setCallback=setCallback;

	return RING_BUFFER_OK;
}

INSECTION(".text_hal")	static int halRingBufferPushCallback(){
	activeRingBuffer->head += activeRingBuffer->pendingCount;
	activeRingBuffer->pendingCount =0;
	return 0;
}

INSECTION(".text_hal")	static int halRingBufferPopCallback(){
	activeRingBuffer->tail += activeRingBuffer->pendingCount;
	activeRingBuffer->pendingCount =0;
	return 0;
}


INSECTION(".text_hal")	void halRingBufferPush(HalRingBuffer* ringBuffer, void* src, size_t count){
	while (ringBuffer->tail+ringBuffer->maxCount-ringBuffer->head < count)
		halSleep(RING_BUFFER_SLEEP);
	
	if (ringBuffer->setCallback){
		while (ringBuffer->pendingCount)
			halSleep(RING_BUFFER_SLEEP);
		ringBuffer->pendingCount=count;
		ringBuffer->setCallback(halRingBufferPushCallback);
		activeRingBuffer=ringBuffer;
	}

	
	size_t tail    = ringBuffer->tail;
	size_t posHead = ringBuffer->head & ringBuffer->maxCountMinus1;			
	size_t posTail = tail & ringBuffer->maxCountMinus1;
	int* addrHead  = ringBuffer->data + posHead*ringBuffer->size;
	
	// [.......<Tail>******<Head>.....]
	if (posTail<posHead || ringBuffer->head==ringBuffer->tail){
		size_t countToEnd = ringBuffer->maxCount - posHead;
		if (count <= countToEnd){
			ringBuffer->singleCopy(src, addrHead ,count*ringBuffer->size);
		}
	// [*******<Head>......<Tail>*****]
		else {
			int firstSize =countToEnd*ringBuffer->size;
			int secondSize=(count-countToEnd)*ringBuffer->size;
			ringBuffer->doubleCopy(src, (int*)src+firstSize, addrHead, ringBuffer->data, firstSize, secondSize);
		}
	}
	else {
		ringBuffer->singleCopy(src, addrHead, count*ringBuffer->size);
	}

	if (ringBuffer->setCallback==0)
		ringBuffer->head += count;
}

INSECTION(".text_hal")	void halRingBufferPop(HalRingBuffer* ringBuffer, void* dst, size_t count){
	while (ringBuffer->head-ringBuffer->tail < count)
		halSleep(RING_BUFFER_SLEEP);
	
	if (ringBuffer->setCallback){
		while (ringBuffer->pendingCount)
			halSleep(RING_BUFFER_SLEEP);
		ringBuffer->pendingCount=count;
		ringBuffer->setCallback(halRingBufferPopCallback);
		activeRingBuffer=ringBuffer;
	}

	size_t head    = ringBuffer->head;
	size_t posHead = head & ringBuffer->maxCountMinus1;			
	size_t posTail = ringBuffer->tail & ringBuffer->maxCountMinus1;
	int* addrTail  = ringBuffer->data + posTail*ringBuffer->size;
	
	// [.......<Tail>******<Head>.....]
	if (posTail<posHead){
		ringBuffer->singleCopy(addrTail, dst, count*ringBuffer->size);
	}
	// [*******<Head>......<Tail>*****]
	else {
		size_t countToEnd = ringBuffer->maxCount - posTail;
		if (count <= countToEnd){
			ringBuffer->singleCopy(addrTail, dst, count*ringBuffer->size);
		}
		else {
			int firstSize = countToEnd*ringBuffer->size;
			int secondSize= (count-countToEnd)*ringBuffer->size;
			ringBuffer->doubleCopy(addrTail, ringBuffer->data, dst, (int*)dst+firstSize, firstSize, secondSize);
		}
	}
	if (ringBuffer->setCallback==0)
		ringBuffer->tail += count;
}

INSECTION(".text_hal")	void* halRingBufferHead(HalRingBuffer* ringBuffer){
	size_t posHead = ringBuffer->head & ringBuffer->maxCountMinus1;			
	int* addrHead  = ringBuffer->data + posHead*ringBuffer->size;
	return addrHead;
}

INSECTION(".text_hal")	void* halRingBufferTail(HalRingBuffer* ringBuffer){
	size_t posTail = ringBuffer->tail & ringBuffer->maxCountMinus1;
	int* addrTail  = ringBuffer->data + posTail*ringBuffer->size;
	return addrTail;
}

INSECTION(".text_hal")	int halRingBufferIsBusy(HalRingBuffer* ringBuffer){
	if (ringBuffer->setCallback==0)
		return false;
	return ringBuffer->pendingCount;
}

INSECTION(".text_hal")	int 	halRingBufferIsEmpty(HalRingBuffer* ringBuffer) {
	return ringBuffer->head == ringBuffer->tail;

}
INSECTION(".text_hal")	int 	halRingBufferIsFull(HalRingBuffer* ringBuffer) {
	return ringBuffer->head == ringBuffer->tail + ringBuffer->maxCount;
}


#ifdef __cplusplus
		};
#endif


#endif
