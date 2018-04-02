#ifndef _PNG2MSX_ISO_
#define _PNG2MSX_ISO_

#ifdef SPANISH

#define TXT_PNG2MSX_Conversor   "    Conversor de imágenes de PNG a MSX, por PipaGerardo.\n"
#define TXT_PNG2MSX_Version     "    Versión beta 8 ( 2018 )       pipagerardo@gmail.com\n"
#ifdef WINDOWS
#define TXT_PNG2MSX_Opciones    "png2msx.exe entrada.png [nombre_salida] [-opciones]\n"
#else
#define TXT_PNG2MSX_Opciones    "png2msx entrada.png [nombre_salida] [-opciones]\n"
#endif
#define TXT_PNG2MSX_Op01        "\t-x[Número]\tPosición de x en pixels [ 0 ~ 2048 ]\n"
#define TXT_PNG2MSX_Op02        "\t-y[Número]\tPosición de y en pixels [ 0 ~ 2048 ]\n"
#define TXT_PNG2MSX_Op03        "\t-w[Número]\tAncho en pixels [ 0 ~ 2048 ]\n"
#define TXT_PNG2MSX_Op04        "\t-h[Número]\tAlto  en pixels [ 0 ~ 2048 ]\n"
#define TXT_PNG2MSX_Op05        "\t\t\t\"w*h\" debe ser igual o menor que 49152\n"
#define TXT_PNG2MSX_Op06        "\t\t\tSi \"w\" no es 256 la imagen no estará alineada\n"
#define TXT_PNG2MSX_Op07        "\t-g[Número]\tUmbral de Gris [ 0 ~ 255 ]\n"
#define TXT_PNG2MSX_Op08        "\t-b[Número]\tContraste [ -255 ~ 255 ]\n"
#define TXT_PNG2MSX_Op09        "\t\t\t-b   - Contraste automático\n"
#define TXT_PNG2MSX_Op10        "\t\t\t-b16 - Blancos más blancos y viceversa\n"
#define TXT_PNG2MSX_Op11        "\t\t\t-b-8 - Blancos más oscuros y viceversa\n"
#define TXT_PNG2MSX_Op12        "\t-c\t\tFiltro de Colisión de Color (8 pixels)\n"
#define TXT_PNG2MSX_Op13        "\t-e\t\tPasa error conversión al siguiente pixel\n"
#define TXT_PNG2MSX_Op14        "\t-z\t\tCambia el color 0 por el 1\n"
#define TXT_PNG2MSX_Op15        "\t-n\t\tCambia el color 1 por el 0\n"
#define TXT_PNG2MSX_Op16        "\t-k[Número]\tColor de fondo de 0 a 15\n"
#define TXT_PNG2MSX_Op17        "\t-f[Número]\tBusca baldosas parecidas de 1 a 16 bits\n"
#define TXT_PNG2MSX_Op18        "\t\t\tRecomiendo valores de 2 a 6\n"
#define TXT_PNG2MSX_Op19        "\t-o[Número]\tQuita las baldosas repetidas en VRAM:\n"
#define TXT_PNG2MSX_Op20        "\t\t\t-o0 Baldosas repetidas 1 banco  VRAM 1-1-1\n"
#define TXT_PNG2MSX_Op21        "\t\t\t-o1 Baldosas repetidas 2 bancos VRAM 1-2-1\n"
#define TXT_PNG2MSX_Op22        "\t\t\t-o2 Baldosas repetidas 2 bancos VRAM 1-1-3\n"
#define TXT_PNG2MSX_Op23        "\t\t\t-o3 Baldosas repetidas 3 bancos VRAM 1-2-3\n"
#define TXT_PNG2MSX_Op24        "\t\t\t-o4 Baldosas repetidas sin restricciones\n"
#define TXT_PNG2MSX_Op25        "\t-p\t\tArchivo PNG de salida\n"
#define TXT_PNG2MSX_Op26        "\t-a\t\tArchivo PNG de salida con canal Alfa\n"
#define TXT_PNG2MSX_Op27        "\t-r\t\tArchivo en bruto para ASM: CHR, CLS y NAM\n"
#define TXT_PNG2MSX_Op28        "\t-t\t\tArchivo en bruto Pletter5 sin longitud\n"
#define TXT_PNG2MSX_Op29        "\t-l\t\tArchivo en bruto Pletter5 con longitud\n"
#define TXT_PNG2MSX_Op30        "\t-s\t\tArchivo SC2 para BLOAD\"ARCHIVO.SC2\",S\n"
#define TXT_PNG2MSX_Op31        "\t-m\t\tArchivo BASIC ASCII que carga SC2\n"
#define TXT_PNG2MSX_Ejemplo1    "\nEjemplo:\n"
#ifdef WINDOWS
#define TXT_PNG2MSX_Ejemplo2    "png2msx.exe entrada.png salida -x8 -y32 -w256 -h192 -c -p\n"
#else
#define TXT_PNG2MSX_Ejemplo2    "png2msx entrada.png salida -x8 -y32 -w256 -h192 -c -p\n"
#endif
#define TXT_PNG2MSX_Obteniendo_datos            "Obteniendo datos...\n"
#define TXT_PNG2MSX_entrada_dos_puntos          "entrada: %s\n"
#define TXT_PNG2MSX_Solo_se_permite_PNG         "Sólo se permite la extensión PNG.\n"
#define TXT_PNG2MSX_Es_obligatorio_PNG          "Es obligatorio una extensión PNG.\n"
#define TXT_PNG2MSX_Error_abriendo_archivo      "Error abriendo archivo: %s\n"
#define TXT_PNG2MSX_Origen_PNG                  "Origen PNG:\n"
#define TXT_PNG2MSX_Destino                     "Destino:\n"
#define TXT_PNG2MSX_salida                      " - salida  = %s\n"
#define TXT_PNG2MSX_entrada                     " - entrada = %s\n"
#define TXT_PNG2MSX_x                           " - x       = %u\n"
#define TXT_PNG2MSX_y                           " - y       = %u\n"
#define TXT_PNG2MSX_ancho                       " - ancho   = %u\n"
#define TXT_PNG2MSX_alto                        " - alto    = %u\n"
#define TXT_PNG2MSX_bits                        " - bits    = %u\n"
#define TXT_PNG2MSX_canales                     " - canales = %u\n"
#define TXT_PNG2MSX_Tipo_de_color               " - Tipo de color = "
#define TXT_PNG2MSX_Escala_de_Grises            "Escala de Grises\n"
#define TXT_PNG2MSX_Paleta                      "Paleta\n"
#define TXT_PNG2MSX_Num_colores_paleta          " - Num. colores de la palera = %d\n"
#define TXT_PNG2MSX_El_indice_0_paleta          "El índice 0 de la paleta es transparente\n"
#define TXT_PNG2MSX_Escala_Grises_Alpha         "Escala de Grises con Alpha\n"
#define TXT_PNG2MSX_No_definido                 "No definido.\n"

