//#include "nmprofiler.h"


//! Определяет макс размер строки имени функции в поле S_ProfilerData.funcname
//#include "nmprofiler_def.h"
#include "string.h"
#include "time.h"
#include <stdio.h>
#include "nmprofiler.h"
#include "section-nmprofiler.h"
//#include "nmprofiler_nmcout.h"
//#define MAX_FUNCNAME_LENGTH 64
//#define MAX_FUNCNAME_LENGTH 16

extern "C"{
 
// Функция выводит результат профилирования одной функции в stdout в форматированном виде
// \param prof указатель структуру профилирования 
// \param format формат вывода аналогичный printf 
INSECTION(".text_nmprofiler") void nmprofiler_printf(ProfilerData* prof, char* format){
	float perelement = 0;
	if (prof->size_summary)
		perelement=float(prof->summary)/prof->size_summary;
	//printf(format, prof->summary, prof->calls,  prof->summary/(prof->calls+(prof->calls==0)) , perelement, prof->funcaddr, prof->funcname);
	printf(format, prof->summary, prof->calls,  prof->summary/(prof->calls+(prof->calls==0)) ,  prof->funcaddr, prof->funcname);
}	

// Функция форматрирует в результат профилирования одной функции в dststr
// \param prof указатель структуру профилирования 
// \param format формат вывода аналогичный sprintf 
INSECTION(".text_nmprofiler") void nmprofiler_sprintf(char* dststr, ProfilerData* prof, char* format, char* full_func_name){
	if (full_func_name==0)
		full_func_name=(char*)prof->funcname;
	float perelement = 0;
	if (prof->size_summary)
		perelement=prof->summary/prof->size_summary;
	//sprintf(dststr, format, prof->summary, prof->calls,  prof->summary/(prof->calls+(prof->calls==0)) , perelement, prof->funcaddr, full_func_name);
	sprintf(dststr, format, prof->summary, prof->calls,  prof->summary/(prof->calls+(prof->calls==0)) ,  prof->funcaddr, full_func_name);
}	


// Функция возвращает указатель на первую структуру профилирования в связном списке 
//S_ProfilerData* nmprofiler_head();
/*
{
	int dummy=reinterpret_cast<int> (nmprofiler_list);
	int *p=reinterpret_cast<int*> (dummy);
	return (S_ProfilerData*)(p+4);
}
*/

// Функция возвращает указатель на последнюю структуру профилирования в связном списке 
// \param head указатель на первую структуру в списке.
//S_ProfilerData* nmprofiler_tail(S_ProfilerData* head)
//{
//	while (head->next){
//		head+=head->next;
//	}
//	return head;
//}

// Функция выводит результаты профилирования в stdout в табличном-формате по списку начиная с head. Для форматированного отображения xml-файла прилагается xsl-преобразование: profile.xsl.
// \param head указатель на первую структуру в списке.
INSECTION(".text_nmprofiler") void  nmprofiler_print2tbl(ProfilerData* head)
{
	if (head==0)
		head=nmprofiler_head();
	//printf("%d\n",(int)head);
	printf("SUMMARY     | CALLS       | AVERAGE     | ADDRESS | FUNCTION \n");
	printf("------------+-------------+-------------+---------+----------\n");
	while (head){
		//printf("dsip:%d\n",head->next);
		nmprofiler_printf(head,NMPROFILER_TBL);
		head=nmprofiler_next(head);
	};
	printf("------------+-------------+-------------+---------+----------\n");
}

//! Функция выводит результаты профилирования в stdout в xml-формате по списку начиная с head. Для форматированного отображения xml-файла прилагается xsl-преобразование: profile.xsl.
//! \param head указатель на первую структуру в списке.
INSECTION(".text_nmprofiler") void  nmprofiler_print2xml(ProfilerData* head)
{
	if (head==0)
		head=nmprofiler_head();
	printf("<?xml version='1.0' ?>\n");
	printf("<?xml-stylesheet type='text/xsl' href='profile.xsl'?>\n");
	printf("<profiling version=\"1.0\">\n");
	while (head->next){
		nmprofiler_printf(head,NMPROFILER_XML);
		head=nmprofiler_next(head);
	};
	printf("</profiling>\n");
}

//! Функция выводит результаты профилирования в stdout в xml-формате по списку начиная с head. Для форматированного отображения xml-файла прилагается xsl-преобразование: profile.xsl.
//! \param head указатель на первую структуру в списке.
};


extern "C" HalRingBufferData<TraceData,1024> nmprofiler_trace;
INSECTION(".text_nmprofiler") void  nmprofiler_trace2tbl(int max_depth)
{
	
	HalRingBufferData<TraceData,1024>& trace=*((HalRingBufferData<TraceData,1024>*)&nmprofiler_trace);
	//HalRingBufferData<TraceData,0>& trace=nmprofiler_trace;
	
	for(;trace.tail<trace.head;trace.tail++){
		TraceData& item=trace.data[trace.size1&trace.tail];
		printf("[%d]\tt:%d  \t",trace.tail,item.time);
		printf("sp:%x\t",item.sp);
		//printf("[%d]",item.depth+max_depth+1);
		int max=item.depth+max_depth>0?item.depth+max_depth:0;
		for(int i=0; i<max; i++)
			printf("  ");
		printf(":%x\n",item.func);
	}

}



