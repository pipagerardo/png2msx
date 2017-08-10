#include "archivo.h"

Archivo::Archivo() {
    p_archivo = NULL; b_abierto = false; b_final = false;
}

Archivo::Archivo( const char *nombre, const char *modo ) {
    p_archivo = NULL; b_abierto = false; b_final = false;
    inicia( nombre, modo );
}

Archivo::~Archivo() {
    quita();
}

bool Archivo::inicia( const char *nombre, const char *modo ) {
    if( b_abierto ) { if( !quita() ) return false; }
// FILE * fopen ( const char * filename, const char * mode ); return (NULL == FAIL)
	p_archivo = fopen( nombre, modo );
	if( p_archivo == NULL ) return false;
	b_abierto = true;
	b_final   = false;
    return b_abierto;
}

bool Archivo::quita() {
    if( !b_abierto ) return !b_abierto;
// int fclose ( FILE * stream ); return ( 0 == OK ) ( EOF == FAIL )
	if( fclose( p_archivo ) != 0 ) return !b_abierto;
	p_archivo  = NULL;
	b_abierto  = false;
	b_final    = false;
    return !b_abierto;
}

bool Archivo::esta_abierto() const { return  b_abierto; }
bool Archivo::esta_cerrado() const { return !b_abierto; }
bool Archivo::final() {
	if( !b_abierto ) return true;
	if( posicion() <= capacidad() ) return false;
	return true;
}
bool Archivo::no_final() {
	return !final();
}

char Archivo::lee() {
	if( ( !b_abierto ) || ( b_final ) ) return '\0';
    char caracter;
//  size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
//	if( SDL_RWread( p_archivo, &caracter, sizeof( char ), 1 ) != 1 ) {
	if( fread( &caracter, sizeof( char ), 1, p_archivo ) != 1 ) {
	 	caracter = '\0'; b_final = true;
	}
	return caracter;
}

bool Archivo::lee( char caracter ) {
	if( ( !b_abierto ) || ( b_final ) ) return false;
//  size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
//  if( SDL_RWread( p_archivo, &caracter, sizeof( char ), 1 ) != 1 ) {
	if( fread( &caracter, sizeof( char ), 1, p_archivo ) != 1 ) {
		 caracter = '\0'; b_final = true; return false;
	}
	return true;
}

bool Archivo::lee( char *cadena, size_t longitud ) {
	if( ( !b_abierto ) || ( b_final ) ) return false;
    if( ( cadena == NULL ) || ( longitud == 0 ) ) return false;
	memset( cadena , 0, longitud );
	char caracter = '\0';
	size_t i;
	for( i = 0; i < longitud - 1; i++ ) {
		caracter = lee();
		if( caracter == '\r' ) continue;
		if( caracter == '\n' ) break;
		cadena[i] = caracter;
		if( caracter == '\0' ) { b_final = true; break; }
	}
	return true;
}

bool Archivo::lee( Cadena &cadena ) {
	if( ( !b_abierto ) || ( b_final ) ) return false;
	cadena.borra();
	char caracter = '\0';
	while( true ) {
		caracter = lee();
		if( caracter == '\r' ) continue;
		if( caracter == '\n' ) break;
		cadena.concatena( caracter );
		if( caracter == '\0' ) { b_final = true; break; }
	}
	return true;
}
void Archivo::limpiaFinal() {
	b_final = false;
}
// -----------------------------------------------------------------------------
//                         ESCRITURA EN MODO TEXTO
// -----------------------------------------------------------------------------

bool Archivo::escribe( char caracter ) {
	if( !b_abierto ) return false;
// size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
// return SDL_RWwrite( p_archivo, &caracter, sizeof( char ), 1 ) != 1 ? false : true;
	return fwrite( &caracter, sizeof( char ), 1, p_archivo ) != 1 ? false : true;
}

bool Archivo::escribe( const char *cadena, size_t longitud ) {
	if( !b_abierto ) return false;
 	if( ( cadena == NULL ) || ( longitud == 0 ) ) return 0;
// size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
// return SDL_RWwrite( p_archivo, cadena, sizeof( char ), longitud ) != longitud ? false : true;
	return fwrite( cadena, sizeof( char ), longitud, p_archivo ) != longitud ? false : true;
}

