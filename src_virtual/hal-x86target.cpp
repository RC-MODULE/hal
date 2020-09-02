#include "hal_target.h"
#include "hal.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>
#include "heap.h"
#pragma comment(lib, "user32.lib")

#include "hal-defs.h"


extern int procNo;


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define TRACE(str) printf("%s", str)

struct SharedMemory {
	SharedMemory* hostOwnAddr;												// адрес под которым хост видит эту структуру
	SharedMemory* procOwnAddr[MAX_COUNT_PROCESSORS];						// адрес под которым каждый процессора видит SharedMemory 
	SyncBuf		 hostSyncBuff[MAX_COUNT_PROCESSORS];						// структура для синхронизация с хостом - nmc
	SyncBuf		 procSyncBuff[MAX_COUNT_PROCESSORS][MAX_COUNT_PROCESSORS];	// nmc-nmc межпроцессорная синхронизция (ипользуется половина)
	long long 	 hostMapDiff [MAX_COUNT_PROCESSORS];						// смещение для пересчета адресов при их пересылке между процессором и хостом 
	long long	 procMapDiff [MAX_COUNT_PROCESSORS][MAX_COUNT_PROCESSORS];	// смещения для пересчета адресов при их пересылке между процессорами
	Heap<0x8200000> heap;													// общая разделяемая куча
	// (256*1024*1024*2+4*1024*1024*2)/4 = 8200000
};

SharedMemory* sharedMemory=0;

// создает разделяемую структура мапируемую в память. Кто первый зашел , тот и создают . Остальные подключаются
SharedMemory* openSharedMemory() {
	
	if (sharedMemory == 0) {
		// округляем размер. так надо
		unsigned sharedSize32 = sizeof(SharedMemory);
		sharedSize32 += (64 * 1024 / 4 - 1);
		sharedSize32 &= (~0x3FFF);

		HANDLE hMapFile;
		// пытаемся открыть существующий мап- объект
		for (int i = 0; i<HOST_CONNECT_TIMEOUT; i += 100) {
			hMapFile = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,   // read/write access
				FALSE,                 // do not inherit the name
				TEXT(SHARED_MEMORY_MAPPING_NAME));               // name of mapping object
			if (hMapFile) break;
			halSleep(100);
		}
		// пытаемся создать сами мап- объект
		bool mapFileCreated = false;
		try {
			if (hMapFile == NULL) {
				hMapFile = CreateFileMapping(
					INVALID_HANDLE_VALUE,    // use paging file
					NULL,                    // default security
					PAGE_READWRITE,          // read/write access
					0,                       // maximum object size (high-order DWORD)
					sharedSize32 ,           // maximum object size (low-order DWORD)
					TEXT(SHARED_MEMORY_MAPPING_NAME));              // name of mapping object

				if (hMapFile == NULL) {
					printf("Could not create file sync mapping object (%d).\n", GetLastError());
					throw (555);
				}

				mapFileCreated = true;
			}
		}
		catch (int err){
			printf("Could not create file sync mapping object (%d).\n", err);
			return 0;
		}
		// Отображаем мап- объект на область памяти
		try {
			sharedMemory = (SharedMemory*)MapViewOfFile(
				hMapFile, // handle to map object
				FILE_MAP_ALL_ACCESS,  // read/write permission
				0,
				0,
				sharedSize32);
		
			if (sharedMemory == NULL) {
				printf("Could not map view of file (%d).\n", GetLastError());
				CloseHandle(hMapFile);
				throw 666;
			}
		}
		catch (int err) {
			printf("Could not map view of file (%d).\n ", err);
			return 0;
		}

		if (mapFileCreated) {
			memset(sharedMemory, 0, sharedSize32 );
			sharedMemory->heap.init();
		}
		// пишем свои адреса
		if (procNo == -1) //host
			sharedMemory->hostOwnAddr = sharedMemory;
		else 
			sharedMemory->procOwnAddr[procNo] = sharedMemory;
		// вычислем смещения для конвертации адресов
		for (int i = 0; i < MAX_COUNT_PROCESSORS; i++)
			sharedMemory->hostMapDiff[i] = (int*)sharedMemory->hostOwnAddr - (int*)sharedMemory->procOwnAddr[i];
		for (int i = 0; i < MAX_COUNT_PROCESSORS; i++)
			for (int j = 0; j < MAX_COUNT_PROCESSORS; j++)
				sharedMemory->procMapDiff[i][j] = (int*)sharedMemory->procOwnAddr[j] - (int*)sharedMemory->procOwnAddr[i];

	}
	return sharedMemory;
}

