#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define TRACE(str) printf("%s", str)
#define SYNC_BUF_SIZE 64*1024
#define SHARED_MEM_SIZE 1024


TCHAR sSyncName[]     =TEXT("Global\\SyncFileMappingObject");
TCHAR sSharedMemName[]=TEXT("Global\\SharedFileMappingObject");

HANDLE hMapFile;
HANDLE hMapFileSharedMem;

// 
struct SyncBuf {
	int counter0;
	int counter1;
	int sync0;
	int sync1;
} *pSyncBuf;

const int MAX_COUNT_PROCESSORS = 8;
//const int MAX_SHARED
static int* pSharedMem;//[MAX_SHARED];
static int  sharedDiff[MAX_COUNT_PROCESSORS];

extern "C"{

int ubcSync(int val,int processor=0){
	printf ("HOST[%d]:Sync[%d] (%8xh)...",processor,pSyncBuf[processor].counter0,val);
	pSyncBuf[processor].sync0=val;
	pSyncBuf[processor].counter0++;
	while (pSyncBuf[processor].counter0!=pSyncBuf[processor].counter1);
	printf("=%xh\n",pSyncBuf[processor].sync1);
	return pSyncBuf[processor].sync1;
}

static int* sharedBuffer[MAX_COUNT_PROCESSORS]={0,0,0,0,0,0,0,0};
static char* absFile[MAX_COUNT_PROCESSORS]={0,0,0,0,0,0,0,0};

int ubcOpen(char* absfile,...){

	
	//unsigned sharedSize32;

	va_list args;
	va_start(args, absfile);
	absFile[0] = absfile;
	absFile[1] = va_arg(args, char*);
	va_end(args);

	//-------------- create sync-memory buffer ---------------
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		SYNC_BUF_SIZE,           // maximum object size (low-order DWORD)
		sSyncName);              // name of mapping object

	if (hMapFile == NULL){
		_tprintf(TEXT("Could not create file sync mapping object (%d).\n"),GetLastError());
		return 1;
	}

	pSyncBuf = (SyncBuf*) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		SYNC_BUF_SIZE);

	if (pSyncBuf == NULL){
		_tprintf(TEXT("Could not map view of sync file (%d).\n"),GetLastError());
		CloseHandle(hMapFile);
		return 1;
	}
	SyncBuf SB={-1,-1,-1,-1};
	CopyMemory((PVOID)&pSyncBuf[0], &SB, sizeof(SB));
	CopyMemory((PVOID)&pSyncBuf[1], &SB, sizeof(SB));

	//--------- handshake  ---------------------------
	int ok;
	unsigned sharedSize32;
	for(int processor=0; absFile[processor]; processor++){
		ok=ubcSync(0x8086,processor);
		
	
		if (processor==0){
			sharedSize32 = ubcSync(0,processor);
			if (sharedBuffer==0){
				printf("Shared memory allocation error on target \n");
				return 1;
			}

			hMapFileSharedMem = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,	// read/write access
				FALSE,					// do not inherit the name
				sSharedMemName);		// name of mapping object

			if (hMapFileSharedMem == NULL){
				_tprintf(TEXT("Could-not open file mapping shared object (%d).\n"),GetLastError());
				continue;//return -1;
			}
		
			if (hMapFileSharedMem == NULL){
				_tprintf(TEXT("Could not open file mapping shared object (%d).\n"),GetLastError());
				return -1;
			}

			pSharedMem  = (int*) MapViewOfFile(
				hMapFileSharedMem,		// handle to map object
				FILE_MAP_ALL_ACCESS,	// read/write permission
				0,
				0,
				sharedSize32*4);

			if (pSharedMem == NULL){
				_tprintf(TEXT("Could not map view of shared file (%d).\n"),GetLastError());
				CloseHandle(hMapFileSharedMem);
				return -1;
			}
			sharedBuffer[processor] = (int*)ubcSync(0, processor);	
			printf("HOST:Shared: %x %x\n",sharedBuffer[processor] ,sharedSize32*4);
			sharedDiff[processor] = pSharedMem - sharedBuffer[processor];
		}
		else {
			ok = ubcSync(sharedSize32, processor);	
			sharedBuffer[processor] = (int*)ubcSync(0, processor);	
			printf("HOST:Shared: %x %x\n",sharedBuffer[processor] ,sharedSize32*4);
			sharedDiff[processor] = pSharedMem - sharedBuffer[processor];
		}
		
	}

	TRACE ("Connected!\n");
	return 0;
}
/*	
int ubcSyncArray(
		int value,          // Value sent to processor
		unsigned outAddress,// Address sent to processor
		unsigned outLen,    // Size sent to processor

		int *returnValue,   // Value received from processor
		unsigned *inAddress,// Address received from processor
		unsigned *inLen,
		int processor=0)    // Size received from processor
{
		sharedSize32 = ubcSync(value,processor);
		if (sharedBuffer==0){
			printf("Shared memory allocation error on target \n");
			return 1;
		}
		if (outAddress==0){
			hMapFileSharedMem = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,	// read/write access
				FALSE,					// do not inherit the name
				sSharedMemName);		// name of mapping object

			if (hMapFileSharedMem == NULL){
				_tprintf(TEXT("Could-not open file mapping shared object (%d).\n"),GetLastError());
				continue;//return -1;
			}
			
			if (hMapFileSharedMem == NULL){
				_tprintf(TEXT("Could not open file mapping shared object (%d).\n"),GetLastError());
				return -1;
			}

			pSharedMem  = (int*) MapViewOfFile(
				hMapFileSharedMem,		// handle to map object
				FILE_MAP_ALL_ACCESS,	// read/write permission
				0,
				0,
				sharedSize32*4);

			if (pSharedMem == NULL){
				_tprintf(TEXT("Could not map view of shared file (%d).\n"),GetLastError());
				CloseHandle(hMapFileSharedMem);
				return -1;
			}
			sharedBuffer[processor] = (int*)ubcSync(0, processor);	
			printf("HOST:Shared: %x %x\n",sharedBuffer[processor] ,sharedSize32*4);
			sharedDiff[processor] = pSharedMem - sharedBuffer[processor];	
		}
		else {
			ok = ubcSync(sharedSize32, processor);	
			sharedBuffer[processor] = (int*)ubcSync(0, processor);	
			printf("HOST:Shared: %x %x\n",sharedBuffer[processor] ,sharedSize32*4);
			sharedDiff[processor] = pSharedMem - sharedBuffer[processor];
		}	
}

*/		
int ubcReadMemBlock (int* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0){
	CopyMemory((PVOID)dstHostAddr, (char*)((int*)srcBoardAddr + sharedDiff[processor]), size32*4);
	return 1;
}

int ubcWriteMemBlock(unsigned long* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0){
	CopyMemory((char*)((int*)dstBoardAddr + sharedDiff[processor]), (PVOID)srcHostAddr,  size32 * 4);
	return 1;
}


int ubcClose(){
	for(int processor=1; absFile[processor]; processor++){  
		ubcSync(0x600DBA10+processor,processor);
	}
	ubcSync(0x600DBA100);
	if (pSyncBuf)
		UnmapViewOfFile(pSyncBuf);
	if (pSharedMem)
		UnmapViewOfFile(pSharedMem);

	if (hMapFile)
		CloseHandle(hMapFile);
	if (hMapFileSharedMem)
		CloseHandle(hMapFileSharedMem);
	

	return 1;
}


int ubcGetResult(unsigned long* returnCode, int processor=0){
	return 1;
	
}

};