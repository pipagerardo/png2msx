#include "formato_png.h"

#define FORMATO_PNG_ANCHO_MAX 2048
#define FORMATO_PNG_ALTO_MAX  2048

Formato_PNG::Formato_PNG()
{
    inicia();
}

Formato_PNG::~Formato_PNG()
{
    quita();
}

void Formato_PNG::inicia()
{

    m_fp 		= NULL;		// FILE*
    m_read      = true;
    m_png_ptr	= NULL;		// png_structp
    m_info_ptr	= NULL;		// png_infop

    m_paleta	     = NULL;	// png_colorp
    m_paleta_colores = 0;		// 	int
    m_paleta_alfa    = false;

    m_filas_pixels = NULL;	// png_bytep*
    m_fila_bytes   = 0;		// png_size_t

    m_ancho				= 0;	// png_uint_32
    m_alto				= 0;	// png_uint_32
    m_tipo_color	    = -1;	// png_byte
    m_profundidad_bit   = 0;	// png_byte
    m_canales			= 0;	// png_byte
    m_interlazado       = PNG_INTERLACE_NONE;
    m_numero_de_pasadas = 0;	// int

}

void Formato_PNG::quita()
{

    // Archivo:
    if( m_fp ) fclose( m_fp );
    m_fp = NULL;

    // Estructuras PNG:
    if( m_png_ptr )
    {
        if( m_read )
        {
            png_destroy_read_struct(
                &m_png_ptr,
                m_info_ptr ? &m_info_ptr : (png_infopp)0,
                (png_infopp)0
            );
        }
        else
        {
            png_destroy_write_struct(
                &m_png_ptr,
                m_info_ptr ? &m_info_ptr : (png_infopp)0
            );
        }
    }
    m_png_ptr  = NULL;
    m_info_ptr = NULL;

    quitaPaleta();
    quitaPixels();

    // Retornos:
    m_ancho	            = 0;
    m_alto	            = 0;
    m_tipo_color        = -1;
    m_profundidad_bit   = 0;
    m_canales           = 0;
    m_interlazado       = PNG_INTERLACE_NONE;
    m_numero_de_pasadas = 0;

}

bool Formato_PNG::abre( const char* archivo_png )
{

    quita();

    // Abrimos el archivo:
    m_fp = fopen( archivo_png, "rb" );
    if( !m_fp )
    {
        printf( "Error abriendo archivo: %s \n", archivo_png );
        quita();
        return false;
    }

    char cabecera[ 8 ];    // 8 is the maximum size that can be checked
    if( fread( cabecera, 1, 8, m_fp ) != 8 ) return false;
    if( png_sig_cmp( (unsigned char*)cabecera, 0, 8 ) )
    {
        printf( "El archivo: %s, no es formato PNG.\n", archivo_png );
        quita();
        return false;
    }

    m_read = true;
    m_png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
    if( !m_png_ptr )
    {
        printf( "png_create_read_struct ha fallado.\n" );
        quita();
        return false;
    }

    m_info_ptr = png_create_info_struct( m_png_ptr );
    if( !m_info_ptr )
    {
        printf( "png_create_info_struct ha fallado.\n" );
        quita();
        return false;
    }

    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error durante init_io.\n" );
        quita();
        return false;
    }
    png_init_io( m_png_ptr, m_fp );

    // Tell lib we have already handled the first <num_bytes> magic bytes.
    // Handling more than 8 bytes from the beginning of the file is an error.
    png_set_sig_bytes( m_png_ptr, 8 );

    png_read_info( m_png_ptr, m_info_ptr );

