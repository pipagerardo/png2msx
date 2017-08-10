#ifndef _FUNCIONES_
#define _FUNCIONES_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef  int8_t  Sint8;
typedef uint8_t  Uint8;

typedef  int16_t Sint16;
typedef uint16_t Uint16;

typedef  int32_t Sint32;
typedef uint32_t Uint32;

typedef  int64_t Sint64;
typedef	uint64_t Uint64;

// -----------------------------------------------------------------------------
// Array:
// -----------------------------------------------------------------------------
// #define arraysize( array )    	(sizeof(array)/sizeof(array[0]))
// #define TABLESIZE( table )    	arraysize(table)
// void qsort(
//		void *base,
//		size_t nmemb,
//		size_t size,
//		int (*compare) (const void *, const void *)
// );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CAST:
// -----------------------------------------------------------------------------
// #define reinterpret_cast(type, expression) 	reinterpret_cast<type>(expression)
// #define static_cast(type, expression) 		static_cast<type>(expression)
// #define const_cast(type, expression) 		const_cast<type>(expression)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// FAMILIA DE FUNCIONES MALLOC:
// -----------------------------------------------------------------------------
// void* malloc( size_t size );
// void* calloc( size_t nmemb, size_t size) ;
// void* realloc( void *mem, size_t size );
// void  free( void *mem );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// MEMORIA:
// -----------------------------------------------------------------------------
// void* memset( void *dst, int c, size_t len );
// void* zero( x ); 	// memset(&(x), 0, sizeof((x)))
// void* zerop( x );	// memset((x), 0, sizeof(*(x)))
// void* memset4( void *dst, Uint32 val, size_t dwords );
// void* memcpy(  void *dst, const void *src, size_t len );
// void* memmove( void *dst, const void *src, size_t len );
// int   memcmp( const void *s1, const void *s2, size_t len );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CADENAS:
// -----------------------------------------------------------------------------
// size_t strlen( const char *str );	// Retorna la longitud de la cadena sin el caracter nulo.
   size_t strlen( const char* str, size_t maxlen );
// size_t strlcpy( char *dst, const char *src, size_t maxlen );	// Copia una cadena en otra.
   size_t strlcpy( char* dst, size_t lendst, const char* src, size_t lensrc );
// size_t strlcat( char *dst, const char *src, size_t maxlen ); 	// Contatena cadenas.
   size_t strlcat( char* dst, size_t lendst, const char* src, size_t lensrc );
// size_t utf8strlcpy( char *dst, const char *src, size_t dst_bytes );
// char* strdup( const char *str );	// Duplica una cadena, luego hay que borrar la con SDL_free.
   char* strdup( const char *str, size_t maxlen );
// char* strrev( char *str ); 		// Invierte la cadena.
   char* strrev( char* str, size_t maxlen );
// char* strupr( char *str );		// Convierte la cadena a mayúsculas. No funciona ni la ñ ni los vocales acentuadas.
#ifndef __MINGW32__
   char* strlwr( char *str );		// Convierte la cadena a minúsculas. No funciona ni la ñ ni los vocales acentuadas.
#endif
// char* strchr(  const char *str, int c );	// Busca la primera aparación de un caracter.
   char* strchr( const char *str, size_t lenstr, int c );
// char* strrchr( const char *str, int c );	// Busca la última aparación de un caracter.
   char* strrchr( const char *str, size_t lenstr, int c );
// char* strstr(  const char *haystack, const char *needle ); // Busca la primera aparición de una cadena.
   char* strstr(  const char *haystack, size_t lenhaystack, const char *needle, size_t lenneedle );
   char* strrstr( const char* haystack, const char* needle ); // Busca la última aparición de una cadena.
   char* strrstr( const char *haystack, size_t lenhaystack, const char *needle, size_t lenneedle );
// int strcmp(  const char *str1, const char *str2 ); 					// Compara dos cadenas, sensitivo a las mayusculas.
// int strncmp( const char *str1, const char *str2, size_t maxlen );	// Compara dos cadenas, sensitivo a las mayúsculas.
   int strncmp( const char *str1, size_t lenstr1, const char *str2, size_t lenstr2 );
// int strcasecmp(  const char *str1, const char *str2 ); 				// Compara dos cadenas, no sensitivo a las mayúsculas, no funciona con ñ  y acentos.
// int strncasecmp( const char *str1, const char *str2, size_t len );	// Compara dos cadenas, no sensitivo a las mayúsculas, no funciona con ñ  y acentos.
   int strncasecmp( const char *str1, size_t lenstr1, const char *str2, size_t lenstr2 );