bool Archivo::escribe( Cadena &cadena ) {
	if( !b_abierto ) return false;
	size_t longitud = cadena.dameLongitud();
 	if( longitud == 0 ) return 0;
// size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
//	return SDL_RWwrite( p_archivo, cadena.dameCadena(), sizeof( char ), longitud ) != longitud ? false : true;
	return fwrite( cadena.dameCadena(), sizeof( char ), longitud, p_archivo ) != longitud ? false : true;
}

// -----------------------------------------------------------------------------
//                        ENTRADA Y SALIDA FORMATEADA:
// -----------------------------------------------------------------------------

size_t Archivo::sprintf( const char *formato, ... ) {
	if( !b_abierto ) return 0;
	Cadena cadena( 256 );
    va_list va; va_start( va, formato );
	size_t resultado = cadena.vsprintf( formato, va );
	va_end( va );
	if( !escribe( cadena ) ) resultado = 0;
    return resultado;
}

size_t Archivo::sprintf( size_t cantidad, const char *formato, ... ) {
	if( !b_abierto ) return 0;
	Cadena cadena;
    va_list va; va_start( va, formato );
	size_t resultado = cadena.vsprintf( cantidad, formato, va );
	va_end( va );
	if( !escribe( cadena ) ) resultado = 0;
    return resultado;
}

size_t Archivo::sscanf( const char *formato, ... ) {
	if( !b_abierto ) return 0;
	Cadena cadena;
	if( !lee( cadena ) ) return 0;
    va_list va; va_start( va, formato );
    size_t resultado =  cadena.vsscanf( formato, va );
    va_end( va );
    return resultado;
}

// -----------------------------------------------------------------------------
//                      LECTURA Y ESCRITURA EN MODO BINARIO
// -----------------------------------------------------------------------------
size_t Archivo::lee( void *puntero, size_t ancho, size_t cantidad ) {
//  size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
//	return ( !b_abierto ) ? 0 : SDL_RWread( p_archivo, puntero, ancho, cantidad );
	return ( !b_abierto ) ? 0 : fread( puntero, ancho, cantidad, p_archivo );
}

size_t Archivo::escribe( void *puntero, size_t ancho, size_t cantidad ) {
// size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
//	return ( !b_abierto ) ? 0 : SDL_RWwrite( p_archivo, puntero, ancho, cantidad );
	return ( !b_abierto ) ? 0 : fwrite( puntero, ancho, cantidad, p_archivo );
}

Uint8 Archivo::leeU8() {
    Uint8 valor = 0;
	if( !b_abierto ) return valor;
    if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return valor;
    return 0;
}

Uint16 Archivo::leeLE16() {
    Uint16 valor = 0;
	if( !b_abierto ) return valor;
    if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapLE16( valor );
    return 0;
}

Uint16 Archivo::leeBE16() {
    Uint16 valor = 0;
	if( !b_abierto ) return valor;
    if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapBE16( valor );
    return 0;
}

Uint32 Archivo::leeLE32() {
    Uint32 valor = 0;
	if( !b_abierto ) return valor;
    if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapLE32( valor );
    return 0;
}

Uint32 Archivo::leeBE32() {
    Uint32 valor = 0;
	if( !b_abierto ) return valor;
    if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapBE32( valor );
    return 0;
}

Uint64 Archivo::leeLE64() {
    Uint64 valor = 0;
	if( !b_abierto ) return valor;
    if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapLE64( valor );
    return 0;
}

Uint64 Archivo::leeBE64() {
    Uint64 valor = 0;
	if( !b_abierto ) return valor;
    if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapBE64( valor );
    return 0;
}

float Archivo::leeLEFloat() {
	float  valor = 0.0f;
	if( !b_abierto ) return valor;
	if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapFloatLE( valor );
	return 0.0f;
}

float Archivo::leeBEFloat() {
	float  valor = 0.0f;
	if( !b_abierto ) return valor;
	if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapFloatBE( valor );
	return 0.0f;
}

