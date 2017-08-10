#ifndef _MATEMATICAS_
#define _MATEMATICAS_
#include "funciones.h"
#include <time.h>
#include <math.h>

// ----------------------------------------------------------------------------
//                         M A T E M Á T I C A S
// ----------------------------------------------------------------------------
// #define M_PI    3.14159265358979323846264338327950288   
// int abs(int x);
// double acos(double x);
// double asin(double x);
// double atan(double x);
// double atan2(double x, double y);
// double ceil(double x);
// double copysign( double x, double y );	// retorna x con el signo de y.
// double cos(double x);
// float  cosf(float x);
// double fabs(double x);			// retorna el flotante en positivo.
// double floor(double x);
// double log(double x);
// double pow(double x, double y);
// double scalbn( double x, int n ); // Multiplica el valor de punto flotante por argumeno n FLT_RADIX elevado a potencia exp
// double sin(double x);
// float  sinf(float x);
// double sqrt(double x);
// ---------------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Seno:   ( cateto opuesto / hipotenusa )
// ------------------------------------------------------------------------
// Coseno: ( cateto contiguo / hipotenusa )
// ------------------------------------------------------------------------
// Tangente: ( cateto opuesto / cateto contiguo ) == ( seno / coseno )
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Números aleatorios:
// ------------------------------------------------------------------------
void   Semilla();
void   Semilla( Uint32 tiempo );
int    Aleatorio( int   min, int   max );
float  Aleatorio( float min, float max ); 
// ------------------------------------------------------------------------
// De Radianes a Grados y Viceversa:
// ------------------------------------------------------------------------
// Las funciones trigonométricas retornan el ángulo en radianes.
// La circunferencia completa en   grados = 360º
// La circunferencia completa en radianes = 2 * Pi
//      Conclusión:       360 grados == 2 * Pi radianes
//                        180 grados == Pi radianes
//     Un radián son 57.2958 grados. Un grado son 0.0174533 radianes.
// ------------------------------------------------------------------------
float Grados( float radianes );
float Radianes( float grados );
// ------------------------------------------------------------------------
// Hipotenusa:  ( h^2 = a^2 + b^2 )
// ------------------------------------------------------------------------
float Hipotenusa( float a, float b );
// ------------------------------------------------------------------------
// Distancia entre dos Puntos 2D: distancia = Hipotenusa( y2 - y1, x2 - x1 );
// ------------------------------------------------------------------------
float Distancia2P( float x1, float y1, float x2, float y2 );
// ------------------------------------------------------------------------
// Ángulo entre dos Puntos 2D: radianes = Atan2( y2 - y1, x2 - x1 );
// ------------------------------------------------------------------------
float Angulo2P( float x1, float y1, float x2, float y2 );
// ------------------------------------------------------------------------
// Igualdad entre dos Puntos 2D: P1( x1, y1 ) == P2( x2, y2 )
// ------------------------------------------------------------------------
bool Igualdad2P( float x1, float y1, float x2, float y2 );
// ------------------------------------------------------------------------
// Ecuación de la Recta entre dos Puntos 2D: P1( x1, y1 ), P2( x2, y2 )
//    m = ( y2 - y1 ) / ( x2 - y2 );    // tangente de alfa.
//    b = y1 - ( m * x1 );              // punto de corte eje vertical Y
//    y = ( m * x ) + b;				// Si x = 0; y = b;		
//    x = ( y - b ) / m;				// Si y = 0; x = -b / m;
// ------------------------------------------------------------------------
bool Recta2P( float x1, float y1, float x2, float y2, float &m, float &b );
// ------------------------------------------------------------------------
// Distancia ente un Punto y una Recta 2D: P( x, y ) R( m, b )
//     distancia = ( ( m * x ) - y + b ) / Raiz2( m * m + 1 );
// ------------------------------------------------------------------------
float Distancia1P1R( float x, float y, float m, float b );
// ------------------------------------------------------------------------
// Recta (Perpendicular) ente un Punto y una Recta 2D: P( x, y ) R( m, b )
//     mp = -m / 2;
//     bp = y - ( mp * x );
// ------------------------------------------------------------------------
void Recta1P1R( float x, float y, float m, float &mp, float &bp );
// ------------------------------------------------------------------------
// Punto de curce entre dos Rectas 2D: R1( m1, b1 ), R2( m2, b2 )
//     if( m1 != m2 ) {
//         x = ( b2 - b1 ) / ( m1 - m2 );
//         y =  x * m2 + b2
//     }
// ------------------------------------------------------------------------
bool Punto2R( float m1, float b1, float m2, float b2, float &x, float &y );
// ------------------------------------------------------------------------
// Angulo entre dos Rectas 2D: R1( m1, b1 ), R2( m2, b2 )
//     Atan( ( m2 - m1 ) / ( 1 + ( m2 * m1 ) ) );
// ------------------------------------------------------------------------
float Angulo2R( float m1, float m2 );
// ------------------------------------------------------------------------
// Distancia entre dos Rectas Paraleas 2D: R1( m1, b1 ), R2( m2, b2 )
// ------------------------------------------------------------------------
float Distancia2R( float m1, float b1, float m2, float b2 );

// ------------------------------------------------------------------------
// Retorna la potencia de 2 más cercana:
// ------------------------------------------------------------------------
int PotenciaCercana( int valor );

// ----------------------------------------------------------------------------
//                      F I N   M A T E M Á T I C A S
// ----------------------------------------------------------------------------
#endif

