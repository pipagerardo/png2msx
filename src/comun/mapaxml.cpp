#include "mapaxml.h"

MapaXML::MapaXML() {
}

MapaXML::~MapaXML() {
	quita();
}
	
bool MapaXML::inicia( const char* ruta, const char* archivo ) {

	quita(); // borramos todo.

	m_cadena = ruta;
	m_cadena << archivo;
	if( !m_archivo.inicia( m_cadena.dameCadena(), "r" ) ) return false;
	m_cadena.quita();
	
	m_parte.inicia( 64, 8 );
	m_cadena.inicia( 512, 128 );

	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;
		if( m_cadena.busca( "<?xml",         m_posicion ) ) procesaXML();
		if( m_cadena.busca( "<!--",          m_posicion ) ) procesaREM();
		if( m_cadena.busca( "<map",          m_posicion ) ) procesaMAP();
		if( m_cadena.busca( "<properties>",  m_posicion ) ) procesaPROPERTIES( map.property );
		if( m_cadena.busca( "<tileset",      m_posicion ) ) procesaTILESET();
		if( m_cadena.busca( "<layer",        m_posicion ) ) procesaLAYER();
		if( m_cadena.busca( "<objectgroup ", m_posicion ) ) procesaOBJECTGROUP();
		if( m_cadena.busca( "</map>",        m_posicion ) ) break;
	}

	m_parte.quita();
	m_cadena.quita();	
	m_archivo.quita();
	
	return true;
}

void MapaXML::quita() {
	
	m_archivo.quita();
	m_cadena.quita();
	m_parte.quita();
	
	map.version.quita();			
	map.orientation = orthogonal;
	map.width       = 0;
	map.height      = 0;
	map.tilewidth   = 0;
	map.tileheight  = 0;
	map.backgroundcolor.quita();
	map.backgroundcolor.tomaAlfa( 0 );
	map.renderorder = left_up;
	Lista_Iterador< XML_property > property_it( map.property );
	for( ; property_it.noFinal(); ++property_it ) {
		property_it->name.quita();
		property_it->value.quita();
	}
	map.property.quita();
	
	Lista_Iterador< XML_tileset > tileset_it( map.tileset );
	for( ; tileset_it.noFinal(); ++tileset_it ) {
		tileset_it->firstgid = 0;
	 // tileset_it->source.quita();
		tileset_it->name.quita();
		tileset_it->tilewidth  = 0;
		tileset_it->tileheight = 0;
		tileset_it->tilecount  = 0;
		tileset_it->spacing    = 0;
		tileset_it->margin     = 0;
 		tileset_it->image.format = no_format;
		tileset_it->image.source.quita();
		tileset_it->image.trans.quita();
		tileset_it->image.trans.tomaAlfa( 0 );
 		tileset_it->image.width  = 0;
		tileset_it->image.height = 0;
		// tileset_it->image.data;
		property_it.alInicio( tileset_it->property );
		for( ; property_it.noFinal(); ++property_it ) {
			property_it->name.quita();
			property_it->value.quita();
		}	
		tileset_it->property.quita();
		Lista_Iterador< XML_tile > tile_it( tileset_it->tile );
		for( ; tile_it.noFinal(); ++tile_it ) {
			tile_it->id = 0;
			property_it.alInicio( tile_it->property );
			for( ; property_it.noFinal(); ++property_it ) {
				property_it->name.quita();
				property_it->value.quita();
			}
			tile_it->property.quita();
		}
		tileset_it->tile.quita();
	}
	map.tileset.quita();
	
	Lista_Iterador< XML_layer > layer_it( map.layer );
	for( ; layer_it.noFinal(); ++layer_it ) {
		layer_it->name.quita();
		layer_it->x = 0;
		layer_it->y = 0;
		layer_it->width  = 0;
		layer_it->height = 0;
		layer_it->opacity = 1.0f;
		layer_it->visible = true;
		property_it.alInicio( layer_it->property );
		for( ; property_it.noFinal(); ++property_it ) {
			property_it->name.quita();
			property_it->value.quita();
		}
		layer_it->property.quita();
		layer_it->data.encoding    = no_encoding;
		layer_it->data.compression = no_compression;
		if( layer_it->data.array != NULL ) {
			layer_it->data.array->quita();
			delete layer_it->data.array;
			layer_it->data.array = NULL;
		}
	}
	map.layer.quita();

	Lista_Iterador< XML_objectgroup > objectgroup_it( map.objectgroup );
	for( ; objectgroup_it.noFinal(); ++objectgroup_it ) {
		objectgroup_it->name.quita();
		objectgroup_it->color.quita();
		objectgroup_it->x = 0;
		objectgroup_it->y = 0;
		objectgroup_it->width  = 0;
		objectgroup_it->height = 0;
		objectgroup_it->opacity = 1.0f; 
		objectgroup_it->visible = true;
 	 // objectgroup_it->draworder;
		property_it.alInicio( objectgroup_it->property );
		for( ; property_it.noFinal(); ++property_it ) {
			property_it->name.quita();
			property_it->value.quita();
		}
 	 	objectgroup_it->property.quita();
		Lista_Iterador< XML_object > object_it( objectgroup_it->object );
		for( ; object_it.noFinal(); ++object_it ) {
			object_it->id = 0;
			object_it->name.quita();
			object_it->type.quita();
			object_it->x = 0.0f;
			object_it->y = 0.0f;
			object_it->width = 0.0f;
			object_it->height = 0.0f;
			object_it->rotation = 0.0f;
			object_it->gid = 0;
			object_it->visible = true;
			property_it.alInicio( object_it->property );
			for( ; property_it.noFinal(); ++property_it ) {
				property_it->name.quita();
				property_it->value.quita();
			}
 	 		object_it->property.quita();
		}
 	 	objectgroup_it->object.quita();
 	}
	map.objectgroup.quita();
 
}

