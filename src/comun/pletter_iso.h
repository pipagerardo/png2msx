#ifndef _PLETTER_ISO_
#define _PLETTER_ISO_

#ifdef SPANISH

#define TXT_Pletter_Error_writing_file      "¡Error escribiendo archivo!\n"
#define TXT_Pletter_Error_opening_file      "Error abriendo archivo: %s\n"
#define TXT_Pletter_Filesize_error          "Error en el tamaño del archivo\n"
#define TXT_Pletter_No_inputfile            "Sin archivo de entrada\n"
#define TXT_Pletter_version                 "Pletter v0.5c1 - www.xl2s.tk\tmodificado por PipaGerardo\n"
#ifdef WINDOWS
#define TXT_Pletter_Usage                   "\nUso:\npletter.exe [-s[ave_length]] sourcefile [[offset [length]] [destinationfile]]\n"
#else
#define TXT_Pletter_Usage                   "\nUso:\npletter [-s[ave_length]] sourcefile [[offset [length]] [destinationfile]]\n"
#endif
#define TXT_Pletter_Option_save             "\t-s\t\t\tguarda la longitud del archivo\n"
#define TXT_Pletter_Option_sourcefile       "\tsourcefile\t\tarchivo de origen\n"
#define TXT_Pletter_Option_offset           "\t-offset [length]\tdesplazamiento [longitud]\n"
#define TXT_Pletter_Option_destinationfile  "\tdestinationfile\t\tarchivo de destino\n"
#define TXT_Pletter_savelength              "guarda longitud    = %u\n"
#define TXT_Pletter_sourcefilename          "archivo de origen  = %s\n"
#define TXT_Pletter_offset                  "desplazamiento     = %u\n"
#define TXT_Pletter_destfilename            "archivo de destino = %s\n"
#define TXT_Pletter_length                  "longitud = %u\n"

#else

#define TXT_Pletter_Error_writing_file      "Error writing file!\n"
#define TXT_Pletter_Error_opening_file      "Error opening file: %s\n"
#define TXT_Pletter_Filesize_error          "Filesize error\n"
#define TXT_Pletter_No_inputfile            "No inputfile\n"
#define TXT_Pletter_version                 "Pletter v0.5c1 - www.xl2s.tk\tmodified by PipaGerardo\n"
#ifdef WINDOWS
#define TXT_Pletter_Usage                   "\nUsage:\npletter.exe [-s[ave_length]] sourcefile [[offset [length]] [destinationfile]]\n"
#else
#define TXT_Pletter_Usage                   "\nUsage:\npletter [-s[ave_length]] sourcefile [[offset [length]] [destinationfile]]\n"
#endif
#define TXT_Pletter_Option_save             "\t-s\t\t\tsave file length\n"
#define TXT_Pletter_Option_sourcefile       "\tsourcefile\t\tsource file\n"
#define TXT_Pletter_Option_offset           "\t-offset [length]\tdisplacement [longitud]\n"
#define TXT_Pletter_Option_destinationfile  "\tdestinationfile\t\tdestination file\n"
#define TXT_Pletter_savelength              "savelength     = %u\n"
#define TXT_Pletter_sourcefilename          "sourcefilename = %s\n"
#define TXT_Pletter_offset                  "offset         = %u\n"
#define TXT_Pletter_destfilename            "destfilename   = %s\n"
#define TXT_Pletter_length                  "length = %u\n"

#endif

#endif