#define TXT_PNG2MSX_El_valor_x_sobrepasa        "El valor de x sobrepasa el ancho de la imágen.\n"
#define TXT_PNG2MSX_Se_usara_como_x             "Se usará %u como valor de x.\n"
#define TXT_PNG2MSX_El_valor_y_sobrepasa        "El valor de y sobrepasa la altura de la imágen.\n"
#define TXT_PNG2MSX_Se_usara_como_y             "Se usará %u como valor de y.\n"
#define TXT_PNG2MSX_La_anchura_sobrepasa        "La anchura de la selección sobrepasa la de la imagen.\n"
#define TXT_PNG2MSX_Se_usara_como_ancho         "Se usará %u como ancho.\n"
#define TXT_PNG2MSX_La_altura_sobrepasa         "La altura de la selección sobrepasa la de la imagen.\n"
#define TXT_PNG2MSX_Se_usara_como_alto          "Se usará %u como alto.\n"

#define TXT_PNG2MSX_Te_recuerdo_memoria         "Te recuerdo que la memoria de  video  del  MSX  trabaja  con\n" \
                                                "baldosas de 8x8 pixels.\n"
#define TXT_PNG2MSX_El_ancho_multiplo_8         "El ancho de la imágen debe ser múltiplo de 8.\n"
#define TXT_PNG2MSX_La_altura_multiplo_8        "La altura de la imágen debe ser múltiplo de 8.\n"

