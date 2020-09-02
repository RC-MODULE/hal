#ifndef HAL_HOST_INCLUDED 
#define HAL_HOST_INCLUDED 

//#ifdef _WIN64
//#define ARCH_ADDR unsigned long long 
#define ARCH_ADDR size_t
//#else 
//#define ARCH_ADDR unsigned long 
//#endif

enum RetValue
	{
		HAL_OK              = 0, // Ok
		HAL_ERROR           = 1, // Error
		HAL_TIMEOUT         = 2, // Timeout of wait for operation
		HAL_FILE            = 3, // Cannot find file for load
		HAL_BADADDRESS      = 4, // Bad address ranges
		HAL_NOT_IMPLEMENTED = -1 // Not implemented
	};

extern "C"{
	int halOpen(char* absfile=0, ...);
	//int halGetShared(unsigned* addr,int *sh);
	int halSync(int val, unsigned processor=0);
	//void* halSyncAddr(void* sendAddr, int processor = 0);
	size_t halSyncAddr(void* sendAddr, int processor = 0);
	int halReadMemBlock (const void* dstHostAddr, size_t srcBoardAddr, size_t size32, unsigned processor=0);
	int halWriteMemBlock(const void* srcHostAddr, size_t dstBoardAddr, size_t size32, unsigned processor=0);
	int halGetResult(unsigned* result,  unsigned processor=0);
	int halSetTimeout(int msec);
	void halSelectBoard(int index);
	int halClose();
};

#endif