void MapaXML::procesaXML() {
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		if( m_cadena.busca( "?>",  m_posicion ) ) break;
	} while( m_archivo.lee( m_cadena ) );
}

void MapaXML::procesaREM() {
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		if( m_cadena.busca( "-->",  m_posicion ) ) break;
	} while( m_archivo.lee( m_cadena ) );
}
	
void MapaXML::procesaMAP() {
	// SDL_Log( "procesaMAP()" );

	map.version.quita(); 
	map.orientation = orthogonal;
	map.width       = 0;
	map.height      = 0;
	map.tilewidth   = 0;
	map.tileheight  = 0;
	map.backgroundcolor.quita();
	map.backgroundcolor.tomaAlfa( 0 );
	map.renderorder = left_up;	
 // map.property;
 //	map.tileset;
 //	map.layer;
 //	map.objectgroup;
	
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {
			
			if( m_parte == "version" ) {
				map.version.inicia( 8, 8 );
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				map.version = m_parte;
			}
					
			if( m_parte == "orientation" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				if     ( m_parte == "orthogonal" ) map.orientation = orthogonal;
				else if( m_parte == "isometric"  ) map.orientation = isometric;
				else if( m_parte == "staggered"  ) map.orientation = staggered;
				else							   map.orientation = no_orientation;
			}
			
			if( m_parte == "width" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), map.width );	
			}

			if( m_parte == "height" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), map.height );	
			}

			if( m_parte == "tilewidth" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), map.tilewidth );	
			}

			if( m_parte == "tileheight" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), map.tileheight );	
			}
		
			if( m_parte == "backgroundcolor" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				map.backgroundcolor.inicia( m_parte.dameCadena() );	
				map.backgroundcolor.tomaAlfa( 255 );
			}
			
			if( m_parte == "renderorder" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				if     ( m_parte == "right-down" ) map.renderorder = right_down;
				else if( m_parte == "right-up"   ) map.renderorder = right_up;
				else if( m_parte == "left-down"  ) map.renderorder = left_down;
				else if( m_parte == "left-up"    ) map.renderorder = left_up;
				else							   map.renderorder = no_renderorder;
			}
						
			if( m_parte == ">" ) fin = true;
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );
	
}

