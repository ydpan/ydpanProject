#ifndef _DATAGRAM_H_
#define _DATAGRAM_H_
#include <stdint.h>
#include <string>
#include <vector>

#ifdef _WIN32
typedef signed   char     int8_t;
typedef unsigned char     uint8_t;

typedef __int16           int16_t;
typedef unsigned __int16  uint16_t;

typedef __int32           int32_t;
typedef unsigned __int32  uint32_t;

typedef __int64           int64_t;
typedef unsigned __int64  uint64_t;
#else

#endif
typedef int8_t         _s8;
typedef uint8_t        _u8;

typedef int16_t        _s16;
typedef uint16_t       _u16;

typedef int32_t        _s32;
typedef uint32_t       _u32;

typedef int64_t        _s64;
typedef uint64_t       _u64;
using namespace std;

typedef std::vector<uint8_t> ByteVector;
typedef struct tagDataGram 
{
	uint8_t sender;
	uint8_t receiver;
	ByteVector data;
}DataGram;

#endif

