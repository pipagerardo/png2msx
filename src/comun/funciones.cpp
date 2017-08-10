#include "funciones.h"

// -----------------------------------------------------------------------------
// CADENAS:
// -----------------------------------------------------------------------------
size_t strlen( const char* str, size_t maxlen ) {
    if( !str ) return 0;
    if( maxlen < 2 ) return 0;
    register const char* tmp;
    for( tmp = str; *tmp && maxlen; ++tmp, --maxlen );
    return tmp - str;
}
size_t strlcpy( char* dst, size_t lendst, const char* src, size_t lensrc ) {
    if( !dst || !src ) return 0;
    if( lensrc == 0 )  return 0;
    if( lendst < 2 ) { *dst = '\0'; return 0; }
    register char* destino = dst;
    register char* origen  = (char*)src;
    for( ; ( *destino = *origen++ ) != '\0' && --lendst && lensrc--; ++destino );
    *destino = '\0';
    return (destino - dst);
}
size_t strlcat( char* dst, size_t lendst, const char* src, size_t lensrc ) {
    if( !dst || !src ) return 0;
    if( lensrc == 0 ) return 0;
    if( lendst < 2 ) { *dst = '\0'; return 0; }
    register char* destino = dst;
    register char* origen  = (char*)src;
    for( ; *destino != '\0' && lendst; ++destino, --lendst );
    if( !lendst ) return 0;
    for( ; ( *destino = *origen++ ) != '\0' && --lendst && lensrc--; ++destino );
    *destino = '\0';
    return (destino - dst);
}
char* strdup( const char *str, size_t maxlen ) {
	if( ( !str ) || ( maxlen < 2 ) ) return NULL;
	size_t len = strlen( str, maxlen ) + 1;
	char* s = (char*)malloc( len );
	if( s ) strlcpy( s, len, str, len );
	return s;
}
char* strrev( char* str, size_t maxlen ) {
    if( !str ) return str;
    if( maxlen == 0 ) return str;
    register char temp, *fin = str;
    for( ; *fin && maxlen; ++fin, --maxlen );
    if( !maxlen ) *--fin = '\0';
    for( fin -= 1; str < fin ; str++, fin-- ) {
        temp = *str; *str = *fin; *fin = temp;
    }
    return str;
}
#ifndef __MINGW32__
#include "ctype.h"
char *strlwr(char *str) {
	unsigned char *p = (unsigned char *)str;
	while (*p) {
		*p = tolower((unsigned char)*p);
		p++;
	}
	return str;
}
#endif
char* strchr( const char *str, size_t lenstr, int c ) {
	if( !str ) return NULL;
	if( lenstr < 2 ) return NULL;
    const char ch = c;
    for( --lenstr; *str != ch ; str++, lenstr-- ) {
    	if( (*str == '\0') || ( lenstr < 2 ) ) return NULL;
	}
    return (char*)str;
}
char* strrchr( const char *str, size_t lenstr, int c ) {
	if( !str ) return NULL;
	if( lenstr < 2 ) return NULL;
    char* save = (char*)NULL;
    const char ch = c;
    for( --lenstr; *str != '\0'; str++, lenstr--  ) {
		if( *str == ch ) save = (char*)str;
		if( lenstr < 2 ) break;
    }
    return save;
}
int strncmp( const char *str1, size_t lenstr1, const char *str2, size_t lenstr2 ) {
	if( !str1 && !str2 ) return 0;
	if( !str1 ) return -1;
	if( !str2 ) return 1;
	for ( ; lenstr1 > 0 && lenstr2 > 0; str1++, str2++, --lenstr1, --lenstr2  ) {
		if( *str1 != *str2 ) return ((*(unsigned char *)str1 < *(unsigned char *)str2) ? -1 : +1);
		else if ((*str1 == '\0') || (*str2 == '\0')) return 0;
	}
    return 0;
}
int strncasecmp( const char *str1, size_t lenstr1, const char *str2, size_t lenstr2 ) {
	char* s1 = strdup( str1, lenstr1 );
	if( !s1 ) return -1;
	char* s2 = strdup( str2, lenstr2 );
	if( !s2 ) { free( s1 ); return 1; }
	strlwr( s1 ); strlwr( s2 );
	int retorno = strncmp( s1, lenstr1, s2, lenstr2 );
	free( s1 ); free( s2 ); 
	return retorno;
}
char* strstr( const char *haystack, size_t lenhaystack, const char *needle, size_t lenneedle ) {
	if( !haystack ) return NULL;
	if( !needle   ) return NULL;
    char c = *needle++;
    size_t len = strlen( needle, lenneedle );
    if( lenneedle < len ) len = lenneedle;
    if( len > 1 ) --len;
    if(!c) return (char*)haystack;	// Trivial empty string case
    do {
        char sc;
        do {
            sc = *haystack++;
            if (!sc)
                return (char *)NULL;
        } while (sc != c);
    } while( strncmp( haystack, lenhaystack, needle, len ) != 0 ); // Esto hay que mejorarlo.
    return (char *) (haystack - 1);
}
char* strrstr( const char* haystack, const char* needle ) {
	char* sz_r = NULL;
	if ( !haystack || !needle[0] ) return sz_r;
	char* sz_p = NULL;
	do {
	    sz_p = (char*)strstr( haystack, needle );
	    if ( sz_p ) { sz_r = sz_p; haystack = sz_p + 1; }
	} while( sz_p );
	return sz_r;
}
char* strrstr( const char *haystack, size_t lenhaystack, const char *needle, size_t lenneedle ) {
	char* sz_r = NULL;
	if ( !haystack || !needle[0] ) return sz_r;
	char* sz_p = NULL;
	do {
	    sz_p = (char*)strstr( haystack, lenhaystack, needle, lenneedle ); // Esto hay que mejorarlo.
	    if ( sz_p ) { sz_r = sz_p; haystack = sz_p + 1; }
	} while( sz_p );
	return sz_r;
}
/*
char* strtok( register char *str, register const char *delimiters ) {
	register char *spanp;
	register int c, sc;
	char *tok;
	static char *last;
	if( str == NULL && (str = last) == NULL) return (NULL);
	// Skip (span) leading delimiters (s += strspn(s, delim), sort of).
cont:
	c = *str++;
	for (spanp = (char *)delimiters; (sc = *spanp++) != 0;) {
		if (c == sc) goto cont;
	}
	if (c == 0) {		// no non-delimiter characters
		last = NULL;
		return (NULL);
	}
	tok = str - 1;
	// Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	// Note that delim must have one NUL; we stop if we see that, too.
	for (;;) {
		c = *str++;
		spanp = (char *)delimiters;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0) str = NULL;
				else        str[-1] = 0;
				last = str;
				return (tok);
			}
		} while (sc != 0);
	}
}
*/
// -----------------------------------------------------------------------------
// DE NUMERO A CADENA:
// -----------------------------------------------------------------------------
#ifdef __MINGW32__
char* uitoa( unsigned int value, char *str, int radix ) {
	return itoa( (int)value, str, radix );
}
#endif

