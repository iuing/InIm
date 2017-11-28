#include "stdafx.h"
#include "utils.h"
#ifdef OS_WIN
#pragma comment(lib,"winmm.lib")
#endif // OS_WIN



int XGetTimestamp(void)
{
#ifdef OS_WIN
	return ::timeGetTime();//∫¡√Î
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec * 1000 + now.tv_usec / 1000;
#endif
}