// -----------------------------------------------------------------------------
   bool esDigito( int c ); 	  // true si '0~9' | '+' | '-' | '.' | ','
   int  esMayuscula( int c ); // Cero si no es mayúcula, retorna el valor para convertir a minúscula.
   int  esMinuscula( int c ); // Cero si no es minúscula, retorna el valor para convertir a mayuscula.
   bool Mayusculas( char* cadena, size_t capacidad ); // Convierte la cadena a mayúsculas, si funciona con ñ y acentos.
   bool Minusculas( char* cadena, size_t capacidad ); // Convierte la cadena a minúsculas, si funciona con ñ y acentos.
// Busca en un buffer cierta coincidencia, no tiene en cuenta el caracter '\0'
   Uint8* buscaBuffer( const Uint8* buffer, size_t capacidad, const Uint8* busca, size_t longitud );
// -----------------------------------------------------------------------------
// CORTAR CADENAS POR PASOS:
// -----------------------------------------------------------------------------
// char texto[] =  "Una cadena\tde ,,muestras\ny algunas   muestras más";
// const char* pasos   = " ,\t\n";
// char* muestra  = NULL;
// Error( "texto: %s\n", texto );
// muestra = strtok( texto, pasos );
// while( muestra != NULL )  {
//    printf( "\t%s\n", muestra );
//    muestra = strtok( NULL, pasos );
// }
// -----------------------------------------------------------------------------
//	char* strtok( register char *str, register const char *delimiters );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// FAMILIA DE FUNCIONES STRING PRINT-SCAN FORMAT:
// -----------------------------------------------------------------------------
// int sscanf( const char *text, const char *fmt, ... );
// int vsscanf( const char *text, const char *fmt, va_list ap );
// int snprintf( char *text, size_t maxlen, const char *fmt, ... );
// int vsnprintf( char *text, size_t maxlen, const char *fmt, va_list ap );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// DE NUMERO A CADENA:
// -----------------------------------------------------------------------------
// char* itoa( int value, char *str, int radix );
#ifdef __MINGW32__
	char* uitoa( unsigned int value, char *str, int radix );
#endif
// char* ltoa( long value, char *str, int radix );
// char* _ultoa( unsigned long value, char *str, int radix );
// char* lltoa( Sint64 value, char *str, int radix );
// char* ulltoa( Uint64 value, char *str, int radix );
// char*  ecvt( double value, int num, int * dec, int * sign );
// char*  fcvt( double value, int num, int * dec, int * sign );
// char*  gcvt( double value, int num, char* buffer );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// DE CADENA A NUMERO:
// -----------------------------------------------------------------------------
// int atoi( const char *str );
// long strtol( const char *str, char **endp, int base );
// unsigned long strtoul( const char *str, char **endp, int base );
// Sint64 strtoll( const char *str, char **endp, int base );
// Uint64 strtoull( const char *str, char **endp, int base );
// double atof( const char *str );
// double strtod( const char *str, char **endp );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// DE CADENA A NÚMERO CON CONFIRMACIÓN : ( Si falla la lectura retorna false )
// -----------------------------------------------------------------------------
bool STRtoI( const char* str, int &number );
bool STRtoUI( const char* str, unsigned int &number );
bool STRtoL( const char* str, long &number );
bool STRtoUL( const char* str, unsigned long &number );
bool STRtoLL( const char* str, long long &number );
bool STRtoULL( const char* str, unsigned long long &number );
bool STRtoF( const char* str, float &number );
bool STRtoD( const char* str, double &number );
bool STRtoLD( const char* str, long double &number );
// -----------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// MATEMÁTICAS:
// ---------------------------------------------------------------------------------
// #define M_PI    3.14159265358979323846264338327950288
// int abs(int x);
// double acos(double x);
// double asin(double x);
// double atan(double x);
// double atan2(double x, double y);
// double ceil(double x);
// double copysign( double x, double y );	// retorna x con el signo de y.
// double cos(double x);
// float  cosf(float x);
// double fabs(double x);			// retorna el flotante en positivo.
// double floor(double x);
// double log(double x);
// double pow(double x, double y);
// double scalbn( double x, int n ); // Multiplica el valor de punto flotante por argumeno n FLT_RADIX elevado a potencia exp
// double sin(double x);
// float  sinf(float x);
// double sqrt(double x);
// ---------------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// BASE64:
// -----------------------------------------------------------------------------
int Base64decode_len( const char *bufcoded );
int Base64decode( char *bufplain, const char *bufcoded );
int Base64encode_len( int len );
int Base64encode( char *encoded, const char *string, int len );
// -----------------------------------------------------------------------------



#endif