#define TXT_PNG2MSX_Imagen_demasiado_grande     "El tamaño de la imágen es demasiado grande para  la  memoria\n" \
                                                "de video de un  MSX. La  resolución  normal  máxima  es  de:\n" \
                                                "    256 x 192 pixels = 49152 pixels.\n"

#define TXT_PNG2MSX_Algunas_opciones_incomp     "Algunas opciones serán incompatibles...\n"

#define TXT_PNG2MSX_Al_ser_el_ancho_diferente   "Al ser el  ancho  diferente  de  256 pixels,  los  datos  no\n" \
                                                "estarán alineados y será su problema.  Si  desea  que  estén\n" \
                                                "alineados utilize la opción -w256\n"

#define TXT_PNG2MSX_Filtro_colision_color_act   " - Filtro de colisón de color activo\n"
#define TXT_PNG2MSX_Contraste_automatico        " - Contraste automático\n"
#define TXT_PNG2MSX_Contraste_manual            " - Contraste manual (%d)\n"
#define TXT_PNG2MSX_Umbral_de_Gris              " - Umbral de Gris = %u\n"
#define TXT_PNG2MSX_Cambia_color_0_por_1        " - Cambia el color 0 por el 1\n"
#define TXT_PNG2MSX_Cambia_color_1_por_0        " - Cambia el color 1 por el 0\n"
#define TXT_PNG2MSX_Color_de_Fondo              " - Color de Fondo = %u\n"
#define TXT_PNG2MSX_Baldosas_repetidas_bits     " - Baldosas repetidas bits = %u\n"
#define TXT_PNG2MSX_Optimizacion_activada_modo  " - Optimización activada, modo = %u\n"
#define TXT_PNG2MSX_Salida_PNG_con_canal_alfa   " - Salida PNG con canal alfa\n"
#define TXT_PNG2MSX_Salida_PNG_sin_canal_alfa   " - Salida PNG sin canal alfa\n"
#define TXT_PNG2MSX_Salida_en_bruto_para_ASM    " - Salida en bruto para ASM activada\n"
#define TXT_PNG2MSX_Salida_Pletter_con_longitud " - Salida Pletter5 con longitud activada\n"
#define TXT_PNG2MSX_Salida_Pletter_sin_longitud " - Salida Pletter5 sin longitud activada\n"
#define TXT_PNG2MSX_Salida_SC2_activada         " - Salida SC2 activada\n"
#define TXT_PNG2MSX_Salida_BASIC_ASCII_activada " - Salida BASIC ASCII activada\n"
#define TXT_PNG2MSX_punto_medio                 " - punto medio = %f\n"
#define TXT_PNG2MSX_contraste                   " - contraste = %d\n"

#define TXT_PNG2MSX_Procesando_la_imagen        "Procesando la imagen...\n"
#define TXT_PNG2MSX_Buscando_Baldosas_parecidas "Buscando Baldosas parecidas...\n"
#define TXT_PNG2MSX_Baldosas_parecidas          "Baldosas parecidas = %u\n"

#define TXT_PNG2MSX_Exportando_a_PNG            "Exportando a PNG:\n"
#define TXT_PNG2MSX_Exportando_Baldosas_a_PNG   "Exportando Baldosas a PNG:\n"
#define TXT_PNG2MSX_Exportando_CHR_CLR_y_NAM    "Exportando CHR, CLR y NAM para Ensamblador:\n"
#define TXT_PNG2MSX_Exportando_a_SC2_BLOAD      "Exportando a SC2 para BLOAD\"ARCHIVO.SC2\",S:\n"
#define TXT_PNG2MSX_Exportando_a_BASIC_ASCII    "Exportando a BASIC ASCII para LOAD\"ARCHIVO.ASC\":\n"
#define TXT_PNG2MSX_Comprimiendo_con_Pletter    "Comprimiendo con Pletter v0.5c1 - www.xl2s.tk:\n"

#define TXT_PNG2MSX_Quitando_baldosas_repetidas "Quitando baldosas repetidas...\n"
#define TXT_PNG2MSX_Quitadas_baldosas_repetidas "Quitadas las baldosas repetidas...\n"

