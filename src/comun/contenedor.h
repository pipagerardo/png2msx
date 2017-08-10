#ifndef _CONTENEDOR_
#define _CONTENEDOR_

#include "funciones.h"
	
template <class TIPO>
class Contenedor {
public:

Contenedor()  { m_longitud = 0; m_datos = NULL; }
~Contenedor() { quita(); }

// Mete al final:
bool mete( const TIPO* datos, const size_t cantidad ) {
	size_t nuevaLongitud = m_longitud + cantidad;
	TIPO*  nuevosDatos   = new TIPO[ nuevaLongitud ];
	if( nuevosDatos == NULL ) return false; // Fallo de memoria.
	bool retorno;
	// Si hay datos viejos los copiamos al principio de los nuevos datos.
	if( m_datos != NULL ) {
		retorno = memcpy( (void*)nuevosDatos, (void*)m_datos, m_longitud * sizeof( TIPO ) ) == (void*)nuevosDatos ? true : false;
		delete[] m_datos;
        if( !retorno ) return retorno;
    }
    // Copiamos los nuevos datos.
    retorno = memcpy( (void*)&nuevosDatos[ m_longitud ], (void*)datos, cantidad * sizeof( TIPO ) ) == (void*)&nuevosDatos[ m_longitud ] ? false : true;
    m_datos     = nuevosDatos;
    m_longitud += cantidad;
    return retorno;
};

// Saca del principio y retorna la que queda o cero en caso de error:
size_t saca( TIPO* datos, const size_t cantidad ) {
	// Copiamos los datos solicitados:
	if( m_datos == NULL ) return 0;
	size_t nuevaCantidad = ( cantidad > m_longitud ) ? m_longitud : cantidad;
	if( memcpy( (void*)datos, (void*)m_datos, nuevaCantidad * sizeof( TIPO ) ) != (void*)datos ) return 0;
	// Creamos nuevos datos esta vez más pequeño:
	size_t nuevaLongitud = m_longitud - nuevaCantidad;
	if( nuevaLongitud == 0 ) {
		// quita();
		delete[] m_datos;
		m_datos = NULL;
		m_longitud = 0;	
		return nuevaCantidad;
	}
	TIPO* nuevosDatos = new TIPO[ nuevaLongitud ];
	if( nuevosDatos == NULL ) return 0; // Fallo de memoria.
	// Si hay datos viejos los copiamos al principio de los nuevos datos.
	if( memcpy( (void*)nuevosDatos, (void*)(m_datos + nuevaCantidad), nuevaLongitud * sizeof( TIPO ) ) != (void*)nuevosDatos ) {
    	delete[] nuevosDatos;
		return 0;
	}
	// Todo bien:
	delete[] m_datos;
	m_datos = nuevosDatos;
	m_longitud = nuevaLongitud;
    return m_longitud;
};

TIPO   operator[] ( size_t posicion ) const	{ return m_datos[ posicion ]; }
TIPO&  operator[] ( size_t posicion )       { return m_datos[ posicion ]; }
TIPO*  operator() (void) const 				{ return m_datos; }
TIPO*  operator() (size_t posicion ) const  { return &m_datos[ posicion ]; }
size_t dameLongitud() const 				{ return m_longitud; }

void quita() {
	m_longitud = 0;
	if( m_datos != NULL ) delete[] m_datos;
	m_datos = NULL;
}
 
private: 
	TIPO*  m_datos;
	size_t m_longitud;
};

#endif

