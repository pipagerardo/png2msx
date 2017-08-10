#ifndef _ENDIAN_
#define _ENDIAN_

#include "funciones.h"

#if ( defined(_WIN16) || defined(_WIN32) || defined(_WIN64) ) && !defined(__WINDOWS__) 
	#define __WINDOWS__ 
#endif 
 
#if defined(__linux__) || defined(__CYGWIN__) 
	#include <endian.h> 
//	#define	__BYTE_ORDER	( __LITTLE_ENDIAN ? __BIG_ENDIAN )
//	#define	__LITTLE_ENDIAN	1234
//	#define	__BIG_ENDIAN	4321
	#define BYTE_ORDER	__BYTE_ORDER     
	#define BIG_ENDIAN	__BIG_ENDIAN    
	#define LITTLE_ENDIAN	__LITTLE_ENDIAN 
#elif defined(__OpenBSD__) 
	#include <sys/endian.h> 
//	#define	BYTE_ORDER	( LITTLE_ENDIAN ? BIG_ENDIAN )
//	#define LITTLE_ENDIAN	1234
//	#define BIG_ENDIAN	4321
#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__) 
	#include <sys/endian.h> 
//	Supongo que igual que en (__OpenBSD__)
//	#define	BYTE_ORDER	( LITTLE_ENDIAN ? BIG_ENDIAN )
//	#define LITTLE_ENDIAN	1234
//	#define BIG_ENDIAN	4321
#elif defined(__WINDOWS__) 
	#include <sys/param.h> 
//	#define BYTE_ORDER	( LITTLE_ENDIAN ? BIG_ENDIAN )
//	#define LITTLE_ENDIAN	1234
//	#define BIG_ENDIAN	4321
#elif defined(__APPLE__) 
	#include <sys/param.h> 
//	#define BYTE_ORDER	( LITTLE_ENDIAN ? BIG_ENDIAN )
//	#define LITTLE_ENDIAN	1234
//	#define BIG_ENDIAN	4321
#else 
	#error platform not supported 
#endif 

inline Uint16 Swap16( Uint16 x ) {
    return static_cast<Uint16>( (x << 8) | (x >> 8) );
}

inline Uint32 Swap32( Uint32 x ) {
	return static_cast<Uint32>( (x << 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | (x >> 24) );
}

inline Uint64 Swap64( Uint64 x ) {
    Uint32 hi, lo;
    lo = static_cast<Uint32>( x & 0xFFFFFFFF );
    x >>= 32;
    hi = static_cast<Uint32>( x & 0xFFFFFFFF );
    x  = Swap32( lo );
    x <<= 32;
    x |= Swap32( hi );
    return (x);
}

inline float SwapFloat(float x) {
    union {
        float f;
        Uint32 ui32;
    } swapper;
    swapper.f = x;
    swapper.ui32 = Swap32( swapper.ui32 );
    return swapper.f;
}

inline double SwapDouble( double x ) {
 	union {
		double f;
 		Uint64 ui64;
 	} swapper;
 	swapper.f = x;
	swapper.ui64 = Swap64( swapper.ui64 );
 	return swapper.f;
};

#if BYTE_ORDER == LITTLE_ENDIAN 
	#define SwapLE16(X)		(X)
	#define SwapLE32(X)		(X)
	#define SwapLE64(X)		(X)
	#define SwapFloatLE(X)	(X)
	#define SwapDoubleLE(X)	(X)
	#define SwapBE16(X)		Swap16(X)
	#define SwapBE32(X)		Swap32(X)
	#define SwapBE64(X)		Swap64(X)
	#define SwapFloatBE(X)	SwapFloat(X)
	#define SwapDoubleBE(X)	SwapDouble(X)
#else
	#define SwapLE16(X)		Swap16(X)
	#define SwapLE32(X)		Swap32(X)
	#define SwapLE64(X)		Swap64(X)
	#define SwapFloatLE(X)	SwapFloat(X)
	#define SwapDoubleLE(X)	SwapDouble(X)
	#define SwapBE16(X)		(X)
	#define SwapBE32(X)		(X)
	#define SwapBE64(X)		(X)
	#define SwapFloatBE(X)	(X)
	#define SwapDoubleBE(X)	(X)
#endif

#endif
