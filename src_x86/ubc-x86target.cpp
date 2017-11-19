#include "ubc_target.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>
#pragma comment(lib, "user32.lib")

#include "ubc-defs.h"
HANDLE hBufferRegistry=0;
BufferRegistry* pBufferRegistry=0;
extern int procNo;
extern  SyncBuf  *pSyncBuf;

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


void ubcSetProcessorNo(int number){
	procNo=number;
}

// Возвращает указатель на буфер синхронизации
SyncBuf* getSyncBuffer(){
	if (pSyncBuf==0){
		HANDLE hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			TEXT(SYNC_BUFFER_MAPPING_NAME));               // name of mapping object

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



extern "C"{
    
	int ncl_getProcessorNo(){

		return procNo;
	}

	int turn=1;
	
	int ncl_hostSync(int val){
		//findBuffer();
		if (pSyncBuf==0)
			pSyncBuf=getSyncBuffer();
		//int procNo=ncl_getProcessorNo();
		//printf ("TARGET[%d]:Sync[%d] (%Xh)...",procNo,pSyncBuf[procNo].counter1,val);

SyncBuf& syncro = pSyncBuf[procNo];

			
	while(syncro.writeCounter[1]>syncro.readCounter[1])
	{}
	syncro.sync1=val;						
	syncro.writeCounter[1]++;
	
	while(syncro.readCounter[0]==syncro.writeCounter[0])		
	{}									
	int sync=syncro.sync0;								
	syncro.readCounter[0]++;

	return sync;



/*
		if (turn==1)
		pSyncBuf[procNo].locked0=true;
		pSyncBuf[procNo].sync1=val;
		pSyncBuf[procNo].counter1++;
		
		while (pSyncBuf[procNo].counter0!=pSyncBuf[procNo].counter1){
			::Sleep(100);
		}
		int sync=pSyncBuf[procNo].sync0;
		pSyncBuf[procNo].locked0=false;
		while ((pSyncBuf[procNo].locked1)){
			::Sleep(100);
		}
		printf("=%Xh\n",pSyncBuf[procNo].sync0);
		return sync;
	}
	*/
		//SyncBuf& syncro = pSyncBuf[procNo];


	}

};

BufferRegistry* createBufferRegistry(){
	if (pBufferRegistry)
		return pBufferRegistry;
	/*
	// пытаемся открыть кем то созданный реест
	hBufferRegistry = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,			// read/write access
		FALSE,							// do not inherit the name
		sBufferRegistryMappingName);	// name of mapping object
*/
	//if (hBufferRegistry  == NULL){
	//TCHAR registryName[256]=TEXT(CONST_REGISTRY_MAPPING_NAME);
	//TCHAR registrySuffix[256];
	//int procNo=ncl_getProcessorNo();
	//_itow(procNo,registrySuffix,16);
	//wcscat(registrySuffix,registryName);
	TCHAR* registryName=createName(TEXT(CONST_REGISTRY_MAPPING_NAME),procNo,procNo);

	// Реест еще не создан , создаем сами
	hBufferRegistry = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		sizeof(BufferRegistry),          // maximum object size (low-order DWORD)
		registryName);         // name of mapping object

	if (hBufferRegistry == NULL) {
		_tprintf(TEXT("ERROR:Could not create registry file mapping object (%d).\n"),GetLastError());
		return 0;
	}
	
	pBufferRegistry = (BufferRegistry*)MapViewOfFile(
		hBufferRegistry,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(BufferRegistry));

	if (pBufferRegistry == NULL) {
		_tprintf(TEXT("ERROR:Could not map registry view of file (%d) \n"),GetLastError());
		return 0;
	}

	memset(pBufferRegistry,0,sizeof(BufferRegistry));
	return pBufferRegistry;
}

// а если повтор ???
/*
int* ubcGetBuffer(int bufferID){
	if (cropenBufferRegistry()==0)
		return 0;
	for(int i=0;i<pBufferRegistry->size;i++){
		MappedBuffer& mappedBuffer=pBufferRegistry->buffer[i];
		if (mappedBuffer.id==bufferID){
			TCHAR bufferName[256]=TEXT(CONST_BUFFER_MAPPING_NAME);
			TCHAR bufferSuffix[256];
			_itow(bufferID,bufferSuffix,16);
			wcscat(bufferSuffix,bufferName);

			hMapFile = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,   // read/write access
				FALSE,                 // do not inherit the name
				bufferName);           // name of mapping object

			if (hMapFile == NULL){
				_tprintf(TEXT("ERROR:Could not open file mapping object (%d).\n"),GetLastError());
				return 0;
			}

			int* pBuffer = (int*) MapViewOfFile(
				hMapFile, // handle to map object
				FILE_MAP_ALL_ACCESS,  // read/write permission
				0,
				0,
				mappedBuffer.size32*4);

			if (pBuffer == NULL){
				_tprintf(TEXT("ERROR:Could not map view of file (%d).\n"),GetLastError());
				CloseHandle(hMapFile);
				return 0;
			}
			return pBuffer;
		}
	}
	return 0;	
}
*/
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
int* ubcMalloc32(int bufferSize32){
	if (createBufferRegistry()==0)
		return 0;
	MappedBuffer mappedBuffer;
	bufferSize32+=(64*1024/4-1);
	bufferSize32&=(~0x3FFF);
	mappedBuffer.size32 = bufferSize32;
	mappedBuffer.owner  = ncl_getProcessorNo(); //must be done on host 
	
	// создаем массив отображаемый в файл
	mappedBuffer.handle = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		mappedBuffer.size32*4,   // maximum object size (low-order DWORD)
		createName(TEXT(CONST_BUFFER_MAPPING_NAME),mappedBuffer.owner,pBufferRegistry->count));			 // name of mapping object

	if (mappedBuffer.handle == NULL) {
		_tprintf(TEXT("ERROR:Could not open file mapping object (%d).\n"),GetLastError());
		return 0;
	}
	// получаем указатель
	mappedBuffer.address = (int*)MapViewOfFile(
		mappedBuffer.handle ,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		mappedBuffer.size32*4);

	if (mappedBuffer.address == NULL) {
		_tprintf(TEXT("ERROR:Could not map view of file (%d) \n"),GetLastError());
		return 0;
	}

	//----------- регистрируем новый буфер в реестре  ----------
	memcpy(pBufferRegistry->buffer+pBufferRegistry->count,&mappedBuffer,sizeof(MappedBuffer));
	pBufferRegistry->count++;
	return mappedBuffer.address;
}
/*
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
*/
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
	//for()
	//UnmapViewOfFile(shared);
	//CloseHandle(hMapFileSharedMem);	
	//UnmapViewOfFile(pSyncBuf);
	//CloseHandle(hMapFile);
	return 0;
}