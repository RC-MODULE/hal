
//TCHAR sMallocRegestryName[]=TEXT(REGISTRY_BUFFER_MAPPNING_NAME);
//TCHAR sSharedMemName[]     =TEXT(SYNC_BUFFER_MAPPING_NAME);

#define CONST_BUFFER_MAPPING_NAME "Global\\FileMappingObject" 
#define BUFFER_REGISTRY_MAPPNING_NAME "Global\\RegistryFileMappingObject" 
#define CONST_REGISTRY_MAPPING_NAME "Global\\RegistryFileMappingObject_" 
#define BUFFER_REGISTRY_MAX_SIZE 100
#define SYNC_BUFFER_MAPPING_NAME "Global\\SyncFileMappingObject"
#define SYNC_BUF_SIZE 64*1024
#define MAX_COUNT_PROCESSORS 8

TCHAR* createName(TCHAR* baseName, int index0, int index2);
struct SyncBuf {
	//int		counter0;
	//int		counter1;
	int		sync0;
	int		sync1;
	//bool	locked0;
	//bool	locked1;
	//bool    flag[2];
	//bool	turn;
	//int		readFrom[2];
	//int		writtenFrom[2];
	//int     counter;
	int		readCounter[2];
	int		writeCounter[2];
};

struct MappedBuffer {
	int*     address;
	unsigned size32;
	int		 owner;
	HANDLE   handle;
};   

struct BufferRegistry {
	int count;
	int numProcessors;
	int isLocked;
	MappedBuffer buffer[BUFFER_REGISTRY_MAX_SIZE];
};

struct MirrorBuffer {
	int*     address;
	unsigned size32;
//	int		 owner;
	int*	 mirror;
	int		 diff;
	HANDLE   handle;
};   

struct MirrorRegistry{
	BufferRegistry* pBufferRegistry;
	HANDLE          hBufferRegistry;
	int				count;
	MirrorBuffer	buffer[BUFFER_REGISTRY_MAX_SIZE];
};


#define SYNC_BUF_SIZE 64*1024


#define REGISTRY_BUF_SIZE 64*1024

