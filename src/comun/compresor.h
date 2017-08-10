#ifndef _COMPRESOR_
#define _COMPRESOR_

#include "funciones.h"
#include "contenedor.h"
// Linker: -lzlib

int Comprime(    const char* datos, size_t largo, Contenedor<char>& contenedor, int nivel );
int Descomprime( const char* datos, size_t largo, Contenedor<char>& contenedor );
// int DescomprimeGZIP( const char* datos, size_t largo, Contenedor<char>& contenedor );

#endif