void MapaXML::procesaTILESET() {
	// SDL_Log( "procesaTILESET()" );
	
	XML_tileset xml_tileset;
	xml_tileset.firstgid   = 0;
 //	xml_tileset.source.quita();
	xml_tileset.name.quita();
	xml_tileset.tilewidth  = 0;
	xml_tileset.tileheight = 0;
	xml_tileset.tilecount  = 0; // tilecount="256"
	xml_tileset.spacing    = 0;
	xml_tileset.margin     = 0;
	xml_tileset.image.format = no_format;
	xml_tileset.image.source.quita();
	xml_tileset.image.trans.quita();
	xml_tileset.image.trans.tomaAlfa( 0 );
 	xml_tileset.image.width  = 0;
	xml_tileset.image.height = 0;
 // xml_tileset.property;
 // xml_tileset.tile;
	
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {
			
			if( m_parte == "<!--"   ) procesaREM();			
	
			if( m_parte == "firstgid" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_tileset.firstgid );	
			}

			if( m_parte == "source" ) {
				// xml_tileset.source.inicia( 16, 8 );
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				// xml_tileset.source = m_parte;
			}

			if( m_parte == "name" ) {
				xml_tileset.name.inicia( 8, 8 );
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				xml_tileset.name = m_parte;
			}

			if( m_parte == "tilewidth" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_tileset.tilewidth );	
			}

			if( m_parte == "tileheight" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_tileset.tileheight );	
			}
	
			if( m_parte == "tilecount" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_tileset.tilecount );	
			}
		
			if( m_parte == "spacing" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_tileset.spacing );	
			}

			if( m_parte == "margin" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_tileset.margin );	
			}
			
			if(  m_parte == "<properties>" ) {
				procesaPROPERTIES( xml_tileset.property );
			}

			if( m_parte == "<image" ) {
				procesaIMAGE(  xml_tileset.image );
			}

			if( m_parte == "<tile" ) {
				procesaTILE( xml_tileset.tile );
			}
			
			// Can contain: tile, tileoffset, properties, terraintypes
			if( m_parte == "</tileset>" ) fin = true;
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );
	
	map.tileset.meteFinal( xml_tileset );
	
}

void MapaXML::procesaIMAGE( XML_image &image ) {
	// SDL_Log( "procesaIMAGE()" );
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {
			
			if( m_parte == "format" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				if     ( m_parte == "bmp" ) image.format = bmp;
				else if( m_parte == "png" ) image.format = png;
				else if( m_parte == "jpg" ) image.format = jpg;
				else if( m_parte == "gif" ) image.format = gif;
				else						image.format = no_format;
			}
			
			if( m_parte == "source" ) {
				image.source.inicia( 16, 8 );
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				image.source = m_parte;
			}

			if( m_parte == "trans" ) {			
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				image.trans.inicia( m_parte.dameCadena() );	
				image.trans.tomaAlfa( 255 );
			}
			
			if( m_parte == "width" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), image.width );	
			}

			if( m_parte == "height" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), image.height );	
			}
			
 			// Can contain: data	
			if( m_parte == "/>" ) fin = true;
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );
}

void MapaXML::procesaTILE( Lista< XML_tile > &tile ) {
	XML_tile new_tile;
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {
			if( m_parte == "id" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), new_tile.id );
			}
			if( m_parte == "<properties>" ) {			
				procesaPROPERTIES( new_tile.property );
			}
			if( m_parte == "</tile>" ) fin = true;
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );
	// Si no hay propiedades ignoramos la baldosa.
	if( new_tile.property.dameLongitud() > 0 ) {
		tile.meteFinal( new_tile );
	}
}

void MapaXML::procesaPROPERTIES( Lista< XML_property > &property ) {
	XML_property new_property;
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {
			
			if( m_parte == "<property" ) {
				new_property.name.quita();
				new_property.value.quita();
			}
			
			if( m_parte == "name" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				new_property.name = m_parte;
			}
			if( m_parte == "value" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				new_property.value = m_parte;				
			}
			
			if( m_parte == "/>" ) {
				property.meteFinal( new_property );
			}
			
			if( m_parte == "</properties>" ) fin = true;
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );

}
	

