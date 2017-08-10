#include "color.h"
#include "matematicas.h"
// #include <math.h>

Uint8 Color::m_paleta[16][4] = {
		{   0,   0,   0,   0 }, //  0 Transparente
		{   0,   0,   0, 255 }, //  1 Negro
		{  33, 200,  66, 255 }, //  2 Verde Medio
		{  94, 220, 120, 255 }, //  3 Verde Claro
		{  84,  85, 237, 255 }, //  4 Azul Oscuro
		{ 125, 118, 252, 255 }, //  5 Azul Claro
		{ 212,  82,  77, 255 }, //  6 Rojo Oscuro
		{  66, 235, 245, 255 }, //  7 Cianógeno
		{ 252,  85,  84, 255 }, //  8 Rojo Medio
		{ 255, 121, 120, 255 }, //  9 Rojo Claro
		{ 212, 193,  84, 255 }, // 10 Amarillo Oscuro
		{ 230, 206, 128, 255 }, // 11 Amarillo Claro
		{  33, 176,  59, 255 }, // 12 Verde Oscuro
		{ 201,  91, 186, 255 }, // 13 Magenta
		{ 204, 204, 204, 255 }, // 14 Gris
		{ 255, 255, 255, 255 }  // 15 Blanco
};
Uint8 Color::m_gris = 20;
float Color::m_y = 0;
bool  Color::m_acarreo = false;

Color::Color() { quita(); }
Color::Color( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa ) {
	inicia( rojo, verde, azul, alfa );
}
Color::Color( COLOR        color ) { inicia( color ); }
Color::Color( const Color& color ) { inicia( color ); }
Color::Color( const char*  html  ) { inicia( html  ); }
Color::~Color() { quita(); }

void Color::inicia( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa ) {
	m_indice  = 0;
	m_rgba[0] = rojo;
	m_rgba[1] = verde;
	m_rgba[2] = azul;
	m_rgba[3] = alfa;
}
void Color::inicia( COLOR color ) {
	m_indice  = (Uint8)color;
	m_rgba[0] = m_paleta[color][0];
	m_rgba[1] = m_paleta[color][1];
	m_rgba[2] = m_paleta[color][2];
	m_rgba[3] = m_paleta[color][3];
}
void Color::inicia( const Color& color ) {
	m_indice  = color.m_indice;
	m_rgba[0] = color.m_rgba[0];
	m_rgba[1] = color.m_rgba[1];
	m_rgba[2] = color.m_rgba[2];
	m_rgba[3] = color.m_rgba[3];
}

void Color::inicia( const char*  html  ) {
	m_indice  = 0;
	size_t longitud = strlen( html );
	size_t inicio   = 0;
	char *p; char str[3] = { '\0' , '\0' , '\0' };
	if( html[0] == '#' ) ++inicio;
	if(( longitud - inicio ) != 6 ) return;
	for( size_t i = 0; i < 3; ++i ) {
		str[0] = html[ inicio++ ];
		str[1] = html[ inicio++ ];
		m_rgba[i] = (Uint8)strtoul( str, &p, 16 );
	}
	m_rgba[3] = 255;
}

void Color::quita() {
	m_indice  = 0;
	m_rgba[0] = 0;
	m_rgba[1] = 0;
	m_rgba[2] = 0;
	m_rgba[3] = 255;
	m_y = 0.0f;
}

Color* Color::operator &() {
	return this;
}

Color& Color::operator = ( const Color& color ) {
	m_indice  = color.m_indice;
	m_rgba[0] = color.m_rgba[0];
	m_rgba[1] = color.m_rgba[1];
	m_rgba[2] = color.m_rgba[2];
	m_rgba[3] = color.m_rgba[3];
	return *this;
}

Color& Color::operator = ( const COLOR color ) {
	m_indice  = (Uint8)color;
	m_rgba[0] = m_paleta[color][0];
	m_rgba[1] = m_paleta[color][1];
	m_rgba[2] = m_paleta[color][2];
	m_rgba[3] = m_paleta[color][3];
	return *this;
}

Color& Color::operator += ( const Color& color ) {
	int m;
	
	m = (int)m_rgba[0] + (int)color.m_rgba[0];
	m >>= 1;
	m_rgba[0] = (Uint8)m;

	m = (int)m_rgba[1] + (int)color.m_rgba[1];
	m >>= 1;
	m_rgba[1] = (Uint8)m;

	m = (int)m_rgba[2] + (int)color.m_rgba[2];
	m >>= 1;
	m_rgba[2] = (Uint8)m;

	m = (int)m_rgba[3] + (int)color.m_rgba[3];
	m >>= 1;
	m_rgba[3] = (Uint8)m;

	return *this;
}
	
