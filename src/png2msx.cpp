// ----------------------------------------------------------------------------
//  png2msx         beta 8
// --------
//		Autor:		PipaGerardo
//      E-Mail:		pipagerardo@gmail.com
// ----------------------------------------------------------------------
// Conversor de imágenes de PNG a MSX, por PipaGerardo.
// Versión beta 8 ( 2018 )       pipagerardo@gmail.com
// png2msx.exe entrada.png [nombre_salida] [-opciones] \n" );
// ----------------------------------------------------------------------
// png2msx.exe entrada.png [nombre_salida] [-opciones]
// 	-x[Número]	Posición de x en pixels [ 0 ~ 2048 ]
// 	-y[Número]	Posición de y en pixels [ 0 ~ 2048 ]
// 	-w[Número]	Ancho en pixels [ 0 ~ 2048 ]
// 	-h[Número]	Alto  en pixels [ 0 ~ 2048 ]
// 				"w*h" debe ser igual o menor que 49152
// 				Si "w" no es 256 la imagen no estará alineada
// 	-g[Número]	Umbral de Gris [ 0 ~ 255 ]
// 	-b[Número]	Contraste [ -255 ~ 255 ]:
//  			-b		contraste automático
//  			-b16	Blancos más blancos y viceversa
//  			-b-8	Blancos más oscuros y viceversa
// 	-c			Filtro de Colisón de Color (8 pixels)
// 	-e			Pasa error conversión al siguiente pixel
//  -z			Cambia el color 0 por el 1
//  -n			Cambia el color 1 por el 0
//  -k[Número]  Color de fondo de 0 a 15
// 	-f[Número]	Busca baldosas parecidas de 1 a 16 bits
// 				Recomiendo valores de 2 a 6
// 	-o[Número]	Quita las baldosas repetidas en VRAM:
// 		 		-o0 Baldosas repetidas 1 banco  VRAM 1-1-1
//  			-o1 Baldosas repetidas 2 bancos VRAM 1-2-1
//  			-o2 Baldosas repetidas 2 bancos VRAM 1-1-3
// 				-o3 Baldosas repetidas 3 bancos VRAM 1-2-3
// 				-o4 Baldosas repetidas sin restricciones
// 	-p			Archivo PNG de salida
// 	-a			Archivo PNG de salida con canal Alfa
// 	-r			Archivo en bruto para ASM: CHR, CLS y NAM
//  -t			Archivo en bruto Pletter5 sin longitud
// 	-l			Archivo en bruto Pletter5 con longitud
// 	-s			Archivo SC2 para BLOAD"ARCHIVO.SC2",S
// 	-m			Archivo BASIC ASCII que carga SC2
// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------
bool  color_fondo   = false;    // Global
Uint8 numColorFondo = 0;        // Global
// ----------------------------------------------------------------------------
struct Baldosa {
	Uint8 chr[8];   // Los patrones de bits de las 8 filas de la baldosa.
	Uint8 clr[8];   // Los colores [Tinta|Fondo] de las filas de la baldosa.
	Uint8 num;		// Si está repetida el indice de la original.
	bool  rep;		// Si es una baldosa repetida o es la original.
};
// ----------------------------------------------------------------------------
struct Colision {
	Color col;	// Color
	Uint8 rep;	// Número de Repeticiones.
};
// ----------------------------------------------------------------------------
int comparar_colision( const void* arg1, const void* arg2 ) {
	if( (*( Colision *)arg1).rep < (*( Colision *)arg2).rep ) return  1;
	if( (*( Colision *)arg1).rep > (*( Colision *)arg2).rep ) return -1;
   	return  0;
}
// ----------------------------------------------------------------------------
// Transforma los colores a c1 y c2:
void procesaColor( Color linea[8], Color c1, Color c2 ) {
	float Y, Y1, Y2;
	Y1 = c1.dameLuminancia();
	Y2 = c2.dameLuminancia();
	for( register int i = 0; i < 8; ++i ) {
		if(	linea[i].dameIndice() == c1.dameIndice() ) continue;
		if(	linea[i].dameIndice() == c2.dameIndice() ) continue;
		Y = linea[i].dameLuminancia();
		if( fabs(Y-Y1) < fabs(Y-Y2) )	linea[i].inicia( c1 );
		else 							linea[i].inicia( c2 );
	}
}
// ----------------------------------------------------------------------------
void procesaLinea( Color linea[8] ) {
	// Iniciamos una tabla de Colisión de Color
	Tabla<Colision> colision( 16 );
	for( register Uint8 i = 0; i < 16; ++i ) {
		colision[i].col.inicia( (COLOR)i );
		colision[i].rep= 0;
	}
	// Llenamos la tabla con los datos de repetición de color
	for( register int i = 0; i < 8; ++i ) ++colision[ linea[i].dameIndice() ].rep;
	colision.ordena( comparar_colision );
	// Contamos los colores repetidos:
	Uint8 rep = 0;
	for( register int i = 0; i < 8; ++i ) {
		if( colision[ i ].rep == 0 ) break;
		++rep;
	}
	// Si hay menos de tres colores repetidos no hacemos nada:
	switch( rep ) {
		case 0:
			printf( "Caso 0?\n" );
		break;
		case 1:
			// printf( "caso 1\n" );
			// procesaColor( linea, colision[ 0 ].col, colision[ 0 ].col );
		break;
  		case 2:
  			// printf( "caso 2\n" );
			// procesaColor( linea, colision[ 0 ].col, colision[ 1 ].col );
		break;
		case 3:
		// 5, 2, 1 <- Usar los dos primeros
		// 4, 3, 1 <- Usar los dos primeros
		// 3, 3, 2 <- Usar los dos primeros
			if( (colision[ 0 ].rep == 5) || (colision[ 1 ].rep == 3) ) {
				procesaColor( linea, colision[ 0 ].col, colision[ 1 ].col );
		// 6, 1, 1 <- El primero y la media de los otros dos.
		// 4, 2, 2 <- El primero y la media de los otros dos.
			} else { // ( colision[ 0 ].rep == 6 ) {
				Color mezcla;
				mezcla  = colision[ 1 ].col;
				mezcla += colision[ 2 ].col;
				mezcla.transformaPaleta(); Color::limpiaAcarreo();
				procesaColor( linea, colision[ 0 ].col, mezcla );
			}
		break;
		case 4:
		// 4, 2, 1, 1	<- Usar los dos primeros
		// 3, 3, 1, 1	<- Usar los dos primeros
			if( (colision[ 0 ].rep == 4) || (colision[ 1 ].rep == 3) ) {
				procesaColor( linea, colision[ 0 ].col, colision[ 1 ].col );
		// 5, 1, 1, 1	<- El primero y la media de los otros tres.
			} else if ( colision[ 0 ].rep == 5 ) {
				Color mezcla;
				mezcla  = colision[ 1 ].col;
				mezcla += colision[ 2 ].col;
				mezcla += colision[ 3 ].col;
				mezcla.transformaPaleta(); Color::limpiaAcarreo();
				procesaColor( linea, colision[ 0 ].col, mezcla );
		// 3, 2, 2, 1	<- El primero y la media de los otros dos.
			} else if ( colision[ 0 ].rep == 3 ) {
				Color mezcla;
				mezcla  = colision[ 1 ].col;
				mezcla += colision[ 2 ].col;
				mezcla.transformaPaleta(); Color::limpiaAcarreo();
				procesaColor( linea, colision[ 0 ].col, mezcla );
		// 2, 2, 2, 2	<- Tresbolillo
			} else {
  				Color mezcla1, mezcla2;
				mezcla1  = colision[ 0 ].col;
				mezcla1 += colision[ 2 ].col;
				mezcla1.transformaPaleta(); Color::limpiaAcarreo();
				mezcla2  = colision[ 1 ].col;
				mezcla2 += colision[ 3 ].col;
				mezcla2.transformaPaleta(); Color::limpiaAcarreo();
				procesaColor( linea, mezcla1, mezcla2 );
			}
		break;
		case 5:
		// 3, 2, 1, 1, 1	<- Usar los dos primeros
			if( colision[ 0 ].rep == 3 ) {
				procesaColor( linea, colision[ 0 ].col, colision[ 1 ].col );
		// 4, 1, 1, 1, 1	<- Usa el primero y la mezcal de los cuatro siguientes.
			} else if ( colision[ 0 ].rep == 4 ) {
				Color mezcla;
				mezcla  = colision[ 1 ].col;
				mezcla += colision[ 2 ].col;
				mezcla += colision[ 3 ].col;
				mezcla += colision[ 4 ].col;
				mezcla.transformaPaleta(); Color::limpiaAcarreo();
				procesaColor( linea, colision[ 0 ].col, mezcla );
		// 2, 2, 2, 1, 1	<- La Media de los tres primeros.
			} else {
				// Mezcla 0 y 2, y Mezcla 1, 3 y 4
  				Color mezcla1, mezcla2;
				mezcla1  = colision[ 0 ].col;
				mezcla1 += colision[ 2 ].col;
				mezcla1.transformaPaleta(); Color::limpiaAcarreo();
				mezcla2  = colision[ 1 ].col;
				mezcla2 += colision[ 3 ].col;
				mezcla2 += colision[ 4 ].col;
				mezcla2.transformaPaleta(); Color::limpiaAcarreo();
				procesaColor( linea, mezcla1, mezcla2 );
				// ??? Aleatorio 0 ~ 1 y 2 ~ 4
				// procesaColor( linea, colision[ Aleatorio( 0, 1 ) ].col , colision[ Aleatorio( 2, 4 ) ].col );
			}
		break;
		case 6:
		// 2, 2, 1, 1, 1, 1	<- Usar los dos primeros
			if( colision[ 0 ].rep == 2 ) {
				procesaColor( linea, colision[ 0 ].col, colision[ 1 ].col );
		// 3, 1, 1, 1, 1, 1	<- El primero la mezcla de los demás:
			} else {
				Color mezcla;
				mezcla  = colision[ 1 ].col;
				mezcla += colision[ 2 ].col;
				mezcla += colision[ 3 ].col;
				mezcla += colision[ 4 ].col;
				mezcla += colision[ 5 ].col;
				mezcla.transformaPaleta(); Color::limpiaAcarreo();
				procesaColor( linea, colision[ 0 ].col, mezcla );
			}
		break;
		case 7:
		{
		// 2, 1, 1, 1, 1, 1, 1 <- El primero y la mezcla de los demás:
			Color mezcla;
			mezcla  = colision[ 1 ].col;
			mezcla += colision[ 2 ].col;
			mezcla += colision[ 3 ].col;
			mezcla += colision[ 4 ].col;
			mezcla += colision[ 5 ].col;
  			mezcla += colision[ 6 ].col;
			mezcla.transformaPaleta(); Color::limpiaAcarreo();
			procesaColor( linea, colision[ 0 ].col, mezcla );
			// procesaColor( linea, colision[ 0 ].col, colision[ Aleatorio( 1, 6 ) ].col );
		}
		break;
		case 8:
		{
		// 1, 1, 1, 1, 1, 1, 1, 1	<- Tresbolillo
  			Color mezcla1, mezcla2;
			mezcla1  = colision[ 0 ].col;
			mezcla1 += colision[ 2 ].col;
			mezcla1 += colision[ 4 ].col;
			mezcla1 += colision[ 6 ].col;
			mezcla1.transformaPaleta(); Color::limpiaAcarreo();
			mezcla2  = colision[ 1 ].col;
			mezcla2 += colision[ 3 ].col;
			mezcla2 += colision[ 5 ].col;
			mezcla2 += colision[ 7 ].col;
			mezcla2.transformaPaleta(); Color::limpiaAcarreo();
			procesaColor( linea, mezcla1, mezcla2 );
			// procesaColor( linea, colision[ Aleatorio( 0, 3 ) ].col, colision[ Aleatorio( 4, 7 ) ].col );
		}
		break;
		default:
			printf( "Hey default?\n" );
		break;
	};
	colision.quita();
}
// ----------------------------------------------------------------------------
bool damePatronColores( Color linea[8], Uint8& chr, Uint8& clr ) {
	// Buscamos los dos colores:
	// c1 = Tinta
	// c2 = Fondo
	// ( Tinta 4bits | Fondo 4bits )
	Uint8 c2, c1 = linea[0].dameIndice();
	for( register int i = 1; i < 8; ++i ) {
		c2 = linea[i].dameIndice();
		if( c1 != c2 ) break;
	}
	// PONEMOS DE FONDO EL COLOR SELECCIONADO:
    if( color_fondo ) {
        // Si la tinta es el color de fondo la cambiamos:
        if( c1 == numColorFondo ) {
            Uint8 t = c1;
            c1 = c2;
            c2 = t;
        // Si el fondo no es el colo seleccionado:
        } else if ( c2 != numColorFondo ) {
            /*
            // Contamos la cantidad de colores repetidos:
            Uint8 n2, n1; n2 = n1 = 0;
            for( register int i = 1; i < 8; ++i ) {
                if( linea[i].dameIndice() == c2 ) ++n2;
                else ++n1;
            }
            // Ponemos de fondo el color más repetido:
            if( n1 > n2 ) {
                Uint8 t = c1;
                c1 = c2;
                c2 = t;
            // Si se repiten las mísmas veces:
            } else {
                // El fondo el índice más alto;
                if( c1 > c2 ) {
                    Uint8 t = c1;
                    c1 = c2;
                    c2 = t;
                }
            }
            */
            // Ponemos de fondo el color más bajo:
            if( c1 > c2 ) {
                Uint8 t = c1;
                c1 = c2;
                c2 = t;
            }
        }
    // PONEMOS DE FONDO EL COLOR MÁS REPETIDO:
    } else {
        /*
        // Contamos la cantidad de colores repetidos:
        Uint8 n2, n1; n2 = n1 = 0;
        for( register int i = 1; i < 8; ++i ) {
            if( linea[i].dameIndice() == c2 ) ++n2;
            else ++n1;
        }
        // Ponemos de fondo el color más repetido:
        if( n1 > n2 ) {
            Uint8 t = c1;
            c1 = c2;
            c2 = t;
        // Si se repiten las mísmas veces:
        } else {
            // El fondo el índice más alto:
            if( c1 > c2 ) {
                Uint8 t = c1;
                c1 = c2;
                c2 = t;
            }
        }
        */
        // Ponemos de fondo el color más bajo:
        if( c1 > c2 ) {
            Uint8 t = c1;
            c1 = c2;
            c2 = t;
        }
    }

	clr = 0;        // Color Tinta | Fondo
	chr = 0;		// Patrón de Colores
	// Si los dos colores son iguales el patrón es cero:
	if( c1 != c2 ) {
		for( register int i = 0; i < 8; ++i ) {
			// Si el color es tinta escribimos un 1:
			if( linea[i].dameIndice() == c1 ) {
				Uint8 tinta = 1;
				tinta <<= ( 7 - i );
				chr |= tinta;
			}
		}
  		// Color Tinta | Color Fondo
 		clr = c1;	// Color Tinta;
		clr <<= 4;
		clr |= c2;	// Color Fondo
	} else {
		clr = c2;	// Color Fondo
	}

	// Comprobación de errores de color:
	for( register int i = 0; i < 8; ++i ) {
		if( linea[i].dameIndice() == c1 ) continue;
		if( linea[i].dameIndice() == c2 ) continue;
		return false;
 	}
	return true;
}
// ----------------------------------------------------------------------------
bool buscaBaldosasParecidas( Lista<Baldosa>& baldosas, Uint8 bits );
bool optimizaBaldosas(     Lista<Baldosa>& baldosas, Lista<Uint8>& nombres );
// ----------------------------------------------------------------------------
bool optimizaBaldosasB111( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres );
bool optimizaBaldosasB121( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres );
bool optimizaBaldosasB113( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres );
bool optimizaBaldosasB123( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres );
// ----------------------------------------------------------------------------
void guardaPNG( Lista<Baldosa>& baldosas, const Cadena& salida, png_uint_32 w, png_uint_32 h, bool salida_alfa );
void guardaASM( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres, const Cadena& salida );
void guardaPLT( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres, const Cadena& salida, bool pletter_longitud );
void guardaSC2( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres, const Cadena& salida );
void guardaBASIC_ASCII( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres, const Cadena& salida, Uint8 bancos_vram );
// ----------------------------------------------------------------------------
void printf_line() { printf( "------------------------------------------------------------\n" ); }
// ----------------------------------------------------------------------------

