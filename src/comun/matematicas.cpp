#include "matematicas.h"

void Semilla() { 
	time_t m_time;
	time( &m_time );
	srand( m_time ); /*SDL_GetTicks()*/
} 

void Semilla( Uint32 tiempo ) {
	srand( tiempo );
} 

int Aleatorio( int   min, int   max ) {
	return min + ( rand() % ( max - min + 1 ) );
}

float Aleatorio( float min, float max ) {
	return min + ( (float)rand() / RAND_MAX * ( max - min ) ) ;
}

float Grados( float radianes ) { 
	return ( 180.0f * radianes ) / M_PI;
}

float Radianes( float grados ) { 
	return ( M_PI * grados ) / 180.0f;
}

float Hipotenusa( float a, float b ) { 
	return sqrt( (a * a) + (b * b) );
}

float Distancia2P( float x1, float y1, float x2, float y2 ) { 
	return Hipotenusa( y2 - y1, x2 - x1 );
}

float Angulo2P( float x1, float y1, float x2, float y2 ) {
	return atan2( y2 - y1, x2 - x1 );
}

bool Igualdad2P( float x1, float y1, float x2, float y2 ) { 
	if ( y2 != y1 ) return false; 
  	if ( x2 != x1 ) return false; 
   	return true; 
}

bool Recta2P( float x1, float y1, float x2, float y2, float &m, float &b ) {
    if( Igualdad2P( x1, y1, x2, y2 ) ) return false;
    if( x2 == x1 ) m = 0;
    else m = ( y2 - y1 ) / ( x2 - x1 ); 
    b = y1 - ( m * x1 ); 
    return true;
}

float Distancia1P1R( float x, float y, float m, float b ) { 
    return ( ( m * x ) - y + b ) / sqrt( m * m + 1.0f ); 
}
void Recta1P1R( float x, float y, float m, float &mp, float &bp ) { 
    mp = -m / 2.0f; 
    bp = y - ( mp * x ); 
}

bool Punto2R( float m1, float b1, float m2, float b2, float &x, float &y ) { 
    if( m1 == m2 ) return false; 
    x = ( b2 - b1 ) / ( m1 - m2 ); 
	y =  x * m2 + b2; 
	return true; 
}

float Angulo2R( float m1, float m2 ) { 
	return atan( ( m2 - m1 ) / ( 1.0f + ( m2 * m1 ) ) );
}

float Distancia2R( float m1, float b1, float m2, float b2 ) {
    if( m1 == m2 ) return ( b1 - b2 ) / sqrt( 1.0f + ( m1 * m2 ) );
    return 0.0f; 
}

int PotenciaCercana( int valor ) {
	if( valor == 0 ) return 1;
    for( register int i = 1; ; ) {
		if( valor == 1 ) return i;
		else if( valor == 3 ) return i << 2; // return i * 4;
		valor >>= 1;
		i <<= 1;
    }
}

// ------------------------------------------------------------------------


