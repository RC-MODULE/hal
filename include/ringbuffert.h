#ifndef HAL_RINGBUFFERT_INCLUDED
#define HAL_RINGBUFFERT_INCLUDED

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

//typedef  void* (*tmemcopy)(void *dst,  void const *src, unsigned int size);
typedef  void* (*tmemcopy32)(const void *src, void *dst, unsigned int size);

#ifdef __cplusplus
		};
#endif


//typedef  void *(*t_bytecpy)(void *to, int toIndex, void const *from, int fromIndex, size_t size) ;


template <class T, int SIZE> struct HalRingBufferData{
	unsigned	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	unsigned 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	
	//bool 		headLocked;
	//bool		tailLocked;
	//long long	size;			/// long long because to do 64-bit align 
	//#ifdef __NM__
	T	 		data[SIZE];			///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	//#else 
	//T*	 		data;				///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	//#endif
	
	HalRingBufferData(){
		//size=SIZE;
		//#ifndef __NM__
		//data = (T*)halMalloc32(SIZE*sizeof32(T));
		//#else 
		head=0;
		tail=0;
		//#endif
	}
	~HalRingBufferData(){
		//printf("destrctor!\n");
		//#ifndef __NM__
		//halFree(data);
		//data=0;
		//#else 
		head=0xdeadb00f;
		tail=0xdeadb00f;
		//#endif
	}
	inline void init(){
		head=0;
		tail=0;
	}
	inline bool isEmpty() {
		return head == tail;
	}

	inline bool isFull() {
		return head == tail + SIZE;
	}

	inline T* ptrHead(){
		return data + (head & (SIZE-1));
		
	}
	
	inline T* ptrTail(){
		return data + (tail & (SIZE-1));
		
	}
	inline T* ptr(unsigned int idx){
		return data + (idx & (SIZE-1));
		
	}

};

//extern int statusDMA;

template <class T, int SIZE> struct HalRingBufferConnector{
public:
	T*	 		data;
	unsigned& 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned&	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	unsigned 	polltime;		///<  время опроса кольцевого буфера если пуст или заполнен в мс
	bool		headExternalControl;
	bool  		tailExternalControl;
	tmemcopy32 	memcopyPush;	///<  указатель на функцию копирования типа halCopy_32s
	tmemcopy32	memcopyPop;		///<  указатель на функцию копирования типа halCopy_32s

	HalRingBufferConnector(HalRingBufferData<T,SIZE>* ringBuffer, tmemcopy32 _memcopyPush=halCopyRISC, tmemcopy32 _memcopyPop=halCopyRISC):head(ringBuffer->head),tail(ringBuffer->tail),data(ringBuffer->data){
		memcopyPush=_memcopyPush;
		memcopyPop =_memcopyPop;
		polltime=10;
		headExternalControl=false;
	  	tailExternalControl=false;
	}
	
	inline bool isEmpty() {
		return head == tail;
	}

	inline bool isFull() {
		return head == tail + SIZE;
	}

	inline T* ptrHead(){
		return data + (head & (SIZE-1));
		
	}
	
	inline T* ptrTail(){
		return data + (tail & (SIZE-1));
		
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
				memcopyPush(src,addrHead,count*sizeof(T));
			}
		// [*******<Head>......<Tail>*****]
			else {
				int firstCount = countToEnd;
				int secondCount = count-countToEnd;
				memcopyPush(src,addrHead, firstCount*sizeof(T));
				memcopyPush(src+firstCount, data, secondCount*sizeof(T));
			}
		}
		else {
			memcopyPush(src,addrHead, count*sizeof(T));
		}
		if (!headExternalControl)
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
			memcopyPop( addrTail, dst, count*sizeof(T));
		}
		// [*******<Head>......<Tail>*****]
		else {
			size_t countToEnd = SIZE - posTail;
			if (count <= countToEnd){
				memcopyPop(addrTail, dst, count*sizeof(T));
			}
			else {
				int firstCount = countToEnd;
				int secondCount= count-countToEnd;
				memcopyPop( addrTail, dst, firstCount*sizeof(T));
				memcopyPop( data, dst+firstCount,secondCount*sizeof(T));
			}
		}
		if (!tailExternalControl)
			tail += count;
	}
};


template <class T, int SIZE> struct HalRingBufferDataDMA{
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
