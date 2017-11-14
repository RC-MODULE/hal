#ifndef _SLEEP_H_INCLUDED_
#define _SLEEP_H_INCLUDED_
#ifdef __cplusplus
		extern "C" {
#endif

void halSleep(int msecs);

void sleep_ms(int ms);
void sleep_ticks(int ticks);


#ifdef __cplusplus
		};
#endif

#endif // _SLEEP_H_INCLUDED_
	
