#include <stdio.h>

//#define DISABLE_STOPWATCH
#include "stopwatch.h"
#include "nmprofiler.h"

int g=1;
extern "C"{
	 

int sum(int * a, int size){
	int sum=0;
	for(int i=0; i<size;i++){
		sum+=a[i];
	}
	return sum;
}


int mycos(){
	for (int i=0; i<1000; i++){
		g+=1;
	}
	return 0;
}
	
	#define SSS 128000
int mysin(){
	int arr [SSS/4];
	
	printf("--");
	for (int i=0; i<SSS/4; i++){
		g++;
		
	}
	mycos();
	for(int i=0; i<SSS/4;i++){
			arr[i]=1;
		}
	int s=sum(arr,SSS/4);
	
	return s;
}
	 
	//int test();
};


//STOPWATCH_INIT();
int  main(){
	ProfilerData* pp=stopwatch_head();
	printf("===%x===\n",pp);
	
	
	
//for(int i=0; i<10000;i ++)
	printf("***************Profiler-output****************:\n");

	//nmprofiler_enable();
	nmprofiler_init();
	
	STOPWATCH_START(tmr_all,"tmr_all");			// ��������� ������ tmr_all.	
	
	int s;
	for(int i=0; i<10; i++){
		STOPWATCH_START(tmr_sin,"tmr_sin");			// ��������� ������ tmr_sin.	
		mysin();
		s=mysin();
		mysin();
		STOPWATCH_STOP(tmr_sin);					// ������������� ������ tmr_sin.
	
		
	   //
	
		STOPWATCH_START(tmr_cos,"tmr_cos");			// ��������� ������ tmr_cos.
		mycos();
		mycos();
		mycos();
		STOPWATCH_STOP(tmr_cos);					// ������������� ������ tmr_cos.
		printf("%d\n",i);
		
		//test();
	}
	
	STOPWATCH_STOP(tmr_all);						// ������������� ������ tmr_all.
	
	//profiler_reset();
	ProfilerData* p=stopwatch_head();
	printf("===%x===\n",p);
	nmprofiler_print2tbl();				// ������� ��������� �������������� � std ������� 

	//STOPWATCH_PRINT2TBL();			// ������� ��������� ������� �� ������� 
	

	return s;
	
}