//	m_ancho	= png_get_image_width( m_png_ptr, m_info_ptr );
//	m_alto  = png_get_image_height( m_png_ptr, m_info_ptr );
//	m_profundidad_bit = png_get_bit_depth( m_png_ptr, m_info_ptr );
//  m_tipo_color  = png_get_color_type( m_png_ptr, m_info_ptr );
//	m_interlazado = png_get_interlace_type( m_png_ptr, m_info_ptr );
    int bit_depth;
    int color_type;
    png_get_IHDR( m_png_ptr, m_info_ptr, &m_ancho, &m_alto, &bit_depth, &color_type, &m_interlazado, NULL, NULL );
    m_profundidad_bit = (png_byte)bit_depth;
    m_tipo_color      = (png_byte)color_type;

    // Have the code handle the interlacing.  Returns the number of passes.
    // MUST be called before png_read_update_info or png_start_read_image,
    // otherwise it will not have the desired effect.  Note that it is still
    // necessary to call png_read_row or png_read_rows png_get_image_height
    // times for each pass.
    m_numero_de_pasadas = png_set_interlace_handling( m_png_ptr );

    // tell libpng to strip 16 bit/color files down to 8 bits/color
    png_set_strip_16( m_png_ptr ) ;

    // Extract multiple pixels with bit depths of 1, 2, and 4 from a single
    // byte into separate bytes (useful for paletted and grayscale images).
    // png_set_packing( m_png_ptr );

    // scale greyscale values to the range 0..255
    if( m_tipo_color == PNG_COLOR_TYPE_GRAY ) png_set_expand( m_png_ptr );

//  Convierte GRAY_ALPHA a RGBA
//	if( m_tipo_color == PNG_COLOR_TYPE_GRAY_ALPHA ) png_set_gray_to_rgb( m_png_ptr );

    png_read_update_info( m_png_ptr, m_info_ptr );

//	m_ancho	= png_get_image_width( m_png_ptr, m_info_ptr );
//	m_alto  = png_get_image_height( m_png_ptr, m_info_ptr );
//	m_profundidad_bit = png_get_bit_depth( m_png_ptr, m_info_ptr );
//  m_tipo_color  = png_get_color_type( m_png_ptr, m_info_ptr );
//	m_interlazado = png_get_interlace_type( m_png_ptr, m_info_ptr );
    png_get_IHDR( m_png_ptr, m_info_ptr, &m_ancho, &m_alto, &bit_depth, &color_type, &m_interlazado, NULL, NULL );
    m_profundidad_bit = (png_byte)bit_depth;
    m_tipo_color      = (png_byte)color_type;
    m_canales = png_get_channels( m_png_ptr, m_info_ptr );

    // Memoria para almacenar los pixels:
    if( !iniciaPixels() )
    {
        quita();
        return false;
    }

    // read file
    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error durante read_image.\n" );
        quita();
        return false;
    }

    // Leemos los pixels a memoria:
    png_read_image( m_png_ptr, m_filas_pixels );

    // In some cases it can't read PNG's created by some popular programs (ACDSEE),
    // we do not want to process comments, so we omit png_read_end
    png_read_end( m_png_ptr, m_info_ptr );

    // Paleta:
    if( m_tipo_color == PNG_COLOR_TYPE_PALETTE )
    {
        png_colorp paleta;
        png_get_PLTE( m_png_ptr, m_info_ptr, &paleta, &m_paleta_colores );
        if( m_paleta_colores > 0 )
        {
            if( !iniciaPaleta( m_paleta_colores ) )
            {
                quita();
                return false;
            }
            for( int i = 0; i < m_paleta_colores; ++i )
            {
                m_paleta[i].red   = paleta[i].red;
                m_paleta[i].green = paleta[i].green;
                m_paleta[i].blue  = paleta[i].blue;
            }
        }
    }

    // For images with a single "transparent colour", set colour key;
    // if more than one index has transparency, or if partially transparent
    // entries exist, use full alpha channel
    if( png_get_valid( m_png_ptr, m_info_ptr, PNG_INFO_tRNS ) )
    {
        png_bytep 		trans_alpha;
        int 			num_trans;
        png_color_16p	trans_color;
        // png_uint_32 png_get_tRNS(
        // 		png_const_structrp png_ptr,
        // 		png_inforp info_ptr,
        // 		png_bytep *trans_alpha,
        // 		int *num_trans,
        // 		png_color_16p *trans_color
        // );
        // typedef struct png_color_16_struct {
        //    png_byte index;    // used for palette files
        //    png_uint_16 red;   // for use in red green blue files
        //    png_uint_16 green;
        //    png_uint_16 blue;
        //    png_uint_16 gray;  // for use in grayscale files
        // } png_color_16;
        // typedef png_color_16 * png_color_16p;
        png_get_tRNS( m_png_ptr, m_info_ptr, &trans_alpha, &num_trans, &trans_color );
        // printf( "Número de colores tansparentes = %d\n", num_trans );
        // if( trans_alpha ) {
        // printf( "trans_alpha[0] = %u\n", trans_alpha[0] );
        // }
        if( trans_color )
        {
            m_paleta_alfa = true;
            /*
            	printf( "trans_color[0].index = %u\n", trans_color[0].index );
            	printf( "trans_color[0].red   = %u\n", trans_color[0].red );
            	printf( "trans_color[0].green = %u\n", trans_color[0].green );
            	printf( "trans_color[0].blue  = %u\n", trans_color[0].blue );
            	printf( "trans_color[0].gray  = %u\n", trans_color[0].gray );
            */
        }
    }

    // Limpiamos la estructura:
    if( m_png_ptr )
    {
        png_destroy_read_struct(
            &m_png_ptr,
            m_info_ptr ? &m_info_ptr : (png_infopp)0,
            (png_infopp)0
        );
    }
    m_png_ptr  = NULL;
    m_info_ptr = NULL;

    // Cerramos el archivo:
    fclose( m_fp );
    m_fp = NULL;
    return true;

}