void closeSharedMemory(char* name) {
	UnmapViewOfFile(sharedMemory);
	CloseHandle(sharedMemory);
}
//=============================================================
extern "C"{

void halSetProcessorNo(int number){
	procNo=number;
}
    
int ncl_getProcessorNo(){

	return procNo;
}

int turn=1;
	
int ncl_hostSync(int val){
	if (openSharedMemory()==0)
		return 0;
	
	SyncBuf& syncro = sharedMemory->hostSyncBuff[procNo];
			
	while(syncro.writeCounter[1]>syncro.readCounter[1]){}
	syncro.sync1=val;						
	syncro.writeCounter[1]++;
	
	while(syncro.readCounter[0]==syncro.writeCounter[0]){}									
	int sync=syncro.sync0;								
	syncro.readCounter[0]++;

	return sync;
}

};


TCHAR* createName(TCHAR* baseName, int index0, int index2){
	static TCHAR bufferName[256];
	TCHAR bufferSuffix[256];
	wcscpy_s(bufferName, baseName);	
	_itow_s(index0, bufferSuffix, 16);
	wcscat_s(bufferName,bufferSuffix);
	wcscat_s(bufferName,TEXT("_"));
	_itow_s(index2,bufferSuffix,16);
	wcscat_s(bufferName,bufferSuffix);
	return bufferName;
}

int* halMalloc32(int bufferSize32){
	if (openSharedMemory()==0)
		return 0;
	int* buffer=sharedMemory->heap.allocate(bufferSize32);
	return buffer;
}

int halHostSyncArray(
					 int value,        // Sync value
					 void *outAddress, // Sended array address (can be NULL)
					 size_t outLen,    // Sended array length (can be 0)
					 void **inAddress, // Received array address pointer (can be NULL)
					 size_t *inLen)   // Received array size pointer (can be NULL)
{
	//int extBuffer=halHostSync((int)masterSharedBuffer);
	//int extSize32=halHostSync((int)masterSharedSize32);
	//if (extBuffer && extSize32){

	//}
	return 1;
}

//int halSyncArray(
//					 int value,        // Sync value
//					 void *outAddress, // Sended array address (can be NULL)
//					 size_t outLen,    // Sended array length (can be 0)
//					 void **inAddress, // Received array address pointer (can be NULL)
//					 size_t *inLen,   // Received array size pointer (can be NULL)
//					 int  procNo)
//{
//	
//	return sync;
//}

//void* halSyncAddr(
//					 void *outAddress, // Sended array address (can be NULL)
//					 int  procNo)
//{
//	void *inAddress=(void*)halSync((int)outAddress,procNo);
//	void* ownAddress=halMapAddrFrom(inAddress, procNo);
//	return ownAddress;
//}

// синхронизация host-nmc и nmc-nmc
void* halSyncAddr(
					 void *outAddress, // Sended array address (can be NULL)
					 int  processor){
	if (openSharedMemory() == 0)
		return 0;

	void* inAddress;

	if (procNo == -1) {		// if host  <-> processoor sync
		SyncBuf& syncro = sharedMemory->hostSyncBuff[processor];

		while (syncro.writeCounter[0]>syncro.readCounter[0])
		{
			::Sleep(100);
		}
		syncro.syncAddr0 = outAddress;
		syncro.writeCounter[0]++;

		while (syncro.readCounter[1] == syncro.writeCounter[1])
		{
			::Sleep(100);
		}
		inAddress = syncro.syncAddr1;
		syncro.readCounter[1]++;
	}
	else
	{
		//if (procNo == 0)
		if (procNo < processor) {
			SyncBuf& syncro = sharedMemory->procSyncBuff[procNo][processor];

			while (syncro.writeCounter[0] > syncro.readCounter[0])
			{
				::Sleep(100);
			}
			syncro.syncAddr0 = outAddress;
			syncro.writeCounter[0]++;

			while (syncro.readCounter[1] == syncro.writeCounter[1])
			{
				::Sleep(100);
			}
			inAddress = syncro.syncAddr1;
			syncro.readCounter[1]++;
		}
		//else if (procNo == 1)
		else {
			SyncBuf& syncro = sharedMemory->procSyncBuff[processor][procNo];

			while (syncro.writeCounter[1] > syncro.readCounter[1])
			{
			}
			syncro.syncAddr1 = outAddress;
			syncro.writeCounter[1]++;

			while (syncro.readCounter[0] == syncro.writeCounter[0])
			{
			}
			inAddress = syncro.syncAddr0;
			syncro.readCounter[0]++;

		}
	}
	void* ownAddress = halMapAddrFrom(inAddress, procNo);
	return ownAddress;
}