Color& Color::operator *= ( float num ) {
	float r;
	r = m_rgba[0] * num;
	if      ( r > 255.0f ) m_rgba[0] = 255;
	else if ( r <   0.0f ) m_rgba[0] = 0;
	else				   m_rgba[0] = (Uint8)r;
	r = m_rgba[1] * num;
	if      ( r > 255.0f ) m_rgba[1] = 255;
	else if ( r <   0.0f ) m_rgba[1] = 0;
	else				   m_rgba[1] = (Uint8)r;
	r = m_rgba[2] * num;
	if      ( r > 255.0f ) m_rgba[2] = 255;
	else if ( r <   0.0f ) m_rgba[2] = 0;
	else				   m_rgba[2] = (Uint8)r;
	return *this;
}
	
void Color::tomaIndice( Uint8 indice ) { m_indice  = indice; }
void Color::tomaRojo(   Uint8 rojo   ) { m_rgba[0] = rojo;   }
void Color::tomaVerde(  Uint8 verde  ) { m_rgba[1] = verde;  }
void Color::tomaAzul(   Uint8 azul   ) { m_rgba[2] = azul;   }
void Color::tomaAlfa(   Uint8 alfa   ) { m_rgba[3] = alfa;   }

Uint8 Color::dameIndice() const { return m_indice;  }
Uint8 Color::dameRojo()   const { return m_rgba[0]; }
Uint8 Color::dameVerde()  const { return m_rgba[1]; }
Uint8 Color::dameAzul()   const { return m_rgba[2]; }
Uint8 Color::dameAlfa()   const { return m_rgba[3]; }

Uint8 Color::dameRojo(  COLOR color ) { return m_paleta[color][0]; }
Uint8 Color::dameVerde( COLOR color ) { return m_paleta[color][1]; }
Uint8 Color::dameAzul(  COLOR color ) { return m_paleta[color][2]; }
Uint8 Color::dameAlfa(  COLOR color ) { return m_paleta[color][3]; }

// Umbral de gris utilizado para transformar la paleta MSX
void  Color::tomaUmbralGris( Uint8 gris ) { m_gris = gris; } 
Uint8 Color::dameUmbralGris() { return m_gris; }
// Acarreo de errores de conversión de color
void  Color::activaAcarreo() { m_acarreo = true; }
void  Color::desactivaAcarreo() { m_acarreo = false; }
void  Color::limpiaAcarreo() { m_y = 0.0f; }
float Color::dameAcarreo() { return m_y; }
void  Color::tomaAcarreo( float Y ) { m_y = Y;}

