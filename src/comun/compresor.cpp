#include "compresor.h"
#include "zlib.h"

#if defined(ZLIB_CONST) && !defined(z_const)
#  define z_const const
#else
#  define z_const
#endif

/*
#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)

#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1)
*/

int Comprime( const char* datos, size_t largo, Contenedor<char>& contenedor, int nivel ) {
	size_t  cantidad = 0;
	size_t  posicion = 0;
	size_t  cuenta   = largo;
    int retorno, flush;
    unsigned int tengo;
	const size_t ancho = 4096; 
    unsigned char salida[  ancho ];
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;
    retorno = deflateInit( &strm, nivel );
    if( retorno != Z_OK ) return retorno;
    do {
        // ------------------------------------------------------
        if( cuenta > ancho ) cantidad = ancho;
        else                 cantidad = cuenta;
        strm.next_in  = (z_const Bytef *)&datos[ posicion ];
		strm.avail_in = (uInt)cantidad;
        cuenta   -= cantidad;
        posicion += cantidad;
		flush = (cuenta == 0) ? Z_FINISH : Z_NO_FLUSH;
        // ------------------------------------------------------
        do {
            strm.avail_out = (uInt)ancho;
            strm.next_out  = (Bytef*)salida;
            retorno = deflate( &strm, flush ); 
            tengo = ancho - strm.avail_out;
            // ------------------------------------------------------
        	if( !contenedor.mete( (const char*)salida, (size_t)tengo ) ) {
                deflateEnd( &strm );
                return Z_ERRNO;
            }
            // ------------------------------------------------------
        } while( strm.avail_out == 0 );
    } while( flush != Z_FINISH );
    deflateEnd( &strm );
    return Z_OK;
}

int Descomprime( const char* datos, size_t largo, Contenedor<char>& contenedor ) {
	size_t  cantidad = 0;
	size_t  posicion = 0;
	size_t  cuenta   = largo;
    int      retorno;
    unsigned tengo;
	const size_t ancho = 4096;
    unsigned char salida[ ancho ];
    z_stream strm;
    strm.zalloc   = Z_NULL;
    strm.zfree    = Z_NULL;
    strm.opaque   = Z_NULL;
    strm.avail_in = 0;
    strm.next_in  = Z_NULL;
    retorno = inflateInit( &strm );
    if( retorno != Z_OK ) return retorno;
    do {
        // ------------------------------------------------------
        if( cuenta > ancho ) cantidad = ancho;
        else                 cantidad = cuenta;
        strm.next_in  = (z_const Bytef *)&datos[ posicion ];
		strm.avail_in = (uInt)cantidad;
        cuenta   -= cantidad;
        posicion += cantidad;
        // ------------------------------------------------------
        if( strm.avail_in == 0 ) break;
        do {
			strm.avail_out = (uInt)ancho;
            strm.next_out  = (Bytef*)salida;
            retorno = inflate( &strm, Z_NO_FLUSH );
            switch( retorno ) {
            	case Z_NEED_DICT: retorno = Z_DATA_ERROR;
            	case Z_DATA_ERROR:
            	case Z_MEM_ERROR:
                	inflateEnd( &strm );
                	return retorno;
            }
            tengo = ancho - strm.avail_out;
            // ------------------------------------------------------
        	if( !contenedor.mete( (const char*)salida, (size_t)tengo ) ) {
        	    inflateEnd( &strm );
                return Z_ERRNO;
            }
            // ------------------------------------------------------
        } while( strm.avail_out == 0 );
    } while( retorno != Z_STREAM_END );
    inflateEnd( &strm );
    return retorno == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

/*
int DescomprimeGZIP( const char* datos, size_t largo, Contenedor<char>& contenedor ) {
    // No se como se hace, Descomprime() de error -3 y no hay mucha documentación.
	return Z_DATA_ERROR;
}
*/


