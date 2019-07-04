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

//typedef  void* (*tmemcpy)(void *dst,  void const *src, unsigned int size);
typedef  void (*tmemcopy32)(const void *src, void *dst, unsigned int size);

#ifdef __cplusplus
		};
#endif


//typedef  void *(*t_bytecpy)(void *to, int toIndex, void const *from, int fromIndex, size_t size) ;


template <class T, int SIZE> struct HalRingBufferT{
	unsigned 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	bool 		headLocked;
	bool		tailLocked;
	#ifdef __NM__
	T	 		data[SIZE];			///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	#else 
	T*	 		data;				///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	#endif
	HalRingBufferT(){
		#ifndef __NM__
		data = halMalloc32(SIZE*sizeof32(T));
		#else 
		head=0;
		tail=0;
		#endif
	}
	~HalRingBufferT(){
		#ifndef __NM__
		halFree32(data);
		data=0;
		#else 
		head=0xDEAD;
		tail=0xDEAD;
		#endif
	}
};

//extern int statusDMA;
template <class T, int SIZE> struct HalRingBufferConnector{
	unsigned& 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned&	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	T*	 		data;
	//HalRingBufferT<T,SIZE> 
	//T*	 		data;			///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	//unsigned*	pHead;
	//unsigned*	pTail;
	//HalRingBufferT<T,SIZE>* ringBuffer;
	unsigned 	polltime;		///<  время опроса кольцевого буфера если пуст или заполнен в мс
	tmemcopy32     memcpyPush;		///<  указатель на функцию копирования типа memcpy
	tmemcopy32     memcpyPop;		///<  указатель на функцию копирования типа memcpy
	
	HalRingBufferConnector(HalRingBufferT<T,SIZE>& ringBuffer, tmemcopy32 _memcpyPush=halCopy_32s, tmemcopy32 _memcpyPop=halCopy_32s, int polltime=10):head(ringBuffer.head),tail(ringBuffer.tail),data(ringBuffer.data){
		memcpyPush=_memcpyPush;
		memcpyPop =_memcpyPop;
		polltime=0;
		//pHead=0;
		//pTail=0;
	}
	//void connect(HalRingBufferT<T,SIZE>& ringBuffer, tmemcopy32 _memcpyPush=memcpy, tmemcopy32 _memcpyPop=memcpy, int polltime=10){
	//	memcpyPush=_memcpyPush;
	//	memcpyPop =_memcpyPop;
	//	//data=ringBuffer->data;
	//	//pHead=&ringBuffer->head;
	//	//pTail=&ringBuffer->tail;
	//}
	
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
		
		size_t tail    = this->tail;
		size_t posHead = head & (SIZE-1);			
		size_t posTail = tail & (SIZE-1);
		T* addrHead    = data+posHead;
		
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead || head==tail){
			size_t countToEnd = SIZE - posHead;
			if (count <= countToEnd){
				memcpyPush(src,addrHead,count*sizeof(T));
			}
		// [*******<Head>......<Tail>*****]
			else {
				int firstCount = countToEnd;
				int secondCount = count-countToEnd;
				memcpyPush(src,addrHead, firstCount*sizeof(T));
				memcpyPush(src+firstCount, data, secondCount*sizeof(T));
			}
		}
		else {
			memcpyPush(src,addrHead, count*sizeof(T));
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
			memcpyPop( addrTail, dst, count*sizeof(T));
		}
		// [*******<Head>......<Tail>*****]
		else {
			size_t countToEnd = SIZE - posTail;
			if (count <= countToEnd){
				memcpyPop(addrTail, dst, count*sizeof(T));
			}
			else {
				int firstCount = countToEnd;
				int secondCount= count-countToEnd;
				memcpyPop( addrTail, dst, firstCount*sizeof(T));
				memcpyPop( data, dst+firstCount,secondCount*sizeof(T));
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




#endif
