#include "cadena.h"

#define CADENA_BLOQUE 64
#define USAR_CADENA_SEGURA 1

Cadena::Cadena() {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	m_parte_inicio = 0;
}

Cadena::Cadena( size_t capacidad ) {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	m_parte_inicio = 0;
	inicia( capacidad );
}

Cadena::Cadena( size_t capacidad, size_t bloque ) {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	m_parte_inicio = 0;
	inicia( capacidad, bloque );
}

Cadena::Cadena( const char* texto ) {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	m_parte_inicio = 0;
	copia( texto );
}

Cadena::Cadena( const Cadena& texto ) {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	m_parte_inicio = 0;
	copia( texto );
}

Cadena::~Cadena() {
	quita();
}

void Cadena::tomaBloque( size_t bloque )  {
	m_bloque = bloque;
	if( m_bloque < 8    ) m_bloque = CADENA_BLOQUE;
	if( m_bloque > 1024 ) m_bloque = 1024;
	size_t resto = m_bloque % 8;
	if( resto ) m_bloque += resto;
}

size_t Cadena::dameBloque() const {
	return m_bloque;
}

bool Cadena::inicia( size_t capacidad ) {
	if( capacidad == 0 ) return false;
	size_t capacidad_vieja = m_capacidad;
	m_capacidad     = capacidad;
	size_t resto    = m_capacidad % m_bloque;
	if( resto ) m_capacidad += ( m_bloque - resto );
	// Ahora m_capacidad es la nueva y capacidad_vieja la vieja.
	// Si la nueva capacidad es menor que la longitud alojada mal asunto:
	if( m_capacidad <= m_longitud ) {
		m_capacidad = capacidad_vieja;
		return false;
	}
	if ( m_cadena == NULL ) m_cadena = (char*)malloc(  m_capacidad );
	else if( capacidad_vieja != m_capacidad ) m_cadena = (char*)realloc( m_cadena, m_capacidad );
	if( m_cadena == NULL ) {
		quita();
		return false;
	}
	// Se ha incrementado la memoria, asi que la limpiamos.
	if( m_capacidad  > capacidad_vieja ) memset( m_cadena + capacidad_vieja, 0, m_capacidad - capacidad_vieja );
	m_parte_inicio = 0;
	return true;
}

bool Cadena::inicia( size_t capacidad, size_t bloque ){
	tomaBloque( bloque );
	return inicia( capacidad );
}

void Cadena::quita() {
	if( m_cadena != NULL ) free( m_cadena );
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	m_parte_inicio = 0;
}

size_t      Cadena::dameCapacidad() const { return m_capacidad; }
size_t      Cadena::dameLongitud()  const { return m_longitud;  }
const char* Cadena::dameCadena()    const { return ( m_cadena != NULL ) ? (const char *)m_cadena : ""; }

size_t Cadena::copia( char caracter ) {
	if( caracter == '\0' ) return 0;
	if( m_capacidad < 2 ) {
		if( !inicia( 2 ) ) return 0;
	}
	m_cadena[0] = caracter;
	m_cadena[1] = '\0';
	m_longitud  = 1;
	return m_longitud;
}

size_t Cadena::copia( const char* texto ) {
	if( texto == NULL ) return 0;
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( texto, 1024 ); // Ponemos un tope máximo
#else
	m_longitud = strlen( texto );
#endif
	if( m_longitud >= m_capacidad ) {
		if( !inicia( m_longitud + 1 ) ) return 0;
	}
#ifdef USAR_CADENA_SEGURA
	return strlcpy( m_cadena, m_capacidad, texto, m_capacidad );
#else
	return strlcpy( m_cadena, texto, m_capacidad );
#endif
}

size_t Cadena::copia( const Cadena& texto ) {
	if( texto.m_longitud == 0 ) return 0;
	m_longitud = texto.m_longitud;
	if( m_longitud >= m_capacidad ) {
		if( !inicia( m_longitud + 1 ) ) return 0;
	}
#ifdef USAR_CADENA_SEGURA
	return strlcpy( m_cadena, m_capacidad, texto.m_cadena, m_capacidad );
#else
	return strlcpy( m_cadena, texto.m_cadena, m_capacidad );
#endif
}

