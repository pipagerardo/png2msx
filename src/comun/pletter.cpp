/*
  Pletter v0.5c1

  XL2S Entertainment
*/

#include "pletter.h"

// #define PLETTER_MAIN

// REQUISITOS DE ENTRADA:
//------------------------
//	bool savelength = false;	// Opcional
//	unsigned offset = 0;		// Opcional
//	unsigned length = 0;		// Opcional
//	Cadena sourcefilename		// Obligatorio
//	Cadena destfilename			// Opcional -> sourcefilename + ".plet5"
Cadena sourcefilename, destfilename;
unsigned length, offset;
bool savelength = false;
// FIN

unsigned *last = NULL;
unsigned *prev = NULL;
unsigned char *d = NULL;

struct metadata {
	unsigned reeks;
	unsigned cpos[7], clen[7];
} *m = NULL;

struct pakdata {
	unsigned cost, mode, mlen;
} *p[7];

unsigned maxlen[7] = { 128,128+128,512+128,1024+128,2048+128,4096+128,8192+128 };
unsigned varcost[65536];

struct saves {
	unsigned char *buf = NULL;
	int ep, dp, p, e;
	void init() {
		ep=dp=p=e=0;
		buf = new unsigned char[length*2];
	}
	void add0() {
		if (p==0) claimevent();
		e*=2; ++p;
		if (p==8) addevent();
	}
	void add1() {
		if (p==0) claimevent();
		e*=2; ++p; ++e;
		if( p == 8 ) addevent();
	}
	void addbit(int b) { if (b) add1(); else add0(); }
	void add3(int b) { addbit(b&4); addbit(b&2); addbit(b&1); }
	void addvar(int i) {
		int j=32768;
		while (!(i&j)) j/=2;
		do {
			if (j==1) { add0(); return; }
			j/=2;
			add1();
			if (i&j) add1();
			else add0();
		} while (1);
	}
	void adddata(unsigned char d) { buf[dp++]=d; }
	void addevent() { buf[ep]=e; e=p=0; }
	void claimevent() { ep=dp; ++dp; }
	bool done() {
		if (p!=0) { while (p!=8) { e*=2; ++p; } addevent(); }
		FILE *file;
		if( !( file = fopen( destfilename.dameCadena(), "wb" ) ) ) {
			printf( TXT_Pletter_Error_writing_file );
			return false;
		}
		fwrite( buf, 1, dp, file );
		fclose( file );
		printf( " %s: %u -> %d\n", destfilename.dameCadena(), length, dp );
		return true;
	}
} s;

// bool loadfile( const Cadena& sourcefilename ) {
bool loadfile() {
	FILE *file;
	if( ( file = fopen( sourcefilename.dameCadena(), "rb" ) ) == NULL ) {
		printf( TXT_Pletter_Error_opening_file, sourcefilename.dameCadena() );
		return false;
	}
	if( !length ) {
		fseek( file, 0, SEEK_END );
		length = (unsigned)ftell( file ) - offset;
		printf( TXT_Pletter_length, length );
	}
	fseek( file, offset, SEEK_SET );
	d = new unsigned char[ length + 1 ];
	m = new metadata[ length + 1 ];
	if( !fread( d, length, 1, file ) ) {
		printf( TXT_Pletter_Filesize_error );
		return false;
	}
	d[ length ] = 0;
	fclose( file );
	return true;
}

void initvarcost() {
	int v=1,b=1,r=1;
	while (r!=65536) {
  		for( int j=0; j!=r; ++j ) varcost[v++]=b;
		b+=2; r*=2;
	}
}

void createmetadata() {
	unsigned i, j;
	last = new unsigned[ 65536 ];
	memset( last, -1, 65536 * sizeof(unsigned) );
	prev = new unsigned[ length + 1 ];
	for( i=0; i!=length; ++i ) {
		m[i].cpos[0]=m[i].clen[0]=0;
		prev[i]=last[d[i]+d[i+1]*256];
		last[d[i]+d[i+1]*256]=i;
	}
	unsigned r=-1,t=0;
	for( i=length-1; i != unsigned(-1); --i ) {
		if( d[i] == r ) m[i].reeks=++t;
		else { r=d[i]; m[i].reeks=t=1; }
	}
	for (int bl=0;bl!=7;++bl) {
		for (i=0;i<length;++i) {
			unsigned l,p;
 			p=i;
			if (bl) {
				m[i].clen[bl]=m[i].clen[bl-1];
				m[i].cpos[bl]=m[i].cpos[bl-1];
				p=i-m[i].cpos[bl];
			}
			while( ( p=prev[p] ) != unsigned(-1) ) {
				if (i-p>maxlen[bl]) break;
        		l=0;
				while (d[p+l]==d[i+l] && (i+l)<length) {
					if (m[i+l].reeks>1) {
						if( ( j=m[i+l].reeks ) > m[p+l].reeks ) j=m[p+l].reeks;
						l+=j;
					} else ++l;
				}
				if( l > m[i].clen[bl] ) { m[i].clen[bl]=l; m[i].cpos[bl]=i-p; }
			}
		}
		printf( "." );
	}
	printf( " " );
}

