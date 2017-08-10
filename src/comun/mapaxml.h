#ifndef _MAPAXML_
#define _MAPAXML_

#include "funciones.h"
#include "matriz.h"
#include "lista.h"
#include "color.h"
#include "cadena.h"
#include "archivo.h"
#include "contenedor.h"
#include "compresor.h"

enum EncodingType    { no_encoding, base64, csv, tile };
enum CompressionType { no_compression, zlib, gzip };
struct XML_data {
	EncodingType    encoding;		// The encoding used to encode the tile layer data
	CompressionType compression;	// The compression used to compress the tile layer data.
	Matriz< Uint16 > *array;
};

enum FormatType	{ no_format, bmp, png, jpg, gif };
struct XML_image {
	FormatType   format;	// File extensions
	Cadena       source;	// The reference to the tileset image file
	Color        trans;		// Defines a specific color that is treated as transparent (example value: "#FF00FF" for magenta). Up until Tiled 0.12, this value is written out without a # but this is planned to change.
 	unsigned int width; 	// The image width in pixels (optional, used for tile index correction when the image changes)
	unsigned int height;	// The image height in pixels (optional)
	// Can contain: data
};

struct XML_property {
	Cadena name;
	Cadena value;
};

struct XML_tile {
	unsigned int id;
	Lista< XML_property > property; // <properties> ... </properties>
};

struct XML_tileset {
	unsigned int firstgid;		// The first global tile ID of this tileset.
 // Cadena source;				// If this tileset is stored in an external TSX (Tile Set XML) file.
 	Cadena name; 				// The name of this tileset.
 	unsigned int tilewidth;		// The (maximum) width of the tiles in this tileset.
	unsigned int tileheight;	// The (maximum) height of the tiles in this tileset.
	unsigned int tilecount;     // El número de tiles.  
	unsigned int spacing;		// The spacing in pixels between the tiles in this tileset (applies to the tileset image).
	unsigned int margin;		// The margin around the tiles in this tileset (applies to the tileset image).
	XML_image image;
	Lista< XML_property > property;
	Lista< XML_tile > tile;
};

struct XML_layer {
	Cadena name;			// The name of the layer.
	unsigned int x; 		// The x coordinate of the layer in tiles. Defaults to 0 and can no longer be changed in Tiled Qt.
	unsigned int y;			// The y coordinate of the layer in tiles. Defaults to 0 and can no longer be changed in Tiled Qt.
	unsigned int width;		// The width of the layer in tiles. Traditionally required, but as of Tiled Qt always the same as the map width.
	unsigned int height;	// The height of the layer in tiles. Traditionally required, but as of Tiled Qt always the same as the map height.
	float opacity;			// The opacity of the layer as a value from 0 to 1. Defaults to 1.
	bool visible; 			// Whether the layer is shown (1) or hidden (0). Defaults to 1.
	Lista< XML_property > property;
	XML_data data;
};

struct XML_object {
 	unsigned int id;	// Unique ID of the object. 
	Cadena name;
	Cadena type;
	float x;			// in pixels.
	float y;			// in pixels.
	float width;		// in pixels.
	float height;		// in pixels.
	float rotation; 	// The rotation of the object in degrees clockwise (defaults to 0). 
	unsigned int gid;	// An reference to a tile (optional). 
	bool visible;		// Whether the object is shown (1) or hidden (0). Defaults to 1. (since 0.9)
	Lista< XML_property > property;
	// Can contain: properties, ellipse (since 0.9), polygon, polyline, image
};

struct XML_objectgroup {
	Cadena name;
	Color  color;
	unsigned int x;  		// in tiles 
	unsigned int y;  		// in tiles
	unsigned int width; 	// in tiles.
	unsigned int height; 	// in tiles
	float opacity; 			// The opacity of the layer as a value from 0 to 1. Defaults to 1. 
	bool visible;			//  Whether the layer is shown (1) or hidden (0). Defaults to 1. 
	// draworder; 			// Whether the objects are drawn according to the order of appearance ("index") or sorted by their y-coordinate ("top-down"). Defaults to "top-down".
	Lista< XML_property > property;
	Lista< XML_object > object;
	// Can contain: properties
};

enum OrientationType { no_orientation, orthogonal, isometric, staggered };
enum RenderOrderType { no_renderorder, right_down, right_up, left_down, left_up };
struct XML_map {
	Cadena version;					// The TMX format version, generally 1.0.			
	OrientationType orientation;	// ["orthogonal"] | "isometric" | "staggered"
	unsigned int width;				// The map width in tiles. 
	unsigned int height;			// The map height in tiles. 
	unsigned int tilewidth;			// The width of a tile. 
	unsigned int tileheight;		// The height of a tile.
	Color backgroundcolor;			// The background color of the map. (since 0.9, optional) 
	RenderOrderType renderorder;	// The order in which tiles on tile layers are rendered.
	Lista< XML_property > property;
	Lista< XML_tileset > tileset;
	Lista< XML_layer   > layer;
	Lista< XML_objectgroup > objectgroup;
	// Can contain: properties, imagelayer
};

class MapaXML {
public:
	
	// Construcción y destrucción:
	MapaXML();
	~MapaXML();
	
	bool inicia( const char* ruta, const char* archivo );
	void quita();

	XML_map map;
	
private:
	Cadena  m_cadena;
	Cadena  m_parte;
	Archivo m_archivo;
	size_t  m_posicion;
	void procesaXML();
	void procesaREM();
	void procesaMAP();
	void procesaTILESET();
	void procesaIMAGE( XML_image &image );
	void procesaTILE( Lista< XML_tile > &tile );
	void procesaPROPERTIES( Lista< XML_property > &property );
	void procesaLAYER();
	void procesaDATA( XML_data &data );
	void procesaBASE64(     Matriz< Uint16 > *array );
	void procesaBASE64ZLIB( Matriz< Uint16 > *array );
 // void procesaBASE64GZIP( Matriz< Uint16 > *array ); // No funciona.
	void procesaCSV(    Matriz< Uint16 > *array );
	void procesaTILES(  Matriz< Uint16 > *array );
	void procesaOBJECT( Lista< XML_object > &object );
	void procesaOBJECTGROUP();
};

#endif

