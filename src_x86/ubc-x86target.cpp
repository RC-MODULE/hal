//#include "mc12101load.h"
#include "stdio.h"
#include "ubc_target.h"


#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#pragma comment(lib, "user32.lib")

#define SYNC_BUF_SIZE 64*1024
TCHAR sSyncName[]=TEXT("Global\\SyncFileMappingObject");

#define SYNC_BUF_SIZE 64*1024
TCHAR sSharedRegestryName[]=TEXT("Global\\SharedRegistryFileMappingObject");

//char sSharedMemName[]="Global\\SharedFileMappingObject" STR(PROCESSOR_ID);
TCHAR sSharedMemName[]=TEXT("Global\\SharedFileMappingObject");


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


static  HANDLE hMapFile;
static  HANDLE hMapFileSharedMem;

//extern int  boardSharedSize;
//extern long long boardSharedBuffer;
//long long * boardSharedBuffer;

struct SyncBuf {
	int counter0;
	int counter1;
	int sync0;
	int sync1;
} *pSyncBuf=0;

// Возвращает указатель на буфер синхронизации
SyncBuf* getSyncBuffer(){
	if (pSyncBuf==0){
		hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			sSyncName);               // name of mapping object

		if (hMapFile == NULL){
			_tprintf(TEXT("Could not open file mapping object (%d).\n"),GetLastError());
			return 0;
		}

		pSyncBuf = (SyncBuf*) MapViewOfFile(
			hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			SYNC_BUF_SIZE);

		if (pSyncBuf == NULL){
			_tprintf(TEXT("Could not map view of file (%d).\n"),GetLastError());
			CloseHandle(hMapFile);
			return 0;
		}
	}
	return pSyncBuf;
}


/*
const struct SyncBuf  syncBuf[10];


void func(const SyncBuf* p){

	
}

void ff(){
	//SyncBuf syncBuf[10];
	//func(syncBuf+2);
	func(&syncBuf[2]);



}
*/
extern "C"{

	int ncl_hostSync(int val){
		int procNo=ubcGetProcessorNo();
		printf ("TARGET[%d]:Sync[%d] (%Xh)...",procNo,pSyncBuf[procNo].counter1,val);
		pSyncBuf[procNo].sync1=val;
		pSyncBuf[procNo].counter1++;
		while (pSyncBuf[procNo].counter0!=pSyncBuf[procNo].counter1);
		printf("=%Xh\n",pSyncBuf[procNo].sync0);
		return pSyncBuf[procNo].sync0;
	}
};
/*
class CMapFile{
	HANDLE hMapFile;
	HANDLE hMapFileSharedMem;
public:
	CMapFile(){
		
		

		hMapFile = OpenFileMapping(
								   FILE_MAP_ALL_ACCESS,   // read/write access
								   FALSE,                 // do not inherit the name
								   sSyncName);               // name of mapping object

		if (hMapFile == NULL)
		{
			_tprintf(TEXT("Could not open file mapping object (%d).\n"),
				GetLastError());
		}

		pSyncBuf = (SyncBuf*) MapViewOfFile(hMapFile, // handle to map object
									  FILE_MAP_ALL_ACCESS,  // read/write permission
									  0,
									  0,
									  BUF_SIZE);
		if (pSyncBuf == NULL)
		{
			_tprintf(TEXT("Could not map view of file (%d).\n"),
				GetLastError());

			CloseHandle(hMapFile);
		}
		//---------------------------------
		hMapFileSharedMem = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			BUF_SIZE,                // maximum object size (low-order DWORD)
			sSharedMemName);              // name of mapping object



		if (hMapFileSharedMem == NULL)
		{
			_tprintf(TEXT("Could not open file mapping object (%d).\n"),
				GetLastError());
		}

		MapViewOfFileEx(
			hMapFileSharedMem ,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			boardSharedSize*8,
			(LPVOID)boardSharedBuffer
			);

		
	}

	

	//MessageBox(NULL, pBuf, TEXT("Process2"), MB_OK);

	
	~CMapFile(){
		UnmapViewOfFile(pSyncBuf);

		CloseHandle(hMapFile);
	}

} mapFile;
*/

