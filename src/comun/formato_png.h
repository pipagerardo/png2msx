#ifndef _FORMATO_PNG_
#define _FORMATO_PNG_

#include "funciones.h"
#include "endian.h"
#include "color.h"
#include "png.h"

class Formato_PNG {
public:
	Formato_PNG();
	~Formato_PNG();
	void inicia();
	void quita();

	// --------------------------------------------------------
	// Funciones para abrir y leer un:
	// --------------------------------------------------------
	// Abre un archivo PNG existente:
	bool abre(   const char* archivo_png );
	bool guarda( const char* archivo_png, int nivel_compresion = 7 ); // 0 - 9
	// Retornos de Información:
	png_uint_32	dameAnchoPixels()   const;	// El ancho de la imagen en pixels.
	png_uint_32	dameAltoPixels()    const;	// El alto  de la imagen en pixels.
	png_byte    dameBitsPorCanal()  const;	// Bits necesarios para almacenar un canal.
	png_byte    dameCanales()       const;	// Número de canales de color.
	int 		dameColoresPaleta() const;	// Número de colores de la paleta.
	bool		damePaletaAlfa()	const;	// Si el índice 0 es transparente.
	png_byte	dameTipoColor()		const;	// Los siguientes son válidos:
		// PNG_COLOR_TYPE_PALETTE		// Paleta de color de 4 o 8 bits 3 canales.
		// PNG_COLOR_TYPE_GRAY_ALPHA	// Color RA de 8 bits y 2 canales
		// PNG_COLOR_TYPE_RGB			// Color RGB de 8 bits y 3 canales
		// PNG_COLOR_TYPE_RGB_ALPHA		// Color RGBA de 8 bits y 4 canales
	bool damePaleta( Color& color );	// Es necesario establecer un índice en el Color:
	bool dameColor( png_uint_32 x, png_uint_32 y, Color& color );
	// --------------------------------------------------------

	// --------------------------------------------------------
	// Funciones para crear y escribir en un PNG:
	// --------------------------------------------------------
	bool iniciaPNG(
		png_uint_32	ancho,
		png_uint_32	alto,
		png_byte	bits,
		png_byte	canales,
		png_byte	tipo_color
 	);
	bool iniciaPaleta( int num_colores, bool alfa = false );
	bool tomaPaleta( const Color& color ); // Es necesario establecer un índice en el Color:
	bool iniciaPixels();
	bool tomaColor( png_uint_32 x, png_uint_32 y, Color& color );
	bool escribePNG( const char* archivo_png, int nivel_compresion );
	// --------------------------------------------------------

// -------------------------------------
// Pasos para crear un nuevo Archivo PNG:
// -------------------------------------
//
// Iniciar un archivo PNG:
//		bool iniciaPNG(
//			png_uint_32	ancho,
//			png_uint_32	alto,
//			png_byte	bits,
//			png_byte	canales,
//			png_byte	tipo_color
//		);
//
//  Si tiene Paleta:
//		Iniciar la paleta:
//			bool iniciaPaleta( int num_colores );
//  	Asignarle los colores a la paleta:
//			bool tomaPaleta( const Color& color );
//
//	Iniciar los pixels:
//		bool iniciaPixels();
//
//  Escribir los pixels:
//		void tomaColor( png_uint_32 x, png_uint_32 y, Color& color );
//
//  Escribe el archivo lo cierra y limpia todo.
//		bool escribePNG(
//			const char*	archivo_png,
//			int			nivel_compresion
//		);
//
// -------------------------------------


protected:
	void quitaPaleta();
	void quitaPixels();

	FILE*		m_fp;
	bool        m_read;
	png_structp	m_png_ptr;
	png_infop	m_info_ptr;

	png_uint_32	m_ancho;				// El ancho de la imagen en pixels.
	png_uint_32	m_alto;					// El alto  de la imagen en pixels.

	// PNG_COLOR_MASK_PALETTE		1
	// PNG_COLOR_MASK_COLOR			2
	// PNG_COLOR_MASK_ALPHA			4
	// PNG_COLOR_TYPE_GRAY			0
	// PNG_COLOR_TYPE_PALETTE		(PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_PALETTE)
	// PNG_COLOR_TYPE_RGB			(PNG_COLOR_MASK_COLOR)
	// PNG_COLOR_TYPE_RGB_ALPHA		(PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_ALPHA)
	// PNG_COLOR_TYPE_GRAY_ALPHA	(PNG_COLOR_MASK_ALPHA)
	png_byte	m_tipo_color;			// Gris, Paleta, RGB, RGBA, Gris+Alpha

	png_byte	m_profundidad_bit;		// bits necesarios para almacenar un pixel.
	png_byte 	m_canales;				// Número de canales de color.
	// Tamaño de pixel en bits = m_profundidad_bit * m_canales;

	// PNG_INTERLACE_NONE        0 // Non-interlaced image
	// PNG_INTERLACE_ADAM7       1 // Adam7 interlacing
	// PNG_INTERLACE_LAST        2 // Not a valid value
	int			m_interlazado;          // Tipo de interlazado.
	int			m_numero_de_pasadas;	// > 1 interlazado

	// Los pixels en bruto y en filas:
	png_bytep*	m_filas_pixels;		// Puntero a las filas de pixels, tantas como m_alto.
	png_size_t	m_fila_bytes;		// El ancho en bytes de una fila de pixels.
									// ( ( m_ancho * m_profundidad_bit ) >> 3 ) + alineación_en_bytes

	// La paleta de Colores:
	// 		int i = [ 0 ~ m_paleta_colores ]
	//		m_paleta[i].red		= [ 0 ~ 255 ]
	//		m_paleta[i].green	= [ 0 ~ 255 ]
	// 		m_paleta[i].blue	= [ 0 ~ 255 ]
	png_colorp 	m_paleta;			// Puntero a la paleta de colores.
	int 		m_paleta_colores;	// Número de colores de la paleta.
	bool		m_paleta_alfa;
};

/*
bool PNG_es_PNG( SDL_RWops *src );
SDL_Surface *PNG_Carga_RW( SDL_RWops *src );

bool PNG_Guarda_PNG( SDL_Surface *surface, const char *file );
*/

#endif

