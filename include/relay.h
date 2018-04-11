#include <windows.h>

HANDLE halOpenRelay(char* PortName);
int halResetRelay(char* PortName, int relay, int delay);