#ifdef LANGUAGE_UTF8
#include "png2msx_utf8.h"
#else
#include "png2msx_iso.h"
#endif

#ifdef WINDOWS
#endif // WINDOWS

#ifdef LINUX
#endif // LINUX

#ifdef APPLE
//	#include "CoreFoundation/CoreFoundation.h"
#endif

int main( int argc, char *argv[] ) {

#ifdef APPLE
/*
{
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL( mainBundle );
   	char path[1024];
   	if( !CFURLGetFileSystemRepresentation( resourcesURL, TRUE, (UInt8 *)path, PATH_MAX ) ) return -1;
   	CFRelease( resourcesURL );
   	chdir( path );
}
*/
#endif

#ifdef SPANISH
// SPANISH:
#ifdef LANGUAGE_UTF8
    setlocale( LC_ALL, "es_ES.UTF-8" );
#else
    setlocale( LC_ALL, ".ACP" );
#endif
#else
// ENGLISH:
    setlocale( LC_ALL, "C" );
#endif

	Lista<Baldosa> baldosas;
	Lista<Uint8>   nombres;
	Cadena entrada, salida, cadena;
	Color color;
	Color linea[8];
	Baldosa tile;
	Formato_PNG png;
	bool umbral_gris = false;
	bool cero_por_uno = false;
	bool uno_por_cero = false;
//	bool color_fondo  = false;  // Global
	bool repetidas = false;
	bool optimiza = false;
	bool salida_png = false;
	bool salida_alfa = false;
	bool salida_bruto = false;
	bool salida_pletter = false;
	bool pletter_longitud = false;
	bool salida_sc2 = false;
	bool salida_basic_ascii = false;
	bool cambia_contraste = false;
	bool contraste_automatico = false;
	bool colision_de_color = false;
	png_uint_32 x = 0;
	png_uint_32 y = 0;
	png_uint_32 w = 0;
	png_uint_32 h = 0;
	int contraste_int = 0;
	int	num_texto = 0;
	Uint8 chr = 0;
	Uint8 clr = 0;
	Uint8 nam = 0;
	Uint8 nivelGris = 10;
//	Uint8 numColorFondo  = 0;   // Global
	Uint8 repetidas_bits = 0;
	Uint8 bancos_vram = 3;
	float contraste = 1.0f;
	float punto_medio = 127.5f;
	float error_fin_linea[8];

    color_fondo   = false;    // Global
    numColorFondo = 0;        // Global

	printf_line();
    printf( TXT_PNG2MSX_Conversor );
    printf( TXT_PNG2MSX_Version );
	printf_line();
	if( ( argc < 2 ) || ( argc > 32 ) ) {
		printf( TXT_PNG2MSX_Opciones );
		printf( TXT_PNG2MSX_Op01 );
		printf( TXT_PNG2MSX_Op02 );
        printf( TXT_PNG2MSX_Op03 );
		printf( TXT_PNG2MSX_Op04 );
		printf( TXT_PNG2MSX_Op05 );
		printf( TXT_PNG2MSX_Op06 );
		printf( TXT_PNG2MSX_Op07 );
		printf( TXT_PNG2MSX_Op08 );
		printf( TXT_PNG2MSX_Op09 );
		printf( TXT_PNG2MSX_Op10 );
		printf( TXT_PNG2MSX_Op11 );
		printf( TXT_PNG2MSX_Op12 );
        printf( TXT_PNG2MSX_Op13 );
		printf( TXT_PNG2MSX_Op14 );
		printf( TXT_PNG2MSX_Op15 );
		printf( TXT_PNG2MSX_Op16 );
		printf( TXT_PNG2MSX_Op17 );
		printf( TXT_PNG2MSX_Op18 );
		printf( TXT_PNG2MSX_Op19 );
		printf( TXT_PNG2MSX_Op20 );
		printf( TXT_PNG2MSX_Op21 );
		printf( TXT_PNG2MSX_Op22 );
        printf( TXT_PNG2MSX_Op23 );
		printf( TXT_PNG2MSX_Op24 );
		printf( TXT_PNG2MSX_Op25 );
		printf( TXT_PNG2MSX_Op26 );
		printf( TXT_PNG2MSX_Op27 );
		printf( TXT_PNG2MSX_Op28 );
		printf( TXT_PNG2MSX_Op29 );
		printf( TXT_PNG2MSX_Op30 );
		printf( TXT_PNG2MSX_Op31 );
		printf( TXT_PNG2MSX_Ejemplo1 );
		printf( TXT_PNG2MSX_Ejemplo2 );
		return 0;
	}

	printf( TXT_PNG2MSX_Obteniendo_datos );
	for( int i = 1; i < argc; ++i ) {
		if( argv[i][0] == '-' ) {		// A ver ese guión
			cadena = &argv[i][2];		// Cogemos despues del guión y la letra
			switch( argv[i][1] ) {		// A ver esa letrita.
				case 'x': case 'X': x = cadena.dameUInt(); break;
				case 'y': case 'Y': y = cadena.dameUInt(); break;
				case 'w': case 'W':
                    w = cadena.dameUInt();
                    if( w == 0 ) w = 256;
                break;
				case 'h': case 'H':
                    h = cadena.dameUInt();
                    if( h == 0 ) h = 192;
                break;
				case 'g': case 'G':
                    umbral_gris = true;
                    nivelGris = (Uint8)cadena.dameUInt();
                break;
				case 'c': case 'C': colision_de_color = true; break;
				case 'e': case 'E': Color::activaAcarreo(); break;
				case 'b': case 'B':
					if( cadena.dameLongitud() == 0 ) {
						contraste_automatico = true;
						cambia_contraste = true;
						break;
					}
					// Valores de 0.25f ~ 4.0f
					contraste_int = cadena.dameInt();
					if( contraste_int == 0 ) break;
					cambia_contraste = true;
					if( contraste_int >  255 ) contraste_int =  255;
					if( contraste_int < -255 ) contraste_int = -255;
					contraste = (float)contraste_int / 85.0f;
					if( contraste_int > 0 ) {
						contraste += 1.0f;
					} else {
						contraste += 4.0f;
						contraste *= 0.25f;
					}
				break;
				case 'z': case 'Z':
					cero_por_uno = true;
					uno_por_cero = false;
				break;
				case 'n': case 'N':
					uno_por_cero = true;
					cero_por_uno = false;
				break;
				case 'k': case 'K':
                    color_fondo = true;
                    numColorFondo = (Uint8)cadena.dameUInt();
				break;
				case 'f': case 'F':
					repetidas = true;
					repetidas_bits = (Uint8)cadena.dameUInt();
					if( repetidas_bits > 16 ) repetidas_bits = 16;
				break;
				case 'o': case 'O':
					optimiza = true;
					bancos_vram = (Uint8)cadena.dameUInt();
					if( bancos_vram > 4 ) bancos_vram = 3;
				break;
				case 'a': case 'A': salida_alfa = true;
				case 'p': case 'P': salida_png = true; break;
				case 'r': case 'R': salida_bruto = true; break;
				case 'l': case 'L': pletter_longitud = true;
				case 't': case 'T': salida_pletter = true; break;
				case 's': case 'S': salida_sc2 = true; break;
				case 'm': case 'M':
					salida_sc2 = true;
					salida_basic_ascii = true;
				break;
			}
		// Es una entrada de texto para archivo de entrada y salida:
		} else {
			const char* texto = &argv[i][0];
			size_t posicion = 0;
			// Ignoramos si hay más de dos entradas de texto:
			if( num_texto < 2 ) {
				// La primera entrada de texto es para la entrada:
				if( num_texto == 0 ) {
					entrada = texto;
					if( entrada.buscaFinal( '.', posicion ) ) {
						cadena.copia( entrada, posicion, entrada.dameLongitud() );
						if( !cadena.comparaN( ".png" ) ) {
							printf( TXT_PNG2MSX_entrada_dos_puntos, entrada.dameCadena() );
							printf( TXT_PNG2MSX_Solo_se_permite_PNG  );
							return -1;
						}
						entrada.borraDesde( posicion );
						salida = entrada;
					} else {
						printf( TXT_PNG2MSX_entrada_dos_puntos, entrada.dameCadena() );
						printf( TXT_PNG2MSX_Es_obligatorio_PNG );
						return -1;
					}
					++num_texto;
				// La segunda entrada de texto es para la salida:
				} else {
					salida = texto;
					++num_texto;
				}
			}
		}
	}

	Color::tomaUmbralGris( nivelGris );
	Color::limpiaAcarreo();

	cadena = entrada;
	cadena << ".png";
	if( !png.abre( cadena.dameCadena() ) ) {
		printf( TXT_PNG2MSX_Error_abriendo_archivo, cadena.dameCadena() );
		return -1;
	}
	if( w == 0 ) w = png.dameAnchoPixels();
	if( h == 0 ) h = png.dameAltoPixels();
	printf_line();
	printf( TXT_PNG2MSX_Origen_PNG );
	printf( TXT_PNG2MSX_entrada, entrada.dameCadena() );
	printf(	TXT_PNG2MSX_ancho, (unsigned int)png.dameAnchoPixels() );
	printf( TXT_PNG2MSX_alto,  (unsigned int)png.dameAltoPixels() );
	printf( TXT_PNG2MSX_bits,  png.dameBitsPorCanal() );
	printf( TXT_PNG2MSX_canales, png.dameCanales() );
	printf( TXT_PNG2MSX_Tipo_de_color );
	switch( png.dameTipoColor() ) {
		case PNG_COLOR_TYPE_GRAY:		printf( TXT_PNG2MSX_Escala_de_Grises ); break;
		case PNG_COLOR_TYPE_PALETTE:
			printf( TXT_PNG2MSX_Paleta );
			printf( TXT_PNG2MSX_Num_colores_paleta, png.dameColoresPaleta() );
			if( png.damePaletaAlfa() ) {
				printf( TXT_PNG2MSX_El_indice_0_paleta );
			}
		break;
		case PNG_COLOR_TYPE_RGB:		printf( "RGB\n" ); break;
		case PNG_COLOR_TYPE_RGB_ALPHA:	printf( "RGBA\n" ); break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:	printf( TXT_PNG2MSX_Escala_Grises_Alpha  ); break;
		default:
			printf( TXT_PNG2MSX_No_definido );
			return -1;
	}
	printf_line();

	if( x >= png.dameAnchoPixels() ) {
		printf( TXT_PNG2MSX_El_valor_x_sobrepasa );
		x = 0;
		printf( TXT_PNG2MSX_Se_usara_como_x, (unsigned int)x );
		printf_line();
	}

	if( y >= png.dameAltoPixels() ) {
		printf( TXT_PNG2MSX_El_valor_y_sobrepasa );
		y = 0;
		printf( TXT_PNG2MSX_Se_usara_como_y, (unsigned int)y );
		printf_line();
	}

	if( w + x > png.dameAnchoPixels() ) {
		printf( TXT_PNG2MSX_La_anchura_sobrepasa );
		w = png.dameAnchoPixels() - x;
		printf( TXT_PNG2MSX_Se_usara_como_ancho, (unsigned int)w );
		printf_line();
	}

	if( h + y > png.dameAltoPixels() ) {
		printf( TXT_PNG2MSX_La_altura_sobrepasa );
		h = png.dameAltoPixels() - y;
		printf( TXT_PNG2MSX_Se_usara_como_alto, (unsigned int)h );
		printf_line();
	}

	if( ( w % 8 ) || ( h % 8 ) ) {
		printf( TXT_PNG2MSX_Te_recuerdo_memoria );
	}

	if( w % 8 )  {
		w -= ( w % 8 );
		printf( TXT_PNG2MSX_El_ancho_multiplo_8 );
		printf( TXT_PNG2MSX_Se_usara_como_ancho, (unsigned int)w );
		printf_line();
	}

	if( h % 8 ) {
		h -= ( h % 8 );
		printf( TXT_PNG2MSX_La_altura_multiplo_8 );
		printf( TXT_PNG2MSX_Se_usara_como_alto, (unsigned int)h );
		printf_line();
	}

	if( ( w * h ) > 49152 ) {
		printf( TXT_PNG2MSX_Imagen_demasiado_grande );
		printf( TXT_PNG2MSX_Algunas_opciones_incomp );
		salida_bruto   = false;
		salida_pletter = false;
		salida_sc2     = false;
		salida_basic_ascii = false;
		bancos_vram = 4;
		printf_line();
	//	png.quita();
	//	return -1;
	}
	if( w != 256 ) {
		printf( TXT_PNG2MSX_Al_ser_el_ancho_diferente );
 		printf_line();
	}

	printf( TXT_PNG2MSX_Destino );
	printf( TXT_PNG2MSX_salida, salida.dameCadena() );
	printf(	TXT_PNG2MSX_x,      (unsigned int)x );
	printf( TXT_PNG2MSX_y,      (unsigned int)y );
	printf(	TXT_PNG2MSX_ancho,  (unsigned int)w );
	printf( TXT_PNG2MSX_alto,   (unsigned int)h );
	if( colision_de_color )
		printf( TXT_PNG2MSX_Filtro_colision_color_act );
	if( contraste_automatico )
		printf( TXT_PNG2MSX_Contraste_automatico );
	else if ( cambia_contraste )
		printf( TXT_PNG2MSX_Contraste_manual, contraste_int );
    if( umbral_gris )
		printf( TXT_PNG2MSX_Umbral_de_Gris, nivelGris );
	if( cero_por_uno )
		printf( TXT_PNG2MSX_Cambia_color_0_por_1 );
	if( uno_por_cero )
		printf( TXT_PNG2MSX_Cambia_color_1_por_0 );
    if( color_fondo )
        printf( TXT_PNG2MSX_Color_de_Fondo, numColorFondo );
	if( repetidas )
		printf( TXT_PNG2MSX_Baldosas_repetidas_bits, repetidas_bits );
	if( optimiza )
		printf( TXT_PNG2MSX_Optimizacion_activada_modo, bancos_vram );
	if( salida_alfa )
		printf( TXT_PNG2MSX_Salida_PNG_con_canal_alfa );
	else if( salida_png )
		printf( TXT_PNG2MSX_Salida_PNG_sin_canal_alfa );
	if( salida_bruto )
		printf( TXT_PNG2MSX_Salida_en_bruto_para_ASM );
	if( salida_pletter ) {
		if( pletter_longitud ) printf( TXT_PNG2MSX_Salida_Pletter_con_longitud );
		else printf( TXT_PNG2MSX_Salida_Pletter_sin_longitud );
	}
	if( salida_sc2 )
		printf( TXT_PNG2MSX_Salida_SC2_activada );
	if( salida_basic_ascii )
		printf( TXT_PNG2MSX_Salida_BASIC_ASCII_activada );
	printf_line();

	printf( TXT_PNG2MSX_Procesando_la_imagen );

	// Contraste automático:
	if( cambia_contraste ) {
		float lum = 0;
		float lum_max = 0;
		float lum_min = 0;
		int num_max = 0;
		int num_min = 0;
		punto_medio = 0.0f;
		for( png_uint_32 yo = y; yo < (h + y); ++yo ) {
			for( png_uint_32 xo = x; xo < (w + x); ++xo ) {
				png.dameColor( xo, yo, color );
				punto_medio += color.dameLuminancia();
			}
		}
		punto_medio /= (w*h);
		printf( TXT_PNG2MSX_punto_medio, punto_medio );
		for( png_uint_32 yo = y; yo < (h + y); ++yo ) {
			for( png_uint_32 xo = x; xo < (w + x); ++xo ) {
				png.dameColor( xo, yo, color );
				lum = color.dameLuminancia();
				if( lum > punto_medio ) { lum_max += lum; ++num_max; }
				else                    { lum_min += lum; ++num_min; }
			}
		}
		lum_max /= (float)num_max;
		lum_min /= (float)num_min;
		if( contraste_automatico ) {
			contraste = (lum_max - lum_min) / (lum_max + lum_min);
			contraste = (2.0f + contraste) / 2.0f;
			if( contraste >= 1.0 ) contraste_int = ( contraste - 1.0f ) * 85.0f;
 			else contraste_int = ( ( contraste / 0.25f ) - 4.0f ) * 85.0f;
			printf( TXT_PNG2MSX_contraste, contraste_int );
		}
	}
	printf_line();

// Algoritmo de detección de colisión de color:
	// Las columnas de 8 en 8:
	for( png_uint_32 yo = y, yd = 0; yo < (h + y); yo+=8, yd+=8 ) {
		// Borramos los errores de fin de linea;
		for( png_uint_32 i = 0; i < 8; ++i ) error_fin_linea[i] = 0.0f;
		// Las filas de 8 en 8:
		for( png_uint_32 xo = x, xd = 0; xo < (w + x); xo+=8, xd+=8 ) {
			// Las 8 columnas de una baldosa:
			for( png_uint_32 j = 0; j < 8; ++j ) {
				// Los 8 pixels de la fila de una baldosa:
				Color::tomaAcarreo( error_fin_linea[j] );
				for( png_uint_32 i = 0; i < 8; ++i ) {
					// Obtenemos el pixel de origen:
					png.dameColor( xo + i, yo + j, linea[i] );
					// Contrastre:
					if( cambia_contraste ) {
						if( linea[i].dameLuminancia() < punto_medio ) linea[i] *= ( 1.0f / contraste );
						else linea[i] *= contraste;
					}
					// Cambiamos el color a la paleta del MSX:
					linea[i].transformaPaleta();
					// Si queremos cambiar el color transparente por negro:
					if( cero_por_uno ) {
						if( linea[i].dameIndice() == 0 ) linea[i].inicia( NEGRO );
					}
					if( uno_por_cero ) {
						if( linea[i].dameIndice() == 1 ) linea[i].inicia( TRANSPARENTE );
					}
				}
				error_fin_linea[j] = Color::dameAcarreo();
				Color::limpiaAcarreo();
				// Procesamos la Colisión de Color de una fila de la baldosa:
				if( colision_de_color ) {
					procesaLinea( linea );
					Color::limpiaAcarreo();
				}
				// Procesar a chr y a clr:
				if( !damePatronColores( linea, chr, clr ) ) {
					printf( TXT_PNG2MSX_Error_en_patrones_y_colores, (unsigned int)xd, (unsigned int)(yd + j) );
				}
				// Escribir archivos chr y clr:
				tile.chr[ j ] = chr;
				tile.clr[ j ] = clr;
			}
			tile.rep = false;   // Si es una baldosa repetida o es la original
			tile.num = 0;       // Si está repetida el indice de la original
			baldosas.meteFinal( tile );
			nombres.meteFinal( nam++ );
		}
	}
	png.quita();

	// Buscamos baldosas repetidas:
	if( repetidas ) {
		printf( TXT_PNG2MSX_Buscando_Baldosas_parecidas );
		buscaBaldosasParecidas( baldosas, repetidas_bits );
		printf_line();
	}

	// Salida en PNG:
	if( salida_png ) {
		printf( TXT_PNG2MSX_Exportando_a_PNG );
		cadena = salida;
		if( salida == entrada ) cadena << "_16col";
		guardaPNG( baldosas, cadena, w, h, salida_alfa );
		printf_line();
	}

	// Procesamos repetición de baldosas...
	if( optimiza ) {
		bool retorno = false;
		switch( bancos_vram ) {
			case 0: retorno = optimizaBaldosasB111( baldosas, nombres ); break;
			case 1: retorno = optimizaBaldosasB121( baldosas, nombres ); break;
			case 2: retorno = optimizaBaldosasB113( baldosas, nombres ); break;
  			case 3: retorno = optimizaBaldosasB123( baldosas, nombres ); break;
  			case 4: retorno = optimizaBaldosas(     baldosas, nombres ); break;
		}
		if( !retorno ) bancos_vram = 3;
		printf_line();
 	// Guardamos el PNG con la baldosas:
 		if( retorno && salida_png ) {
 			if( bancos_vram == 4 ) {
 				w = 256; h = 192;
			}
 			printf( TXT_PNG2MSX_Exportando_Baldosas_a_PNG );
			cadena = salida;
			if( salida == entrada ) cadena << "_16col";
			cadena << "_bal";
			guardaPNG( baldosas, cadena, w, h, salida_alfa );
			printf_line();
		}
	}

	// Salida en Bruto para Ensamblador:
	if( salida_bruto ) {
		printf( TXT_PNG2MSX_Exportando_CHR_CLR_y_NAM );
		guardaASM( baldosas, nombres, salida );
		printf_line();
	}

	// Compresión PLETTER:
	if( salida_pletter ) {
		printf( TXT_PNG2MSX_Comprimiendo_con_Pletter );
		guardaPLT( baldosas, nombres, salida, pletter_longitud );
		printf_line();
	}

	// Salida SC2:
	if( salida_sc2 ) {
		printf( TXT_PNG2MSX_Exportando_a_SC2_BLOAD );
		guardaSC2( baldosas, nombres, salida );
		printf_line();
	}

	// Salida BASIC ASCII para cargar salida SC2:
	if( salida_basic_ascii ) {
		printf( TXT_PNG2MSX_Exportando_a_BASIC_ASCII );
		guardaBASIC_ASCII( baldosas, nombres, salida, bancos_vram );
		printf_line();
	}

	baldosas.quita();
	nombres.quita();
	return 0;
}
// ----------------------------------------------------------------------------
bool buscaBaldosasParecidas( Lista<Baldosa>& baldosas, Uint8 bits ) {
	if( baldosas.dameLongitud() == 0  ) return false;
	if( bits == 0  ) bits = 3;
	if( bits >  16 ) bits = 16;
	Uint16 baldosas_parecidas = 0;
	Lista_Iterador<Baldosa> it_bal1, it_bal2;
	it_bal1.alInicio( baldosas );
	it_bal2.alInicio( baldosas );
	for( it_bal1.alInicio(); it_bal1.noPenultimo(); ++it_bal1 ) {
		if( it_bal1.ver().rep ) continue;
		it_bal2 = it_bal1;
		for( ++it_bal2; it_bal2.noFinal(); ++it_bal2 ) {
			if( it_bal2.ver().rep ) continue;
			// Si son iguales continuamos;
			bool continuar = true;
			for( register Uint8 i = 0; i < 8; ++i ) {
				if( it_bal1.ver().chr[i] != it_bal2.ver().chr[i] ) { continuar = false; break; }
				if( it_bal1.ver().clr[i] != it_bal2.ver().clr[i] ) { continuar = false; break; }
			}
		// Solo un color de tinta puede ser diferente:
			Uint8 colores_diferentes = 0;
			for( register Uint8 i = 0; i < 8; ++i ) {
				if( it_bal1.ver().clr[i] != it_bal2.ver().clr[i] ) {
				// El fondo siempre debe ser igual:
					if( ( it_bal1.ver().clr[i] & 0x0F ) !=
						( it_bal2.ver().clr[i] & 0x0F )
					) { colores_diferentes = 64; break; }
					++colores_diferentes;
				}
			}
			if( colores_diferentes > 2 ) continue;
		// Contamos los bits que son diferentes:
			Uint8 bits_diferentes = 0;
			for( register Uint8 i = 0; i < 8; ++i ) {
				Uint8 bits1 = it_bal1.ver().chr[i];
				Uint8 bits2 = it_bal2.ver().chr[i];
				Uint8 bit = 1;
				Uint8 diferencia = 0;
				for( register Uint8 j = 0; j < 8; ++j ) {
					if( (bits1 & bit) != (bits2 & bit) ) {
						++diferencia;
						// Tiene que haber menos de un bit diferentes:
						if( diferencia > 2 ) { diferencia = 64; break; }
					}
					bit = bit << 1;
				}
				bits_diferentes += diferencia;
				if( bits_diferentes > bits ) { continuar = true; break; }
			}
			if( continuar ) continue;
   		// Si hemos llegado hasta aquí es que son parecidas y las mezclamos:
			for( register Uint8 i = 0; i < 8; ++i ) {
				it_bal2->chr[i] = it_bal1.ver().chr[i] & it_bal2.ver().chr[i];
			//	it_bal2->chr[i] = it_bal1.ver().chr[i];
			//	it_bal1->chr[i] = it_bal2.ver().chr[i];
				it_bal2->rep = true;
			}
			++baldosas_parecidas;
		}
  	}
	for( it_bal1.alInicio(); it_bal1.noPenultimo(); ++it_bal1 ) {
		it_bal1->rep = false;
	}
	printf( TXT_PNG2MSX_Baldosas_parecidas, baldosas_parecidas );
	return ( baldosas_parecidas ) ? true : false;
}
// ----------------------------------------------------------------------------
// Sin bancos ni restricciones, ideal para mapas:
bool optimizaBaldosas( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres ) {
	printf( TXT_PNG2MSX_Quitando_baldosas_repetidas );
	if( baldosas.dameLongitud() == 0  ) return false;
	Uint16 baldosas_repetidas = 0;
	Lista_Iterador<Baldosa> it_bal1, it_bal2;
	Lista_Iterador<Uint8> it_nom;
	Lista<Baldosa> bal( baldosas ); // Copiar las baldosas:
	it_bal1.alInicio( bal );
	it_bal2.alInicio( bal );
	Uint8 rep = 0;
	for( it_bal1.alInicio(); it_bal1.noPenultimo(); ++it_bal1 ) {
		// Si está repetida pasamos:
		if( it_bal1.ver().rep ) continue;
		// Si no está repetida, es padre y le asignamos un número:
		else it_bal1->num = rep++;
		it_bal2 = it_bal1;
		for( ++it_bal2; it_bal2.noFinal(); ++it_bal2 ) {
		// Si la baldosa hija está repetida:
			if( it_bal2.ver().rep ) continue;
			bool iguales = true;
			for( register Uint8 i = 0; i < 8; ++i ) {
				if( it_bal1.ver().chr[i] != it_bal2.ver().chr[i] ) iguales = false;
				if( it_bal1.ver().clr[i] != it_bal2.ver().clr[i] ) iguales = false;
			}
   		// Si son iguales:
			if( iguales ) {
				it_bal2->num = it_bal1.ver().num;
				it_bal2->rep = true;
				++baldosas_repetidas;
			}
		}
  	}
	size_t longitud = (bal.dameLongitud() - baldosas_repetidas );
	printf( TXT_PNG2MSX_baldosas_totales,  (unsigned)bal.dameLongitud() );
	printf( TXT_PNG2MSX_baldosas_repetidas, baldosas_repetidas );
	printf( TXT_PNG2MSX_baldosas_restantes,(unsigned)longitud );
	if( baldosas_repetidas == 0 ) {
		printf( TXT_PNG2MSX_No_hay_baldosas_repetidas );
		bal.quita();
		return false;
	}
	baldosas.quita();
	nombres.quita();
// Guardamos los nuevos datos:
	for( it_bal1.alInicio(); it_bal1.noFinal(); ++it_bal1 ) {
		nombres.meteFinal( it_bal1.ver().num );
		if( !it_bal1.ver().rep ) {
			baldosas.meteFinal( it_bal1.ver() );
		}
	}
	bal.quita();
	printf( TXT_PNG2MSX_Quitadas_baldosas_repetidas );
	return true;
}
// ----------------------------------------------------------------------------
// MODO DE UN SOLO BANCO DE MEMORIA Y 256 BALDOSAS:
// 10 SCREEN 2
// 20 COLOR 15,1,1
// 30 VDP(4) = &H00			-> &H0000   &H0000  &H0000  // PATRONES
// 40 VDP(3) = &H9F    		-> &H2000   &H2000  &H2000  // COLOR
// 50 BLOAD"ARCH.SC2",S		-> &H1800 - &H18FF -> 256	// TABLA DE NOMBRES 1
// 60 A$=INKEY$				-> &H1900 - &H19FF -> 256	// TABLA DE NOMBRES 2
// 70 IF A$<>" " GOTO 60	-> &H1A00 - &H1AFF -> 256	// TABLA DE NOMBRES 3
bool optimizaBaldosasB111( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres ) {
	printf( TXT_PNG2MSX_Intentando_optimizar_111 );
	if( baldosas.dameLongitud() == 0  ) return false;
	else if( baldosas.dameLongitud() > 768 ) {
		printf( TXT_PNG2MSX_Demasiadas_baldosas );
		return false;
	}
	Uint16 baldosas_repetidas = 0;
	Lista_Iterador<Baldosa> it_bal1, it_bal2;
	Lista_Iterador<Uint8> it_nom;
	Lista<Baldosa> bal( baldosas ); // Copiar las baldosas:
	it_bal1.alInicio( bal );
	it_bal2.alInicio( bal );
	Uint8 rep = 0;
	for( it_bal1.alInicio(); it_bal1.noPenultimo(); ++it_bal1 ) {
		// Si está repetida pasamos:
		if( it_bal1.ver().rep ) continue;
		// Si no está repetida, es padre y le asignamos un número:
		else it_bal1->num = rep++;
		it_bal2 = it_bal1;
		for( ++it_bal2; it_bal2.noFinal(); ++it_bal2 ) {
		// Si la baldosa hija está repetida:
			if( it_bal2.ver().rep ) continue;
			bool iguales = true;
			for( register Uint8 i = 0; i < 8; ++i ) {
				if( it_bal1.ver().chr[i] != it_bal2.ver().chr[i] ) iguales = false;
				if( it_bal1.ver().clr[i] != it_bal2.ver().clr[i] ) iguales = false;
			}
   		// Si son iguales:
			if( iguales ) {
				it_bal2->num = it_bal1.ver().num;
				it_bal2->rep = true;
				++baldosas_repetidas;
			}
		}
  	}
	size_t longitud = (bal.dameLongitud() - baldosas_repetidas );
	printf( TXT_PNG2MSX_baldosas_totales,   (unsigned)bal.dameLongitud() );
	printf( TXT_PNG2MSX_baldosas_repetidas,  baldosas_repetidas );
	printf( TXT_PNG2MSX_baldosas_restantes, (unsigned)longitud );
	if( longitud > 256 ) {
		printf( TXT_PNG2MSX_No_se_puede_reducir_un_bloq, (unsigned)(longitud-256) );
		bal.quita();
		return false;
	}
	baldosas.quita();
	nombres.quita();
// Guardamos los nuevos datos:
	for( it_bal1.alInicio(); it_bal1.noFinal(); ++it_bal1 ) {
		nombres.meteFinal( it_bal1.ver().num );
		if( !it_bal1.ver().rep ) {
			baldosas.meteFinal( it_bal1.ver() );
		}
	}
	bal.quita();
	// La tabla de nombres siempre debe estar completa 768 bytes:
	for( size_t j = nombres.dameLongitud(); j < 768; ++j ) nombres.meteFinal( 0xFF );
	printf( TXT_PNG2MSX_Optimizado_a_un_bloque );
	return true;
}
// ----------------------------------------------------------------------------
// DOS BANCOS DE VRAM Y 512 BALDOSAS:
// 10 COLOR 15,1,1
// 20 SCREEN 2				-> USAR ESTE MODO POR SER MÁS COMPACTO
// 30 VDP(3) = &HBF			-> &H2000   &H2800  &H2000  // COLOR
// 40 VDP(4) = &H01			-> &H0000   &H0800  &H0000  // PATRONES
// 50 BLOAD"ARCH.SC2",S		-> &H1800 - &H18FF -> 256	// TABLA DE NOMBRES 1
// 60 A$=INKEY$				-> &H1900 - &H19FF -> 256	// TABLA DE NOMBRES 2
// 70 IF A$="" GOTO 60		-> &H1A00 - &H1AFF -> 256	// TABLA DE NOMBRES 3
bool optimizaBaldosasB121( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres ) {
	printf( TXT_PNG2MSX_Intentando_optimizar_121 );
	if( baldosas.dameLongitud() == 0  ) return false;
	else if( baldosas.dameLongitud() > 768 ) {
		printf( TXT_PNG2MSX_Demasiadas_baldosas );
		return false;
	}
	const Uint8 num_bancos = 2;
	Uint16 baldosas_repetidas[num_bancos];
	Lista_Iterador<Baldosa> it_bal1, it_bal2;
	Lista_Iterador<Uint8> it_nom;
	Lista<Baldosa> bal[num_bancos];	// 0 = 512 Baldosas tercio superior e inferior.
									// 1 = 256 Baldosas del tercio central.
	// Copiar baldosas a los bancos:
	size_t longitud = baldosas.dameLongitud();
	it_bal1.alInicio( baldosas );
	// Meter las primeras 256 baldosas en el banco 0;
	for( register Uint16 i = 0; ( i < 256 ) && ( longitud > 0 ); ++i, --longitud ) {
		bal[0].meteFinal( it_bal1.ver() ); ++it_bal1;
	}
	// Meter las 256 baldosas siguietnes enel banco 1;
	for( register Uint16 i = 0; ( i < 256 ) && ( longitud > 0 ); ++i, --longitud ) {
		bal[1].meteFinal( it_bal1.ver() ); ++it_bal1;
	}
	// Meter las últimas baldosas en el banco 0;}
	for( register Uint16 i = 0; ( i < 256 ) && ( longitud > 0 ); ++i, --longitud ) {
		bal[0].meteFinal( it_bal1.ver() ); ++it_bal1;
	}
// EMPEZAMOS;
	for( register Uint8 banco = 0; banco < num_bancos; ++banco ) {
		it_bal1.alInicio( bal[banco] );
		it_bal2.alInicio( bal[banco] );
		baldosas_repetidas[banco] = 0;
		Uint8 rep = 0;
		for( it_bal1.alInicio(); it_bal1.noPenultimo(); ++it_bal1 ) {
			// Si está repetida pasamos:
			if( it_bal1.ver().rep ) continue;
			// Si no está repetida, es padre y le asignamos un número:
			else it_bal1->num = rep++;
			it_bal2 = it_bal1;
			for( ++it_bal2; it_bal2.noFinal(); ++it_bal2 ) {
			// Si la baldosa hija está repetida:
				if( it_bal2.ver().rep ) continue;
				bool iguales = true;
				for( register Uint8 i = 0; i < 8; ++i ) {
					if( it_bal1.ver().chr[i] != it_bal2.ver().chr[i] ) iguales = false;
					if( it_bal1.ver().clr[i] != it_bal2.ver().clr[i] ) iguales = false;
				}
   			// Si son iguales:
				if( iguales ) {
					it_bal2->num = it_bal1.ver().num;
					it_bal2->rep = true;
					++baldosas_repetidas[banco];
				}
			}
		}
	}
// FIN
	for( register Uint8 banco = 0; banco < num_bancos; ++banco ) {
		printf( TXT_PNG2MSX_baldosas_totales_u_u,   banco+1, (unsigned)bal[banco].dameLongitud() );
		printf( TXT_PNG2MSX_baldosas_repetidas_u_u, banco+1, baldosas_repetidas[banco] );
		baldosas_repetidas[banco] = bal[banco].dameLongitud() - baldosas_repetidas[banco];
		printf( TXT_PNG2MSX_baldosas_restantes_u_u, banco+1, baldosas_repetidas[banco] );
	}
	if( ( baldosas_repetidas[0] > 256 ) || ( baldosas_repetidas[1] > 256 ) ) {
		printf( TXT_PNG2MSX_Demasiadas_baldosas_2_banc );
		bal[0].quita(); bal[1].quita();
		return false;
	}
	baldosas.quita();
	nombres.quita();
// Guardamos los nuevos datos:
	Baldosa baldosa_vacia;
	memset( (void *)&baldosa_vacia, 0, sizeof( Baldosa ) );
	Uint8 nom[3][256];
	memset( (void *)nom, 0xFF, 3 * 256 * sizeof( Uint8 )  );
	// Primer banco que es el superior y el inferior:
	Uint8 banco = 0;
	it_bal1.alInicio( bal[0] );
	for( register Uint16 i = 0; it_bal1.noFinal(); ++i, ++it_bal1 ) {
		// Necesitamos saltar del banco 0 al 2:
		if( i > 255 ) { i = 0; banco = 2; }
		nom[banco][i] = it_bal1.ver().num;
		if( !it_bal1.ver().rep ) baldosas.meteFinal( it_bal1.ver() );
	}
	bal[0].quita();
	// Metemos baldosas vacias para alinear los datos:
	for( register size_t i = baldosas.dameLongitud(); i < 256; ++i ) baldosas.meteFinal( baldosa_vacia );
	// Segundo banco que es el central:
	it_bal1.alInicio( bal[1] );
	for( register Uint16 i = 0; it_bal1.noFinal(); ++i, ++it_bal1 ) {
		nom[1][i] = it_bal1.ver().num;
		if( !it_bal1.ver().rep ) baldosas.meteFinal( it_bal1.ver() );
	}
	bal[1].quita();
	// Metemos baldosas vacias para alinear los datos: (No es necesario )
	// for( size_t i = baldosas.dameLongitud(); i < 512; ++i ) baldosas.meteFinal( baldosa_vacia );
	// Guardamos los bancos:
	for( banco = 0; banco < 3; ++banco ) {
		for( register Uint16 i = 0; i < 256; ++i ) nombres.meteFinal( nom[banco][i] );
	}
	printf( TXT_PNG2MSX_Optimizado_a_dos_bloques );
	return true;
}
// ----------------------------------------------------------------------------
// DOS BANCOS DE VRAM Y 512 BALDOSAS:
// 10 COLOR 15,1,1
// 20 SCREEN 2				-> SALTO EN MEMORIA:
// 30 VDP(3) = &HDF    		-> &H2000   &H2000  &H3000  // COLOR
// 40 VDP(4) = &H02 		-> &H0000   &H0000  &H1000  // PATRONES
// 50 BLOAD"ARCH.SC2",S		-> &H1800 - &H18FF -> 256	// TABLA DE NOMBRES 1
// 60 A$=INKEY$				-> &H1900 - &H19FF -> 256	// TABLA DE NOMBRES 2
// 70 IF A$="" GOTO 60		-> &H1A00 - &H1AFF -> 256	// TABLA DE NOMBRES 3
bool optimizaBaldosasB113( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres ) {
	printf( TXT_PNG2MSX_Intentando_optimizar_113 );
	if( baldosas.dameLongitud() == 0  ) return false;
	else if( baldosas.dameLongitud() > 768 ) {
		printf( TXT_PNG2MSX_Demasiadas_baldosas );
		return false;
	}
	const Uint8 num_bancos = 2;
	Uint16 baldosas_repetidas[num_bancos];
	Lista_Iterador<Baldosa> it_bal1, it_bal2;
	Lista_Iterador<Uint8> it_nom;
	Lista<Baldosa> bal[num_bancos];	// 0 = 512 Baldosas dos tercios superiores.
									// 1 = 256 Baldosas del tercio inferior.
	// Copiar baldosas a los bancos:
	size_t longitud = baldosas.dameLongitud();
	it_bal1.alInicio( baldosas );
	// Meter las primeras 512 baldosas en el banco 0;
	for( register Uint16 i = 0; ( i < 512 ) && ( longitud > 0 ); ++i, --longitud ) {
		bal[0].meteFinal( it_bal1.ver() ); ++it_bal1;
	}
	// Meter las 256 baldosas siguientes en el banco 1;
	for( register Uint16 i = 0; ( i < 256 ) && ( longitud > 0 ); ++i, --longitud ) {
		bal[1].meteFinal( it_bal1.ver() ); ++it_bal1;
	}
// EMPEZAMOS;
	for( register Uint8 banco = 0; banco < num_bancos; ++banco ) {
		it_bal1.alInicio( bal[banco] );
		it_bal2.alInicio( bal[banco] );
		baldosas_repetidas[banco] = 0;
		Uint8 rep = 0;
		for( it_bal1.alInicio(); it_bal1.noPenultimo(); ++it_bal1 ) {
			// Si está repetida pasamos:
			if( it_bal1.ver().rep ) continue;
			// Si no está repetida, es padre y le asignamos un número:
			else it_bal1->num = rep++;
			it_bal2 = it_bal1;
			for( ++it_bal2; it_bal2.noFinal(); ++it_bal2 ) {
			// Si la baldosa hija está repetida:
				if( it_bal2.ver().rep ) continue;
				bool iguales = true;
				for( register Uint8 i = 0; i < 8; ++i ) {
					if( it_bal1.ver().chr[i] != it_bal2.ver().chr[i] ) iguales = false;
					if( it_bal1.ver().clr[i] != it_bal2.ver().clr[i] ) iguales = false;
				}
   			// Si son iguales:
				if( iguales ) {
					it_bal2->num = it_bal1.ver().num;
					it_bal2->rep = true;
					++baldosas_repetidas[banco];
				}
			}
		}
	}
// FIN
	for( register Uint8 banco = 0; banco < num_bancos; ++banco ) {
		printf( TXT_PNG2MSX_baldosas_totales_u_u, banco+1, (unsigned)bal[banco].dameLongitud() );
		printf( TXT_PNG2MSX_baldosas_repetidas_u_u, banco+1, baldosas_repetidas[banco] );
		baldosas_repetidas[banco] = bal[banco].dameLongitud() - baldosas_repetidas[banco];
		printf( TXT_PNG2MSX_baldosas_restantes_u_u, banco+1, baldosas_repetidas[banco] );
	}
	if( ( baldosas_repetidas[0] > 256 ) || ( baldosas_repetidas[1] > 256 ) ) {
		printf( TXT_PNG2MSX_Demasiadas_baldosas_2_banc );
		bal[0].quita(); bal[1].quita();
		return false;
	}
	baldosas.quita();
	nombres.quita();
// Guardamos los nuevos datos:
	Baldosa baldosa_vacia;
	memset( (void *)&baldosa_vacia, 0, sizeof( Baldosa ) );
	// Primer banco:
	Uint16 i = 0;
	for( it_bal1.alInicio( bal[0] ); it_bal1.noFinal(); ++it_bal1 ) {
		nombres.meteFinal( it_bal1.ver().num );
		if( !it_bal1.ver().rep ) {
			baldosas.meteFinal( it_bal1.ver() );
			++i;
		}
	}
	bal[0].quita();
	for( ; i < 256; ++i ) baldosas.meteFinal( baldosa_vacia );
	for( size_t j = nombres.dameLongitud(); j < 512; ++j ) nombres.meteFinal( 0xFF );
	// Segundo banco:
	i = 0;
	for( ; i < 256; ++i ) baldosas.meteFinal( baldosa_vacia );
	// Tercer banco:
	i = 0;
	for( it_bal1.alInicio( bal[1] ); it_bal1.noFinal(); ++it_bal1 ) {
		nombres.meteFinal( it_bal1.ver().num );
		if( !it_bal1.ver().rep ) {
			baldosas.meteFinal( it_bal1.ver() );
			++i;
		}
	}
	bal[1].quita();
	// for( ; i < 256; ++i ) baldosas.meteFinal( baldosa_vacia );
	for( size_t j = nombres.dameLongitud(); j < 768; ++j ) nombres.meteFinal( 0xFF );
	printf( TXT_PNG2MSX_Optimizado_a_dos_bloques );
	return true;
}
// ----------------------------------------------------------------------------
// ESTE ES EL MODO POR DEFECTO AL ARRANCAR BASIC, TRES BANCOS VRAM Y 768 BALDOSAS:
// 10 COLOR 15,0,0
// 20 SCREEN 2
// 30 VDP(3) = &HFF			-> &H2000   &H2800  &H3000  // COLOR
// 40 VDP(4) = &H03			-> &H0000   &H0800  &H1000  // PATRONES
// 50 BLOAD"ARCH.SC2",S		-> &H1800 - &H18FF -> 256	// TABLA DE NOMBRES 1
// 60 A$=INKEY$				-> &H1900 - &H19FF -> 256	// TABLA DE NOMBRES 2
// 70 IF A$="" GOTO 60		-> &H1A00 - &H1AFF -> 256	// TABLA DE NOMBRES 3
bool optimizaBaldosasB123( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres ) {
	printf( TXT_PNG2MSX_Intentando_optimizar_123 );
	if( baldosas.dameLongitud() == 0  ) return false;
	else if( baldosas.dameLongitud() > 768 ) {
		printf( TXT_PNG2MSX_Demasiadas_baldosas );
		return false;
	}
	const Uint8 num_bancos = 3;
	Uint16 baldosas_repetidas[num_bancos];
	size_t longitud;
	Lista_Iterador<Baldosa> it_bal1, it_bal2;
	Lista_Iterador<Uint8> it_nom;
	Lista<Baldosa> bal[num_bancos];
	longitud = baldosas.dameLongitud();
	it_bal1.alInicio( baldosas );
	for( register Uint8 banco = 0; banco < num_bancos; ++banco ) {
		for( register Uint16 i = 0; ( i < 256 ) && ( longitud > 0 ); ++i, --longitud ) {
			bal[banco].meteFinal( it_bal1.ver() ); ++it_bal1;
		}
	}
// EMPEZAMOS;
	for( register Uint8 banco = 0; banco < num_bancos; ++banco ) {
		it_bal1.alInicio( bal[banco] );
		it_bal2.alInicio( bal[banco] );
        baldosas_repetidas[banco] = 0;
		Uint8 rep = 0;
		for( it_bal1.alInicio(); it_bal1.noPenultimo(); ++it_bal1 ) {
			// Si está repetida pasamos:
			if( it_bal1.ver().rep ) continue;
			// Si no está repetida, es padre y le asignamos un número:
			else it_bal1->num = rep++;
			it_bal2 = it_bal1;
			for( ++it_bal2; it_bal2.noFinal(); ++it_bal2 ) {
			// Si la baldosa hija está repetida:
				if( it_bal2.ver().rep ) continue;
				bool iguales = true;
				for( register Uint8 i = 0; i < 8; ++i ) {
					if( it_bal1.ver().chr[i] != it_bal2.ver().chr[i] ) iguales = false;
					if( it_bal1.ver().clr[i] != it_bal2.ver().clr[i] ) iguales = false;
				}
   			// Si son iguales:
				if( iguales ) {
					it_bal2->num = it_bal1.ver().num;
					it_bal2->rep = true;
					++baldosas_repetidas[banco];
				}
			}
		}
	}
// FIN
	longitud = 0;
	for( register Uint8 banco = 0; banco < num_bancos; ++banco ) {
		printf( TXT_PNG2MSX_baldosas_totales_u_u, banco+1, (unsigned)bal[banco].dameLongitud() );
		printf( TXT_PNG2MSX_baldosas_repetidas_u_u, banco+1, baldosas_repetidas[banco] );
		baldosas_repetidas[banco] = bal[banco].dameLongitud() - baldosas_repetidas[banco];
		printf( TXT_PNG2MSX_baldosas_restantes_u_u, banco+1, baldosas_repetidas[banco] );
		longitud += baldosas_repetidas[banco];
	}
	if( longitud > 767 ) {
		printf( TXT_PNG2MSX_No_merece_la_pena );
		bal[0].quita(); bal[1].quita(); bal[2].quita();
		return false;
	}
	baldosas.quita();
	nombres.quita();
// Guardamos los nuevos datos:
	Baldosa baldosa_vacia;
	memset( (void *)&baldosa_vacia, 0, sizeof( Baldosa ) );
	for( register Uint8 banco = 0; banco < num_bancos; ++banco ) {
		Uint16 i = 0;
		for( it_bal1.alInicio( bal[banco] ); it_bal1.noFinal(); ++it_bal1 ) {
			nombres.meteFinal( it_bal1.ver().num );
			if( !it_bal1.ver().rep ) {
				baldosas.meteFinal( it_bal1.ver() );
				++i;
			}
		}
		bal[banco].quita();
		// El banco 0 y 1 deben tener baldosas vacias para alinear datos:
		if( banco < 2 ) {
			for( ; i < 256; ++i ) baldosas.meteFinal( baldosa_vacia );
		}
		// La tabla de nombres siempre debe estar completa 768 bytes:
		for( ; i < 256; ++i ) nombres.meteFinal( 0xFF );
	}
	printf( TXT_PNG2MSX_Optimizado_a_tres_bloques );
	return true;
}
// ----------------------------------------------------------------------------
void guardaPNG( Lista<Baldosa>& baldosas, const Cadena& salida, png_uint_32 w, png_uint_32 h, bool salida_alfa ) {
	if( baldosas.dameLongitud() == 0 ) return;
	if( salida.dameLongitud()   == 0 ) return;
	if( ( w % 8 ) || ( h % 8 ) ) return;
	if( ( w * h / 64 ) < baldosas.dameLongitud() ) return;
	Formato_PNG msx;
	if( !msx.iniciaPNG(
			w, h,
			4, 1, // 1 | 3
			PNG_COLOR_TYPE_PALETTE // PNG_COLOR_TYPE_RGB | PNG_COLOR_TYPE_PALETTE
		)
	) {
		printf( TXT_PNG2MSX_Error_iniciando_PNG );
		return;
	}
	if( !msx.iniciaPixels() ) {
		printf( TXT_PNG2MSX_Error_iniciando_Pixels );
		return;
	}
	if( !msx.iniciaPaleta( 16, salida_alfa ) ) {
		printf( TXT_PNG2MSX_Error_iniciando_Paleta );
		return;
	}
	Color color;
	for( int c = 0; c < 16; ++c ) {
		color.inicia( (COLOR)c );
		msx.tomaPaleta( color );
	}
	Uint16 cont = 0;
	png_uint_32 xd = 0;
	png_uint_32 yd = 0;
	Lista_Iterador<Baldosa> it_baldosa( baldosas );
	for( it_baldosa.alInicio(); it_baldosa.noFinal(); ++it_baldosa ) {
	// Las 8 columnas de una baldosa:
		for( png_uint_32 j = 0; j < 8; ++j ) {
			Uint8 fondo = it_baldosa.ver().clr[j];
			Uint8 tinta = ( fondo >> 4 );
			fondo &= 0x0F;
			Uint8 chr   = it_baldosa.ver().chr[j];
		// Escribimos los colores:
			for( png_uint_32 i = 0; i < 8; ++i ) {
				// Escribir el PNG de salida:
				if( chr & 0x80 ) color.tomaIndice( tinta );
				else			 color.tomaIndice( fondo );
				chr <<= 1;
				msx.tomaColor( xd + i, yd + j, color );
			}
		}
		xd+=8;
		if( xd >= w ) {
			xd=0;
			yd+=8;
			if( yd >= h ) break;
		}
		++cont;
	}
	color.tomaIndice( 0 );
	for( ; yd < h; yd+=8 ) {
		for( ; xd < w; ++xd ) {
			for( png_uint_32 j = 0; j < 8; ++j )
				msx.tomaColor( xd, yd + j, color );
		}
		xd = 0;
	}
	Cadena cadena = salida;
	cadena << ".png";
	msx.escribePNG( cadena.dameCadena(), 9 );
	msx.quita();
	cadena.quita();
 	return;
}
// ----------------------------------------------------------------------------
void guardaASM( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres, const Cadena& salida ) {
	if( salida.dameLongitud() == 0 ) return;
	// Guardamos las Baldosas = CHR + CLR
	if( baldosas.dameLongitud() == 0 ) return;
	else if( baldosas.dameLongitud() > 768 ) {
		printf( TXT_PNG2MSX_Demasiadas_baldosas );
		return;
	}
	Cadena cadena = salida; cadena << ".chr";
	Archivo archivo_chr( cadena.dameCadena(), "wb" );
	cadena = salida; cadena << ".clr";
	Archivo archivo_clr( cadena.dameCadena(), "wb" );
	Lista_Iterador<Baldosa> it_baldosa( baldosas );
	for( it_baldosa.alInicio(); it_baldosa.noFinal(); ++it_baldosa ) {
		for( register Uint8 i = 0; i < 8; ++i ) {
			archivo_chr.escribeU8( it_baldosa.ver().chr[i] );
			archivo_clr.escribeU8( it_baldosa.ver().clr[i] );
		}
	}
	archivo_chr.quita();
	archivo_clr.quita();
	// Guardamos la tabla de Nombres:
	if( nombres.dameLongitud() == 0 ) return;
	else if( nombres.dameLongitud() > 768 ) {
		printf( TXT_PNG2MSX_Demasiados_Nombres );
		return;
	}
	cadena = salida; cadena << ".nam";
	Archivo archivo_nam( cadena.dameCadena(), "wb" );
	Lista_Iterador<Uint8> it_nombre( nombres );
	for( it_nombre.alInicio(); it_nombre.noFinal(); ++it_nombre ) {
		archivo_nam.escribeU8( it_nombre.ver() );
	}
	archivo_nam.quita();
	return;
}
// ----------------------------------------------------------------------------
void guardaPLT( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres, const Cadena& salida, bool pletter_longitud ) {
	if( salida.dameLongitud()   == 0 ) return;
	if( baldosas.dameLongitud() == 0 ) return;
	else if( baldosas.dameLongitud() > 768 ) {
		printf( TXT_PNG2MSX_Demasiado_grande_VRAM_MSX1 );
		return;
	}
	// Usando la lista de baldosas...
	// Tabla de Patrones:
	Lista_Iterador<Baldosa> it_baldosa( baldosas );
	size_t longitud = baldosas.dameLongitud() * 8;
	size_t j = 0;
	unsigned char* array = new unsigned char[ longitud + 1 ];
	for( it_baldosa.alInicio(); it_baldosa.noFinal(); ++it_baldosa ) {
		for( register Uint8 i = 0; i < 8; ++i ) {
			array[j++] = it_baldosa.ver().chr[i];
		}
	}
	Cadena cadena = salida; cadena << ".chr";
	pletter( array, longitud, cadena, pletter_longitud );
	// La Tabla de Colores:
	j = 0;
	array = new unsigned char[ longitud + 1 ];
	for( it_baldosa.alInicio(); it_baldosa.noFinal(); ++it_baldosa ) {
		for( register Uint8 i = 0; i < 8; ++i ) {
			array[j++] = it_baldosa.ver().clr[i];
		}
	}
	cadena = salida; cadena << ".clr";
	pletter( array, longitud, cadena, pletter_longitud );
	// Guardamos la tabla de Nombres:
	if( nombres.dameLongitud() == 0 ) return;
	else if( nombres.dameLongitud() > 768 ) {
		printf( TXT_PNG2MSX_Demasiados_Nombres );
		return;
	}
	Lista_Iterador<Uint8> it_nombre( nombres );
 	longitud = nombres.dameLongitud();
 	j = 0;
	array = new unsigned char[ longitud + 1 ];
	for( it_nombre.alInicio(); it_nombre.noFinal(); ++it_nombre ) {
		array[j++] = it_nombre.ver();
   	}
	cadena = salida; cadena << ".nam";
	pletter( array, longitud, cadena, pletter_longitud );
	return;
}
// ----------------------------------------------------------------------------
void guardaSC2( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres, const Cadena& salida ) {
	if( baldosas.dameLongitud() == 0 ) return;
	if( salida.dameLongitud()   == 0 ) return;
	if( ( baldosas.dameLongitud() > 768 ) || ( nombres.dameLongitud() > 768 ) ) {
		printf( TXT_PNG2MSX_Demasiado_grande_VRAM_MSX1 );
		return;
	}
	Cadena sc2  = salida;
	Cadena ruta = salida;
	size_t posicion;
	if( ruta.buscaFinal( '/',  posicion ) ) ruta.borraDesde( posicion + 1 );
	if( ruta.buscaFinal( '\\', posicion ) ) ruta.borraDesde( posicion + 1 );
	if( sc2.buscaFinal( '/',  posicion ) ) sc2.borraHasta( posicion + 1 );
	if( sc2.buscaFinal( '\\', posicion ) ) sc2.borraHasta( posicion + 1 );
	sc2.mayusculas();
	if( sc2.dameLongitud() > 6 ) sc2.borraDesde(6);
	else while( sc2.dameLongitud() < 6 ) sc2 << "0";
	ruta << sc2 << ".SC2";
	printf( "sc2 ->%s<-\n", ruta.dameCadena() );
	// La cabecera 7 bytes:
	Archivo archivo_sc2( ruta.dameCadena(), "wb" );
	archivo_sc2.escribeU8( 0xFE );
	archivo_sc2.escribeLE16( 0x0000 );
	archivo_sc2.escribeLE16( 0x37FF );
	archivo_sc2.escribeLE16( 0x0000 );
	// La Tabla de Patrones 6144 bytes de la chr:
	Lista_Iterador<Baldosa> it_baldosa( baldosas );
	for( it_baldosa.alInicio(); it_baldosa.noFinal(); ++it_baldosa ) {
		for( register Uint8 i = 0; i < 8; ++i ) {
			archivo_sc2.escribeU8( it_baldosa.ver().chr[i] );
		}
	}
	size_t longitud = baldosas.dameLongitud();
	if( longitud < 768 ) {
		for( size_t i = longitud; i < 768; i++ ) {
			archivo_sc2.escribeLE64( 0 );
		}
	}
	// La Tabla de Nombres 768 bytes:
	Lista_Iterador<Uint8> it_nombre( nombres );
	for( it_nombre.alInicio(); it_nombre.noFinal(); ++it_nombre ) {
		archivo_sc2.escribeU8( it_nombre.ver() );
	}
	longitud = nombres.dameLongitud();
	if( longitud < 768 ) {
		for( size_t i = longitud; i < 768; i++ ) {
			archivo_sc2.escribeU8( 0 );
		}
	}
	// Los Atributos de Sprite: 32 sprites x 4 bytes = 128 bytes
	// 128 bytes de { 0xD1 0x00 0x00 0xF0 } a { 0xD1 0x00 0x1F 0xF0 }
	for( register Uint8 i = 0; i < 32; i++ ) {
		archivo_sc2.escribeBE16( 0XD100 );
		archivo_sc2.escribeU8( i );
		archivo_sc2.escribeU8( 0xF0 );
	}
	// 1152 bytes a cero 1152 / 8 = 144;
	for( register Uint8 i = 0; i < 144; i++ ) archivo_sc2.escribeLE64( 0 );
	// La Tabla de Colores 6144 bytes de la clr:
	for( it_baldosa.alInicio(); it_baldosa.noFinal(); ++it_baldosa ) {
		for( register Uint8 i = 0; i < 8; ++i ) {
			archivo_sc2.escribeU8( it_baldosa.ver().clr[i] );
		}
	}
	longitud = baldosas.dameLongitud();
	if( longitud < 768 ) {
		for( size_t i = longitud; i < 768; i++ ) {
			archivo_sc2.escribeLE64( 0 );
		}
	}
	archivo_sc2.quita();
	return;
}
// ----------------------------------------------------------------------------
void guardaBASIC_ASCII( Lista<Baldosa>& baldosas, Lista<Uint8>& nombres, const Cadena& salida, Uint8 bancos_vram ) {
	if( bancos_vram > 3 ) return;
	if( baldosas.dameLongitud() == 0 ) return;
	if( salida.dameLongitud()   == 0 ) return;
	if( ( baldosas.dameLongitud() > 768 ) || ( nombres.dameLongitud() > 768 ) ) {
		printf( TXT_PNG2MSX_Demasiado_grande_VRAM_MSX1 );
		return;
	}
	Cadena cadena = salida;
	Cadena ruta   = salida;
	size_t posicion;
	if( ruta.buscaFinal(   '/',  posicion ) ) ruta.borraDesde(   posicion + 1 );
	if( ruta.buscaFinal(   '\\', posicion ) ) ruta.borraDesde(   posicion + 1 );
	if( cadena.buscaFinal( '/',  posicion ) ) cadena.borraHasta( posicion + 1 );
	if( cadena.buscaFinal( '\\', posicion ) ) cadena.borraHasta( posicion + 1 );
	cadena.mayusculas();
	if( cadena.dameLongitud() > 6 ) cadena.borraDesde(6);
	else while( cadena.dameLongitud() < 6 ) cadena << "0";
	ruta << cadena << ".ASC";
	printf( "sc2 ->%s<-\n", ruta.dameCadena() );
	Archivo archivo( ruta.dameCadena(), "wb" );
	ruta = cadena << ".SC2";
	cadena = "1 COLOR 15,0,0:SCREEN 2";
	archivo.escribe( cadena ); archivo.escribeBE16( 0x0D0A );
	switch( bancos_vram ) {
 		case 0: cadena = "2 VDP(3)=&H9F:VDP(4)=&H0"; break;
		case 1: cadena = "2 VDP(3)=&HBF:VDP(4)=&H1"; break;
		case 2: cadena = "2 VDP(3)=&HDF:VDP(4)=&H2"; break;
 		case 3: cadena = "2 VDP(3)=&HFF:VDP(4)=&H3"; break;
	}
	archivo.escribe( cadena ); archivo.escribeBE16( 0x0D0A );
	cadena = "3 BLOAD\""; cadena << ruta; cadena << "\",S";
	archivo.escribe( cadena ); archivo.escribeBE16( 0x0D0A );
	cadena = "4 A$=INKEY$:IF A$=\"\" GOTO 4";
	archivo.escribe( cadena ); archivo.escribeBE16( 0x0D0A );
	archivo.escribeU8( 0xA1 );
	archivo.quita();
	return;
}
// ----------------------------------------------------------------------------
//                              F I N
// ----------------------------------------------------------------------------