#define TXT_PNG2MSX_baldosas_totales            "baldosas totales   = %u\n"
#define TXT_PNG2MSX_baldosas_totales_u_u        "baldosas totales   %u = %u\n"
#define TXT_PNG2MSX_baldosas_repetidas          "baldosas repetidas = %u\n"
#define TXT_PNG2MSX_baldosas_repetidas_u_u      "baldosas repetidas %u = %u\n"
#define TXT_PNG2MSX_baldosas_restantes          "baldosas restantes = %u\n"
#define TXT_PNG2MSX_baldosas_restantes_u_u      "baldosas restantes %u = %u\n"

#define TXT_PNG2MSX_Demasiadas_baldosas         "Demasiadas baldosas para VRAM.\n"
#define TXT_PNG2MSX_Demasiados_Nombres          "Demasiados Nombres para la VRAM\n"
#define TXT_PNG2MSX_Demasiado_grande_VRAM_MSX1  "Demasiado grande para la VRAM del MSX1\n"

#define TXT_PNG2MSX_Intentando_optimizar_111    "Intentado optimizar a un sólo bloque...\n"
#define TXT_PNG2MSX_Intentando_optimizar_121    "Intentado optimizar a dos bloques 121...\n"
#define TXT_PNG2MSX_Intentando_optimizar_113    "Intentado optimizar a dos bloques 113...\n"
#define TXT_PNG2MSX_Intentando_optimizar_123    "Intentado optimizar a tres bloques...\n"

#define TXT_PNG2MSX_No_hay_baldosas_repetidas   "No hay baldosas repetidas.\n"
#define TXT_PNG2MSX_No_se_puede_reducir_un_bloq "No se puede reducir a un sólo bloque sobran %u baldosas\n"
#define TXT_PNG2MSX_No_merece_la_pena           "No merece la pena.\n"

#define TXT_PNG2MSX_Demasiadas_baldosas_2_banc  "Hay demasiadas baldosas para dos bancos.\n"

#define TXT_PNG2MSX_Optimizado_a_un_bloque      "Optimizado a un sólo bloque de VRAM...\n"
#define TXT_PNG2MSX_Optimizado_a_dos_bloques    "Optimizado a dos bloques de VRAM...\n"
#define TXT_PNG2MSX_Optimizado_a_tres_bloques   "Optimizado a tres bloques de VRAM...\n"

#define TXT_PNG2MSX_Error_en_patrones_y_colores "Error en patrones y colores: (%u,%u)\n"
#define TXT_PNG2MSX_Error_iniciando_PNG         "Error iniciando PNG.\n"
#define TXT_PNG2MSX_Error_iniciando_Pixels      "Error iniciando Pixels.\n"
#define TXT_PNG2MSX_Error_iniciando_Paleta      "Error iniciando Paleta.\n"

#else