int getlen( pakdata *p, unsigned q ) {
	unsigned i,j,cc,ccc,kc,kmode,kl;
	p[length].cost=0;

	for( i=length-1; i!= unsigned(-1); --i ) {
		kmode=0; kl=0;
		kc=9+p[i+1].cost;

		j=m[i].clen[0];
		while( j > 1 ) {
			cc=9+varcost[j-1]+p[i+j].cost;
			if (cc<kc) { kc=cc; kmode=1; kl=j; }
			--j;
		}

		j=m[i].clen[q];
		if (q==1) ccc=9; else ccc=9+q;
		while (j>1) {
			cc=ccc+varcost[j-1]+p[i+j].cost;
			if (cc<kc) { kc=cc; kmode=2; kl=j; }
			--j;
		}

		p[i].cost=kc; p[i].mode=kmode; p[i].mlen=kl;
	}
	return p[0].cost;
}

bool save( pakdata *p, unsigned q ) {
	s.init();
	unsigned i, j;

	if( savelength ) {
		s.adddata( length & 255 );
 		s.adddata( length >> 8 );
	}

	s.add3(q-1);
	s.adddata(d[0]);

	i=1;
	while( i < length ) {
		switch (p[i].mode) {
			case 0:
				s.add0();
				s.adddata(d[i]);
				++i;
			break;
			case 1:
				s.add1();
				s.addvar(p[i].mlen-1);
				j=m[i].cpos[0]-1;
				if( j > 127 ) printf( "-j>128-" );
				s.adddata(j);
				i+=p[i].mlen;
			break;
 			case 2:
				s.add1();
				s.addvar(p[i].mlen-1);
				j=m[i].cpos[q]-1;
				if( j < 128 ) printf( "-j<128-" );
				j-=128;
				// & tiene prioridad respecto a |
				// s.adddata( 128 | j & 127 );
				s.adddata( 128 | (j & 127) );
      			switch (q) {
					case 6: s.addbit(j&4096);
					case 5: s.addbit(j&2048);
					case 4: s.addbit(j&1024);
					case 3: s.addbit(j&512);
 					case 2: s.addbit(j&256);
							s.addbit(j&128);
 					case 1:
					break;
					default:
 						printf( "-2-" );
					break;
				}
				i+=p[i].mlen;
			break;
			default:
				printf( "-?-" );
			break;
		}
	}

	for( i=0; i!=34; ++i ) s.add1();
	return s.done();
}

void clean() {
// unsigned *last = NULL;
// unsigned *prev = NULL;
// unsigned char *d = NULL;
// unsigned char *s.buf; // struct saves
// struct metadata *m;
// struct pakdata *p[7];
	if( last != NULL ) delete[] last;
	last = NULL;
	if( prev != NULL ) delete[] prev;
	prev = NULL;
	if( d != NULL ) delete[] d;
	d = NULL;
	if( s.buf != NULL ) delete[] s.buf;
	s.buf = NULL;
	if( m != NULL ) delete[] m;
	m = NULL;
	for( int i=1; i!=7; ++i ) {
		if( p[i] != NULL ) delete p[i];
		p[i] = NULL;
	}
}

#ifdef PLETTER_MAIN
#ifdef WINDOWS
// algo...
#endif
#ifdef LINUX
// algo...
#endif
#ifdef APPLE
/*
	#include "CoreFoundation/CoreFoundation.h"
	// link: libCoreFoundation.so
*/
#endif
#include <locale.h>
int main( int argc, char *argv[] ) {

    sourcefilename.quita();
    destfilename.quita();
    length = 0u;
    offset = 0u;
    savelength = false;

#ifdef APPLE
/*
{
    // En principio esto no es requerido en MAC, pero por si acacaso:

    // Returns an application’s main bundle:
    // CFBundleRef CFBundleGetMainBundle( void );
    CFBundleRef mainBundle = CFBundleGetMainBundle();

    // Returns the location of a bundle’s Resources directory:
    // CFURLRef CFBundleCopyResourcesDirectoryURL( CFBundleRef bundle );
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL( mainBundle );

    // Fills a buffer with the file system's native string representation of a given URL's path:
    // bool CFURLGetFileSystemRepresentation(
    //      url,                // The CFURL object whose native file system representation you want to obtain.
    //      resolveAgainstBase, // Pass true to return an absolute path name.
    //      buffer,             // A pointer to a character buffer. On return, the buffer holds the native file
    //                          // system's representation of url. The buffer is null-terminated.
    //                          // This parameter must be at least maxBufLen in size for the file system in
    //                          // question to avoid failures for insufficiently large buffers.
    //      maxBufLen           // The maximum number of characters that can be written to buffer.
    // );
   	char path[1024];
   	if( !CFURLGetFileSystemRepresentation( resourcesURL, TRUE, (UInt8 *)path, PATH_MAX ) ) return -1;

    // Releases a Core Foundation object:
    // void CFRelease( CFTypeRef cf );
   	CFRelease( resourcesURL );

   	// Vhange working directory:
   	// #include <unistd.h>
   	// int chdir( const char *path );
   	chdir( path );
*/
}
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

	if( argc==1 ) printf( "\n" );

	printf( TXT_Pletter_version );
	if( argc==1 ) {
        printf( TXT_Pletter_Usage );
 		printf( TXT_Pletter_Option_save );
 		printf( TXT_Pletter_Option_sourcefile );
		printf( TXT_Pletter_Option_offset );
  		printf( TXT_Pletter_Option_destinationfile );
		clean();
		return 0;
	}

	int i=1;
	if (argv[i][0]=='-') {
		savelength = ( argv[i][1]=='s' ) || ( argv[i][1]=='S' );
		++i;
	}
	if( argv[i] ) sourcefilename=argv[i++];
	if( argv[i] && esDigito( argv[i][0] ) ) {
		offset=atoi( argv[i++] );
		if( argv[i] && esDigito( argv[i][0] ) ) length = atoi( argv[i++] );
	}
	if( argv[i] ) destfilename = argv[i++];

 	if( sourcefilename.dameLongitud() == 0 ) {
		printf( TXT_Pletter_No_inputfile );
		clean();
		return 1;
	}
	if( destfilename.dameLongitud() == 0 ) {
		destfilename = sourcefilename + ".plet5";
	}

    printf( TXT_Pletter_savelength,     savelength );
    printf( TXT_Pletter_sourcefilename, sourcefilename.dameCadena() );
    printf( TXT_Pletter_offset,         offset );
    printf( TXT_Pletter_destfilename,   destfilename.dameCadena() );