void MapaXML::procesaLAYER() {
	// SDL_Log( "procesaLAYER()" );

	XML_layer xml_layer;
	xml_layer.name.quita();
	xml_layer.x = 0;
	xml_layer.y = 0;
	xml_layer.width  = map.width;
	xml_layer.height = map.height;
	xml_layer.opacity = 1.0f;
	xml_layer.visible = true;
 // xml_layer.property;
	xml_layer.data.encoding = no_encoding;
	xml_layer.data.compression = no_compression;
	xml_layer.data.array = NULL;
	
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {

			if( m_parte == "<!--"     ) procesaREM();
						
			if( m_parte == "name" ) {
				xml_layer.name.inicia( 8, 8 );
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				xml_layer.name = m_parte;
			}

			if( m_parte == "x" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_layer.x );	
			}

			if( m_parte == "y" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_layer.y );	
			}

			if( m_parte == "width" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_layer.width );	
			}

			if( m_parte == "height" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_layer.height );	
			}

			if( m_parte == "opacity" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoF( m_parte.dameCadena(), xml_layer.opacity );	
			}

			if( m_parte == "visible" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				xml_layer.visible = ( m_parte == "0" ) ? false : true;
			}

			if( m_parte == "<properties>" ) {			
				procesaPROPERTIES( xml_layer.property );
			}

			if( m_parte == "<data" ||  m_parte == "<data>" ) {
				if( xml_layer.data.array == NULL ) {
					xml_layer.data.array = new Matriz< Uint16 >;
					if( xml_layer.data.array == NULL ) break;
				}
				if( !xml_layer.data.array->inicia( xml_layer.width,  xml_layer.height ) ) {
					// printf( "ERROR array" ); 
					break;
				} 
				if( m_parte == "<data" ) procesaDATA( xml_layer.data );
				else {
					xml_layer.data.encoding = tile;
					procesaTILES( xml_layer.data.array );
				}
			}
			if( m_parte == "</layer>" ) fin = true;
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );
	
	map.layer.meteFinal( xml_layer );
}
	
void MapaXML::procesaDATA( XML_data &data ) {
	// SDL_Log( "procesaDATA()" );
	data.encoding  = tile;
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {
			
			if( m_parte == "encoding" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				if     ( m_parte == "base64" ) data.encoding = base64;
				else if( m_parte == "csv"    ) data.encoding = csv;
				else   						   data.encoding = no_encoding;
			}

			if( m_parte == "compression" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				if     ( m_parte == "zlib" ) data.compression = zlib;
				else if( m_parte == "gzip" ) data.compression = gzip;
				else   					     data.compression = no_compression;
			}
	
			if( m_parte == ">"    ) fin = true;
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );
			
	switch( data.encoding ) {
		case base64:
			if     ( data.compression == zlib ) procesaBASE64ZLIB( data.array ); 
		 // else if( data.compression == gzip ) procesaBASE64GZIP( data.array ); 
			else 								procesaBASE64(     data.array );
		break;
		case csv:
			procesaCSV(  data.array ); 
		break;
		case tile:		
		case no_encoding:
			procesaTILES( data.array ); 
		break;
	}

}

void MapaXML::procesaBASE64( Matriz< Uint16 > *array ) {
	
	// Buscamos la primera línea con datos:
	size_t primer = 0;
	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;
		else { // Los datos deben estar en una sola línea, quitar espacios en blanco
			while( m_cadena[ primer ] == ' ' ) ++primer;
			break;
		}
	}
	
	// Decodificar BASE64.
	int len = Base64decode_len( &m_cadena[primer] );
	char* bufplain = (char*)malloc( len * sizeof( char ) );
	if( bufplain == NULL ) {
		// printf( "Fallo de memoria" );
		return;
	}
	int retorno = Base64decode( bufplain, &m_cadena[primer] );
	if( retorno < 4 ) {
		// printf( "Fallo decodificando Base64" );
		free( bufplain );
		return;
	}
	
	// Almacenar los datos en un array de dos dimensiones.
	int *p = (int*)bufplain;
	for( size_t y = 0; y < array->dameY(); ++y ) {
		for( size_t x = 0; x < array->dameX(); ++x ) {
			Uint16 dato = *p;
			array->toma( x, y, dato );
			++p;
		}
	}
	free( bufplain );
	
	// Buscamos la etiqueta </data>:
	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;
		if( m_cadena.busca( "</data>",  m_posicion ) ) break;
	}

}

