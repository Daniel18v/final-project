#pragma once
#ifndef WIN32
#	include <emscripten/bind.h>
#endif // #ifndef WIN32

typedef unsigned char UINT8;
typedef short INT16;
typedef int INT32;
typedef unsigned int UINT, UINT32;
typedef long long INT64;
typedef unsigned long long UINT64;

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
//#define NULL 0