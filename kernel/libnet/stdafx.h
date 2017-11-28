
//平台定义
#if defined(_WIN32) || defined(_WIN64)
#define OS_WIN
#elif defined (__linux__)
#define OS_LINUX
#elif defined(__mac__)
#define OS_IOS
#endif

//windows平台
#if defined(OS_WIN)

#pragma once
#define WIN32_LEAN_AND_MEAN            
#include <windows.h>
#include<WinSock2.h>
#include <WS2tcpip.h>
#include <timeapi.h>
#pragma comment(lib,"ws2_32.lib")

//导出标识
#if !defined(LIBNET_EXPORT)
#define NET_EXPORT __declspec(dllexport)
#else
#define NET_EXPORT __declspec(dllimport)
#endif

#endif

//linux
#if defined(OS_LINUX)
#define NET_EXPORT
#include <unistd.h>
#endif



#ifndef SOCKET
#define SOCKET int
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif


#include <string>
#include <map>
#include <list>

#include "Lock.h"
typedef int int32_t;
typedef unsigned int uint32_t;