#define TXT_PNG2MSX_Conversor   "    PNG  to  MSX  image  converter,  by  PipaGerardo.\n"
#define TXT_PNG2MSX_Version     "    Version beta 8 ( 2018 )       pipagerardo@gmail.com\n"
#ifdef WINDOWS
#define TXT_PNG2MSX_Opciones    "png2msx.exe source.png [output_name] [-options]\n"
#else
#define TXT_PNG2MSX_Opciones    "png2msx source.png [output_name] [-options]\n"
#endif
#define TXT_PNG2MSX_Op01        "\t-x[Number]\tPosition of x in pixels [ 0 ~ 2048 ]\n"
#define TXT_PNG2MSX_Op02        "\t-y[Number]\tPosition of y in pixels [ 0 ~ 2048 ]\n"
#define TXT_PNG2MSX_Op03        "\t-w[Number]\tWidth in pixels [ 0 ~ 2048 ]\n"
#define TXT_PNG2MSX_Op04        "\t-h[Number]\tHigh  in pixels [ 0 ~ 2048 ]\n"
#define TXT_PNG2MSX_Op05        "\t\t\t\"w*h\" must be equal to or less than 49152\n"
#define TXT_PNG2MSX_Op06        "\t\t\tSi \"w\" not 256 the image will not be aligned\n"
#define TXT_PNG2MSX_Op07        "\t-g[Number]\tThreshold of Gray [ 0 ~ 255 ]\n"
#define TXT_PNG2MSX_Op08        "\t-b[Number]\tContrast [ -255 ~ 255 ]\n"
#define TXT_PNG2MSX_Op09        "\t\t\t-b   - Auto Contrast\n"
#define TXT_PNG2MSX_Op10        "\t\t\t-b16 - Whites whiter and vice versa\n"
#define TXT_PNG2MSX_Op11        "\t\t\t-b-8 - Darker targets and vice versa\n"
#define TXT_PNG2MSX_Op12        "\t-c\t\tColor Collision Filter (8 pixels)\n"
#define TXT_PNG2MSX_Op13        "\t-e\t\tPass conversion error to next pixel\n"
#define TXT_PNG2MSX_Op14        "\t-z\t\tChange color 0 to 1\n"
#define TXT_PNG2MSX_Op15        "\t-n\t\tChange color 1 to 0\n"
#define TXT_PNG2MSX_Op16        "\t-k[Number]\tBackground color from 0 to 15\n"
#define TXT_PNG2MSX_Op17        "\t-f[Number]\tSearch for similar tiles from 1 to 16 bits\n"
#define TXT_PNG2MSX_Op18        "\t\t\tI recommend values from 2 to 6\n"
#define TXT_PNG2MSX_Op19        "\t-o[Number]\tRemove tiles repeated in VRAM:\n"
#define TXT_PNG2MSX_Op20        "\t\t\t-o0 Repeat tile  1 VRAM bench   1-1-1\n"
#define TXT_PNG2MSX_Op21        "\t\t\t-o1 Repeat tiles 2 VRAM benches 1-2-1\n"
#define TXT_PNG2MSX_Op22        "\t\t\t-o2 Repeat tiles 2 VRAM benches 1-1-3\n"
#define TXT_PNG2MSX_Op23        "\t\t\t-o3 Repeat tiles 2 VRAM benches 1-2-3\n"
#define TXT_PNG2MSX_Op24        "\t\t\t-o4 Unconstrained repeated tiles\n"
#define TXT_PNG2MSX_Op25        "\t-p\t\tOutput PNG file\n"
#define TXT_PNG2MSX_Op26        "\t-a\t\tOutput PNG file with Alpha channel\n"
#define TXT_PNG2MSX_Op27        "\t-r\t\tRaw File for ASM: CHR, CLS y NAM\n"
#define TXT_PNG2MSX_Op28        "\t-t\t\tRaw File Pletter5 without length\n"
#define TXT_PNG2MSX_Op29        "\t-l\t\tRaw File Pletter5 with length\n"
#define TXT_PNG2MSX_Op30        "\t-s\t\tSC2 file for BLOAD\"ARCHIVO.SC2\",S\n"
#define TXT_PNG2MSX_Op31        "\t-m\t\tBASIC ASCII file loading SC2\n"
#define TXT_PNG2MSX_Ejemplo1    "\nExample:\n"
#ifdef WINDOWS
#define TXT_PNG2MSX_Ejemplo2    "png2msx.exe source.png output -x8 -y32 -w256 -h192 -c -p\n"
#else
#define TXT_PNG2MSX_Ejemplo2    "png2msx source.png output -x8 -y32 -w256 -h192 -c -p\n"
#endif
#define TXT_PNG2MSX_Obteniendo_datos            "Getting data ...\n"
#define TXT_PNG2MSX_entrada_dos_puntos          "entry: %s\n"
#define TXT_PNG2MSX_Solo_se_permite_PNG         "Only the PNG extension is allowed.\n"
#define TXT_PNG2MSX_Es_obligatorio_PNG          "A PNG extension is required.\n"
#define TXT_PNG2MSX_Error_abriendo_archivo      "Error opening file: %s\n"
#define TXT_PNG2MSX_Origen_PNG                  "PNG Source:\n"
#define TXT_PNG2MSX_Destino                     "Destination:\n"
#define TXT_PNG2MSX_salida                      " - output   = %s\n"
#define TXT_PNG2MSX_entrada                     " - input    = %s\n"
#define TXT_PNG2MSX_x                           " - x        = %u\n"
#define TXT_PNG2MSX_y                           " - y        = %u\n"
#define TXT_PNG2MSX_ancho                       " - width    = %u\n"
#define TXT_PNG2MSX_alto                        " - high     = %u\n"
#define TXT_PNG2MSX_bits                        " - bits     = %u\n"
#define TXT_PNG2MSX_canales                     " - channels = %u\n"
#define TXT_PNG2MSX_Tipo_de_color               " - Type of color = "
#define TXT_PNG2MSX_Escala_de_Grises            "Grayscale\n"
#define TXT_PNG2MSX_Paleta                      "Palette\n"
#define TXT_PNG2MSX_Num_colores_paleta          " - Number of palette colors = %d\n"
#define TXT_PNG2MSX_El_indice_0_paleta          "The index 0 of the palette is transparent\n"
#define TXT_PNG2MSX_Escala_Grises_Alpha         "Greyscale with Alpha\n"
#define TXT_PNG2MSX_No_definido                 "Undefined.\n"