size_t Cadena::copia( Cadena& origen, size_t inicio, size_t final ) {
	if( inicio > final ) return 0;
	size_t longitud = final - inicio + 1;
	if( longitud > m_capacidad ) {
		if( !inicia( longitud ) ) return 0;
	}
	m_longitud = longitud - 1;
#ifdef USAR_CADENA_SEGURA
	return strlcpy( m_cadena, m_capacidad, (const char*)&origen.m_cadena[inicio], (size_t)longitud );
#else
	return strlcpy( m_cadena, (const char*)&origen.m_cadena[inicio], (size_t)longitud );
#endif
}

size_t Cadena::concatena( char caracter ) {
	if( caracter == '\0' ) return 0;
	if( ( m_capacidad - m_longitud ) < 2 ) {
		if( !inicia( m_capacidad + 2 ) ) return 0;
	}
	m_cadena[   m_longitud ] = caracter;
	m_cadena[ ++m_longitud ] = '\0';
	return m_longitud;
}

size_t Cadena::concatena( const char* texto ) {
	if( texto == NULL ) return 0;
#ifdef USAR_CADENA_SEGURA
	size_t longitud = strlen( texto, 1024 );
#else
	size_t longitud = strlen( texto );
#endif
	size_t libre    = m_capacidad - m_longitud;
	if( longitud >= libre ) {
		if( !inicia( m_capacidad + longitud + 1 ) ) return 0;
	}
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlcat( m_cadena, m_capacidad, texto, m_capacidad );
#else
	m_longitud = strlcat( m_cadena, texto, m_capacidad );
#endif
	return m_longitud;
}

size_t Cadena::concatena( const Cadena& texto ) {
	if( texto.m_longitud == 0 ) return 0;
	size_t longitud = texto.m_longitud;
	size_t libre    = m_capacidad - m_longitud;
	if( longitud >= libre ) {
		if( !inicia( m_capacidad + longitud + 1 ) ) return 0;
	}
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlcat( m_cadena, m_capacidad, texto.m_cadena, m_capacidad );
#else
	m_longitud = strlcat( m_cadena, texto.m_cadena, m_capacidad );
#endif
	return m_longitud;
}

const Uint8* Cadena::dameBuffer() const { return (const Uint8*)m_cadena; }

size_t Cadena::copiaBuffer( const Uint8* buffer, size_t cantidad ) {
	if( buffer == NULL ) return 0;
	m_longitud = cantidad;
	if( m_longitud >= m_capacidad ) {
		if( !inicia( m_longitud + 1 ) ) return 0;
	}
	memcpy( (void*)m_cadena, (void*)buffer, cantidad );
	m_cadena[ cantidad ] = '\0';
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
	return m_longitud;
}

size_t Cadena::concatenaBuffer( const Uint8* buffer, size_t cantidad ) {
	if( buffer == NULL ) return 0;
	size_t longitud = cantidad;
	size_t libre    = m_capacidad - m_longitud;
	if( longitud >= libre ) {
		if( !inicia( m_capacidad + longitud + 1 ) ) return 0;
	}
	memcpy( (void*)m_cadena, (void*)buffer, cantidad );
	m_cadena[ cantidad ] = '\0';
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
	return m_longitud;
}

size_t Cadena::Cadena::sprintf( const char* formato, ... ) {
	if( formato == NULL ) return 0;
	if( m_capacidad == 0 ) return 0;
   	va_list va; va_start( va, formato );
   	m_longitud = (size_t)vsnprintf( m_cadena, m_capacidad, formato, va );
	va_end( va );
   	return m_longitud;
}

