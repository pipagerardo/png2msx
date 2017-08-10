#ifndef _PLETTER_
#define _PLETTER_

#include "funciones.h"
#include "cadena.h"

#ifdef LANGUAGE_UTF8
#include "pletter_utf8.h"
#else
#include "pletter_iso.h"
#endif

#ifndef PLETTER_MAIN
bool pletter( const Cadena& archivo, bool guarda_longitud = false );
// El array debe tener un elemento de más y es borrado por la función.
// La longitud debe ser la real, osea sin sumarle uno.
bool pletter( unsigned char* array, size_t longitud, const Cadena& cadena, bool guarda_longitud = false );
#endif

#endif