// -----------------------------------------------------------------------------
bool esDigito( int c ) {
	if( c > 42 && c < 47 ) return true; // '+' ',' '-' '.'
	if( c > 47 && c < 58 ) return true; // '0' ~ '9'
	return false; 
}
int esMayuscula( int c ) {
	if( c < 65 || c > 233 ) return 0; 
	if( c < 91 ) return 32; // 'A' ~ 'Z' + 32
	switch( c ) {
		case 144: return -14; // 'É' - 14
		case 154: return -25; // 'Ü' - 25
		case 165: return  -1; // 'Ñ' - 1
		case 181: return -21; // 'Á' - 21 
		case 214: return -53; // 'Í' - 53
		case 224: return -62; // 'Ó' - 62
		case 233: return -70; // 'Ú' - 70	
	}
	return 0;
}
int esMinuscula( int c ) {
	if( c < 97 || c > 164 ) return 0;
	if( c < 123 ) return -32; // 'a' ~ 'z' - 32
	switch( c ) {
		case 129: return 25; // 'ü' + 25
		case 130: return 14; // 'é' + 14
		case 160: return 21; // 'á' + 21
		case 161: return 53; // 'í' + 53
		case 162: return 62; // 'ó' + 62
		case 163: return 70; // 'ú' + 70
		case 164: return  1; // 'ñ'	+ 1
	}
	return 0;
}
bool Mayusculas( char* cadena, size_t capacidad ) {
	if( !cadena ) return false;
	if( capacidad < 2 ) return false;
	for( ; *cadena != '\0' && --capacidad; ++cadena ) *cadena += esMinuscula( *cadena );
	return true;
}
bool Minusculas( char* cadena, size_t capacidad ) {
	if( !cadena ) return false;
	if( capacidad < 2 ) return false;
	for( ; *cadena != '\0' && --capacidad; ++cadena ) *cadena += esMayuscula( *cadena );
	return true;
}
Uint8* buscaBuffer( const Uint8* buffer, size_t capacidad, const Uint8* busca, size_t longitud ) {
	if( !buffer || !busca ) return NULL;
	if( longitud > capacidad ) return NULL;
	register Uint8* pbuffer = (Uint8*)buffer;
	Uint8* pbusca = (Uint8*)busca;
	Uint8 c = *(pbusca++);
	for( ; capacidad > 0; --capacidad ) {
		if( c != *(pbuffer++) ) continue;
		size_t l = longitud - 1;
		for( register Uint8* s = pbusca; l > 0; --l ) if( *(s++) != *(pbuffer++) ) break;
		if( l == 0 ) return (pbuffer - longitud);
	}
	return NULL;
}
 