#define TXT_PNG2MSX_El_valor_x_sobrepasa        "The value of x exceeds the width of the image.\n"
#define TXT_PNG2MSX_Se_usara_como_x             "We will use %u as the value of x.\n"
#define TXT_PNG2MSX_El_valor_y_sobrepasa        "The value of y exceeds the height of the image.\n"
#define TXT_PNG2MSX_Se_usara_como_y             "We will use %u as the value of y.\n"
#define TXT_PNG2MSX_La_anchura_sobrepasa        "The width of the selection exceeds that of the image.\n"
#define TXT_PNG2MSX_Se_usara_como_ancho         "%u will be used as width.\n"
#define TXT_PNG2MSX_La_altura_sobrepasa         "The height of the selection exceeds that of the image.\n"
#define TXT_PNG2MSX_Se_usara_como_alto          "%u will be used as height.\n"
#define TXT_PNG2MSX_Te_recuerdo_memoria         "I remember that MSX video memory works with tiles of 8x8 pixels.\n"
#define TXT_PNG2MSX_El_ancho_multiplo_8         "The width of the image should be multiple of 8.\n"
#define TXT_PNG2MSX_La_altura_multiplo_8        "The height of the image should be multiple of 8.\n"

#define TXT_PNG2MSX_Imagen_demasiado_grande     "The size of the image is too large for  the video memory  of\n" \
                                                "an MSX. The maximum normal resolution is:\n" \
                                                "    256 x 192 pixels = 49152 pixels.\n"

#define TXT_PNG2MSX_Algunas_opciones_incomp     "Some options will be incompatible ...\n"


#define TXT_PNG2MSX_Al_ser_el_ancho_diferente   "Being the different width of 256 pixels, the data  will  not\n" \
                                                "be aligned and will be your problem. If you want them  to be\n" \
                                                "aligned use the -w256 option\n"

#define TXT_PNG2MSX_Filtro_colision_color_act   " - Active color collision filter\n"
#define TXT_PNG2MSX_Contraste_automatico        " - Auto Contrast\n"
#define TXT_PNG2MSX_Contraste_manual            " - Manual Contrast (%d)\n"
#define TXT_PNG2MSX_Umbral_de_Gris              " - Threshold of Gray = %u\n"
#define TXT_PNG2MSX_Cambia_color_0_por_1        " - Change color 0 to 1\n"
#define TXT_PNG2MSX_Cambia_color_1_por_0        " - Change color 1 to 0\n"
#define TXT_PNG2MSX_Color_de_Fondo              " - Background color = %u\n"
#define TXT_PNG2MSX_Baldosas_repetidas_bits     " - Repeated tiles bits = %u\n"
#define TXT_PNG2MSX_Optimizacion_activada_modo  " - Optimization enabled, mode = %u\n"
#define TXT_PNG2MSX_Salida_PNG_con_canal_alfa   " - PNG output with alpha channel\n"
#define TXT_PNG2MSX_Salida_PNG_sin_canal_alfa   " - PNG output without alpha channel\n"
#define TXT_PNG2MSX_Salida_en_bruto_para_ASM    " - Raw output for ASM enabled\n"
#define TXT_PNG2MSX_Salida_Pletter_con_longitud " - Pletter5 output with activated length\n"
#define TXT_PNG2MSX_Salida_Pletter_sin_longitud " - Pletter5 output without length on\n"
#define TXT_PNG2MSX_Salida_SC2_activada         " - Output SC2 on\n"
#define TXT_PNG2MSX_Salida_BASIC_ASCII_activada " - BASIC ASCII output activated\n"
#define TXT_PNG2MSX_punto_medio                 " - middle point = %f\n"
#define TXT_PNG2MSX_contraste                   " - contrast = %d\n"

