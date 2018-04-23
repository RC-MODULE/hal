#include <relay.h>

int main()
{

	HANDLE ComPort = halOpenRelay(0);
	int st = halResetRelay(0, 0, 500);
	st = halResetRelay(0, 1, 500);
	return 0;
}