double Archivo::leeLEDouble() {
	double  valor = 0.0;
	if( !b_abierto ) return valor;
	if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapDoubleLE( valor );
	return 0.0;
}

double Archivo::leeBEDouble() {
	if( !b_abierto ) return 0.0;
	double  valor = 0;
	if( fread( &valor, sizeof( valor ), 1, p_archivo ) == 1 ) return SwapDoubleBE( valor );
    return 0.0;
}

size_t Archivo::escribeU8( Uint8 valor ) {
	return ( !b_abierto ) ? 0 : fwrite( &valor, sizeof(valor), 1, p_archivo );
}

size_t Archivo::escribeLE16( Uint16 valor ) {
	if( !b_abierto ) return 0;
    const Uint16 swapped = SwapLE16( valor );
    return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeBE16( Uint16 valor ) {
	if( !b_abierto ) return 0;
    const Uint16 swapped = SwapBE16( valor );
    return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeLE32( Uint32 valor ) {
	if( !b_abierto ) return 0;
    const Uint32 swapped = SwapLE32( valor );
    return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeBE32( Uint32 valor ) {
	if( !b_abierto ) return 0;
    const Uint32 swapped = SwapBE32( valor );
    return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeLE64( Uint64 valor ) {
	if( !b_abierto ) return 0;
    const Uint64 swapped = SwapLE64( valor );
    return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeBE64( Uint64 valor ) {
	if( !b_abierto ) return 0;
    const Uint64 swapped = SwapBE64( valor );
    return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeLEFloat( float valor ) {
	if( !b_abierto ) return 0.0f;
	const float swapped = SwapFloatLE( valor );
	return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeBEFloat( float valor ) {
	if( !b_abierto ) return 0.0f;
	const float swapped = SwapFloatBE( valor );
	return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeLEDouble( double valor ) {
	if( !b_abierto ) return 0.0;
	const double swapped = SwapDoubleLE( valor );
	return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

size_t Archivo::escribeBEDouble( double valor ) {
	if( !b_abierto ) return 0.0;
	const double swapped = SwapDoubleBE( valor );
	return fwrite( &swapped, sizeof(swapped), 1, p_archivo );
}

// -----------------------------------------------------------------------------
//                             ACCESO ALEATORIO
// -----------------------------------------------------------------------------
long int Archivo::capacidad() {
	if( !b_abierto )  return 0;
 	long int pos = ftell( p_archivo );	// Guardamos la posición actual.
 	fseek( p_archivo, 0, SEEK_END );	// Nos vamos al final del archivo.
 	long int cap = ftell( p_archivo );	// Obtenemos la posción del final.
	fseek( p_archivo, pos, SEEK_SET );	// Restauramos la posición.
	return cap;							// Retornamos la capacidad.
}

long int Archivo::posicion() {
// long int ftell ( FILE * stream );
	return ( !b_abierto ) ? 0 : ftell( p_archivo );
}

void Archivo::al_inicio() {
	if( !b_abierto ) return;
	b_final = false;
// int fseek ( FILE * stream, long int offset, int origin );
// SEEK_SET Beginning of file
// SEEK_CUR Current position of the file pointer
// SEEK_END End of file *
	fseek( p_archivo, 0, SEEK_SET );
}

void Archivo::al_final() {
	if( !b_abierto ) return;
	b_final = true;
	fseek( p_archivo, 0, SEEK_END );
}

bool Archivo::desplaza_desde_inicio( long int desplazamiento ) {
	if( !b_abierto ) return false;
	return fseek( p_archivo, desplazamiento, SEEK_SET )  < 0 ? false : true;
}

bool Archivo::desplaza_desde_final(  long int desplazamiento ) {
	if( !b_abierto ) return false;
    return fseek( p_archivo, desplazamiento, SEEK_END )  < 0 ? false : true;
}

bool Archivo::desplaza_desde_actual( long int desplazamiento ) {
	if( !b_abierto ) return false;
	return fseek( p_archivo, desplazamiento, SEEK_CUR ) < 0 ? false : true;
}