bool Formato_PNG::guarda( const char* archivo_png, int nivel_compresion )
{

    m_read = false;
    m_png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
    if( !m_png_ptr )
    {
        printf( "png_create_read_struct ha fallado.\n" );
        quita();
        return false;
    }

    m_info_ptr = png_create_info_struct( m_png_ptr );
    if( !m_info_ptr )
    {
        printf( "png_create_info_struct ha fallado.\n" );
        quita();
        return false;
    }

    // Abrimos el archivo:
    m_fp = fopen( archivo_png, "wb" );
    if( !m_fp )
    {
        printf( "Error abriendo archivo: %s \n", archivo_png );
        quita();
        return false;
    }
    png_set_compression_level( m_png_ptr, nivel_compresion );

    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error durante init_io.\n" );
        quita();
        return false;
    }
    png_init_io( m_png_ptr, m_fp );

    // write header
    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error al escribir la cabecera.\n" );
        quita();
        return false;
    }

    int bit_depth = (int)m_profundidad_bit;
    png_set_IHDR(
        m_png_ptr, m_info_ptr, m_ancho, m_alto, bit_depth, m_tipo_color,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
    );

    // La paleta
    if( m_tipo_color == PNG_COLOR_TYPE_PALETTE )
    {
        png_set_PLTE( m_png_ptr, m_info_ptr, m_paleta, m_paleta_colores );
        if( m_paleta_alfa )
        {
            int 			num_trans = 1;
            png_bytep 		trans_alpha = NULL;
            int ancho_pixel = (m_profundidad_bit * m_canales) >> 3;
            if( ancho_pixel == 0 ) ancho_pixel = 1;
            trans_alpha = (png_bytep)malloc( sizeof( png_byte ) * num_trans * ancho_pixel );
            png_color_16p	trans_color = NULL;
            trans_color = (png_color_16p)malloc( sizeof( png_color_16 ) * num_trans );
            trans_alpha[0] = 0;
            trans_color[0].index = 0;
            trans_color[0].red = 0;
            trans_color[0].green = 0;
            trans_color[0].blue = 0;
            trans_color[0].gray = 0;
            png_set_tRNS( m_png_ptr, m_info_ptr, trans_alpha, num_trans, trans_color );
            // free( trans_color );
        }
    }

    png_write_info( m_png_ptr, m_info_ptr );

    // write bytes
    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error al escribir bytes.\n" );
        quita();
        return false;
    }
    png_write_image( m_png_ptr, m_filas_pixels );

    // end write
    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error al escribir el final del archivo.\n" );
        quita();
        return false;
    }
    png_write_end( m_png_ptr, NULL );

    // Limpiamos la estructura:
    if( m_png_ptr )
    {
        png_destroy_write_struct(
            &m_png_ptr,
            m_info_ptr ? &m_info_ptr : (png_infopp)0
        );
    }
    m_png_ptr = NULL;
    m_info_ptr = NULL;

    // Cerramos el archivo:
    if( m_fp ) fclose( m_fp );
    m_fp = NULL;
    return true;

}

