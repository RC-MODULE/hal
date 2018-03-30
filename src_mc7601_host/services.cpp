//////////////////////////////////////////////////////////////////////////
//                                                                      //
// services.cpp -                                                       //
//                                                                      //
//      Механизм предоставления доступа к периферии хоста нм-программе. //
//                                                                      //
//      Замечание: реализация рассчитана на разреженное                 //
//      представление char-ов, (обычное для НМ) см. charToSparse        //
//         ответная часть на цели - env_call.c                          //
//////////////////////////////////////////////////////////////////////////
#include <cassert>
#include <io.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <set>
#include "nm_io_host.h"
#include "host_duties.h"
#include "libelf.h"

#include "mc7601_ll_host.h"
//	Рассчитываем на использование любой библиотеки загрузки и обмена

//	END #include "mc5101load.h"

using namespace std;

#define NM_SERV_DBG 0

bool NM_IO_Service::work= false;

unsigned int __stdcall nmServiceDispatcher( void* v_nms );

int* 
NM_IO_Service::readStatus()
{
    if ( WR_ReadMemBlock( a, (WR_Word*)bsStatus, datSecAddr, bobSize) != WR_OK )
        throw "status block reading failure";
    return bsStatus;
}

void 
NM_IO_Service::writeBackStatus()
{
    if ( WR_WriteMemBlock( a, (WR_Word*)bsStatus +bobSizeIn, 
                                    datSecAddr +bobSizeIn, 
                                    bobSize -bobSizeIn) != WR_OK )
        throw "status block writing failure";
}

NM_IO_Service::~NM_IO_Service()
{
	if ( valid ){
		letsShutDown= true;
		while ( letsShutDown )
			Sleep(100);   //  Ждем завершения nmServiceDispatcher
		delete[] bsStatus;
		delete[] bsBuf;
	}
//	WR_CloseAccess( a );!!!!!!!!!!!!!!!!!!!!!!
    work= false;
}

//string lastS;
//bool lastB=true;

const size_t charBufSize= 0x8000;

size_t
sparseCharToChar( char* dst, volatile int* src, size_t count ){
    size_t res= 0;
    size_t i;
    if ( count > charBufSize )
        throw "string buffer overflow";
    for ( i=0; i<count; i++ ){
        char c= *dst++= (char)*src++;
        res++;
        //if ( c==0 )
        //    break;
    }
    return res; 
}

size_t
charToSparse( WR_Word* dst, char* src, size_t count ){
    size_t res= 0;
    size_t i;
    if ( count > backBufferSize )
        throw "host-to-board buffer overflow";
    for ( i=0; i<count; i++ ){
        WR_Word c= *dst++= (WR_Word)(unsigned char)*src++;
        res++;
        //if ( c==0 )
        //    break;
    }
    return res; 
}

WR_Word 
NM_IO_Service::processPrintf( int* bufferFromBoard, int received  )
{
    char buf[ charBufSize];
    sparseCharToChar( buf, bufferFromBoard, received );
    int i;
    for ( i=0; i<received; i++ ){
        *nm_cout << buf[i];
    }
    return WR_OK;
}

NM_IO_Service::NM_IO_Service( const char* fName, WR_Access* access, 
		ostream* _cout, ostream* _cerr, istream* _cin )
:letsShutDown(false), a( access ), valid(true), nm_cout(_cout)
{
    if ( work )
        throw "only one nmservice object allowed";
    work= true;

	ELF *elf = new ELF(fName, ELF_C_READ);

    int ready=0;
    try {
        ELF_SCN *scn;
        for( scn= elf->NextScn(0); scn!=0; scn = elf->NextScn(scn) ) {
            Elf32_Shdr *shdr = scn->GetShdr();
            if ( strcmp( scn->GetName(), ".rpc_services" )==0 ){
                //  адрес и размер на плате
                datSecAddr= shdr->sh_addr;
                datSecSize= shdr->sh_size/4;
				if ( datSecSize ){
					bsStatus= new int[bobSize];
					ready|= 1;
				}
            }
            if ( strcmp( scn->GetName(), ".rpc_services.bss" )==0 ){
                //  адрес и размер на плате
                bssSecAddr= shdr->sh_addr;
                bssSecSize= shdr->sh_size/4;
				if( bssSecSize ){
					assert( bssSecSize==b2hBufSize + backBufferSize );
					bsBuf= new int[b2hBufSize];
					ready|= 2;
				}
            }
        }
    }
    catch (ELF_ERROR) {
        cerr << endl << "error in NM_IO_Service" << endl;
        throw;
    }
    catch (const char* msg) {
        cerr << endl << "error in NM_IO_Service: " << msg << endl;
        throw;
        //return false;
    }

	if (ready==3){
        _beginthreadex( 0, 0, nmServiceDispatcher, (void*)this, 0, 0);
	}
	else{
		valid= false;
		//throw( "can't start NM_IO_Service - no .rpc_services section" );
	}
}