// -----------------------------------------------------------------------------
// DE CADENA A NÚMERO CON CONFIRMACIÓN : ( Si falla la lectura retorna false )
// -----------------------------------------------------------------------------
bool STRtoI( const char* str, int &number ) {
    char *p;
	number = (int)strtol( str, &p, 10 );
    if( number != 0     ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
bool STRtoUI( const char* str, unsigned int &number ) {
    char *p;
    number = (unsigned int)strtoul( str, &p, 10 );
    if( number != 0     ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
bool STRtoL( const char* str, long &number )  {
    char *p;
	number = strtol( str, &p, 10 );
    if( number != 0     ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
bool STRtoUL( const char* str, unsigned long &number )  {
    char *p;
    number = strtoul( str, &p, 10 );
    if( number != 0     ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
bool STRtoLL( const char* str, long long &number )  {
    char *p;
	number = strtoll( str, &p, 10 );
    if( number != 0     ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
bool STRtoULL( const char* str, unsigned long long &number )  {
    char *p;
	number = strtoull( str, &p, 10 );
    if( number != 0     ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
bool STRtoF( const char* str, float &number ) {
    char *p;
	number = (float)strtod( str, &p );
    if( number != 0.0F  ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
bool STRtoD( const char* str, double &number ) {
    char *p;
	number = strtod( str, &p );
    if( number != 0.0   ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
bool STRtoLD( const char* str, long double &number ) {
    char *p;
	number = (long double)strtod( str, &p );
    if( number != 0.0L  ) return true;
    if( p == str        ) return false;
    if( *(p - 1) == '0' ) return true;
    if( *p != '\0'      ) return false;
    return true;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// BASE64:
// -----------------------------------------------------------------------------
static const unsigned char pr2six[256] = { // ASCII table
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int Base64decode_len( const char *bufcoded ) {
    int nbytesdecoded;
    register const unsigned char *bufin;
    register int nprbytes;
    bufin = (const unsigned char *) bufcoded;
    while( pr2six[*(bufin++)] <= 63 );
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
    return nbytesdecoded + 1;
}

int Base64decode( char *bufplain, const char *bufcoded ) {
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;
    bufin = (const unsigned char *) bufcoded;
    while( pr2six[*(bufin++)] <= 63 );
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;
    while( nprbytes > 4 ) {
		*(bufout++) = (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    	*(bufout++) = (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    	*(bufout++) = (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    	bufin += 4;
    	nprbytes -= 4;
    }
    // Note: (nprbytes == 1) would be an error, so just ingore that case
    if (nprbytes > 1) {
    *(bufout++) = (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
    *(bufout++) = (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
    *(bufout++) = (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }
    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

static const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64encode_len( int len ) {
    return ((len + 2) / 3 * 4) + 1;
}

int Base64encode( char *encoded, const char *string, int len ) {
    int i;
    char *p;
    p = encoded;
    for( i = 0; i < len - 2; i += 3 ) {
    	*p++ = basis_64[(string[i] >> 2) & 0x3F];
    	*p++ = basis_64[((string[i] & 0x3) << 4) |
                    ((int) (string[i + 1] & 0xF0) >> 4)];
    	*p++ = basis_64[((string[i + 1] & 0xF) << 2) |
                    ((int) (string[i + 2] & 0xC0) >> 6)];
    	*p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
    	*p++ = basis_64[(string[i] >> 2) & 0x3F];
    	if (i == (len - 1)) {
        	*p++ = basis_64[((string[i] & 0x3) << 4)];
        	*p++ = '=';
    	} else {
        	*p++ = basis_64[((string[i] & 0x3) << 4) |
                        ((int) (string[i + 1] & 0xF0) >> 4)];
        	*p++ = basis_64[((string[i + 1] & 0xF) << 2)];
    	}
    	*p++ = '=';
    }
    *p++ = '\0';
    return p - encoded;
}
// -----------------------------------------------------------------------------