#define TXT_PNG2MSX_Procesando_la_imagen        "Processing image ...\n"
#define TXT_PNG2MSX_Buscando_Baldosas_parecidas "Looking for similar tiles ...\n"
#define TXT_PNG2MSX_Baldosas_parecidas          "Similar tiles = %u\n"

#define TXT_PNG2MSX_Exportando_a_PNG            "Exporting PNG:\n"
#define TXT_PNG2MSX_Exportando_Baldosas_a_PNG   "Exporting Tiles to PNG:\n"
#define TXT_PNG2MSX_Exportando_CHR_CLR_y_NAM    "Exporting CHR, CLR and NAM to Assembler:\n"
#define TXT_PNG2MSX_Exportando_a_SC2_BLOAD      "Exporting SC2 to BLOAD\"FILE.SC2\",S:\n"
#define TXT_PNG2MSX_Exportando_a_BASIC_ASCII    "Exporting BASIC ASCII to LOAD\"FILE.ASC\":\n"
#define TXT_PNG2MSX_Comprimiendo_con_Pletter    "Compressing with Pletter v0.5c1 - www.xl2s.tk:\n"

#define TXT_PNG2MSX_Quitando_baldosas_repetidas "Removing repeated tiles...\n"
#define TXT_PNG2MSX_Quitadas_baldosas_repetidas "Removed tiles repeated...\n"

#define TXT_PNG2MSX_baldosas_totales            "total tiles     = %u\n"
#define TXT_PNG2MSX_baldosas_totales_u_u        "total tiles     %u = %u\n"
#define TXT_PNG2MSX_baldosas_repetidas          "repeated tiles  = %u\n"
#define TXT_PNG2MSX_baldosas_repetidas_u_u      "repeated tiles  %u = %u\n"
#define TXT_PNG2MSX_baldosas_restantes          "tiles remaining = %u\n"
#define TXT_PNG2MSX_baldosas_restantes_u_u      "tiles remaining %u = %u\n"

#define TXT_PNG2MSX_Demasiadas_baldosas         "Too many tiles for VRAM.\n"
#define TXT_PNG2MSX_Demasiados_Nombres          "Too many names for the VRAM\n"
#define TXT_PNG2MSX_Demasiado_grande_VRAM_MSX1  "Too large for the MSX1 VRAM\n"

#define TXT_PNG2MSX_Intentando_optimizar_111    "Trying to optimize a single block...\n"
#define TXT_PNG2MSX_Intentando_optimizar_121    "Trying to optimize two blocks 121...\n"
#define TXT_PNG2MSX_Intentando_optimizar_113    "Trying to optimize two blocks 113...\n"
#define TXT_PNG2MSX_Intentando_optimizar_123    "Trying to optimize three blocks...\n"

#define TXT_PNG2MSX_No_hay_baldosas_repetidas   "There are no repeated tiles.\n"
#define TXT_PNG2MSX_No_se_puede_reducir_un_bloq "You can not reduce to a single block %u tiles remaining\n"
#define TXT_PNG2MSX_No_merece_la_pena           "Not worth it.\n"

#define TXT_PNG2MSX_Demasiadas_baldosas_2_banc  "There are too many tiles for two benches.\n"

#define TXT_PNG2MSX_Optimizado_a_un_bloque      "Optimized to a single block of VRAM...\n"
#define TXT_PNG2MSX_Optimizado_a_dos_bloques    "Optimized for two VRAM blocks...\n"
#define TXT_PNG2MSX_Optimizado_a_tres_bloques   "Optimized for three VRAM blocks...\n"

#define TXT_PNG2MSX_Error_en_patrones_y_colores "Error in patterns and colors: (%u,%u)\n"
#define TXT_PNG2MSX_Error_iniciando_PNG         "Error initiating PNG.\n"
#define TXT_PNG2MSX_Error_iniciando_Pixels      "Error initiating Pixels.n"
#define TXT_PNG2MSX_Error_iniciando_Paleta      "Error initiating Palette.\n"

#endif

#endif
