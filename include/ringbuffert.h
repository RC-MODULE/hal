#ifndef HAL_RING_INCLUDED
#define HAL_RING_INCLUDED

#include "stdlib.h"
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

typedef  void* (*tmemcpy)(void *dst,  void const *src, unsigned int size);

//typedef  void *(*t_bytecpy)(void *to, int toIndex, void const *from, int fromIndex, size_t size) ;


template <class T, int SIZE> struct HalRingBuffer{
	unsigned 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	bool 		headLocked;
	bool		tailLocked;
	#ifdef __NM__
	T	 		data[SIZE];			///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	#else 
	T*	 		data;				///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	#endif
	HalRingBuffer(){
		#ifndef __NM__
		data = halMalloc32(SIZE*sizeof32(T));
		#else 
		head=0;
		tail=0;
	}
	~HalRingBuffer(){
		#ifndef __NM__
		halFree32(data);
		data=0;
		#else 
		head=0xDEAD;
		tail=0xDEAD;
	}
}

//extern int statusDMA;
template <class T, int SIZE> struct HalRingBufferConnector{
	
	T*	 		data[SIZE];		///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	unsigned*	pHead;
	unsigned*	pTail;
	unsigned 	polltime;		///<  время опроса кольцевого буфера если пуст или заполнен в мс
	tmemcpy     memcpyPush;		///<  указатель на функцию копирования типа memcpy
	tmemcpy     memcpyPop;		///<  указатель на функцию копирования типа memcpy
	
	HalRingBufferT(int pollTime=10){
		memcpyPush=0;
		memcpyPop =0;
		polltime=pollTime;
		head=0;
		tail=0;
	}
	void connect(void* ringBuffer, tmemcpy memcpyPush=memcpy, tmemcpy memcpyPop=memcpy, int polltime=10){
		memcpyPush=memcpy32;
		memcpyPop =memcpy32;
		data=ringBuffer->data;
		pHead=&ringBuffer->head;
		pTail=&ringBuffer->tail;
	}
	
	inline bool isEmpty() {
		return *phead == *ptail;
	}

	inline bool isFull() {
		return *phead == tail + SIZE;
	}

	inline T* ptrHead(){
		return data + head & (SIZE-1);
	}
	
	inline T* ptrTail(){
		return data + tail & (SIZE-1);
	}

	void 	push   (const T* src, 	size_t count){
		while (tail+SIZE-head < count)
			halSleep(polltime);
		
		size_t tail    = this->tail;
		size_t posHead = head & (SIZE-1);			
		size_t posTail = tail & (SIZE-1);
		T* addrHead    = data+posHead;
		
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead || head==tail){
			size_t countToEnd = SIZE - posHead;
			if (count <= countToEnd){
				memcpyPush(addrHead,src,count*sizeof(T));
			}
		// [*******<Head>......<Tail>*****]
			else {
				int firstCount = countToEnd;
				int secondCount = count-countToEnd;
				memcpyPush(addrHead,src, firstCount*sizeof(T));
				memcpyPush(data, src+firstCount, secondCount*sizeof(T));
			}
		}
		else {
			memcpyPush(addrHead, src, count*sizeof(T));
		}
		head += count;
	}


	void pop(T* dst, size_t count){
		while (head-tail < count)
			halSleep(polltime);
	
		size_t head = this->head;
		size_t posHead = head & (SIZE-1);			
		size_t posTail = tail & (SIZE-1);
		T* addrTail    = data + posTail;
	
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead){
			memcpyPop( dst, addrTail, count*sizeof(T));
		}
		// [*******<Head>......<Tail>*****]
		else {
			size_t countToEnd = SIZE - posTail;
			if (count <= countToEnd){
				memcpyPop(dst, addrTail, count*sizeof(T));
			}
			else {
				int firstCount = countToEnd;
				int secondCount= count-countToEnd;
				memcpyPop( dst,  addrTail,  firstCount*sizeof(T));
				memcpyPop( dst+firstCount, data, secondCount*sizeof(T));
			}
		}
		tail += count;
	}
};


template <class T, int SIZE> struct HalRingBufferTDMA{
//	size_t 		maxCount;		///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
//	size_t 		maxCountMinus1;	///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
//	size_t 		size;			///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	unsigned 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	size_t		pendingCount;
	T	 		data[SIZE];			///<  физический адрес кольцевого буфера входных данных 
	int 		polltime;
	FuncSingleCopy singleCopy;
	FuncDoubleCopy doubleCopy;
	FuncSetCallbackDMA setCallback;
	
	int 	init   (T* buffer, 	size_t count, FuncSingleCopy singleMemCopy, FuncDoubleCopy doubleMemCopy,  FuncSetCallbackDMA setCallback);
	
	inline int isBusy(){
		if (setCallback==0)
			return false;
		return pendingCount;
	}

	inline bool isEmpty() {
		return head == tail;
	}

	inline bool isFull() {
		return head == tail + SIZE;
	}

	inline T* ptrHead(){
		return data + head & (SIZE-1);
	}
	
	inline T* ptrTail(){
		return data + tail & (SIZE-1);
	}

	void 	push   (const T* src, 	size_t count){
		while (tail+SIZE-head < count)
			halSleep(polltime);
		
		//if (ringBuffer->setCallback){
		//	while (ringBuffer->pendingCount)
		//		halSleep(RING_BUFFER_SLEEP);
		//	ringBuffer->pendingCount=count;
		//	ringBuffer->setCallback(halRingBufferPushCallback);
		//	activeRingBuffer=ringBuffer;
		//}

		
		size_t tail    = this->tail;
		size_t posHead = head & (SIZE-1);			
		size_t posTail = tail & (SIZE-1);
		T* addrHead    = data+posHead;
		
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead || head==tail){
			size_t countToEnd = SIZE - posHead;
			if (count <= countToEnd){
				singleCopy(src, addrHead ,count*sizeof(T));
			}
		// [*******<Head>......<Tail>*****]
			else {
				int firstCount = countToEnd;
				int secondCount = count-countToEnd;
				doubleCopy(src, src+firstCount, addrHead, data, firstCount*sizeof(T), secondCount*sizeof(T));
			}
		}
		else {
			singleCopy(src, addrHead, count);
		}

		if (setCallback==0)
			head += count;
	}


	void pop(T* dst, size_t count){
		while (head-tail < count)
			halSleep(polltime);
	
		//if (setCallback){
		//	while (pendingCount)
		//		halSleep(RING_BUFFER_SLEEP);
		//	pendingCount=count;
		//	setCallback(halRingBufferPopCallback);
		//	activeRingBuffer=ringBuffer;
		//}

		size_t head = this->head;
		size_t posHead = head & (SIZE-1);			
		size_t posTail = tail & (SIZE-1);
		T* addrTail    = data + posTail;
	
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead){
			singleCopy(addrTail, dst, count*sizeof(T));
		}
		// [*******<Head>......<Tail>*****]
		else {
			size_t countToEnd = SIZE - posTail;
			if (count <= countToEnd){
				singleCopy(addrTail, dst, count*sizeof(T));
			}
			else {
				int firstCount = countToEnd;
				int secondCount= count-countToEnd;
				doubleCopy(addrTail, data, dst, dst+firstCount, firstCount*sizeof(T), secondCount*sizeof(T));
			}
		}
		if (setCallback==0)
			tail += count;
	}
};


#ifdef __cplusplus
		};
#endif


#endif
