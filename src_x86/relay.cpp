#include <windows.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

HANDLE OpenComPort(char* PortName)
{
    HANDLE hCom = CreateFileA(PortName,                //port name
                             GENERIC_READ | GENERIC_WRITE, //Read/Write
                             0,                            // No Sharing
                             NULL,                         // No Security
                             OPEN_EXISTING,// Open existing port only
                             0,            // Non Overlapped I/O
                             NULL);

    if (hCom == INVALID_HANDLE_VALUE) {
        cout << "Error in opening serial port";
        return INVALID_HANDLE_VALUE;
    }
    cout << "Opening serial port successful";
    return hCom;
}

bool CloseComPort(HANDLE ComPort)
{
    if(CloseHandle(ComPort)) return true;
    return false;
}

bool SetComPortParams(HANDLE ComPort, DCB** ConfSP)
{
    DCB* conf = new DCB[1];
    //*conf = 0;
    conf->DCBlength = sizeof(*conf);
    if(GetCommState(ComPort, conf)) {
        conf->BaudRate = CBR_9600;  // Setting BaudRate = 9600
        conf->ByteSize = 8;         // Setting ByteSize = 8
        conf->StopBits = ONESTOPBIT;// Setting StopBits = 1
        conf->Parity   = NOPARITY;  // Setting Parity = None
        *ConfSP = conf;
        return true;
    }
    return false;
}

bool ConfigureComPort(HANDLE ComPort, DCB* Params)
{
   if(SetCommState(ComPort, Params)) return true;
   return false;
}

int SizeOf(const char* dat)
{
    int size = 0;
    while(*dat != '\0') {
        size += sizeof(*dat);
        dat++;
    }
    return ++size;
}

bool WriteComPort(HANDLE ComPort, const char data)
{
    DWORD dNoOFBytestoWrite = sizeof(data);         // No of bytes to write into the port
    DWORD dNoOfBytesWritten = 0;                    // No of bytes written to the port
    bool Status = WriteFile(ComPort,        // Handle to the Serial port
                       &data,     // Data to be written to the port
                       dNoOFBytestoWrite,  //No of bytes to write
                       &dNoOfBytesWritten, //Bytes written
                       NULL);
    return Status;
}

void ReadComPort(HANDLE ComPort)
{
    DWORD dwEventMask;
    bool Status = SetCommMask(ComPort, EV_RXCHAR);
    Status = WaitCommEvent(ComPort, &dwEventMask, NULL);

    char TempChar; //Temporary character used for reading
    char SerialBuffer[256];//Buffer for storing Rxed Data
    DWORD NoBytesRead;
    int i = 0;

    do
    {
       ReadFile(ComPort,           //Handle of the Serial port
                 &TempChar,       //Temporary character
                 sizeof(TempChar),//Size of TempChar
                 &NoBytesRead,    //Number of bytes read
                 NULL);

       SerialBuffer[i] = TempChar;// Store Tempchar into buffer
       i++;
     }
     while (NoBytesRead > 0);
}

HANDLE InitComPort(char* PortName)
{
    HANDLE RetVal = INVALID_HANDLE_VALUE;
    HANDLE ComPort = OpenComPort(PortName);
    if(ComPort == RetVal) return RetVal;
    DCB* ConfSP;
    if(!SetComPortParams(ComPort, &ConfSP)) return RetVal;
    if(!ConfigureComPort(ComPort, ConfSP)) return RetVal;
    if(!WriteComPort(ComPort, 0x50)) return RetVal;
    Sleep(300);
    if(!WriteComPort(ComPort, 0x51)) return RetVal;
    Sleep(100);
    return ComPort;
}

HANDLE halOpenRelay(char* PortName)
{
    if(!PortName) {
        PortName = getenv("RELAY_COMPORT");
    }
    HANDLE ComPort = InitComPort(PortName);
    //setenv(RELAY_STATE);
    if(ComPort == INVALID_HANDLE_VALUE) return ComPort;
    CloseComPort(ComPort);
    return ComPort;
}


BOOL halResetRelay(char* PortName, int relay, int delay)
{
    if(!PortName) {
        PortName = getenv("RELAY_COMPORT");
    }
    //char* state=getenv("RELAY_STATE");
    //int state=atoi(state);
    

    BOOL RetVal;// = INVALID_HANDLE_VALUE;
    HANDLE ComPort = OpenComPort(PortName);
    if(ComPort == INVALID_HANDLE_VALUE) return 1;
    DCB* ConfSP;
    if(!SetComPortParams(ComPort, &ConfSP)) return 1;
    if(!ConfigureComPort(ComPort, ConfSP)) return 1;
   

    if(!(RetVal=WriteComPort(ComPort, 1<<relay))) return 1;
    Sleep(delay);
    if(!(RetVal=WriteComPort(ComPort, 0 ))) return 1;
    Sleep(delay);
    CloseComPort(ComPort);
    return 0;
}