//	REQUISITOS DE ENTRADA:
//-------------------------
//	bool savelength = false;	// Opcional
//	unsigned offset = 0;		// Opcional
//	unsigned length = 0;		// Opcional
//	Cadena sourcefilename		// Obligatorio
//	Cadena destfilename			// Opcional -> sourcefilename + ".plet5"

	if( !loadfile() ) {
		clean();
		return 1;
	}

	initvarcost();
	createmetadata();

	int minlen=length*1000;
	int minbl=0;
	for( i=1; i!=7; ++i ) {
		p[i] = new pakdata[ length + 1 ];
		int l=getlen( p[i], i );
 		if( l < minlen && i ) { minlen=l; minbl=i; }
		printf( "." );
	}

	if( !save( p[minbl], minbl ) ) {
		clean();
		return 1;
	}

	clean();
	return 0;
}

#else

bool pletter( const Cadena& archivo, bool guarda_longitud ) {
//	REQUISITOS DE ENTRADA:
//-------------------------
//	bool savelength = false;	// Opcional
//	unsigned offset = 0;		// Opcional
//	unsigned length = 0;		// Opcional
//	Cadena sourcefilename		// Obligatorio
//	Cadena destfilename			// Opcional -> sourcefilename + ".plet5"

	savelength =  guarda_longitud;
	offset = 0;
	length = 0;
	sourcefilename = archivo;
	destfilename = sourcefilename + ".plet5";
	int i=1;

//	if( !loadfile( sourcefilename ) ) {
	if( !loadfile() ) {
		clean();
		return false;
	}

	initvarcost();
	createmetadata();

	int minlen=length*1000;
	int minbl=0;
	for( i=1; i!=7; ++i ) {
		p[i] = new pakdata[ length + 1 ];
		int l=getlen( p[i], i );
 		if( l < minlen && i ) { minlen=l; minbl=i; }
		printf( "." );
	}

	if( !save( p[minbl], minbl ) ) {
		clean();
		return false;
	}

	clean();
	return true;
}

bool pletter( unsigned char* array, size_t longitud, const Cadena& cadena, bool guarda_longitud ) {
//	REQUISITOS DE ENTRADA:
//-------------------------
//	bool savelength = false;	// Opcional
//	unsigned offset = 0;		// Opcional
//	unsigned length = 0;		// Opcional
//	Cadena sourcefilename		// Obligatorio
//	Cadena destfilename			// Opcional -> sourcefilename + ".plet5"

	if( array == NULL ) return false;
	if( longitud == 0 ) {
		delete[] array;
		array = NULL;
		return false;
	}
	savelength = guarda_longitud;
	offset = 0;
	length = 0;
	sourcefilename = cadena;
	destfilename = sourcefilename + ".plet5";
	int i=1;
	length = (unsigned)longitud;

	d = array; 	// new unsigned char[ length + 1 ];
	m = new metadata[ length + 1 ];
	d[ length ] = 0;

	initvarcost();
	createmetadata();

	int minlen=length*1000;
	int minbl=0;
	for( i=1; i!=7; ++i ) {
		p[i] = new pakdata[ length + 1 ];
		int l=getlen( p[i], i );
 		if( l < minlen && i ) { minlen=l; minbl=i; }
		printf( "." );
	}

	if( !save( p[minbl], minbl ) ) {
		clean();
		return false;
	}

	clean();
	return true;
}

#endif

//eof