/*
COLOR Color::escalaGris( float Y ) {
	if( Y <  60.71f ) { m_y = Y;			return NEGRO; }			// 1    50.71f  +10
	if( Y < 113.41f ) { m_y = Y - 101.42f;	return AZUL_OSCURO; }	// 4    143.41f -30
	if( Y < 164.7f  ) { m_y = Y - 185.40f;	return CIAN; }			// 7	194.7f	-30
	if( Y < 219.5f  ) { m_y = Y - 204.00f;	return GRIS; }			// 14   229.5f  -10
	m_y = Y - 255.00f; return BLANCO; 								// 15
}
COLOR Color::escalaVerde( float Y ) {
	if( Y < 60.115f  ) { m_y = Y;			return NEGRO; }			// 1    60.115f
	if( Y < 127.695f ) { m_y = Y - 120.23f;	return VERDE_OSCURO; }	// 12   127.695f
	if( Y < 153.18f  ) { m_y = Y - 135.16f;	return VERDE; }			// 2    153.18f
	if( Y < 213.1f   ) { m_y = Y - 171.20f;	return VERDE_CLARO; }	// 3    213.1f
	m_y = Y - 255.00f; return BLANCO; 								// 15
}
COLOR Color::escalaCiano( float Y ) {
	if( Y <  64.42f ) { m_y = Y;			return NEGRO; }			// 1    64.42f
	if( Y < 160.12f ) { m_y = Y - 134.84f;	return AZUL_CLARO; }	// 4    160.12f
	if( Y < 194.7f  ) { m_y = Y - 185.40f;	return CIAN; }			// 7    194.7f
	if( Y < 229.5f  ) { m_y = Y - 204.00f;	return GRIS; }			// 14   229.5f
	m_y = Y - 255.00f; return BLANCO; 								// 15
}
COLOR Color::escalaAzul( float Y ) {
	if( Y <  40.71f ) { m_y = Y;			return NEGRO; }			// 1	50.71f -10
	if( Y < 118.13f ) { m_y = Y - 101.42f;	return AZUL_OSCURO;	}	// 4    118.13f
	if( Y < 160.12f ) { m_y = Y - 134.84f;	return AZUL_CLARO; }	// 5    160.12f
	if( Y < 220.2f  ) { m_y = Y - 185.40f;	return CIAN; }			// 7    220.2f
	m_y = Y - 255.00f; return BLANCO; 								// 15
}
COLOR Color::escalaMagenta( float Y ) {
	if( Y <  60.225f ) { m_y = Y;			return NEGRO; } 		// 1	60.225f
	if( Y < 107.45f  ) { m_y = Y - 120.45f;	return ROJO_OSCURO;	}	// 6    127.45f
	if( Y < 167.77f  ) { m_y = Y - 134.45f;	return MAGENTA;	}		// 13   147.77f	134
	if( Y < 208.045f ) { m_y = Y - 161.09f;	return ROJO_CLARO; }	// 9    208.045f
	m_y = Y - 255.00f; return BLANCO; 								// 15
}
COLOR Color::escalaRojo( float Y ) {
	if( Y <  60.225f ) { m_y = Y;			return NEGRO; } 		// 1    60.225f
	if( Y < 127.72f  ) { m_y = Y - 120.45f;	return ROJO_OSCURO;	} 	// 6    127.72f
	if( Y < 148.04f  ) { m_y = Y - 134.99f;	return ROJO; }			// 8    148.04f
	if( Y < 208.045f ) { m_y = Y - 161.09f;	return ROJO_CLARO; }	// 9    208.045f
	m_y = Y - 255.00f; return BLANCO; 								// 15
}
COLOR Color::escalaAmarillo( float Y ) {
	if( Y <  67.495f ) { m_y = Y;			return NEGRO; }				// 1    67.495f
	if( Y < 140.85f	 ) { m_y = Y - 134.99f;	return ROJO;	}			// 6    160.85f     -20
	if( Y < 195.665f ) { m_y = Y - 186.71f;	return AMARILLO_OSCURO; }	// 10   195.665f			186.71
	if( Y < 239.81f  ) { m_y = Y - 204.62f;	return AMARILLO_CLARO; }	// 11   229.81f     +10		204.62
	m_y = Y - 255.00f; return BLANCO; 									// 15
}
*/

