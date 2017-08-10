#ifndef _COLOR_
#define _COLOR_

#include "funciones.h"

enum COLOR {
	TRANSPARENTE = 0,	// 0
	NEGRO,				// 1
	VERDE,				// 2
	VERDE_CLARO,		// 3
	AZUL_OSCURO,		// 4
	AZUL_CLARO,			// 5
	ROJO_OSCURO,		// 6
	CIAN,				// 7 Mezcla de Verde y Azul.
	ROJO,				// 8
	ROJO_CLARO,			// 9
	AMARILLO_OSCURO,	// 10
	AMARILLO_CLARO,		// 11
	VERDE_OSCURO,		// 12
	MAGENTA,			// 13 Mezcla de Azul y Rojo.
	GRIS,				// 14
	BLANCO				// 15
};

class Color {
public:

	// Construcción y destrucción:
	Color(); 	// 0, 0, 0, 255
	Color( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa = 255 );
	Color( COLOR color );
	Color( const Color& color );
	Color( const char*  html  );
	~Color();
	
	void  inicia( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa = 255 );
	void  inicia( COLOR color );
	void  inicia( const Color& color );
	void  inicia( const char*  html  ); // "#aabbcc" "#aabbccdd"
	void  quita();

	Color* operator &();
	Color& operator = ( const Color& color );
	Color& operator = ( const COLOR  color );

	Color& operator += ( const Color& color );
	Color& operator *= ( float num );
	
	// Funciones de trabajo:
	void  tomaIndice( Uint8 indice );
	void  tomaRojo(   Uint8 rojo   );
	void  tomaVerde(  Uint8 verde  );
	void  tomaAzul(   Uint8 azul   );
	void  tomaAlfa(   Uint8 alfa   );

	// Retornos:
	Uint8 dameIndice() const;
	Uint8 dameRojo()   const;
	Uint8 dameVerde()  const;
	Uint8 dameAzul()   const;
	Uint8 dameAlfa()   const;

	// Tomando como origen las componentes RGBA, retorna el 
	// índice de color más parecido al de la Paleta:
	COLOR damePaleta(); 
	
	// Tansforma el color RGBA al más parecido de la Paleta:
	void  transformaPaleta();

	float dameLuminancia();
	float dameTono();
	
	// Retornos estáticos retorna los colores de la paleta:
	static Uint8 dameRojo(  COLOR color );
	static Uint8 dameVerde( COLOR color );	
	static Uint8 dameAzul(  COLOR color );
	static Uint8 dameAlfa(  COLOR color );
	
	// Umbral de gris utilizado para transformar la paleta MSX
	static void  tomaUmbralGris( Uint8 gris );
	static Uint8 dameUmbralGris();
	// Acarreo de error de trasnformación de color
	static void  activaAcarreo();
	static void	 desactivaAcarreo();
	static void  limpiaAcarreo();
	static float dameAcarreo();
	static void  tomaAcarreo( float Y );
	
private:
	Uint8			m_indice;
	Uint8			m_rgba[4];
	static Uint8	m_paleta[16][4];	// Paleta MSX
	static Uint8	m_gris;				// Umbral para el gris (20)
	static bool		m_acarreo;
	static float 	m_y;
	COLOR escalaGris( float Y );
	COLOR escalaVerde( float Y );
	COLOR escalaCiano( float Y );
	COLOR escalaAzul( float Y );
	COLOR escalaMagenta( float Y );
	COLOR escalaRojo( float Y );
	COLOR escalaAmarillo( float Y );
};

#endif


