#include <stdio.h>

//#define DISABLE_STOPWATCH
//#include "stopwatch.h"
#include "nmprofiler.h"

int g=1;
extern "C"{
	 int mysin(){
		
		for (int i=0; i<1000; i++){
			g++;
			
		}

		return g;
	}
	 int mycos(){
		for (int i=0; i<1000; i++){
			g+=1;
		}
		return 0;
	}
	//int test();
};



//STOPWATCH_INIT();
int  main(){
	
	
//for(int i=0; i<10000;i ++)

	//nmprofiler_enable();
	nmprofiler_init();
	
	//STOPWATCH_START(tmr_all,"tmr_all");			// Запускаем таймер tmr_all.	
	
	//static Stopwatch StopwatchID("StopwatchName");	
	
	
	for(int i=0; i<10; i++){
		//STOPWATCH_START(tmr_sin,"tmr_sin");			// Запускаем таймер tmr_sin.	
		mysin();
		mysin();
		mysin();
		//STOPWATCH_STOP(tmr_sin);					// Останавливаем таймер tmr_sin.
	
		//STOPWATCH_START(tmr_cos,"tmr_cos");			// Запускаем таймер tmr_cos.
		mycos();
		mycos();
		mycos();
		//STOPWATCH_STOP(tmr_cos);					// Останавливаем таймер tmr_cos.
		//printf("%d\n",i);
		//test();
	}
	
	//STOPWATCH_STOP(tmr_all);						// Останавливаем таймер tmr_all.
	
	//profiler_reset();

	nmprofiler_print2xml();				// выводим результат профилирования в std формате 

	//STOPWATCH_PRINT2TBL();			// выводим результат замеров по таймеру 
	

	return g;
	
}