// --------------------------------------------------------
// Retornos de Información:
png_uint_32	Formato_PNG::dameAnchoPixels()   const
{
    return m_ancho;
}
png_uint_32	Formato_PNG::dameAltoPixels()    const
{
    return m_alto;
}
png_byte    Formato_PNG::dameBitsPorCanal()  const
{
    return m_profundidad_bit;
}
png_byte    Formato_PNG::dameCanales()       const
{
    return m_canales;
}
int 		Formato_PNG::dameColoresPaleta() const
{
    return m_paleta_colores;
}
bool		Formato_PNG::damePaletaAlfa()	 const
{
    return m_paleta_alfa;
}
png_byte    Formato_PNG::dameTipoColor()     const
{
    return	m_tipo_color;
}
// --------------------------------------------------------

bool Formato_PNG::iniciaPaleta( int num_colores, bool alfa )
{
    quitaPaleta();
    if( ( num_colores < 1 ) || ( num_colores > 256 ) ) return false;
    m_paleta = (png_colorp)malloc( sizeof(png_color) * num_colores );
    if( !m_paleta )
    {
        printf( "Sin memoria para la paleta = %d.\n", num_colores );
        return false;
    }
    m_paleta_colores = num_colores;	// Número de colores de la paleta.
    // Creamos una paleta de grises:
    for( int i = 0; i < m_paleta_colores; ++i )
    {
        m_paleta[i].red   = i;
        m_paleta[i].green = i;
        m_paleta[i].blue  = i;
    }
    if( alfa ) m_paleta_alfa = true;
    return true;
}

void Formato_PNG::quitaPaleta()
{
    if( m_paleta ) free( m_paleta );
    m_paleta = NULL;
    m_paleta_colores = 0;
    m_paleta_alfa    = false;
}

bool Formato_PNG::tomaPaleta( const Color& color )
{
    if( !m_paleta ) return false;
    png_byte indice = color.dameIndice();
    if( m_paleta_colores < indice ) return false;
    m_paleta[indice].red   = color.dameRojo();
    m_paleta[indice].green = color.dameVerde();
    m_paleta[indice].blue  = color.dameAzul();
    return true;
}

bool Formato_PNG::damePaleta( Color& color )
{
    if( !m_paleta ) return false;
    png_byte indice = color.dameIndice();
    if( m_paleta_colores < indice ) return false;
    color.tomaRojo(  m_paleta[indice].red   );
    color.tomaVerde( m_paleta[indice].green );
    color.tomaAzul(  m_paleta[indice].blue  );
    return true;
}