void MapaXML::procesaBASE64ZLIB( Matriz< Uint16 > *array ) {
	
	// Buscamos la primera línea con datos:
	size_t primer = 0;
	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;
		else { // Los datos deben estar en una sola línea, quitar espacios en blanco
			while( m_cadena[ primer ] == ' ' ) ++primer;
			break;
		}
	}
	
	// Decodificar BASE64.
	int len = Base64decode_len( &m_cadena[primer] );
	char* bufplain = (char*)malloc( len * sizeof( char ) );
	if( bufplain == NULL ) {
		// printf( "Fallo de memoria" );
		return;
	}
	int retorno = Base64decode( bufplain, &m_cadena[primer] );
	if( retorno < 4 ) {
		// printf( "Fallo decodificando Base64" );
		free( bufplain );
		return;
	}

	// Descomprimimos zlib:
	Contenedor<char> contenedor;
	retorno = Descomprime( bufplain, len, contenedor );
	free( bufplain );
	if( retorno < 0 ) {
		// printf( "Fallo descomprimiendo zlib" );
		contenedor.quita();
		return;
	}
	
	// Almacenar los datos en un array de dos dimensiones.
	int *p = (int*)(&contenedor[0]);
	for( size_t y = 0; y < array->dameY(); ++y ) {
		for( size_t x = 0; x < array->dameX(); ++x ) {
			Uint16 dato = *p;
			array->toma( x, y, dato );
			++p;
		}
	}
	contenedor.quita();
	
	// Buscamos la etiqueta </data>:
	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;
		if( m_cadena.busca( "</data>",  m_posicion ) ) break;
	}

}

/*
void MapaXML::procesaBASE64GZIP( Matriz< Uint16 > *array ) {
	
	// Buscamos la primera línea con datos:
	size_t primer = 0;
	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;
		else { // Los datos deben estar en una sola línea, quitar espacios en blanco
			while( m_cadena[ primer ] == ' ' ) ++primer;
			break;
		}
	}
		
	// Decodificar BASE64.
	int len = Base64decode_len( &m_cadena[primer] );
	char* bufplain = (char*)SDL_malloc( len * sizeof( char ) );
	if( bufplain == NULL ) {
		// printf( "Fallo de memoria" );
		return;
	}
	int retorno = Base64decode( bufplain, &m_cadena[primer] );
	if( retorno < 4 ) {
		// printf( "Fallo decodificando Base64" );
		SDL_free( bufplain );
		return;
	}
	
	// Descomprimimos gzip:
	Contenedor<char> contenedor;
	retorno = DescomprimeGZIP( bufplain, len, contenedor );
	SDL_free( bufplain );
	if( retorno < 0 ) {
		// printf( "Fallo descomprimiendo gzip %d\n", retorno );
		contenedor.quita();
		return;
	}
	
	// Almacenar los datos en un array de dos dimensiones.
	int *p = (int*)(&contenedor[0]);
	for( size_t y = 0; y < array->dameY(); ++y ) {
		for( size_t x = 0; x < array->dameX(); ++x ) {
			Uint16 dato = *p;
			array->toma( x, y, dato );
			++p;
		}
	}
	contenedor.quita();

	
	// Buscamos la etiqueta </data>:
	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;
		if( m_cadena.busca( "</data>",  m_posicion ) ) break;
	}

}
*/

void MapaXML::procesaCSV( Matriz< Uint16 > *array ) {
	size_t x = 0;
	size_t y = 0;
	unsigned int dato;
	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;
		if( m_cadena.busca( "</data>",  m_posicion ) ) break;
		while( m_cadena.parte( m_parte, "," ) ) {
			STRtoUI( m_parte.dameCadena(), dato );
			array->toma( x, y, (Uint16)(dato) );
			++x;
		}
		x = 0; ++y; 
	}
}

void MapaXML::procesaTILES( Matriz< Uint16 > *array ) {
	size_t x = 0;
	size_t y = 0;
	unsigned int dato;
	while( m_archivo.lee( m_cadena ) ) {
		if( m_cadena.dameLongitud() == 0 ) continue;		
		if( m_cadena.busca( "</data>",  m_posicion ) ) break;
		if( !m_cadena.busca( "<tile", m_posicion ) ) continue;
		if( m_cadena.busca( "gid=\"",  m_posicion ) ) {
			char *p = &m_cadena[ m_posicion + 5 ];
			while( *p != '"' ) ++p; *p = '\0';
			dato = atoi( &m_cadena[ m_posicion + 5 ] );
			array->toma( x, y, (Uint16)(dato) );
			++x; 
			if( x >= array->dameX() ) { x = 0; ++y; }
		}
	}
}

