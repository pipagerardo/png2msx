#ifndef _CADENA_
#define _CADENA_

#include "funciones.h"
// #define CADENA_USAR_LONGLONG 1

class Cadena {
public:
// -----------------------------------------------------------------------------
///                        CONSTRUCTORES Y DESTRUCTOR
// -----------------------------------------------------------------------------
	Cadena();
	Cadena( size_t capacidad );
	Cadena( size_t capacidad, size_t bloque );
	Cadena( const char*   texto );
	Cadena( const Cadena& texto );
	~Cadena();
	
	// El tamaño de los bloques de memoria. No hace nada hasta iniciar la cadena.
	// Tamaño de bloque > 8 y < 1024, por defecto 64. Múltiplo de 8.
	void   tomaBloque( size_t bloque ); 
	// Retorna el tamaño de bloque de memoria. Múltiplo de 8.
	size_t dameBloque() const;			
	
	// Constructor y destructor de memoria.
	bool inicia( size_t capacidad );				// Reserva memoria en un multiplo de bloque.
	bool inicia( size_t capacidad, size_t bloque );	// Reserva memoria en un multiplo de bloque.
	void quita();									// Libera la memoria y restaura todos los valores.
	// Nota: La función Inicia también vale para ajustar la capacidad de una cadena 
	//       previamente iniciada, nunca borra los datos contenidos.
	
	// La capacidad de memoria reservada es un multiplo de bloque.
	size_t      dameCapacidad() const;		    // Retorna la memoria reservada.
	size_t      dameLongitud() const;			// Retorna la longitud de la cadena alojada.
	const char* dameCadena()   const;			// Retorna la cadena.
	
	// Copiar datos a la cadena:
	size_t copia( char       caracter );
	size_t copia( const char*   texto );
	size_t copia( const Cadena& texto );
	size_t copia( Cadena& origen, size_t inicio, size_t final );
	
	// Concatenar datos a la cadena:
	size_t concatena( char       caracter );
	size_t concatena( const char*   texto );
	size_t concatena( const Cadena& texto );

	// Gestión tipo buffer (hay que saber lo que se hace):
	const Uint8* dameBuffer() const;
	size_t copiaBuffer( const Uint8* buffer, size_t cantidad );
	size_t concatenaBuffer( const Uint8* buffer, size_t cantidad );
	
	// sprintf y sscanf:
	size_t sprintf(  const char* formato, ... );		// Requiere iniciar la cadena con una capacidad.
	size_t vsprintf( const char* formato, va_list va ); // Requiere iniciar la cadena con una capacidad.
	size_t sprintf(  size_t cantidad, const char* formato, ... );
	size_t vsprintf( size_t cantidad, const char* formato, va_list va );
	size_t sscanf(   const char* formato, ... );
	size_t vsscanf(  const char* formato, va_list va );

	// Comparaciones sesibles a las mayúsculas:
	bool comparaS( const char*   texto );
	bool comparaS( const char*   texto, size_t longitud );
	bool comparaS( const Cadena& texto );
	bool comparaS( const Cadena& texto, size_t longitud );
	
	// Comparaciones no sesibles a las mayúsculas:
	bool comparaN( const char*   texto );
	bool comparaN( const char*   texto, size_t longitud );
	bool comparaN( const Cadena& texto );
	bool comparaN( const Cadena& texto, size_t longitud );
	
	// Busquedas:
	bool busca( char       caracter, size_t& posicion );
	bool busca( const char*   texto, size_t& posicion );
	bool busca( const Cadena& texto, size_t& posicion );

	bool buscaFinal( char       caracter, size_t& posicion );
	bool buscaFinal( const char*   texto, size_t& posicion );
	bool buscaFinal( const Cadena& texto, size_t& posicion );
	
	// Borrar partes:
	void borra(); 								// Borra todo;
	void borra( size_t inicio, size_t final );	// Borra desde inicia hasta final.
	void borraHasta( size_t posicion );			// Borra desde el inicio hasta posición.
	void borraDesde( size_t posicion );			// Borra desde posición hata el final.
	bool parte( Cadena& cadena, const char *delimitadores ); // Ojo que esta función es compleja, destruye el origen.
	// Cadena parte, cadena( "Hola,cara-caballo/burro?asno");
	// while( cadena.parte( parte, ",/?" ) ) SDL_LOG( "%s", parte.dameCadena() );

	void mayusculas();
	void minusculas();

	// Conversiones:
	int                dameInt()    const;
	unsigned int       dameUInt()   const;
	long               dameLong()   const;
	unsigned long      dameULong()  const;
#ifdef CADENA_USAR_LONGLONG
	long long          dameLLong()  const;
	unsigned long long dameULLong() const;
#endif
	float              dameFloat()  const;
	double             dameDouble() const;

	bool tomaInt( int numero );
	bool tomaUInt( unsigned int numero );
	bool tomaLong( long numero );
	bool tomaULong( unsigned long numero );
#ifdef CADENA_USAR_LONGLONG
	bool tomaLLong( long long numero );
	bool tomaULLong( unsigned long long numero );
#endif
	bool tomaFloat( float numero, int cantidad );
	bool tomaDouble( double numero, int cantidad );
	
	// Estilo C++
	Cadena& operator = ( char       caracter );
	Cadena& operator = ( const char*   texto );
	Cadena& operator = ( const Cadena& texto );
	Cadena& operator = ( int numero );
	Cadena& operator = ( unsigned int numero );
	Cadena& operator = ( long numero );
	Cadena& operator = ( unsigned long numero );
#ifdef CADENA_USAR_LONGLONG
	Cadena& operator = ( long long numero );
	Cadena& operator = ( unsigned long long numero );
#endif
	Cadena& operator = ( float numero );
	Cadena& operator = ( double numero );
	
	Cadena& operator << ( char       caracter );
	Cadena& operator << ( const char*   texto );
	Cadena& operator << ( const Cadena& texto );
	Cadena& operator << ( int numero );
	Cadena& operator << ( unsigned int numero );
	Cadena& operator << ( long numero );
	Cadena& operator << ( unsigned long numero );
#ifdef CADENA_USAR_LONGLONG
	Cadena& operator << ( long long numero );
	Cadena& operator << ( unsigned long long numero );
#endif
	Cadena& operator << ( float numero );
	Cadena& operator << ( double numero );

	Cadena  operator + ( char       caracter );
	Cadena  operator + ( const char*   texto );
	Cadena  operator + ( const Cadena& texto );
	Cadena  operator + ( int numero );
	Cadena  operator + ( unsigned int numero );
	Cadena  operator + ( long numero );
	Cadena  operator + ( unsigned long numero );
#ifdef CADENA_USAR_LONGLONG
	Cadena  operator + ( long long numero );
	Cadena  operator + ( unsigned long long numero );
#endif
	Cadena  operator + ( float numero );
	Cadena  operator + ( double numero );
		
	char&       operator[]( size_t posicion );
	const char& operator[]( size_t posicion ) const;

	bool operator == ( const char*   texto );
	bool operator == ( const Cadena& texto );

	bool operator != ( const char*   texto );
	bool operator != ( const Cadena& texto );
	
private:
	size_t m_bloque;
	size_t m_capacidad;
	size_t m_longitud;
	char*  m_cadena;
	size_t m_parte_inicio;
};

#endif

