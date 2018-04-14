#ifndef HAL_HOST_INCLUDED 
#define HAL_HOST_INCLUDED 
extern "C"{
	int halOpen(char* absfile=0, ...);
	//int halGetShared(unsigned* addr,int *sh);
	int halSync(int val, unsigned processor=0);
	int halReadMemBlock (unsigned* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0);
	int halWriteMemBlock(unsigned* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0);
	int halGetResult(unsigned* result,  unsigned processor=0);
	int halSetTimeout(int msec);
	int halClose();
};

#endif