// ---------------------------------------------------------
// SCREEN 2 / GRAPHIC 2
//
// 0000H - 07FFH	-->	Pattern generator table 1 -> 256 * 8 = 2048
// 0800H - 0FFFH	-->	Pattern generator table 2 -> 256 * 8 = 2048
// 1000H - 17FFH	-->	Pattern generator table 3 -> 256 * 8 = 2048
// de 0000H a 17FFH = 1800H -> 6144 bytes

// 1800H - 18FFH	-->	Pattern name table 1 -> 256
// 1900H - 19FFH	-->	Pattern name table 2 -> 256
// 1A00H - 1AFFH	-->	Pattern name table 3 -> 256
// de 1800H a 1AFFH = 300H -> 768 bytes

// 1B00H - 1B7FH	-->	Sprite attribute table
// de 1B00H a 1B7FH = 80H -> 128 bytes

// 1B80H - 1BAFH	-->	Palette table
// 1BB0H - 1FFFH	--> ???
// de 1B80H a 1FFFH = 480H -> 1152 bytes a cero

// 2000H - 27FFH	-->	Colour table 1 -> 256 * 8 = 2048
// 2800H - 2FFFH	-->	Colour table 2 -> 256 * 8 = 2048
// 3000H - 37FFH	-->	Colour table 3 -> 256 * 8 = 2048
// de 2000H a 37FFH = 1800H -> 6144