size_t Cadena::vsprintf( const char* formato, va_list va ) {
	if( formato == NULL ) return 0;
	if( m_capacidad == 0 ) return 0;
   	m_longitud = (size_t)vsnprintf( m_cadena, m_capacidad, formato, va );
   	return m_longitud;
}

size_t Cadena::sprintf( size_t cantidad, const char* formato, ... ) {
	if( ( cantidad == 0 ) || ( formato == NULL ) ) return 0;
    va_list va; va_start( va, formato );
 	inicia( cantidad );
    m_longitud = (size_t)vsnprintf( m_cadena, cantidad, formato, va );
	va_end( va );
    return m_longitud;
}

size_t Cadena::vsprintf( size_t cantidad, const char* formato, va_list va ) {
	if( ( formato == NULL ) || ( cantidad == 0 ) ) return 0;
 	inicia( cantidad );
    m_longitud = (size_t)vsnprintf( m_cadena, cantidad, formato, va );
    return m_longitud;
}

size_t Cadena::sscanf( const char* formato, ... ) {
	if( ( formato == NULL ) || ( m_longitud == 0 ) ) return 0;
    va_list va; va_start( va, formato );
	int retorno = ::vsscanf( (const char *)m_cadena, formato, va );
	va_end( va );
    return (size_t)retorno;
}

size_t Cadena::vsscanf( const char* formato, va_list va ) {
	if( ( formato == NULL ) || ( m_longitud == 0 ) ) return 0;
	int retorno = ::vsscanf( (const char *)m_cadena, formato, va );
    return (size_t)retorno;
}

bool Cadena::comparaS( const char*   texto ) {
#ifdef USAR_CADENA_SEGURA
	return ( strncmp( m_cadena, m_capacidad, texto, m_capacidad ) == 0 );
#else
	return ( strcmp( m_cadena, texto ) == 0 );
#endif
}

bool Cadena::comparaS( const char*   texto, size_t longitud ) {
#ifdef USAR_CADENA_SEGURA
	return ( strncmp( m_cadena, m_capacidad, texto, longitud ) == 0 );
#else
	return ( strncmp( m_cadena, texto, longitud ) == 0 );
#endif
}

bool Cadena::comparaS( const Cadena& texto ) {
#ifdef USAR_CADENA_SEGURA
	return ( strncmp( m_cadena, m_capacidad, texto.m_cadena, texto.m_capacidad ) == 0 );
#else
	return ( strcmp( m_cadena, texto.m_cadena ) == 0 );
#endif
}

bool Cadena::comparaS( const Cadena& texto, size_t longitud ) {
#ifdef USAR_CADENA_SEGURA
	return ( strncmp( m_cadena, m_capacidad, texto.m_cadena, longitud ) == 0 );
#else
	return ( strncmp( m_cadena, texto.m_cadena, longitud ) == 0 );
#endif
}

bool Cadena::comparaN( const char*   texto ) {
#ifdef USAR_CADENA_SEGURA
	return ( strncasecmp( m_cadena, m_capacidad, texto, m_capacidad ) == 0 );
#else
	return ( strcasecmp( m_cadena, texto ) == 0 );
#endif
}

bool Cadena::comparaN( const char*   texto, size_t longitud ) {
#ifdef USAR_CADENA_SEGURA
	return ( strncasecmp( m_cadena, m_capacidad, texto, longitud ) == 0 );
#else
	return ( strncasecmp( m_cadena, texto, longitud ) == 0 );
#endif
}

bool Cadena::comparaN( const Cadena& texto ) {
#ifdef USAR_CADENA_SEGURA
	return ( strncasecmp( m_cadena, m_capacidad, texto.m_cadena, texto.m_capacidad ) == 0 );
#else
	return ( strcasecmp( m_cadena, texto.m_cadena ) == 0 );
#endif
}

bool Cadena::comparaN( const Cadena& texto, size_t longitud ) {
#ifdef USAR_CADENA_SEGURA
	return ( strncasecmp( m_cadena, m_capacidad, texto.m_cadena, longitud ) == 0 );
#else
	return ( strncasecmp( m_cadena, texto.m_cadena, longitud ) == 0 );
#endif
}