bool Formato_PNG::iniciaPixels()
{
    quitaPixels();
    if( !m_png_ptr  )  return false;
    if( !m_info_ptr )  return false;
    if( ( m_ancho == 0 ) || ( m_ancho > FORMATO_PNG_ANCHO_MAX ) ) return false;
    if( ( m_alto  == 0 ) || ( m_ancho >= FORMATO_PNG_ALTO_MAX ) ) return false;
    // Memoria para almacenar los pixels:
    m_filas_pixels = (png_bytep*) malloc( sizeof(png_bytep) * m_alto );
    if( !m_filas_pixels )
    {
        printf( "Sin memoria para pixels, alto=%u\n", (unsigned int)m_alto );
        return false;
    }
    // Determinamos el ancho en bytes de una fila:
    m_fila_bytes = png_get_rowbytes( m_png_ptr, m_info_ptr );
    if( m_fila_bytes == 0 )
    {
        printf( "Error en el ancho de fila en bytes.\n" );
        quitaPixels();
        return false;
    }
    for( png_uint_32 y = 0; y < m_alto; y++ )
    {
        m_filas_pixels[y] = (png_byte*) malloc( m_fila_bytes );
        if( !m_filas_pixels[y] )
        {
            printf( "Sin memoria para pixels, y=%u, ancho=%u\n", (unsigned int)y, (unsigned int)m_fila_bytes );
            m_alto = y;
            quitaPixels();
            return false;
        }
    }
    return true;
}

void Formato_PNG::quitaPixels()
{
    // Memoria para almacenar los pixels:
    if( m_filas_pixels )
    {
        for( png_uint_32 y = 0; y < m_alto; y++ )
        {
            if( m_filas_pixels[y] ) free( m_filas_pixels[y] );
        }
        free( m_filas_pixels );
    }
    m_filas_pixels = NULL;
    m_fila_bytes = 0;
}

bool Formato_PNG::dameColor( png_uint_32 x, png_uint_32 y, Color& color )
{
    if( !m_filas_pixels ) return false;
    if( ( x > m_ancho ) || ( y > m_alto ) ) return false;

    png_byte* fila = m_filas_pixels[ y ];
    png_uint_32 des = 0;

    // PALETA DE COLOR:
    if( m_tipo_color == PNG_COLOR_TYPE_PALETTE )
    {
        png_byte i = 0;
        switch( m_profundidad_bit )
        {
        case 4:
            des = ( x * m_canales ) >> 1;
            i   = fila[ des ];
            if( x % 2 ) i &= 0x0F;	// 12 = 2
            else		i >>= 4;	// 12 = 1
            break;
        case 8:
            des = x * m_canales;
            i   = fila[ des ];
            break;
        default:
            return false;
        }
        color.tomaIndice( i );
        color.tomaRojo(  m_paleta[i].red   );
        color.tomaVerde( m_paleta[i].green );
        color.tomaAzul(  m_paleta[i].blue  );
        if( i == 0 ) color.tomaAlfa( 0 );
        else		 color.tomaAlfa( 255 );
        return true;
    }

    // COLOR RGBA:
    if( m_profundidad_bit != 8 ) return false;
    des = x * m_canales;
    switch( m_canales )
    {
    case 1:	// GRAY
        color.tomaRojo(  fila[ des ] );
        color.tomaVerde( color.dameRojo() );
        color.tomaAzul(  color.dameRojo() );
        color.tomaAlfa( 255 );
        break;
    case 2:	// GRAY + ALPHA
#if BYTE_ORDER == LITTLE_ENDIAN
        color.tomaRojo(  fila[ des + 0 ] );
        color.tomaAlfa(  fila[ des + 1 ] );
#else
        color.tomaAlfa(  fila[ des + 0 ] );
        color.tomaRojo(  fila[ des + 1 ] );
#endif
        color.tomaVerde( color.dameRojo() );
        color.tomaAzul(  color.dameRojo() );
        break;
    case 3:	// RGB
#if BYTE_ORDER == LITTLE_ENDIAN
        color.tomaRojo(  fila[ des + 0 ] );
        color.tomaVerde( fila[ des + 1 ] );
        color.tomaAzul(  fila[ des + 2 ] );
#else
        color.tomaAzul(  fila[ des + 0 ] );
        color.tomaVerde( fila[ des + 1 ] );
        color.tomaRojo(  fila[ des + 2 ] );
#endif
        color.tomaAlfa( 255 );
        break;
    case 4:	// RGBA
#if BYTE_ORDER == LITTLE_ENDIAN
        color.tomaRojo(  fila[ des + 0 ] );
        color.tomaVerde( fila[ des + 1 ] );
        color.tomaAzul(  fila[ des + 2 ] );
        color.tomaAlfa(  fila[ des + 3 ] );
#else
        color.tomaAlfa(  fila[ des + 0 ] );
        color.tomaAzul(  fila[ des + 1 ] );
        color.tomaVerde( fila[ des + 2 ] );
        color.tomaRojo(  fila[ des + 3 ] );
#endif
        break;
    default:
        return false;
    }
    return true;
}

