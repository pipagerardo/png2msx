#include "comun/funciones.h"
#include "comun/endian.h"
#include "comun/cadena.h"
#include "comun/archivo.h"
#include "comun/color.h"
// #include "comun/tiempo.h"
#include "comun/matematicas.h"
#include "comun/tabla.h"
// #include "comun/matriz.h"
#include "comun/lista.h"
// #include "comun/contenedor.h"
// #include "comun/compresor.h"
// #include "comun/mapaxml.h"
#include "comun/formato_png.h"
#include "comun/pletter.h"

#include <locale.h>

int main( int argc, char *argv[] ) {

	// Activamos el el idioma local:
	setlocale( LC_ALL, ".ACP" );
	// setlocale( LC_ALL, "C" );

	if( argc == 1 ) {
		printf( "ruta: %s\n", argv[0] );
	}
	printf( "Proyecto en construcción\n" );	
	return 0;
	
}