bool Cadena::busca( char caracter, size_t& posicion ) {
	if( m_longitud == 0 ) return false;
#ifdef USAR_CADENA_SEGURA
	char* retorno = strchr( (const char*)m_cadena, m_capacidad, caracter );
#else
	char* retorno = strchr( (const char*)m_cadena, caracter );
#endif
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	posicion = m_longitud + 1;
	return false;
}

bool Cadena::busca( const char* texto, size_t& posicion  ) {
	if( m_longitud == 0 ) return false;
	if( texto == NULL   ) return false;
#ifdef USAR_CADENA_SEGURA
	char* retorno = strstr( (const char*)m_cadena, m_capacidad, texto, m_capacidad );
#else
	char* retorno = strstr( (const char*)m_cadena, texto );
#endif
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	posicion = m_longitud + 1;
	return false;
}

bool Cadena::busca( const Cadena& texto, size_t& posicion  ) {
	if( m_longitud       == 0 ) return false;
	if( texto.m_longitud == 0 ) return false;
#ifdef USAR_CADENA_SEGURA
	char* retorno = strstr( (const char*)m_cadena, m_capacidad, texto.m_cadena, texto.m_capacidad );
#else
	char* retorno = strstr( (const char*)m_cadena, texto.m_cadena );
#endif
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	posicion = m_longitud + 1;
	return false;
}

bool Cadena::buscaFinal( char caracter, size_t& posicion ) {
	if( m_longitud == 0 ) return false;
#ifdef USAR_CADENA_SEGURA
	char* retorno = strrchr( (const char*)m_cadena, m_capacidad, caracter );
#else
	char* retorno = strrchr( (const char*)m_cadena, caracter );
#endif
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	/*
	// Error raro pero hay que hacer seguimiento:
	if( m_cadena[0] == caracter ) {
		posicion = 0;
		return true;
	}
	*/
	posicion = m_longitud + 1;
	return false;
}

bool Cadena::buscaFinal( const char* texto, size_t& posicion  ) {
	if( m_longitud == 0 ) return false;
	if( texto == NULL   ) return false;
#ifdef USAR_CADENA_SEGURA
	char* retorno = strrstr( (const char*)m_cadena, m_capacidad, texto, m_capacidad );
#else
	char* retorno = strrstr( (const char*)m_cadena, texto );
#endif
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	posicion = m_longitud + 1;
	return false;
}

bool Cadena::buscaFinal( const Cadena& texto, size_t& posicion  ) {
	if( m_longitud       == 0 ) return false;
	if( texto.m_longitud == 0 ) return false;
#ifdef USAR_CADENA_SEGURA
	char* retorno = strrstr( (const char*)m_cadena, m_capacidad, texto.m_cadena, texto.m_capacidad );
#else
	char* retorno = strrstr( (const char*)m_cadena, texto.m_cadena );
#endif
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	posicion = m_longitud + 1;
	return false;
}

void Cadena::borra() {
	if( m_capacidad == 0 ) return;
	memset( m_cadena, 0, m_capacidad );
	m_longitud = 0;
}

void Cadena::borra( size_t inicio, size_t final ) {
	if( inicio > final ) return;
	if( final > m_longitud ) final = m_longitud;
#ifdef USAR_CADENA_SEGURA
	strlcpy( m_cadena + inicio, m_capacidad - inicio, m_cadena + final, m_longitud - final + 1 );
#else
	strlcpy( m_cadena + inicio, m_cadena + final, m_longitud - final + 1 );
#endif
	m_longitud -= ( final - inicio );
	memset( m_cadena + m_longitud, 0, m_capacidad - m_longitud );
}

