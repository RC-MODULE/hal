#ifndef STOPWATCH_INCLUDED
#define STOPWATCH_INCLUDED

#include "nmprofiler.h"
//#include "profiler_nmcout.h"
extern "C" {


//==================== Stopwatch class ===============================


/*! \page manual_timer ������ � ��������� 
	��� ������� ������� ���������� ��������� �� ���������� ���� ���������� ����� Stopwatch. 
	
	������ � ������� �������� �������������� � ������� �������-������ ������  Start() � Stop(), ���� ��� �������� ����� ������������ ���������:
	\code 
	STOPWATCH_START(timer_id,"timer_name");	
	STOPWATCH_STOP(timer_id");	
	\endcode
	
	����������� ������ ������������� ������� ������� ������ �� ����� ���� ProfilerData. ��������� �� ������ ��������� ���������� ������� stopwatch_head(). 
	���������� ���������� ���������� ����� ������� �� stdout � ������� �������� STOPWATCH_PRINT() � STOPWATCH_PRINT2XML().
	
	������������ ����� ����� ������� 8 ��������.
	
	\code 
	
#include <stdio.h>

//#define DISABLE_STOPWATCH
#include "stopwatch.h"
extern "C"{
    int mysin(){
        int g=1;
        for (int i=0; i<1000; i++){
            g*=2;
        }

        return g;
    }
    void mycos(){
        for (int i=0; i<1000; i++){
        }
    }
};
STOPWATCH_INIT();

int  main(){
    
	
    STOPWATCH_START(tmr_all,"tmr_all");             // ��������� ������ tmr_all.    
    
    for(int i=0; i<3; i++){
        STOPWATCH_START(tmr_sin,"tmr_sin");         // ��������� ������ tmr_sin.    
        mysin();
        mysin();
        mysin();
        STOPWATCH_STOP(tmr_sin);                    // ������������� ������ tmr_sin.
        
        
        STOPWATCH_START(tmr_cos,"tmr_cos");         // ��������� ������ tmr_cos.
        mycos();
        mycos();
        mycos();
        STOPWATCH_STOP(tmr_cos);                    // ������������� ������ tmr_cos.
    
    }
    STOPWATCH_STOP(tmr_all);                        // ������������� ������ tmr_all.

    //STOPWATCH_PRINT2XML();     // ������� ��������� ������� �� ������� � xml ������� 
    STOPWATCH_PRINT();           // ������� ��������� ������� �� ������� 
	
    return 123;
}
\endcode
	
*/
	
//!  �����-������ ��� ������ ������� ���������� �� ���������� ����  
/*!
	���������� ������ ������������� ������� ������� ������ �� ����� ���� ProfilerData. ��������� �� ������ ��������� ��������� ������� stopwatch_head. 
*/

extern ProfilerData *pHeadStopwatchProfile; // ��������� �� ������ ������� � ������
extern ProfilerData *pTailStopwatchProfile; // ��������� �� ��������� ������� � ������
//extern ProfilerData *pLastStopwatchProfile; // ��������� �� ��������� ����������� �������


class Stopwatch {
public:
	//! ��������� �� ���������� ������� 
	ProfilerData profdata;
	
	//! ���������� � ������ ������� 
	//! \param str ��� �������
	Stopwatch(const char* str){
		
		profdata.next=0;
		profdata.summary=0;
		profdata.calls=0;
		profdata.funcaddr=(unsigned)this;
		strncpy((char*)profdata.funcname, str, MAX_FUNCNAME_LENGTH);
		profdata.stopwatch = 0;

		if (pHeadStopwatchProfile==0){
			pHeadStopwatchProfile=&profdata;
			pTailStopwatchProfile=&profdata;
		}
		else {
			pTailStopwatchProfile->next=int(&profdata)-int(pTailStopwatchProfile);	
			pTailStopwatchProfile=&profdata;
		}
	}
	
	//! ���������� ������� �������� ������� � profdata.stopwatch
	void start() {
		profdata.stopwatch=clock();
		//pLastStopwatchProfile=&profdata;
	}	
	
	//! ��������� ������� ����� ������� �������� ������� � �������� �� ������ ���������� Start ��� Stop. 	����������� ���������� ������� � profdata.summary. ���������� ������� ������� ������� � profdata.stopwatch. �������������� profdata.calls 
	void stop(){
		unsigned t=clock();
		profdata.calls++;
		profdata.summary+=t-profdata.stopwatch;
		profdata.stopwatch=t;
		//pLastStopwatchProfile=&profdata;
	}	

};
	


 //! \defgroup stopwatchfuncs ������� ������ � ��������� 
 //! \{

	//! ������� ���������� ��������� �� ������ ��������� �������������� � ������� ������ 	
	ProfilerData* stopwatch_head(){
		return pHeadStopwatchProfile;	
	}
	
	// ��������� ��� ������� ��������
	//inline void stopwatch_print2tbl()	{ nmprofiler_print2tbl(stopwatch_head()); }
	//inline void stopwatch_print2xml()	{ nmprofiler_print2xml(stopwatch_head()); }
#ifndef DISABLE_STOPWATCH 

	//! ������ ������ �������� �������� ����� stdout � ���� ������� 
	#define STOPWATCH_PRINT2TBL()	printf("stopwatch:\n"); if (stopwatch_head()) nmprofiler_print2tbl(stopwatch_head()); else printf("empty\n");
	
	//! ������ ������ �������� �������� ����� stdout � xml-������� 
	#define STOPWATCH_PRINT2XML()	if (stopwatch_head()) nmprofiler_print2xml(stopwatch_head());
	
	//! ������� � ��������� ������
	#define STOPWATCH_CREATE(StopwatchID,StopwatchName)	\
		static Stopwatch StopwatchID(StopwatchName);	


	//! ������� � ��������� ������
	#define STOPWATCH_START(StopwatchID,StopwatchName)	\
		StopwatchID.start(); 
		
		//static Stopwatch StopwatchID(StopwatchName);	\

	//! ������� � ��������� ������
	#define STOPWATCH_RESUME(StopwatchID)	\
		static Stopwatch StopwatchID(StopwatchName);	\
		StopwatchID.start();

	//! ������������� ������	
	#define STOPWATCH_STOP(StopwatchID) \
		StopwatchID.stop();
	
#else 
	#define STOPWATCH_START(StopwatchID,StopwatchName)
	#define STOPWATCH_STOP(StopwatchID) 
	#define STOPWATCH_PRINT2TBL()	
	#define STOPWATCH_PRINT2XML()	
#endif 

//! \}


};
#endif 
