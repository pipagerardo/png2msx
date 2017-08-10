#ifndef _MATRIZ_
#define _MATRIZ_

#include "funciones.h"
	
template <class TIPO>
class Matriz {
public:

	Matriz() {
		m_x = 0;
		m_y = 0;
		m_matriz = NULL;	
	}
		
	Matriz( size_t x, size_t y ) {
		m_x = 0;
		m_y = 0;
		m_matriz = NULL;
		inicia( x, y );
	}
	
	~Matriz() {
		quita();
	}

	bool inicia( size_t x, size_t y ) {
		if( m_matriz != NULL ) return false;
		m_x = x;
		m_y = y;
		m_matriz = new TIPO*[ m_y ];
		if( m_matriz == NULL ) return false;
		for( size_t i = 0; i < m_y; ++i ) {
			m_matriz[i] = new TIPO[ m_x ];
			if( m_matriz[i] == NULL ) {
				m_y = i;
				quita();
				return false;
			}
		}
		return true;
	}
	
	void quita() {
		if( m_matriz == NULL ) return;
		for( size_t i = 0; i < m_y; ++i ) {
			if( m_matriz[i] != NULL ) delete[] m_matriz[i];
			m_matriz[i] = NULL;
		}
		delete[] m_matriz;
		m_x = 0;
		m_y = 0;
		m_matriz = NULL;
	}

	TIPO& operator()( size_t x, size_t y  ) {
		if( x >= m_x ) x = m_x - 1;
		if( y >= m_y ) y = m_y - 1;
		return m_matriz[y][x];
	}
	
	TIPO  operator()( size_t x, size_t y  ) const { 
		if( x >= m_x ) x = m_x - 1;
		if( y >= m_y ) y = m_y - 1;
		return m_matriz[y][x];
	}
	
	TIPO dame( size_t x, size_t y  ) const { 
		if( x >= m_x ) x = m_x - 1;
		if( y >= m_y ) y = m_y - 1;
		return m_matriz[y][x];
	}
	
	size_t dameY() const { return m_y; }
	size_t dameX() const { return m_x; }

	void toma( size_t x, size_t y, TIPO dato ) { 
		if( x >= m_x ) x = m_x - 1;
		if( y >= m_y ) y = m_y - 1;
		m_matriz[y][x] = dato;
	}


private:
	size_t m_x;
	size_t m_y;
	TIPO **m_matriz;
};

#endif