/*
int ubcMalloc64(long long* shared,unsigned sharedSize64){
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		sSyncName);               // name of mapping object

	if (hMapFile == NULL){
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
		GetLastError());
	}

	pSyncBuf = (SyncBuf*) MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUF_SIZE);
	
	if (pSyncBuf == NULL){
		_tprintf(TEXT("Could not map view of file (%d).\n"),
		GetLastError());
		CloseHandle(hMapFile);
	}
	//------------------------------

	hMapFileSharedMem = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		1024*64,                // maximum object size (low-order DWORD)
		sSharedMemName);              // name of mapping object

	if (hMapFileSharedMem == NULL){
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
		GetLastError());
	}
	
	
	LPVOID sharedBuffer = (LPVOID)shared;
	int   offset = ((int)sharedBuffer) & 0xFFFF;
	LPVOID sharedBufferAligned = (char*)sharedBuffer-offset + 0x10000;
	_SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	LPVOID p=MapViewOfFileEx(
		hMapFileSharedMem ,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		1024,
		sharedBufferAligned
	);
	shared[0] = 0xbabadeda;
	int err=GetLastError();

	ncl_hostSync((unsigned)sharedBuffer);
	ncl_hostSync(sharedSize64);

	return 0;
}
*/

/*
int* ubcConnInit(int handshake)
	int* sharedBuffer;


	return ok;
}
*/
int* ubcMalloc32(int sharedSize32){
	int* sharedBuffer;
	sharedSize32+=(64*1024/4-1);
	sharedSize32&=(~0x3FFF);
	hMapFileSharedMem = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		sharedSize32*4,          // maximum object size (low-order DWORD)
		sSharedMemName);         // name of mapping object

	if (hMapFileSharedMem == NULL) {
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),GetLastError());
		return 0;
	}

	//ubcHostSync(sharedSize32);

	sharedBuffer = (int*)MapViewOfFile(
		hMapFileSharedMem,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sharedSize32*4);

	printf("Shared: %X %X\n",sharedBuffer ,sharedSize32*4);
	if (sharedBuffer == NULL) {
		_tprintf(TEXT("Could not map view of file (%d) \n"),GetLastError());
		return 0;
	}
	return sharedBuffer;
	//ubcHostSync((int)sharedBuffer);
}

int ubcConnect(int* masterSharedBuffer, int masterSharedSize32, int** sharedBuffer, int* sharedSize32){

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		sSyncName);               // name of mapping object

	if (hMapFile == NULL){
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),GetLastError());
		return 0;
	}

	pSyncBuf = (SyncBuf*) MapViewOfFile(
		hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		SYNC_BUF_SIZE);

	if (pSyncBuf == NULL){
		_tprintf(TEXT("Could not map view of file (%d).\n"),GetLastError());
		CloseHandle(hMapFile);
		return 0;
	}


	int ok=ubcHostSync(0x64060000+ubcGetProcessorNo());
	//------------------------------
	//LPVOID sharedBuffer = 0;
	//LPVOID sharedBuffer = 0;
	if (masterSharedBuffer==0 && masterSharedSize32==0){
		*sharedSize32=ubcHostSync(0);
		hMapFileSharedMem = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			sSharedMemName);               // name of mapping object

		if (hMapFileSharedMem == NULL){
			_tprintf(TEXT("Could not open file mapping object (%d).\n"),GetLastError());
			return 0;
		}

		*sharedBuffer = (int*)MapViewOfFile(
			hMapFileSharedMem,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			*sharedSize32*4);

		printf("Shared: %X %X\n",*sharedBuffer ,*sharedSize32*4);
		if (sharedBuffer == NULL) {
			_tprintf(TEXT("Could not map view of file (%d) \n"),GetLastError());
			return 0;
		}
		ubcHostSync((int)*sharedBuffer);
	}
	else {
		ubcHostSync((int)masterSharedSize32);
		ubcHostSync((int)masterSharedBuffer);
		
	}


	//printf("Target" STR(PROCESSOR_ID)  "-ok\n");
	
}

int ubcHostSyncArray(
					 int value,        // Sync value
					 void *outAddress, // Sended array address (can be NULL)
					 size_t outLen,    // Sended array length (can be 0)
					 void **inAddress, // Received array address pointer (can be NULL)
					 size_t *inLen)   // Received array size pointer (can be NULL)
{
	//int extBuffer=ubcHostSync((int)masterSharedBuffer);
	//int extSize32=ubcHostSync((int)masterSharedSize32);
	//if (extBuffer && extSize32){

	//}
	return 1;
}


//void ubcFree(){
//	
//}

int ubcDisconnect(int* shared){
	ubcHostSync((int)0x600DBA1+ubcGetProcessorNo());
	UnmapViewOfFile(shared);
	CloseHandle(hMapFileSharedMem);	
	UnmapViewOfFile(pSyncBuf);
	CloseHandle(hMapFile);
	return 0;
}