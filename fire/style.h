#pragma once
#include "typedefs.h"

#if 0
#pragma warning( disable : 4351 ) // array zero initialized in initalizer list
#pragma warning( disable : 4201 ) // nameless structs
#pragma warning( disable : 4996 ) // swap ranges
#pragma warning( disable : 4800 ) // !!
#pragma warning( disable : 4239 ) // tmporary rval
#pragma warning( disable : 4005 ) // just die
#pragma warning( disable : 4706 ) // assignment in conditional
#pragma warning( disable : 4189 ) // intialization without use
#pragma warning( disable : 4100 ) // unreferenced formal parameter
#pragma warning( disable : 4305 ) // truncation from double to float
#pragma warning( disable : 4018 ) // signed/unsigned mismatch
#pragma warning( disable : 4244 )
#pragma warning( disable : 4005 )
#pragma warning( disable : 4995 ) // Depreciated functions
#endif

#define TIME_UTC_ TIME_UTC
#include <boost/lexical_cast.hpp>
#include <cassert>
#include <algorithm>
#include <vector>

#ifndef LITTLE_ENDIAN
	#define LITTLE_ENDIAN
#endif
#ifndef LITTLE_ENDIAN
        #define BIG_ENDIAN
#endif

#define deny(x) assert(!(x))
#define fail deny(true)
#define self (*this)
#define STLALL(x) (x).begin(), (x).end()
#define DO_TIMES(i, n) for( int (i)=(n)-1; (i)>=0; --(i))
#define BLANKIFY(s) memset(&s, 0, sizeof(s))
#define CCALL(x, y) if( x ) x->y
#define elif else if
#define bswitch switch
#define bcase break; case
#define bdefault break; default
//#define chdir SetCurrentDirectoryW

#define hash_map unordered_map
#define hash_set unordered_set

#define WITH(v)      \
    if(int cnt_ = 1) \
        for(auto& _ = v; cnt_; --cnt_)

typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ulonglong;
typedef int8_t             int8;
typedef int16_t            int16;
typedef int32_t            int32;
typedef int64_t            int64;
typedef uint8_t            uint8;
typedef uint16_t           uint16;
typedef uint32_t           uint32;
typedef uint64_t           uint64;
typedef float              real32;
typedef double             real64;


typedef void* voidP;
typedef void   const* voidCP;
typedef  int   const* intCP;
typedef uint   const* uintCP;
typedef  short const* shortCP;
typedef ushort const* ushortCP;
typedef char   const* charCP;
typedef double const* doubleCP;

typedef std::wstring const& wstringCR;
typedef std::string const&  stringCR;
typedef std::wstring        filename;
typedef filename const&     filenameCR;

typedef std::vector<uint16> IndexList16;
typedef std::vector<uint32> IndexList32;
typedef IndexList16 const*  IndexList16CP;
typedef IndexList16 const&  IndexList16CR;
typedef IndexList32 const*  IndexList32CP;
typedef IndexList32 const&  IndexList32CR;

template<typename T> void zap (T*& p)                              {delete p; p = nullptr;}
template<typename T> T    max3(T const& a, T const& b, T const& c) {return std::max(a, std::max(b, c));}
template<typename T> T    min3(T const& a, T const& b, T const& c) {return std::min(a, std::min(b, c));}

#if defined( _DEBUG ) || defined( DEBUG )
        #ifndef DEBUG
                #define DEBUG
        #endif
#else
        #ifndef RELEASE
                #define RELEASE
        #endif
#endif

#ifdef DEBUG
        #define verify(x) assert(x)
#else
        #define verify(x) (x)
#endif