unsigned int __stdcall nmServiceDispatcher( void* v_nms )
{
    NM_IO_Service& nms= *static_cast<NM_IO_Service*>(v_nms);
    return nms.dispatch();
}

#pragma pack(4); //  int alignment

struct _stat64i32_sparse {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned int st_mode;
        int      st_nlink;
        int      st_uid;
        int      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;

        void absorb( struct _stat64i32 &A )
        {
             st_dev= A.st_dev;
             st_ino= A.st_ino;
             st_mode= A.st_mode;
             st_nlink= A.st_nlink;
             st_uid= A.st_uid;
             st_gid= A.st_gid;
             st_rdev= A.st_rdev;
             st_size= A.st_size;
             st_atime= A.st_atime;
             st_mtime= A.st_mtime;
             st_ctime= A.st_ctime;
        }
};

#pragma pack(); //  default

unsigned int 
NM_IO_Service::dispatch()
{
    int hostReceive= 0;
    int sendConfirmed= bssSecAddr;
    bool proceed= true;
    WR_Word bufferToBoard[ backBufferSize ];
    set<int> isTextMode;
    while ( proceed )
    {
	    Sleep(10);
        proceed= !letsShutDown;
        int* bsStatus= readStatus();
        //  модифицируются только с платы+ (sended - bssSecAddr)
        const int& boardSend=      bsStatus[0];       //  .rpc_services +0
        if ( hostReceive==boardSend )
            continue;   //  ничего нового
        const int& sended=         bsStatus[1];       //  .rpc_services +1
        const int& sendMostDistant=bsStatus[2];       //  .rpc_services +2
        //assert( ((boardSend&0xffff0000) ==0) && ((sended&0xfff00000) ==0x10000000));
#if NM_SERV_DBG
        if ( sended!=sendConfirmed ){
            cerr << "  ptr:" << hex << sended;
            cerr << "  count_s:" << dec << boardSend;
            cerr << "  count_r:" << hostReceive;
            cerr << "  sended:" << hex << sended;
            cerr << "  received:" << sendConfirmed << dec;
            cerr << endl;
        }
#endif
        //  модифицируются только с хоста
        assert ( hostReceive   ==bsStatus[4]); //  .rpc_services +4
        assert ( sendConfirmed ==bsStatus[5]); //  .rpc_services +5
        //  забираем данные с платы
        int& test=         bsStatus[3];       //  .rpc_services +3
        WR_Word r;
        int* hostBuf= bsBuf;
        if ( sended< sendConfirmed ){
            //  нужен новый проход по буферу
            //  читаем от sendedOld до sendMostDistant
            int words= sendMostDistant- sendConfirmed;
            assert (words>=0);
            if ( words ){
                r= WR_ReadMemBlock( a, (WR_Word*)hostBuf, sendConfirmed, words);
                if ( r!= WR_OK )
                    throw "error reading block from board";
                hostBuf+= words;
            }
            sendConfirmed= bssSecAddr;
        }
        volatile int& backBufferReady=bsStatus[6];       //  .rpc_services +6
        //  читаем от sendedOld до sended
        int words= sended- sendConfirmed;
        if (words){
            r= WR_ReadMemBlock( a, (WR_Word*)hostBuf, sendConfirmed, words);
            if ( r!= WR_OK )
                throw "error reading block from board";
            //  сколько всего, для проверки
            words+= hostBuf- bsBuf;
            sendConfirmed= sended;

            volatile int* sendedHere= bsBuf;
            char charBuf[ charBufSize];

            while  ( hostReceive <boardSend ){
                //  принять пакет
                int pSize= *sendedHere++;
                NM_IO_ServiceID pKind= (NM_IO_ServiceID)( *sendedHere++);
                //  обработать пакет
                errno= 0;
                switch ( pKind )
                {
                case NMSYNC_SERVICE_PRINTF:
                    processPrintf( (int*)sendedHere, pSize );
                    break;
                case INDEX_PUTCHAR:
                    putchar( *(char*)sendedHere );   //  x86 is little-endian
                    break;
                case INDEX_GETCHAR:
                    assert( backBufferReady ==0 );
                    backBufferReady= 1;
                    bufferToBoard[0]= (WR_Word)getchar(); 
                    break;
                case INDEX_OPEN:
                    assert( backBufferReady ==0 );
                    {
                        backBufferReady= 2;
                        sparseCharToChar( charBuf, sendedHere, pSize-1 );
                        int mode= *(sendedHere+pSize-1);
                        int errno1 = errno;
                        int errno1d = _doserrno;
                        int fd= (WR_Word)_open( charBuf, mode, _S_IREAD | _S_IWRITE ); 
                        int errno2 = errno; //  Непонятно: При mode==0x10a возвращает валидный дескриптор, но errno устанавливается в 22(EINVAL)
                        int errno2d = _doserrno;
                        if ( fd !=-1 && ( ( mode & _O_TEXT ) || 
                                        ( ( ( mode & _O_BINARY )==0 )&& 
                                        ( ( _fmode & _O_BINARY )==0 ) ) ) ){
                            isTextMode.insert( fd );
                        }
                        bufferToBoard[0]= fd;
                        bufferToBoard[1]= ((fd !=-1) ? 0 : errno);
                    }
                    break;
                case INDEX_CLOSE:
                    assert( backBufferReady ==0 );
                    backBufferReady= 2;
                    bufferToBoard[0]= (WR_Word)!_close( *sendedHere ); 
                    isTextMode.erase( (int)*sendedHere );
                    bufferToBoard[1]= errno;
                    break;
                case INDEX_READ:
                    assert( backBufferReady ==0 );
                    {
                        volatile int* ptr= sendedHere;
                        int fd= *ptr++;
                        int isText= (int)isTextMode.count( fd );
                        WR_Addr brdBuf= *ptr++;
                        unsigned int count= *ptr++;
                        if ( backBufferReady > backBufferSize )
                            throw ( "_read function overflows buffer" );
                        int bytes;
                        if ( isText ){
                            bytes= (WR_Word)_read( fd, charBuf, count ); 
                        }
                        else{
                            bytes= (WR_Word)_read( fd, bufferToBoard+2, count*4 ); 
                        }
                        bufferToBoard[1]= errno;
                        if ( bytes != WR_Word(-1) ){
                            if ( isText ){
                                bufferToBoard[0]= bytes;
                                charToSparse( bufferToBoard+2, charBuf, bufferToBoard[0] );
                            }
                            else{
                                bufferToBoard[0]= (bytes+3)/4;
                            }
                            backBufferReady= bufferToBoard[0]+2;
                        }
                        else{
                            bufferToBoard[0]= -1;
                            backBufferReady= 2;
                        }
                    }
                    break;
                case INDEX_WRITE:
                    assert( backBufferReady ==0 );
                    {
                        volatile int* ptr= sendedHere;
                        int fd= *ptr++;
                        int isText= (int)isTextMode.count( fd );
                        unsigned int count= *ptr++;
                        int bytes;
                        if ( isText ){
                            sparseCharToChar( charBuf, ptr, count );
                            bytes= (WR_Word)_write( fd, charBuf, count ); 
                            bufferToBoard[0]= bytes;
                        }
                        else{
                            bytes= (WR_Word)_write( fd, (int*)ptr, count*4 ); 
                            bufferToBoard[0]= ( bytes== -1 ? -1: bytes/4 );
                        }
                        bufferToBoard[1]= errno;
                        backBufferReady= 2;
                    }
                    break;
                case INDEX_FLUSH:
                    _flushall();    //  какое облегчение.
                    break;
                case INDEX_GETPOS:
                {
                    volatile int* ptr= sendedHere;
                    int fd= *ptr++;
                    long pos= _tell( fd ); 
                    assert( sizeof(long)==sizeof(WR_Word) );
                    bufferToBoard[0]= (WR_Word)pos;
                    bufferToBoard[1]= errno;
                    backBufferReady= 2;
                    break;
                }
                case INDEX_SETPOS:
                {
                    volatile int* ptr= sendedHere;
                    assert( sizeof(long)==sizeof(WR_Word) );
                    int fd= *ptr++;
                    long pos= *ptr++;
                    bufferToBoard[0]= _lseek( fd, pos, SEEK_SET );
                    bufferToBoard[1]= errno;
                    backBufferReady= 2;
                    break;
                }
                case INDEX_RENAME:
                {
                    volatile int* ptr= sendedHere;
                    sparseCharToChar( charBuf, ptr, pSize );
                    char* oldOne= charBuf;
                    char* newOne= charBuf+ strlen( oldOne )+1;
                    assert( strlen( oldOne )+strlen( newOne )+2 ==pSize );
                    bufferToBoard[0]= rename( oldOne, newOne );
                    bufferToBoard[1]= errno;
                    backBufferReady= 2;
                    break;
                }
                case INDEX_REMOVE:
                {
                    volatile int* ptr= sendedHere;
                    sparseCharToChar( charBuf, ptr, pSize );
                    char* path= charBuf;
                    assert( strlen( path )+1 ==pSize );
                    bufferToBoard[0]= remove( path );
                    bufferToBoard[1]= errno;
                    backBufferReady= 2;
                    break;
                }
                case INDEX_GETENV:
                {
                    volatile int* ptr= sendedHere;
                    sparseCharToChar( charBuf, ptr, pSize );
                    char* varName= charBuf;
                    assert( strlen( varName )+1 ==pSize );
                    char* val= getenv( varName );
                    bufferToBoard[0]= errno;
                    int valLen= strlen( val );
                    charToSparse( bufferToBoard+1, val, valLen+1 );
                    backBufferReady= valLen+2;
                    break;
                }
                case INDEX_SYSTEM:
                {
                    volatile int* ptr= sendedHere;
                    sparseCharToChar( charBuf, ptr, pSize );
                    char* command= charBuf;
                    assert( strlen( command )+1 ==pSize );
                    bufferToBoard[0]= system( command );
                    bufferToBoard[1]= errno;
                    backBufferReady= 2;
                    break;
                }
                case INDEX_FSTAT:
                {
                    volatile int* ptr= sendedHere;
                    int fd= *ptr++;
                    _stat64i32_sparse* stsp( (_stat64i32_sparse*)( bufferToBoard+2) );
                    struct _stat64i32 st;
                    bufferToBoard[0]= _fstat64i32( fd, &st );
                    stsp->absorb( st );
                    bufferToBoard[1]= errno;
                    backBufferReady= 2+sizeof(struct _stat64i32_sparse)/sizeof(WR_Word);
                    break;
                }
                default:
			        cerr << endl << "unknown service called" << endl;
//                    throw( "unknown service called" );
                };
                sendedHere+= pSize;
                hostReceive++;
            }
            assert( sendedHere-bsBuf == words);
        }
        bsStatus[4]= hostReceive;
        bsStatus[5]= sendConfirmed;
        bsStatus[6]= backBufferReady;
        if ( sendConfirmed>sended )
            assert( sendConfirmed<=sendMostDistant );
        if ( backBufferReady )
            if ( WR_WriteMemBlock( a, bufferToBoard, 
                                    bssSecAddr +b2hBufSize, 
                                    backBufferReady) != WR_OK )
                throw "_read data writing to board failure";

        writeBackStatus();
    }
#if NM_SERV_DBG
    cerr << "===fin===";
    char c;
    cin >> c;
#endif
    letsShutDown= false;
    return 0;
}