bool Formato_PNG::tomaColor( png_uint_32 x, png_uint_32 y, Color& color )
{
    if( !m_filas_pixels ) return false;
    if( ( x > m_ancho ) || ( y > m_alto ) ) return false;

    png_byte* fila = m_filas_pixels[ y ];
    png_uint_32 des = 0;

    // PALETA DE COLOR:
    if( m_tipo_color == PNG_COLOR_TYPE_PALETTE )
    {
        png_byte i = 0;
        switch( m_profundidad_bit )
        {
        case 4:
        {
            des = ( x * m_canales ) >> 1;
            i   = fila[ des ];
            png_byte j = color.dameIndice();
            j &= 0x0F;
            if( x % 2 )
            {
                i &= 0xF0;
            }
            else
            {
                i &= 0x0F;
                j <<= 4;
            }
            i |= j;
        }
        break;
        case 8:
            des = x * m_canales;
            i = color.dameIndice();
            break;
        default:
            return false;
        }
        fila[ des ] = i;
        return true;
    }

    // COLOR RGBA:
    if( m_profundidad_bit != 8 ) return false;
    des = x * m_canales;
    switch( m_canales )
    {
    case 1:	// GRAY
        fila[ des ] = color.dameRojo();
        break;
    case 2:	// GRAY + ALPHA
#if BYTE_ORDER == LITTLE_ENDIAN
        fila[ des + 0 ] = color.dameRojo();
        fila[ des + 1 ] = color.dameAlfa();
#else
        fila[ des + 0 ] = color.dameAlfa();
        fila[ des + 1 ] = color.dameRojo();
#endif
        break;
    case 3:	// RGB
#if BYTE_ORDER == LITTLE_ENDIAN
        fila[ des + 0 ] = color.dameRojo();
        fila[ des + 1 ] = color.dameVerde();
        fila[ des + 2 ] = color.dameAzul();
#else
        fila[ des + 0 ] = color.dameAzul();
        fila[ des + 1 ] = color.dameVerde();
        fila[ des + 2 ] = color.dameRojo();
#endif
        break;
    case 4:	// RGBA
#if BYTE_ORDER == LITTLE_ENDIAN
        fila[ des + 0 ] = color.dameRojo();
        fila[ des + 1 ] = color.dameVerde();
        fila[ des + 2 ] = color.dameAzul();
        fila[ des + 3 ] = color.dameAlfa();
#else
        fila[ des + 0 ] = color.dameAlfa();
        fila[ des + 1 ] = color.dameAzul();
        fila[ des + 2 ] = color.dameVerde();
        fila[ des + 3 ] = color.dameRojo();
#endif
        break;
    default:
        return false;
    }
    return true;
}

