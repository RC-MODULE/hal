#include <string.h>

struct DmaTask{
	void* src;
	void* dst;
	int size;
	int status;
};


typedef  int (*tcallback)();
//int dmaServerCallback();													// обработчик DMA прерывания по завершению задачи

template <int BUFFER_SIZE, int NUM_CHANNELS> class DmaServer {
	tcallback originCallback;												// адрес преустановленного обработчика DMA   
public:
	bool	isWorking;														// флаг сервер находится в режиме исполнения DMA задач
	HalRingBufferConnector<DmaTask,BUFFER_SIZE>  channel[NUM_CHANNELS];		// DMA каналы 
	HalRingBufferConnector<DmaTask,BUFFER_SIZE>* currentChannel;			// текущий ДМА канал
	DmaTask* currentTask;													// Указатель на текущую задчу. По нему в callback функции можно , например, выставить статус завершенной
	// Иниализация кольцевых буферов DMA задач
	// Иниализация DMA , сохранение обработчика ПДП
	DmaServer(tcallback dmaCallback){
		isWorking=false;
		currentChannel=&channel[0];
		for(int ch=0; ch<NUM_CHANNELS; ch++){
			channel[ch].create();											// выделяем кольцевой буфер задач
		}
		halDmaInit();														// инициализация ПДП (halDmaInit halDmaInitC halDmaInitA halDmaInitM ... )
		//originCallback=halDmaGetCallback();									// сохраняем текущий обработчик ПДП
		halDmaSetCallback(dmaCallback);								// назначаем новый callback (обработчик ПДП)
	}

	~DmaServer(){	
		halDmaSetCallback(originCallback);									// Восстановлиям исходный обработчик ПДП
	}
	
	// возвращает адрес кольцевого буфера канала для присоединения DMA клиентом 
	HalRingBufferData<DmaTask,BUFFER_SIZE>* getChannelBuffer(int idxChannel){
		return channel[idxChannel].container;
	}
	// возвращает статус , что все задачи во всех каналах выполнены
	inline bool isJobCompleted(){
		for (int ch=0; ch<NUM_CHANNELS; ch++)
			if (!channel[ch].isEmpty())
				return false;
		return true;
	}
	
	// возвращает статус , что все задачи в данном канале выполнены
	inline bool isChannelCompleted(int ch){
		return channel[ch].isEmpty();
	}
	
	
	// здесь установливается порядок обхода задач, (например, стратегия где задачи выибираются по одной поочередно из каждого канала. В данном примере важно чтобы NUM_CHANNELS - степерь двойка
	inline void startNextTask(){
		isWorking=true;					// флаг что сервер ишачит
		static int currentChannelIndex=0;
		for (int i=0; i<NUM_CHANNELS; i++){
			currentChannel=&channel[(currentChannelIndex++)&(NUM_CHANNELS-1)];
			if (!currentChannel->isEmpty()){
				currentTask=currentChannel->ptrTail();
				//if (currentTask->size!=256){
				//	printf("start DMA Task: channel[%d] %x %x %x %x %x\n",
				//		currentChannelIndex,
				//		currentChannel->container->bufferId,
				//		currentTask->src,
				//		currentTask->dst,
				//		currentTask->size,
				//		currentTask->status);
				//
				//}
				halDmaStart(currentTask->src,currentTask->dst,currentTask->size);
				return;
			}
		}
		isWorking=false;				// флаг что сервер чилит
	}
	// Запустить все задачи на на исполнение 
	void startJob(){
		if (isWorking){						// Не отвелекай ,  уже работаю 
			return;
		}
		//printf("New job started\n");
		//currentChannel=&channel[startChannel];
		startNextTask();
	}
};


template <int BUFFER_SIZE>	class DmaClient: HalRingBufferConnector<DmaTask,BUFFER_SIZE> {
	public:

	inline void bind(void* channelBuffer){
		HalRingBufferConnector<DmaTask,BUFFER_SIZE>::connect(channelBuffer);
	}
	inline void add(DmaTask& task){
		HalRingBufferConnector<DmaTask,BUFFER_SIZE>::push(&task,1);
	}
	void run(){
		
	}
};