COLOR Color::escalaGris( float Y ) {
	if( Y <  50.71f ) { m_y = Y;			return NEGRO; }			// Negro       0.00f       50.71f
	if( Y < 118.13f ) { m_y = Y - 101.42f;	return AZUL_OSCURO;	}	// Azul Oscuro 101.42f     118.13f
	if( Y < 160.12f ) { m_y = Y - 134.84f;	return AZUL_CLARO; }	// Azul Claro  134.84f     160.12f
	if( Y < 194.7f  ) { m_y = Y - 185.40f;	return CIAN; }			// Cianógeno   185.40f     194.7f
	if( Y < 229.5f  ) { m_y = Y - 204.00f;	return GRIS; }			// Gris        204.00f     229.5f
	m_y = Y - 255.00f; return BLANCO; 								// Blanco      255.00f
}
COLOR Color::escalaVerde( float Y ) {
	if( Y < 50.115f  ) { m_y = Y;			return NEGRO; }			// Negro           0.00f       60.115f  - 10
	if( Y < 127.695f ) { m_y = Y - 120.23f;	return VERDE_OSCURO; }	// Verde Oscuro    120.23f     127.695f
	if( Y < 153.18f  ) { m_y = Y - 135.16f;	return VERDE; }			// Verde Medio     135.16f     153.18f
	if( Y < 197.6f   ) { m_y = Y - 171.20f;	return VERDE_CLARO; }	// Verde Claro     171.20f     187.6f   + 10
	if( Y < 229.5f   ) { m_y = Y - 204.00f;	return GRIS; }			// Gris            204.00f     229.5f
	m_y = Y - 255.00f; return BLANCO; 								// Blanco          255.00f
}
COLOR Color::escalaCiano( float Y ) {
	if( Y <  50.71f ) { m_y = Y;			return NEGRO; }			// Negro       0.00f       50.71f
	if( Y < 118.13f ) { m_y = Y - 101.42f;	return AZUL_OSCURO;	}	// Azul Oscuro 101.42f     118.13f
	if( Y < 160.12f ) { m_y = Y - 134.84f;	return AZUL_CLARO; }	// Azul Claro  134.84f     160.12f
	if( Y < 194.7f  ) { m_y = Y - 185.40f;	return CIAN; }			// Cianógeno   185.40f     194.7f  
	if( Y < 229.5f  ) { m_y = Y - 204.00f;	return GRIS; }			// Gris        204.00f     229.5f 
	m_y = Y - 255.00f; return BLANCO; 								// Blanco      255.00f
}
COLOR Color::escalaAzul( float Y ) {
	if( Y <  40.71f ) { m_y = Y;			return NEGRO; }			// Negro       0.00f       50.71f   - 10
	if( Y < 118.13f ) { m_y = Y - 101.42f;	return AZUL_OSCURO;	}	// Azul Oscuro 101.42f     118.13f 
	if( Y < 160.12f ) { m_y = Y - 134.84f;	return AZUL_CLARO; }	// Azul Claro  134.84f     160.12f
	if( Y < 194.7f  ) { m_y = Y - 185.40f;	return CIAN; }			// Cianógeno   185.40f     194.7f
	if( Y < 229.5f  ) { m_y = Y - 204.00f;	return GRIS; }			// Gris        204.00f     229.5f
	m_y = Y - 255.00f; return BLANCO; 								// Blanco      255.00f
}
COLOR Color::escalaMagenta( float Y ) {
	if( Y <  60.225f ) { m_y = Y;			return NEGRO; } 		// Negro           0.00f		60.225f
	if( Y < 127.45f  ) { m_y = Y - 120.45f;	return ROJO_OSCURO;	}	// Rojo Oscuro     120.45f		127.45f
	if( Y < 157.77f  ) { m_y = Y - 134.45f;	return MAGENTA;	}		// Magenta         134.45f		147.77f     + 20
	if( Y < 202.545f ) { m_y = Y - 161.09f;	return ROJO_CLARO; }	// Rojo Claro      161.09f		182.545f	+ 20
	if( Y < 229.5f   ) { m_y = Y - 204.00f;	return GRIS; }			// Gris            204.00f		229.5f
	m_y = Y - 255.00f; return BLANCO; 								// Blanco          255.00f
}
COLOR Color::escalaRojo( float Y ) {
	if( Y <  60.225f ) { m_y = Y;			return NEGRO; } 			// Negro       	0.00f       60.225f
	if( Y < 127.72f  ) { m_y = Y - 120.45f;	return ROJO_OSCURO;	} 		// Rojo Oscuro	120.45f     127.72f 
	if( Y < 148.04f  ) { m_y = Y - 134.99f;	return ROJO; }				// Rojo Medio  	134.99f     148.04f
	if( Y < 202.545f ) { m_y = Y - 161.09f;	return ROJO_CLARO; }		// Rojo Claro  	161.09f     182.545f	+ 20
	if( Y < 229.5f   ) { m_y = Y - 204.00f;	return GRIS; }				// Gris        	204.00f     229.5f
	m_y = Y - 255.00f; return BLANCO; 									// Blanco      	255.00f
}
COLOR Color::escalaAmarillo( float Y ) {
	if( Y <  60.225f ) { m_y = Y;			return NEGRO; }             // Negro               0.00        60.225f
	if( Y < 127.72f  ) { m_y = Y - 120.45f;	return ROJO_OSCURO;	}       // Rojo Oscuro         120.45      127.72f
	if( Y < 150.85f  ) { m_y = Y - 134.99f;	return ROJO;	}           // Rojo Medio          134.99      160.85f  - 10
	if( Y < 195.665f ) { m_y = Y - 186.71f;	return AMARILLO_OSCURO; }   // Amarillo Oscuro     186.71      195.665f
	if( Y < 239.81f  ) { m_y = Y - 204.62f;	return AMARILLO_CLARO; }    // Amarillo Claro      204.62      229.81f	+ 10
	m_y = Y - 255.00f; return BLANCO;                                   // Blanco              255.00
}

