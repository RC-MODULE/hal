#include "windows.h"

#ifdef __cplusplus
    extern "C" {
#endif

void halSleep(int msecs){
	::Sleep(msecs);
}

#ifdef __cplusplus
	};
#endif
