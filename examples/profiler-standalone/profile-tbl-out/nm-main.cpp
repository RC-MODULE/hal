#include <stdio.h>

//#define DISABLE_STOPWATCH
#include "stopwatch.h"
#include "nmprofiler.h"

volatile int g=1;
extern "C"{
	 

int sum(int * a, int size){
	int sum=0;
	for(int i=0; i<size;i++){
		sum+=a[i];
	}
	g++;
	return sum;
}


int mycos(){
	for (int i=0; i<1000; i++){
		g+=g*g;
	}
	return 0;
}

	
	
int mysin(){
	for (int i=0; i<10/4; i++){
		g++;
		
	}
	mycos();
	return g;
}
	 
};


STOPWATCH_CREATE(tmr_all,"tmr_all");			//  tmr_all.	
STOPWATCH_CREATE(tmr_sin,"tmr_sin");			//  tmr_sin.	
STOPWATCH_CREATE(tmr_cos,"tmr_cos");			//  tmr_cos.

int  main(){
	
	PROFILER_START();
	
	
	for(int i=0; i<10; i++){
	
		STOPWATCH_START(tmr_sin,"tmr_sin");			//  tmr_sin.	
		mysin();
		mysin();
		STOPWATCH_STOP(tmr_sin);					// tmr_sin.
	
		STOPWATCH_START(tmr_sin,"tmr_sin");			//  tmr_sin.	
		mysin();
		STOPWATCH_STOP(tmr_sin);					// tmr_sin.
	
	
		
		STOPWATCH_START(tmr_cos,"tmr_cos");			//  tmr_cos.
		mycos();
		mycos();
		mycos();
		STOPWATCH_STOP(tmr_cos);					//  tmr_cos.
		
	}
	
	STOPWATCH_STOP(tmr_all);						//  tmr_all.
	
	
	PROFILER_PRINT2TBL();				//  std 
	STOPWATCH_PRINT2TBL();			
	
	return 10;

}