/*
float SeudoAleatorio() {
	static float retorno = 2.0f;
	if( retorno > 4.4f ) retorno = 2.0f;
	else retorno += 0.6f; 
	return retorno;	
}
*/
/*
float SeudoAleatorio() {
//	static float rodin[24] = { 1.1, 1.1, 1.2, 1.3, 1.5, 1.8, 1.4, 1.3, 1.7, 1.1, 1.8, 1.9, 1.8, 1.8, 1.7, 1.6, 1.4, 1.1, 1.5, 1.6, 1.2, 1.8, 1.1, 1.9 };
//	static float rodin[24] = { 2.1, 3.1, 2.2, 3.3, 2.5, 3.8, 2.4, 3.3, 2.7, 3.1, 2.8, 3.9, 2.8, 3.8, 2.7, 3.6, 2.4, 3.1, 2.5, 3.6, 2.2, 3.8, 2.1, 3.9 };
//	static float rodin[24] = { 2.1, 3.1, 5.2, 2.3, 3.5, 5.8, 2.4, 3.3, 5.7, 2.1, 3.8, 5.9, 2.8, 3.8, 5.7, 2.6, 3.4, 5.1, 2.5, 3.6, 5.2, 2.8, 3.1, 5.9 };
//	static float rodin[24] = { 1.1, 2.1, 3.2, 2.3, 1.5, 2.8, 3.4, 2.3, 1.7, 2.1, 3.8, 2.9, 1.8, 2.8, 3.7, 2.6, 1.4, 2.1, 3.5, 2.6, 1.2, 2.8, 3.1, 2.9 };
	static float rodin[24] = { 2.1, 3.1, 4.2, 5.3, 4.5, 3.8, 2.4, 3.3, 4.7, 5.1, 4.8, 3.9, 2.8, 3.8, 4.7, 5.6, 4.4, 3.1, 2.5, 3.6, 4.2, 5.8, 4.1, 3.9 };
	static size_t indice = 23;
	++indice;
	if( indice > 23 ) indice = 23;
	return rodin[indice];
}
*/

float SeudoAleatorio() {
	static int rodin[24] = { 1, 1, 2, 3, 5, 8, 4, 3, 7, 1, 8, 9, 8, 8, 7, 6, 4, 1, 5, 6, 2, 8, 1, 9 };
	static size_t indice = 23;
	++indice;
	if( indice > 23 ) indice = 23;
	return ( ( rodin[indice] / 9.0f ) * 3.0f ) + 2.0f;
}


COLOR Color::damePaleta() {
	if( m_rgba[3] < 128 ) return TRANSPARENTE;
	Uint8 max, dif;
	max = abs( (int)m_rgba[0] - (int)m_rgba[1] );
	dif = abs( (int)m_rgba[1] - (int)m_rgba[2] );
	if( dif > max ) max = dif;
	dif = abs( (int)m_rgba[0] - (int)m_rgba[2] );
	if( dif > max ) max = dif;
	// Podría rondar valores entre 0 y 100 y hasta 256;
	float Y = ( m_rgba[0] * 0.3f ) + ( m_rgba[1] * 0.59f ) + ( m_rgba[2] * 0.11f );
	float RY = (float)m_rgba[0] - Y;
	float BY = (float)m_rgba[2] - Y;
	float C = atan2( RY, BY );
	// if( m_acarreo ) Y += ( m_y / 3.141592f );
	// if( m_acarreo ) Y += ( m_y / Aleatorio( 1.0f, 3.0f ) );
	// if( m_acarreo ) Y += ( m_y / Aleatorio( 2.0f, 5.0f ) );
	if( m_acarreo ) Y += ( m_y / SeudoAleatorio() );
	if( max <= m_gris ) return escalaGris( Y );		// Puro		MSX		Media
	if( C < -1.765f ) return escalaVerde( Y );		// -2.36	-2.16	-1.765
	if( C < -0.585f ) return escalaCiano( Y );		// -1.17	-1.11	-0.585
	if( C <  0.0f   ) return escalaAzul( Y );		// -0.12	-0.13	 0.0
	if( C <  1.385f ) return escalaMagenta( Y );	//  0.79	 0.91	 1.385
	if( C <  2.52f  ) return escalaRojo( Y );		//  1.98 	 2.01 	 2.5
	return                   escalaAmarillo( Y );	//  3.02	 2.81	 3.1415
}

void  Color::transformaPaleta() {
	inicia( damePaleta() );
}
	
float Color::dameLuminancia() {
	return ( m_rgba[0] * 0.3f ) + ( m_rgba[1] * 0.59f ) + ( m_rgba[2] * 0.11f );
}

float Color::dameTono() {
	if( m_rgba[3] < 128.0f ) return 0.0f;	// Transparente
	float Y = ( m_rgba[0] * 0.3f ) + ( m_rgba[1] * 0.59f ) + ( m_rgba[2] * 0.11f );
	float RY = m_rgba[0] - Y;
	float BY = m_rgba[2] - Y;
	float RYBY = RY + BY; // Gris
	if( ( RYBY > -1.0f ) &&  ( RYBY < 1.0f )  ) return 0.0f; // Gris
	return atan2( RY, BY );	
}

