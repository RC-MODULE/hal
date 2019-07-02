#ifndef STOPWATCH_INCLUDED
#define STOPWATCH_INCLUDED

#include "nmprofiler.h"
//#include "profiler_nmcout.h"
extern "C" {


//==================== Stopwatch class ===============================


/*! \page manual_timer Работа с таймерами 
	Для замеров времени исполнения программы на фрагментах кода существует класс Stopwatch. 
	
	Запуск и останов таймеров осуществляется с помощью функций-членов класса  Start() и Stop(), либо для удобства можно пользоваться макросами:
	\code 
	STOPWATCH_START(timer_id,"timer_name");	
	STOPWATCH_STOP(timer_id");	
	\endcode
	
	Объявленные классы автоматически создают связный список из полей типа ProfilerData. Указатель на первую структуру возвращает функция stopwatch_head(). 
	Аналогично профайлеру статистику можно вывести на stdout с помощью макросов STOPWATCH_PRINT() и STOPWATCH_PRINT2XML().
	
	максимальная длина имени таймера 8 сивмолов.
	
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
    
	
    STOPWATCH_START(tmr_all,"tmr_all");             // Запускаем таймер tmr_all.    
    
    for(int i=0; i<3; i++){
        STOPWATCH_START(tmr_sin,"tmr_sin");         // Запускаем таймер tmr_sin.    
        mysin();
        mysin();
        mysin();
        STOPWATCH_STOP(tmr_sin);                    // Останавливаем таймер tmr_sin.
        
        
        STOPWATCH_START(tmr_cos,"tmr_cos");         // Запускаем таймер tmr_cos.
        mycos();
        mycos();
        mycos();
        STOPWATCH_STOP(tmr_cos);                    // Останавливаем таймер tmr_cos.
    
    }
    STOPWATCH_STOP(tmr_all);                        // Останавливаем таймер tmr_all.

    //STOPWATCH_PRINT2XML();     // выводим результат замеров по таймеру в xml формате 
    STOPWATCH_PRINT();           // выводим результат замеров по таймеру 
	
    return 123;
}
\endcode
	
*/
	
//!  Класс-таймер для замера времени выполнения на фрагментах кода  
/*!
	Обявленные классы автоматически создают связный список из полей типа ProfilerData. Указатель на первую стурктуру возращает функция stopwatch_head. 
*/

extern ProfilerData *pHeadStopwatchProfile; // указатель на первый профайл в списке
extern ProfilerData *pTailStopwatchProfile; // указатель на последний профайл в списке
//extern ProfilerData *pLastStopwatchProfile; // указатель на последний обновленный профайл


class Stopwatch {
public:
	//! Структура по статистике вызовов 
	ProfilerData profdata;
	
	//! Констуктор с именем таймера 
	//! \param str имя таймера
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
	
	//! Запоминает отсчеку текущего времени в profdata.stopwatch
	void start() {
		profdata.stopwatch=clock();
		//pLastStopwatchProfile=&profdata;
	}	
	
	//! Вычисляет разницу между текущей отсечкой времени и отсечкой на момент последнего Start или Stop. 	Накапливает полученную разницу в profdata.summary. Запоминает текущую отсчеку времени в profdata.stopwatch. Инкрементирует profdata.calls 
	void stop(){
		unsigned t=clock();
		profdata.calls++;
		profdata.summary+=t-profdata.stopwatch;
		profdata.stopwatch=t;
		//pLastStopwatchProfile=&profdata;
	}	

};
	


 //! \defgroup stopwatchfuncs Функции работы с таймерами 
 //! \{

	//! Функция возвращает указатель на первую структуру профилирования в связном списке 	
	ProfilerData* stopwatch_head(){
		return pHeadStopwatchProfile;	
	}
	
	// Отключает все макросы таймеров
	//inline void stopwatch_print2tbl()	{ nmprofiler_print2tbl(stopwatch_head()); }
	//inline void stopwatch_print2xml()	{ nmprofiler_print2xml(stopwatch_head()); }
#ifndef DISABLE_STOPWATCH 

	//! макрос вывода значения таймеров через stdout в виде таблицы 
	#define STOPWATCH_PRINT2TBL()	printf("stopwatch:\n"); if (stopwatch_head()) nmprofiler_print2tbl(stopwatch_head()); else printf("empty\n");
	
	//! макрос вывода значения таймеров через stdout в xml-формате 
	#define STOPWATCH_PRINT2XML()	if (stopwatch_head()) nmprofiler_print2xml(stopwatch_head());
	
	//! Создает и запускает таймер
	#define STOPWATCH_CREATE(StopwatchID,StopwatchName)	\
		static Stopwatch StopwatchID(StopwatchName);	


	//! Создает и запускает таймер
	#define STOPWATCH_START(StopwatchID,StopwatchName)	\
		StopwatchID.start(); 
		
		//static Stopwatch StopwatchID(StopwatchName);	\

	//! Создает и запускает таймер
	#define STOPWATCH_RESUME(StopwatchID)	\
		static Stopwatch StopwatchID(StopwatchName);	\
		StopwatchID.start();

	//! Останавливает таймер	
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