void Cadena::borraHasta( size_t posicion ) {
	if( posicion >= m_longitud ) {
		borra();
		return;
	}
#ifdef USAR_CADENA_SEGURA
	strlcpy( m_cadena, m_capacidad - posicion, m_cadena + posicion, m_capacidad );
#else
	strlcpy( m_cadena, m_cadena + posicion, m_capacidad );
#endif
	m_longitud -= posicion;
	memset( m_cadena + m_longitud, 0, m_capacidad - m_longitud );
}

void Cadena::borraDesde( size_t posicion ) {
	if( posicion > m_capacidad ) return;
	memset( m_cadena + posicion, 0, m_capacidad - posicion );
	m_longitud = posicion;
}

bool Cadena::parte( Cadena& cadena, const char *delimitadores ) {
	// static size_t m_parte_inicio = 0;
	// Ponemos '\0' en todas las coincidencias con los delimitadores:
	if( m_parte_inicio == 0 ) {
	#ifdef USAR_CADENA_SEGURA
		size_t longitud = strlen( delimitadores, 1024 );
	#else
		size_t longitud = strlen( delimitadores );
	#endif
		for( size_t i = 0; i < m_longitud; i++ ) {
			for( size_t j = 0; j < longitud; j++ ) {
				if( m_cadena[i] == delimitadores[j] ) {
					m_cadena[i] = '\0';
					break;
				}
			}
		}
	}
	// Saltamos los '\0' hasta encontrar un tozo de cadena:
	size_t longitud = 0;
	for( ; m_parte_inicio < m_longitud; ++m_parte_inicio ) {
	#ifdef USAR_CADENA_SEGURA
		longitud = strlen( m_cadena + m_parte_inicio, m_capacidad - m_parte_inicio );
	#else
		longitud = strlen( m_cadena + m_parte_inicio );
	#endif
		if( longitud > 0 ) break;
	}
	// Hemos acabado:
	if( m_parte_inicio >= m_longitud ) {
		m_parte_inicio = 0;
		quita();
		return false;
	}
	// Copiamos el trozo de cadena:
	cadena.copia( (const char*)(m_cadena + m_parte_inicio) );
	m_parte_inicio += longitud + 1;
	return true;
}

void Cadena::mayusculas() { Mayusculas( m_cadena, m_capacidad ); }
void Cadena::minusculas() { Minusculas( m_cadena, m_capacidad );	}

int            Cadena::dameInt()   const { return atoi( m_cadena ); }
unsigned int   Cadena::dameUInt()  const { char *p; return (unsigned int)strtoul( m_cadena, &p, 10 ); }
long           Cadena::dameLong()  const { char *p; return strtol(  m_cadena, &p, 10 ); }
unsigned long  Cadena::dameULong() const { char *p; return strtoul( m_cadena, &p, 10 ); }
#ifdef CADENA_USAR_LONGLONG
long long          Cadena::dameLLong()  const { char *p; return strtoll(  m_cadena, &p, 10 ); }
unsigned long long Cadena::dameULLong() const { char *p; return strtoull( m_cadena, &p, 10 ); }
#endif
float  Cadena::dameFloat()  const { return (float)atof( m_cadena ); }
double Cadena::dameDouble() const { return atof( m_cadena ); }

bool Cadena::tomaInt( int numero ) {
	size_t ancho = (size_t)( sizeof( int ) * 2.5f ) + 3;
	if( ancho > m_capacidad ) inicia( ancho );
#ifdef __MINGW32__
	itoa( numero, m_cadena, 10 );
#else
	sprintf( m_cadena, "%i", numero );
#endif
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
    if( m_longitud == 0 ) return false;
    return true;
}

bool Cadena::tomaUInt( unsigned int numero ) {
	size_t ancho = (size_t)( sizeof( unsigned int ) * 2.5f ) + 3;
	if( ancho > m_capacidad ) inicia( ancho );
#ifdef __MINGW32__
	uitoa( numero, m_cadena, 10 );
#else
	sprintf( m_cadena, "%u", numero );
#endif
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
    if( m_longitud == 0 ) return false;
    return true;
}