// 3800H - 3FFFH	-->	Sprite generator table
// ---------------------------------------------------------

// FORMATO SC2
// -------------
// byte	FE						0xFE
// word	Dirección inicial		0x00 0x00
// word	Direccion final			0xFF 0x37
// word	Direccion de arranque	0x00 0x00
//
// La dirección de arranque sólo se usa con BLOAD"CAS:",R
// Para copiar a la VRAM usar BLOAD"CAS:",S
//
// 10 SCREEN 2
// 20 BLOAD"NOMBRE.SC2",S
// 30 GOTO 30
//
// La Cabecera:
// 7 bytes { 0xFE, 0x00, 0x00, 0xFF, 0x37, 0x00, 0x00 }
//
// La Tabla de Patrones:
// 		2048 bytes
//		2048 bytes
//		2048 bytes
// 6144 bytes de la chr
//
// La Tabla de Nombres:
//		256 bytes de 0x00 a 0xFF
//		256 bytes de 0x00 a 0xFF
//		256 bytes de 0x00 a 0xFF
// 768 bytes de nombres

// Los Atributos de Sprite: 32 sprites x 4 bytes = 128 bytes
// 128 bytes de { 0xD1 0x00 0x00 0xF0 } a { 0xD1 0x00 0x1F 0xF0 }
//
// 1152 bytes a cero
//
// La Tabla de Colores
// 		2048 bytes
// 		2048 bytes
// 		2048 bytes
// 6144 bytes de la clr
//
// Total = 7 + 6144 + 768 + 128 + 1152 + 6144 = 14343 bytes