void MapaXML::procesaOBJECT( Lista< XML_object > &object ) {

	// SDL_Log( "procesaprocesaOBJECT()" );

	XML_object new_object;
 	new_object.id = 0; 				// unsigned int 
	new_object.name.quita(); 		// Cadena
	new_object.type.quita(); 		// Cadena
	new_object.x = 0.0f;			// float
	new_object.y = 0.0f;			// float
	new_object.width = 0.0f;		// float
	new_object.height = 0.0f;		// float
	new_object.rotation = 0.0f;		// float
	new_object.gid = 0;				// unsigned int
	new_object.visible = true;
 // new_object.property;
		
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {

			if( m_parte == "<!--"     ) procesaREM();

			if( m_parte == "id" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), 	new_object.id );	
			}
			
			if( m_parte == "name" ) {
				new_object.name.inicia( 8, 8 );
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				new_object.name = m_parte;
			}

			if( m_parte == "type" ) {
				new_object.type.inicia( 8, 8 );
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				new_object.type = m_parte;
			}
			
			if( m_parte == "x" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoF( m_parte.dameCadena(), new_object.x );	
			}

			if( m_parte == "y" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoF( m_parte.dameCadena(), new_object.y );	
			}

			if( m_parte == "width" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoF( m_parte.dameCadena(), new_object.width );	
			}

			if( m_parte == "height" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoF( m_parte.dameCadena(), new_object.height );	
			}

			if( m_parte == "rotation" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoF( m_parte.dameCadena(), new_object.rotation );	
			}

			if( m_parte == "gid" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), 	new_object.gid );	
			}
			
			if( m_parte == "visible" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				new_object.visible = ( m_parte == "0" ) ? false : true;
			}
			
			if( m_parte == "<properties>" ) {			
				procesaPROPERTIES( new_object.property );
			}

			if( m_parte == "/>" ) fin = true;
			if( m_parte == "</object>" ) fin = true;
			
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );
	
	object.meteFinal( new_object );
}

void MapaXML::procesaOBJECTGROUP() {
	// SDL_Log( "procesaOBJECTGROUP()" );

	XML_objectgroup xml_objectgroup;

	xml_objectgroup.name.quita();
	xml_objectgroup.color.quita();
	xml_objectgroup.x = 0;
	xml_objectgroup.y = 0;
	xml_objectgroup.width  = 0;
	xml_objectgroup.height = 0;
	xml_objectgroup.opacity = 1.0f; 
	xml_objectgroup.visible = true;
 // xml_objectgroup.draworder;
 // xml_objectgroup.property;
 // xml_objectgroup.object;
	
	bool fin = false;
	do {
		if( m_cadena.dameLongitud() == 0 ) continue;
		while( m_cadena.parte( m_parte, " =\"" ) ) {

			if( m_parte == "<!--"     ) procesaREM();
						
			if( m_parte == "name" ) {
				xml_objectgroup.name.inicia( 8, 8 );
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				xml_objectgroup.name = m_parte;
			}
			
			if( m_parte == "color" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				xml_objectgroup.color.inicia( m_parte.dameCadena() );	
				xml_objectgroup.color.tomaAlfa( 255 );
			}
			
			if( m_parte == "x" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_objectgroup.x );	
			}

			if( m_parte == "y" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_objectgroup.y );	
			}

			if( m_parte == "width" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_objectgroup.width );	
			}

			if( m_parte == "height" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoUI( m_parte.dameCadena(), xml_objectgroup.height );	
			}

			if( m_parte == "opacity" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				STRtoF( m_parte.dameCadena(), xml_objectgroup.opacity );	
			}

			if( m_parte == "visible" ) {
				if( !m_cadena.parte( m_parte, NULL ) ) break;
				xml_objectgroup.visible = ( m_parte == "0" ) ? false : true;
			}

			if( m_parte == "<properties>" ) {			
				procesaPROPERTIES( xml_objectgroup.property );
			}

			if( m_parte == "<object" ) {
				procesaOBJECT( xml_objectgroup.object );
			}
			
			if( m_parte == "</objectgroup>" ) fin = true;
			
		}
		if( fin ) break;
	} while( m_archivo.lee( m_cadena ) );
	
	map.objectgroup.meteFinal( xml_objectgroup );
}