bool Cadena::tomaLong( long numero ) {
	size_t ancho = (size_t)( sizeof( long ) * 2.5f ) + 3;
	if( ancho > m_capacidad ) inicia( ancho );
#ifdef __MINGW32__
	ltoa( numero, m_cadena, 10 );
#else
	sprintf( m_cadena, "%li", numero );
#endif
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
    if( m_longitud == 0 ) return false;
    return true;
}

bool Cadena::tomaULong( unsigned long numero ) {
	size_t ancho = (size_t)( sizeof( unsigned long ) * 2.5f ) + 3;
	if( ancho > m_capacidad ) inicia( ancho );
#ifdef __MINGW32__
	_ultoa( numero, m_cadena, 10 );
#else
	sprintf(m_cadena, "%lu", numero );
#endif
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
    if( m_longitud == 0 ) return false;
    return true;
}

#ifdef CADENA_USAR_LONGLONG
bool Cadena::tomaLLong( long long numero ) {
	size_t ancho = (size_t)( sizeof( long long ) * 2.5f ) + 3;
	if( ancho > m_capacidad ) inicia( ancho );
#ifdef __MINGW32__
	lltoa( numero, m_cadena, 10 );
#else
	sprintf( m_cadena, "%lli", numero );
#endif
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
    if( m_longitud == 0 ) return false;
    return true;
}

bool Cadena::tomaULLong( unsigned long long numero ) {
	size_t ancho = (size_t)( sizeof( unsigned long long ) * 2.5f ) + 3;
	if( ancho > m_capacidad ) inicia( ancho );
#ifdef __MINGW32__
	ulltoa( numero, m_cadena, 10 );
#else
	sprintf( m_cadena, "%llu", numero );
#endif
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
    if( m_longitud == 0 ) return false;
    return true;
}
#endif

bool Cadena::tomaFloat( float numero, int cantidad ) {
	size_t ancho = (size_t)cantidad + 3;
	if( ancho > m_capacidad ) inicia( ancho );
	if( gcvt( (double)numero, cantidad, m_cadena ) == NULL ) return false;
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
    if( m_longitud == 0 ) return false;
    return true;
}

bool Cadena::tomaDouble( double numero, int cantidad ) {
	size_t ancho = (size_t)cantidad + 3;
	if( ancho > m_capacidad ) inicia( ancho );
	if( gcvt( numero, cantidad, m_cadena ) == NULL ) return false;
#ifdef USAR_CADENA_SEGURA
	m_longitud = strlen( m_cadena, m_capacidad );
#else
	m_longitud = strlen( m_cadena );
#endif
    if( m_longitud == 0 ) return false;
    return true;
}

Cadena& Cadena::operator = ( char       caracter ) { this->copia( caracter ); return *this; }
Cadena& Cadena::operator = ( const char*   texto ) { this->copia( texto );    return *this; }
Cadena& Cadena::operator = ( const Cadena& texto ) { this->copia( texto );    return *this; }
Cadena& Cadena::operator = ( int numero ) { this->tomaInt( numero ); return *this; }
Cadena& Cadena::operator = ( unsigned int numero ) { this->tomaUInt( numero ); return *this; }
Cadena& Cadena::operator = ( long numero ) { this->tomaLong( numero ); return *this; }
Cadena& Cadena::operator = ( unsigned long numero ) { this->tomaULong( numero ); return *this; }
#ifdef CADENA_USAR_LONGLONG
Cadena& Cadena::operator = ( long long numero ) { this->tomaLLong( numero ); return *this; }
Cadena& Cadena::operator = ( unsigned long long numero ) { this->tomaULLong( numero ); return *this; }
#endif
Cadena& Cadena::operator = ( float numero ) { this->tomaFloat( numero, 16 ); return *this; }
Cadena& Cadena::operator = ( double numero ) { this->tomaDouble( numero, 24 ); return *this; }