extern "C" {
void halFree(void* p){
	sharedMemory->heap.release(p);
}

void halHandshake(int procNo){
	halSync(777, procNo);
}
};

int halDisconnect(int* shared){
	halHostSync((int)0x600DBA1+halGetProcessorNo());
	
	return 0;
}


//void* halMapAddr(const void* srcAddr) {
//	return srcAddr;
//}

void* halMapAddrTo(const void* ownAddress, int toProccessor) {
	long long diff;
	if (procNo == -1)
		diff = sharedMemory->hostMapDiff[toProccessor];
	else 
		diff=sharedMemory->procMapDiff[procNo][toProccessor];
	void* extAddress = (void*)((int*)(ownAddress)+diff);
	return extAddress;
}
void* halMapAddrFrom(const void* extAddress, int fromProccessor) {
	long long diff;
	if (procNo==-1)
		diff = sharedMemory->hostMapDiff[fromProccessor];
	else 
		diff = sharedMemory->procMapDiff[fromProccessor][procNo];
	void* ownAddress = (void*)((int*)(extAddress)+diff);
	return ownAddress;
}

// синхронизация host-nmc и nmc-nmc
int halSync(int val, int processor) {
	if (openSharedMemory() == 0)
		return 0;

	int sync;

	if (procNo == -1) {		// if host  <-> processoor sync
		SyncBuf& syncro = sharedMemory->hostSyncBuff[processor];

		while (syncro.writeCounter[0]>syncro.readCounter[0])
		{
			::Sleep(100);
		}
		syncro.sync0 = val;
		syncro.writeCounter[0]++;

		while (syncro.readCounter[1] == syncro.writeCounter[1])
		{
			::Sleep(100);
		}
		sync = syncro.sync1;
		syncro.readCounter[1]++;
	}
	else
	{
		//if (procNo == 0)
		if (procNo < processor) {
			SyncBuf& syncro = sharedMemory->procSyncBuff[procNo][processor];

			while (syncro.writeCounter[0] > syncro.readCounter[0])
			{
				::Sleep(100);
			}
			syncro.sync0 = val;
			syncro.writeCounter[0]++;

			while (syncro.readCounter[1] == syncro.writeCounter[1])
			{
				::Sleep(100);
			}
			sync = syncro.sync1;
			syncro.readCounter[1]++;
		}
		//else if (procNo == 1)
		else {
			SyncBuf& syncro = sharedMemory->procSyncBuff[processor][procNo];

			while (syncro.writeCounter[1] > syncro.readCounter[1])
			{
			}
			syncro.sync1 = val;
			syncro.writeCounter[1]++;

			while (syncro.readCounter[0] == syncro.writeCounter[0])
			{
			}
			sync = syncro.sync0;
			syncro.readCounter[0]++;

		}
	}
	return sync;
}

HANDLE hMapFile;


extern int procNo = 0;
extern "C" {


	void halSetActiveHeap(int heap) {
		//activeHeap=heap;
	}

	static char* absFile[MAX_COUNT_PROCESSORS] = { 0,0,0,0,0,0,0,0 };

	int halOpen(char* absfile, ...) {
		procNo = -1;

		//unsigned sharedSize32;

		va_list args;
		va_start(args, absfile);
		absFile[0] = absfile;
		absFile[1] = va_arg(args, char*);
		va_end(args);

		TRACE("Connected!\n");
		return 0;
	}

	
	int halReadMemBlock (int* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0)
	{
		//CopyMemory((PVOID)dstHostAddr, (char*)(halMapAddrFrom((void*)srcBoardAddr,processor), size32 * 4);
		memcpy(dstHostAddr, (char*)(halMapAddrFrom((void*)srcBoardAddr,processor)), size32 * 4);

		return 0;

	}

	int halWriteMemBlock(unsigned long* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0){
		//CopyMemory((char*)(halMapAddrFrom((void*)dstBoardAddr,processor ), (PVOID)srcHostAddr,  size32 * 4);
		memcpy(halMapAddrFrom((void*)dstBoardAddr, processor),srcHostAddr, size32 * 4);
	return 0;
	}


	int halClose()
	{
		//closeSharedMemory();
		return 1;
	}


	int halGetResult(unsigned long* returnCode, int processor = 0)
	{
		return 1;

	}
};