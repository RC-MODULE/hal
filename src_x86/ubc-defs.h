#define SYNC_BUF_SIZE 64*1024
TCHAR sSyncName[]=TEXT("Global\\SyncFileMappingObject");

#define SYNC_BUF_SIZE 64*1024
TCHAR sMallocRegestryName[]=TEXT("Global\\RegistryFileMappingObject");


TCHAR sSharedMemName[]=TEXT("Global\\SharedFileMappingObject");

struct MappedBuffer {
	unsigned ownerCore;
	unsigned ownerAddress;
	unsigned size32;
	unsigned locked;
};