Cadena& Cadena::operator << ( char       caracter ) { this->concatena( caracter ); return *this; }
Cadena& Cadena::operator << ( const char*   texto ) { this->concatena( texto );    return *this; }
Cadena& Cadena::operator << ( const Cadena& texto ) { this->concatena( texto );    return *this; }
Cadena& Cadena::operator << ( int numero ) { Cadena cadena; cadena.tomaInt( numero );  this->concatena( cadena ); return *this; }
Cadena& Cadena::operator << ( unsigned int numero ) { Cadena cadena; cadena.tomaUInt( numero );  this->concatena( cadena ); return *this; }
Cadena& Cadena::operator << ( long numero ) { Cadena cadena; cadena.tomaLong( numero );  this->concatena( cadena ); return *this; }
Cadena& Cadena::operator << ( unsigned long numero ) { Cadena cadena; cadena.tomaULong( numero );  this->concatena( cadena ); return *this; }
#ifdef CADENA_USAR_LONGLONG
Cadena& Cadena::operator << ( long long numero ) { Cadena cadena; cadena.tomaLLong( numero );  this->concatena( cadena ); return *this; }
Cadena& Cadena::operator << ( unsigned long long numero ) { Cadena cadena; cadena.tomaULLong( numero );  this->concatena( cadena ); return *this; }
#endif
Cadena& Cadena::operator << ( float numero ) { Cadena cadena; cadena.tomaFloat( numero, 16 );  this->concatena( cadena ); return *this; }
Cadena& Cadena::operator << ( double numero ) { Cadena cadena; cadena.tomaDouble( numero, 24 );  this->concatena( cadena ); return *this; }

Cadena  Cadena::operator + ( char       caracter ) { Cadena cadena( *this ); cadena << caracter; return cadena; }
Cadena  Cadena::operator + ( const char*   texto ) { Cadena cadena( *this ); cadena << texto; return cadena; }
Cadena  Cadena::operator + ( const Cadena& texto ) { Cadena cadena( *this ); cadena << texto; return cadena; }
Cadena  Cadena::operator + ( int numero ) { Cadena cadena( *this ); cadena << numero; return cadena; }
Cadena  Cadena::operator + ( unsigned int numero ) { Cadena cadena( *this ); cadena << numero; return cadena; }
Cadena  Cadena::operator + ( long numero ) { Cadena cadena( *this ); cadena << numero; return cadena; }
Cadena  Cadena::operator + ( unsigned long numero ) { Cadena cadena( *this ); cadena << numero; return cadena; }
#ifdef CADENA_USAR_LONGLONG
Cadena  Cadena::operator + ( long long numero ) { Cadena cadena( *this ); cadena << numero; return cadena; }
Cadena  Cadena::operator + ( unsigned long long numero ) { Cadena cadena( *this ); cadena << numero; return cadena; }
#endif
Cadena  Cadena::operator + ( float numero ) { Cadena cadena( *this ); cadena << numero; return cadena; }
Cadena  Cadena::operator + ( double numero ) { Cadena cadena( *this ); cadena << numero; return cadena; }

//char&       Cadena::operator[]( size_t posicion )       { char retorno = '\0'; return ( posicion < m_longitud ) ? m_cadena[ posicion ] : retorno; }
//const char& Cadena::operator[]( size_t posicion ) const { char retorno = '\0'; return ( posicion < m_longitud ) ? m_cadena[ posicion ] : retorno; }
char&       Cadena::operator[]( size_t posicion )       { return ( posicion < m_longitud ) ? m_cadena[ posicion ] : m_cadena[ m_longitud-1 ]; }
const char& Cadena::operator[]( size_t posicion ) const { return ( posicion < m_longitud ) ? m_cadena[ posicion ] : m_cadena[ m_longitud-1 ]; }

bool Cadena::operator == ( const char*   texto ) { return this->comparaS(  texto ); }
bool Cadena::operator == ( const Cadena& texto ) { return this->comparaS(  texto ); }
bool Cadena::operator != ( const char*   texto ) { return !this->comparaS( texto ); }
bool Cadena::operator != ( const Cadena& texto ) { return !this->comparaS( texto ); }


