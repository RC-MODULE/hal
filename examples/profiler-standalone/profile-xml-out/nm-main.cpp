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
	
	//STOPWATCH_START(tmr_all,"tmr_all");			// ��������� ������ tmr_all.	
	
	//static Stopwatch StopwatchID("StopwatchName");	
	
	
	for(int i=0; i<10; i++){
		//STOPWATCH_START(tmr_sin,"tmr_sin");			// ��������� ������ tmr_sin.	
		mysin();
		mysin();
		mysin();
		//STOPWATCH_STOP(tmr_sin);					// ������������� ������ tmr_sin.
	
		//STOPWATCH_START(tmr_cos,"tmr_cos");			// ��������� ������ tmr_cos.
		mycos();
		mycos();
		mycos();
		//STOPWATCH_STOP(tmr_cos);					// ������������� ������ tmr_cos.
		//printf("%d\n",i);
		//test();
	}
	
	//STOPWATCH_STOP(tmr_all);						// ������������� ������ tmr_all.
	
	//profiler_reset();

	nmprofiler_print2xml();				// ������� ��������� �������������� � std ������� 

	//STOPWATCH_PRINT2TBL();			// ������� ��������� ������� �� ������� 
	

	return g;
	
}