bool Formato_PNG::iniciaPNG(
    png_uint_32	ancho,
    png_uint_32	alto,
    png_byte	bits,
    png_byte	canales,
    png_byte	tipo_color
)
{
    quita();
    m_ancho = ancho;
    m_alto = alto;
    m_profundidad_bit = bits;
    m_canales = canales;
    m_profundidad_bit = bits;
    m_tipo_color = tipo_color;

    // Comprobaciones:
    if( ( m_ancho == 0 ) || ( m_ancho > FORMATO_PNG_ANCHO_MAX ) ) return false;
    if( ( m_alto  == 0 ) || ( m_ancho >= FORMATO_PNG_ALTO_MAX ) ) return false;
    switch( m_canales )
    {
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    default:
        return false;
    }
    switch(	m_tipo_color )
    {
    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_GRAY_ALPHA:
    case PNG_COLOR_TYPE_RGB:
    case PNG_COLOR_TYPE_RGB_ALPHA:
        if( m_profundidad_bit == 8 ) break;
        return false;
    case PNG_COLOR_TYPE_PALETTE:
        if( m_profundidad_bit == 4 ) break;
        if( m_profundidad_bit == 8 ) break;
    default:
        return false;
    }

    m_read = false;
    m_png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
    if( !m_png_ptr )
    {
        printf( "png_create_read_struct ha fallado.\n" );
        quita();
        return false;
    }

    m_info_ptr = png_create_info_struct( m_png_ptr );
    if( !m_info_ptr )
    {
        printf( "png_create_info_struct ha fallado.\n" );
        quita();
        return false;
    }

    // Iniciando la cabecera:
    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error al iniciar la cabecera.\n" );
        quita();
        return false;
    }

    int bit_depth = (int)m_profundidad_bit;
    png_set_IHDR(
        m_png_ptr, m_info_ptr, m_ancho, m_alto, bit_depth, m_tipo_color,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
    );

    return true;
}

bool Formato_PNG::escribePNG(  const char* archivo_png, int nivel_compresion )
{

    // Abrimos el archivo:
    m_fp = fopen( archivo_png, "wb" );
    if( !m_fp )
    {
        printf( "Error abriendo archivo: %s \n", archivo_png );
        quita();
        return false;
    }
    png_set_compression_level( m_png_ptr, nivel_compresion );

    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error durante init_io.\n" );
        quita();
        return false;
    }
    png_init_io( m_png_ptr, m_fp );

    if( m_fila_bytes != png_get_rowbytes( m_png_ptr, m_info_ptr ) )
    {
        printf( "Error en el cálculo de ancho de fila en bytes.\n" );
    }

    // La paleta
    if( m_tipo_color == PNG_COLOR_TYPE_PALETTE )
    {
        if( !m_paleta )
        {
            printf( "Error falta la paleta de colores.\n" );
            quita();
            return false;
        }
        png_set_PLTE( m_png_ptr, m_info_ptr, m_paleta, m_paleta_colores );
        if( m_paleta_alfa )
        {
            int 			num_trans = 1;
            png_bytep 		trans_alpha = NULL;
            int ancho_pixel = (m_profundidad_bit * m_canales) >> 3;
            if( ancho_pixel == 0 ) ancho_pixel = 1;
            trans_alpha = (png_bytep)malloc( sizeof( png_byte ) * num_trans * ancho_pixel );
            png_color_16p	trans_color = NULL;
            trans_color = (png_color_16p)malloc( sizeof( png_color_16 ) * num_trans );
            trans_alpha[0] = 0;
            trans_color[0].index = 0;
            trans_color[0].red = 0;
            trans_color[0].green = 0;
            trans_color[0].blue = 0;
            trans_color[0].gray = 0;
            png_set_tRNS( m_png_ptr, m_info_ptr, trans_alpha, num_trans, trans_color );
            // free( trans_color );
        }
    }

    // write info
    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error al escribir la informacion.\n" );
        quita();
        return false;
    }
    png_write_info( m_png_ptr, m_info_ptr );

    // write bytes
    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error al escribir los bytes.\n" );
        quita();
        return false;
    }
    png_write_image( m_png_ptr, m_filas_pixels );

    // end write
    if( setjmp( png_jmpbuf( m_png_ptr ) ) )
    {
        printf( "Error al escribir el final del archivo.\n" );
        quita();
        return false;
    }
    png_write_end( m_png_ptr, NULL );

    // Limpiamos la estructura:
    if( m_png_ptr )
    {
        png_destroy_write_struct(
            &m_png_ptr,
            m_info_ptr ? &m_info_ptr : (png_infopp)0
        );
    }
    m_png_ptr  = NULL;
    m_info_ptr = NULL;

    // Cerramos el archivo:
    if( m_fp ) fclose( m_fp );
    m_fp = NULL;
    return true;

}

