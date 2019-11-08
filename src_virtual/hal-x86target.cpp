#include "hal_target.h"
#include "hal.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>
#pragma comment(lib, "user32.lib")

#include "hal-defs.h"
HANDLE hBufferRegistry=0;
BufferRegistry* pBufferRegistry=0;
extern int procNo;
extern  SyncBuf  *pSyncBuf;

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

extern "C"{

	void halSetProcessorNo(int number){
		procNo=number;
	}
};
// ¬озвращает указатель на буфер синхронизации
SyncBuf* getSyncBuffer(){
	if (pSyncBuf==0){
		HANDLE hMapFile;
		// пытаемс€ открыть существующий буфер
		for (int i=0; i<HOST_CONNECT_TIMEOUT; i+=100){
			hMapFile = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,   // read/write access
				FALSE,                 // do not inherit the name
				TEXT(SYNC_BUFFER_MAPPING_NAME));               // name of mapping object
			if (hMapFile) break;
			halSleep(100);
		}
		// пытаемс€ создать сами 
		if (hMapFile == NULL){
			hMapFile = CreateFileMapping(
				INVALID_HANDLE_VALUE,    // use paging file
				NULL,                    // default security
				PAGE_READWRITE,          // read/write access
				0,                       // maximum object size (high-order DWORD)
				SYNC_BUF_SIZE,           // maximum object size (low-order DWORD)
				TEXT(SYNC_BUFFER_MAPPING_NAME));              // name of mapping object

			if (hMapFile == NULL) {
				printf("Could not create file sync mapping object (%d).\n", GetLastError());
				return 0;
			}
		}

		pSyncBuf = (SyncBuf*) MapViewOfFile(
			hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			SYNC_BUF_SIZE);

		if (pSyncBuf == NULL){
			printf("Could not map view of file (%d).\n",GetLastError());
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
	// пытаемс€ открыть кем то созданный реест
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

	// –еест еще не создан , создаем сами
	hBufferRegistry = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		sizeof(BufferRegistry),          // maximum object size (low-order DWORD)
		registryName);         // name of mapping object

	if (hBufferRegistry == NULL) {
		printf("ERROR:Could not create registry file mapping object (%d).\n",GetLastError());
		return 0;
	}
	
	pBufferRegistry = (BufferRegistry*)MapViewOfFile(
		hBufferRegistry,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(BufferRegistry));

	if (pBufferRegistry == NULL) {
		printf("ERROR:Could not map registry view of file (%d) \n",GetLastError());
		return 0;
	}

	memset(pBufferRegistry,0,sizeof(BufferRegistry));
	return pBufferRegistry;
}

// а если повтор ???
/*
int* halGetBuffer(int bufferID){
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
int* halMalloc32(int bufferSize32){
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
		printf("ERROR:Could not open file mapping object (%d).\n",GetLastError());
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
		printf("ERROR:Could not map view of file (%d) \n",GetLastError());
		return 0;
	}

	//----------- регистрируем новый буфер в реестре  ----------
	memcpy(pBufferRegistry->buffer+pBufferRegistry->count,&mappedBuffer,sizeof(MappedBuffer));
	pBufferRegistry->count++;
	return mappedBuffer.address;
}
/*
int halConnect(int* masterSharedBuffer, int masterSharedSize32, int** sharedBuffer, int* sharedSize32){

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


	int ok=halHostSync(0x64060000+halGetProcessorNo());
	//------------------------------
	//LPVOID sharedBuffer = 0;
	//LPVOID sharedBuffer = 0;
	if (masterSharedBuffer==0 && masterSharedSize32==0){
		*sharedSize32=halHostSync(0);
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
		halHostSync((int)*sharedBuffer);
	}
	else {
		halHostSync((int)masterSharedSize32);
		halHostSync((int)masterSharedBuffer);
		
	}


	//printf("Target" STR(PROCESSOR_ID)  "-ok\n");
	
}
*/
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

int halSyncArray(
					 int value,        // Sync value
					 void *outAddress, // Sended array address (can be NULL)
					 size_t outLen,    // Sended array length (can be 0)
					 void **inAddress, // Received array address pointer (can be NULL)
					 size_t *inLen,   // Received array size pointer (can be NULL)
					 int  procNo)
{
	int sync=halSync(value,procNo);
	if (inAddress){
		*inAddress=(void*)halSync((int)outAddress);
		MirrorBuffer* mirrorBuffer=findBuffer((unsigned)(*inAddress),procNo);
		if (mirrorBuffer==0){
			*inAddress=0;
			*inLen=0;
			return sync;
		}
		*inAddress=(void*)((int*)(*inAddress)+mirrorBuffer->diff);
	}
	else 
		halSync((int)outAddress);
	if (inLen)
		*inLen=halSync(outLen);
	else 
		halSync(outLen);

	
	return sync;
}

void* halSyncAddr(
					 void *outAddress, // Sended array address (can be NULL)
					 int  procNo)
{
	void *inAddress=(void*)halSync((int)outAddress,procNo);
	if (inAddress){
		MirrorBuffer* mirrorBuffer=findBuffer((unsigned)(inAddress),procNo);
		if (mirrorBuffer==0){
			return 0;
		}
		inAddress=(void*)((int*)(inAddress)+mirrorBuffer->diff);
	}
	return inAddress;
}


extern "C" {
void halFree(void* ){
	/*
	halHostSync((int)0x600DBA1+halGetProcessorNo());
	UnmapViewOfFile(shared);
	CloseHandle(hMapFileSharedMem);	
	UnmapViewOfFile(pSyncBuf);
	CloseHandle(hMapFile);
	*/
}

void halHandshake(int procNo){
	halSync(777, procNo);
}
};

int halDisconnect(int* shared){
	halHostSync((int)0x600DBA1+halGetProcessorNo());
	//for()
	//UnmapViewOfFile(shared);
	//CloseHandle(hMapFileSharedMem);	
	//UnmapViewOfFile(pSyncBuf);
	//CloseHandle(hMapFile);
	return 0;
}