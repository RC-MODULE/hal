
#define nm64u unsigned __int64
#define __int64 __int64
#define nm32u unsigned int
#define nm32s int


#ifdef __NM__
#define DISABLE_PRINTF() extern "C" {	int printf ( const char * format, ... ){	return 0;	}};
#else 
#define DISABLE_PRINTF() 
#endif

extern "C" {
	void halSetActiveHeap(int heapNo);
};

