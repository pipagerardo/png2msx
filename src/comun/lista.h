#ifndef _LISTA_
#define _LISTA_

#include "funciones.h"
	
template< class TIPO > class Lista;
template< class TIPO > class Lista_Iterador;
template< class TIPO >
class Lista_Nodo {
	public:
    	Lista_Nodo( const TIPO &dato, Lista_Nodo<TIPO> *anterior, Lista_Nodo<TIPO> *siguiente ) { 
        	puntero_dato      = new TIPO( dato );
        	puntero_anterior  = anterior;
        	puntero_siguiente = siguiente;
    	}
    	Lista_Nodo( Lista_Nodo<TIPO> &copia ) {
        	puntero_dato      = new TIPO( *copia.puntero_dato );
        	puntero_anterior  = copia.puntero_anterior;
        	puntero_siguiente = copia.puntero_siguiente;
    	}
    	~Lista_Nodo() {
    		if( puntero_dato != NULL ) delete puntero_dato; 
        	puntero_dato      = NULL;
        	puntero_anterior  = NULL;
        	puntero_siguiente = NULL;
		}
	private:
    	TIPO 	         *puntero_dato;
    	Lista_Nodo<TIPO> *puntero_anterior, *puntero_siguiente;
    	friend class Lista<TIPO>;
    	friend class Lista_Iterador<TIPO>;
};

template < class TIPO >
class Lista {
	public:
		
    	Lista()  { 
			numero_de_elementos = 0; 
			puntero_inicio      = NULL; 
			puntero_final       = NULL; 
		}

    	Lista( const Lista< TIPO >& copia )  { 
			numero_de_elementos = 0; 
			puntero_inicio      = NULL; 
			puntero_final       = NULL; 
			Lista_Iterador<TIPO> it(  (Lista<TIPO>&)copia );
			for( it.alInicio(); it.noFinal(); ++it ) {
				meteFinal( it.ver() );
			}
			 
		}
		
    	~Lista() { 
			quita(); 
		}

    	void quita() {  
			while( puntero_final ) sacaFinal(); 
		}
		
    	bool meteInicio( const TIPO &dato ) {
    	    Lista_Nodo<TIPO> *auxiliar = new Lista_Nodo<TIPO>( dato, puntero_final, puntero_inicio );
    	    if( auxiliar == NULL ) return false;
    	    if( !numero_de_elementos ) { // Si la Lista está vacia, todo apunta al único elemento.
    	        puntero_inicio = auxiliar;
    	        puntero_final  = auxiliar;
    	        auxiliar->puntero_anterior  = auxiliar;
    	        auxiliar->puntero_siguiente = auxiliar;
    	        ++numero_de_elementos;
    	        return true;
    	    }
    	    puntero_inicio->puntero_anterior = auxiliar;
    	    puntero_inicio = auxiliar;
    	    puntero_final->puntero_siguiente = puntero_inicio; 
    	    ++numero_de_elementos;
    	    return true;
    	}
    	
    	bool meteFinal( const TIPO &dato ) {
    	    Lista_Nodo<TIPO> *auxiliar = new Lista_Nodo<TIPO>( dato, puntero_final, puntero_inicio );
    		if( auxiliar == NULL ) return false;
    	    if( !numero_de_elementos ) {  // Si la Lista está vacia, todo apunta al único elemento.
    	        puntero_inicio = auxiliar;
    	        puntero_final  = auxiliar;
    	        auxiliar->puntero_anterior  = auxiliar;
    	        auxiliar->puntero_siguiente = auxiliar;
    	        ++numero_de_elementos;
    	        return true;
    	    }
    	    puntero_final->puntero_siguiente = auxiliar; 
    	    puntero_final = auxiliar;
    	    puntero_inicio->puntero_anterior = puntero_final;        
    	    ++numero_de_elementos;
    	    return true;
    	}
    	
   		TIPO sacaInicio() {
        	if( numero_de_elementos ) {
        	    TIPO retorno( *puntero_inicio->puntero_dato ); 
        	    Lista_Nodo<TIPO> *auxiliar = puntero_inicio;
        	    puntero_inicio = auxiliar->puntero_siguiente;
        	    puntero_inicio->puntero_anterior = puntero_final;
        	    puntero_final->puntero_siguiente = puntero_inicio;
        	    delete auxiliar;
        	    --numero_de_elementos;
        	    if( !numero_de_elementos ) {
        	        puntero_inicio = NULL; 
        	        puntero_final  = NULL;
        	    }
        	    return retorno;
        	 }
        	 return TIPO();
   		}
   		
    	TIPO sacaFinal() {
    	    if( numero_de_elementos ) {
    	        TIPO retorno( *puntero_final->puntero_dato );
    	        Lista_Nodo<TIPO> *auxiliar = puntero_final;
    	        puntero_final = auxiliar->puntero_anterior;
    	        puntero_final->puntero_siguiente = puntero_inicio;
    	        puntero_inicio->puntero_anterior = puntero_final;
    	        delete auxiliar;
    	        --numero_de_elementos;
    	        if( !numero_de_elementos ) {
    	            puntero_inicio = NULL; 
    	            puntero_final  = NULL;
    	        }
    	        return retorno;
    	    }
    	    return TIPO();
    	}
    	
    	bool estaVacia() {  
			return puntero_inicio == NULL; 
		}
		
    	size_t dameLongitud() const { 
			return numero_de_elementos; 
		}
		
    	void ordena( int (*comparar)(const void*, const void*) ) {
       		Lista_Iterador<TIPO> inicio, final;
        	inicio.alInicio( *this );    
        	final.alFinal(   *this );    
        	p_ordena( inicio, final, comparar );
    	}
    	
    	Lista_Iterador<TIPO> busca( TIPO base, int (*comparar)(const void*, const void*) ) {
    	    Lista_Iterador<TIPO> it; int resultado; size_t posicion;
    	    it.alFinal( *this ); // Si el valor del último elemento es menor que la base...
    	    if ( comparar( (const void *)it.puntero(), (const void *)&base ) <= 0 ) return it;
    	    it.alInicio();      // Si el valor del primer elemento es mayor que la base...
    	    if ( comparar( (const void *)it.puntero(), (const void *)&base ) >= 0 ) return it;
    	    posicion = ( numero_de_elementos - 1 ) >> 1; it + posicion;  // Saltamos la mitad de la longitud.
    	    do {
    	        if ( posicion % 2 ) ++posicion; posicion >>= 1;
    	        resultado = comparar( (const void *)it.puntero(), (const void *)&base );
    	        if      ( resultado >   0 ) it - posicion;          
    	        else if ( resultado <   0 ) it + posicion; 
    	        else                        return it;
    	    } while( posicion > 1 );
    	    return it;                     
    	}
    	
	private:
		size_t              numero_de_elementos;
	    Lista_Nodo< TIPO > *puntero_inicio, *puntero_final;
	    friend class Lista_Iterador<TIPO>;
	    void p_ordena( Lista_Iterador<TIPO> inicio, Lista_Iterador<TIPO> final, int (*comparar)(const void*, const void*) ) {
    	    Lista_Iterador< TIPO > I, D, P;
        	I = inicio; D = final; P = I; 
        	while( D != I ) {
        	    while( ( D != I ) && ( comparar( (const void *)D.puntero(), (const void *)P.puntero() ) != -1 ) ) --D;
        	    while( ( D != I ) && ( comparar( (const void *)P.puntero(), (const void *)I.puntero() ) != -1 ) ) ++I;
        	    if( D != I ) I.cambia(D); 
        	    else         I.cambia(P);
        	}
        	if (  D != final  ) { ++D; p_ordena( D,      final, comparar ); }
        	if (  I != inicio ) { --I; p_ordena( inicio, I    , comparar ); }
    	}
};

template< class TIPO >
class Lista_Iterador {    
	public:
		
	    Lista_Iterador() { 
			puntero_lista = NULL; 
			puntero_nodo_lista = NULL; 
			b_retorno = false;
		}
		
	    Lista_Iterador( Lista<TIPO> &lista ) { 
			puntero_lista = &lista;
			puntero_nodo_lista = puntero_lista->puntero_inicio;
			b_retorno = false;
		}

	    Lista_Iterador( const Lista_Iterador<TIPO> &iterador ) {
			puntero_lista = iterador.puntero_lista;
			puntero_nodo_lista = iterador.puntero_nodo_lista;
			b_retorno = iterador.b_retorno;
		} 
		
	    ~Lista_Iterador() { }
	    
	    void alInicio( Lista<TIPO> &lista  ) { 
			puntero_lista = &lista; 
			puntero_nodo_lista = puntero_lista->puntero_inicio; 
			b_retorno = false;
		}
		
	    void alSegundo( Lista<TIPO> &lista  ) { 
			puntero_lista = &lista; 
			puntero_nodo_lista = puntero_lista->puntero_inicio->puntero_siguiente;
			b_retorno = false;
		}
		
	    void alPenultimo( Lista<TIPO> &lista  ) { 
			puntero_lista = &lista; 
			puntero_nodo_lista = puntero_lista->puntero_final->puntero_anterior;
			b_retorno = false;
		}
		
	    void alFinal( Lista<TIPO> &lista  ) { 
			puntero_lista = &lista;
			puntero_nodo_lista = puntero_lista->puntero_final;
			b_retorno = false;
		}
		
	    void alInicio() { 
			puntero_nodo_lista = puntero_lista->puntero_inicio; 
			b_retorno = false;
		}
		
	    void alSegundo() { 
			puntero_nodo_lista = puntero_lista->puntero_inicio->puntero_siguiente; 
			b_retorno = false;
		}
		
	    void alPenultimo() { 
			puntero_nodo_lista = puntero_lista->puntero_final->puntero_anterior;
			b_retorno = false;
		}
		
	    void alFinal() { 
			puntero_nodo_lista = puntero_lista->puntero_final; 
			b_retorno = false;
		}
		
	    TIPO ver() const  { 
			return *(puntero_nodo_lista->puntero_dato);
		}

	    TIPO* operator &() { 
			return puntero_nodo_lista->puntero_dato;
		}
		
	    TIPO operator *() { 
			return *(puntero_nodo_lista->puntero_dato);
		}
		
    	TIPO *operator ->() { 
			return puntero_nodo_lista->puntero_dato;
		}

	    void borra() {
	        if( this->inicio() ) { puntero_lista->sacaInicio(); puntero_nodo_lista = puntero_lista->puntero_inicio; return; }
	        if( this->final() )  { puntero_lista->sacaFinal();  puntero_nodo_lista = puntero_lista->puntero_final;  return; }      
	        if( puntero_lista->numero_de_elementos ) {
	            Lista_Nodo<TIPO> *auxiliar  = puntero_nodo_lista;
	            puntero_nodo_lista->puntero_anterior->puntero_siguiente = puntero_nodo_lista->puntero_siguiente;
	            puntero_nodo_lista->puntero_siguiente->puntero_anterior = auxiliar->puntero_anterior;
	     		puntero_nodo_lista = puntero_nodo_lista->puntero_siguiente; // Después de Borrar se desplaza al siguiente.
	            delete auxiliar;
	            --puntero_lista->numero_de_elementos;
	        }
	    }
	    
	    void insertaArriba( const TIPO &dato ) {
    	    if( this->inicio() ) { puntero_lista->meteInicio( dato ); return; }
    	    if( puntero_lista->numero_de_elementos ) {
    	        Lista_Nodo<TIPO> *auxiliar = new Lista_Nodo<TIPO>( dato, puntero_nodo_lista->puntero_anterior, puntero_nodo_lista );
    	        puntero_nodo_lista->puntero_anterior->puntero_siguiente = auxiliar;
    	        puntero_nodo_lista->puntero_anterior = auxiliar;
    	        ++puntero_lista->numero_de_elementos;
    	    }
    	}
    	
    	void insertaAbajo(  const TIPO &dato ) {
    	    if( this->final() )  { puntero_lista->meteFinal( dato ); return; }      
    	    if( puntero_lista->numero_de_elementos ) {
   		        Lista_Nodo<TIPO> *auxiliar = new Lista_Nodo<TIPO>( dato, puntero_nodo_lista ,puntero_nodo_lista->puntero_siguiente );
    	        puntero_nodo_lista->puntero_siguiente->puntero_anterior = auxiliar;
    	        puntero_nodo_lista->puntero_siguiente = auxiliar;
    	        ++puntero_lista->numero_de_elementos;    
    	    }
    	}
    	
    	void cambia( Lista_Iterador< TIPO > &iterador ) {   
    	    TIPO *auxiliar = this->puntero_nodo_lista->puntero_dato;
    	    this->puntero_nodo_lista->puntero_dato = iterador.puntero_nodo_lista->puntero_dato;
    	    iterador.puntero_nodo_lista->puntero_dato = auxiliar;
    	}
    	
    	Lista_Iterador<TIPO>& operator = ( const Lista_Iterador<TIPO> &iterador ) {
			this->puntero_lista = iterador.puntero_lista;
			this->puntero_nodo_lista = iterador.puntero_nodo_lista;
			this->b_retorno = iterador.b_retorno;
			return *this;
		}
		
    	Lista_Iterador<TIPO>& operator =( const TIPO &dato ) { 
			*this->puntero_nodo_lista->puntero_dato = dato; return *this;
		}
		
    	void copia( const TIPO &dato ) {
			*this->puntero_nodo_lista->puntero_dato = dato;
		}
		
    	void copia( const Lista_Iterador<TIPO> &iterador ) { 
			*this->puntero_nodo_lista->puntero_dato = *iterador.puntero_nodo_lista->puntero_dato;
		}
		
    	bool operator ==( const Lista_Iterador<TIPO> &iterador ) { 
			return puntero_nodo_lista == iterador.puntero_nodo_lista;
		}
		
    	bool operator !=( const Lista_Iterador<TIPO> &iterador ) { 
			return puntero_nodo_lista != iterador.puntero_nodo_lista;
		}
		
    	void operator ++() { 
			if( puntero_nodo_lista ) puntero_nodo_lista = puntero_nodo_lista->puntero_siguiente;
		}
		
    	void operator +( size_t n ) { 
			if( puntero_nodo_lista )
				for( size_t i = 0; i<n; ++i )
					puntero_nodo_lista = puntero_nodo_lista->puntero_siguiente;
		}
		
    	void operator --()          { 
			if( puntero_nodo_lista ) puntero_nodo_lista = puntero_nodo_lista->puntero_anterior;
		}
		
    	void operator -( size_t n ) { 
			if( puntero_nodo_lista ) 
				for( size_t i = 0; i<n; ++i ) 
					puntero_nodo_lista = puntero_nodo_lista->puntero_anterior;
		}
		
    	bool inicio() {
    	    if( puntero_nodo_lista ) {
        	    if( puntero_nodo_lista == puntero_lista->puntero_inicio ) return true;
        	} return false;
    	}
    	
    	bool segundo() {
    	    if( puntero_nodo_lista ) {
    	        if( puntero_nodo_lista == puntero_lista->puntero_inicio->puntero_siguiente ) return true;
    	    } return false;
    	}
    	
    	bool penultimo() {
    	    if( puntero_nodo_lista ) {     
    	        if( puntero_nodo_lista == puntero_lista->puntero_final->puntero_anterior ) return true;
    	    } return false;
   		}
   		
    	bool final() {
    	    if( puntero_nodo_lista ) {     
    	        if( puntero_nodo_lista == puntero_lista->puntero_final ) return true;
    	    } return false;
    	}
    	
   		bool noInicio() { 
    	    if( puntero_nodo_lista ) {
    	        if( b_retorno ) { b_retorno = false; return false; }
    	        if( puntero_nodo_lista == puntero_lista->puntero_inicio ) b_retorno = true;
    	        return true;
    	    } return false;
    	}
    	
    	bool noSegundo() { 
    		if( puntero_nodo_lista ) {
        	    if( puntero_nodo_lista == puntero_lista->puntero_inicio ) return false;
        	    return true;
        	} return false;
    	}
    	
    	bool noPenultimo() { 
    	    if( puntero_nodo_lista ) {      
    	       if( puntero_nodo_lista == puntero_lista->puntero_final ) return false;
    	       return true;
    	    } return false;
    	}
    	
    	bool noFinal() {
    	    if( puntero_nodo_lista ) {
    	        if( b_retorno ) { b_retorno = false; return false; }
    	        if( puntero_nodo_lista == puntero_lista->puntero_final ) b_retorno = true;
    	        return true;
    	    } return false;
    	}
    	
	private:	
    	Lista<TIPO>      *puntero_lista;
    	Lista_Nodo<TIPO> *puntero_nodo_lista;
    	bool              b_retorno;
};

#endif

// -----------------------------------------------------------------------------
// Lista();                             // Constructor.
// ~Lista();                            // Destructor.
// void quita();                        // borra todo.
// void meteInicio( const TIPO &dato ); // Mete un dato al inicio.
// void meteFinal( const TIPO &dato );  // Mete un dato al final.
// TIPO sacaInicio();                   // Saca un dato del inicio.
// TIPO sacaFinal();                    // Saca un dato del final.
// bool estaVacia();                    // Retorna true si la Lista está vacia.
// size_t dameLongitud() const;         // Retorna el número de elementos de la Lista.
// void ordena( int (*comparar)(const void*, const void*) );
// Lista_Iterador<TIPO> busca( TIPO base, int (*comparar)(const void*, const void*) );
//   
//  Ejemplo de función para comparar dos tipos (TIPO): 
//  --------------------------------------------------
//  #define DE_MENOR_A_MAYOR  // Definimos Ordenar de Menor a Mayor.
//  int comparar( const void *arg1, const void *arg2 ) {
//    #ifdef DE_MENOR_A_MAYOR  
//      if( *(TIPO*)arg1 > *(TIPO*)arg2  ) return  1; // De Menor a Mayor.
//      if( *(TIPO*)arg1 < *(TIPO*)arg2  ) return -1;
//                                         return  0;
//    #else
//      if( *(TIPO*)arg1 > *(TIPO*)arg2  ) return  -1; // De Mayor a Menor.
//      if( *(TIPO*)arg1 < *(TIPO*)arg2  ) return   1;
//                                         return   0;
//    #endif
//  }
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CLASE: Lista_Iterador
// --------------------------
// Lista_Iterador();                                       // Crea un Iterador a Lista.
// Lista_Iterador( Lista<TIPO> &lista );                   // Crea un Iterador a una Lista y lo posiciona al principio.
// Lista_Iterador( const Lista_Iterador<TIPO> &iterador ); // Crea un Iterador de otro Iterador y lo posiciona al principio.
// ~Lista_Iterador();                                      // Destructor. 
// void alInicio(    Lista<TIPO> &lista ); // Asigna el iterador al inicio de una la Lista.
// void alSegundo(   Lista<TIPO> &lista ); // Asigna el iterador al segundo elemento de una la Lista. 
// void alPenultimo( Lista<TIPO> &lista ); // Asigna el iterador al penúltimo elemento de una la Lista. 
// void alFinal(     Lista<TIPO> &lista ); // Asigna el iterador al final  de una la Lista. 
// void alInicio();                        // Posiciona el iterador al inicio de la Lista.
// void alSegundo();                       // Posiciona el iterador en el segundo elemento de la Lista.
// void alPenultimo();                     // Posiciona el iterador en el penúltimo elemtento de la Lista.
// void alFinal();                         // Posiciona el iterador al final  de la Lista.
// TIPO ver() const;        // Retorna sin destruir un dato de la Lista.
// TIPO* ->  const;   		// Retorna un puntero al dato de la Lista.
// void borra();            // borra un dato y se desplaza al siguiente.
// void insertaArriba( const TIPO &dato );   // Inserta un dato por arriba del actual.
// void insertaAbajo(  const TIPO &dato );   // Inserta un dato por abajo  del actual.
// void cambia( Lista_Iterador< TIPO > &iterador );
// Lista_Iterador<TIPO>& operator =( const Lista_Iterador<TIPO> &iterador ); // copia un dato, machaca el existente.
// Lista_Iterador<TIPO>& operator =( const TIPO &dato );     // copia un dato, machaca el existente.
// void copia( const TIPO &dato );                           // copia un dato, machaca el existente.
// void copia( const Lista_Iterador<TIPO> &iterador );       // copia el dato de un iterador, machaca el existente.
// bool operator ==( const Lista_Iterador<TIPO> &iterador ); // Compara dos iteradores.
// bool operator !=( const Lista_Iterador<TIPO> &iterador ); // Compara dos iteradores.
// void operator ++();                     // Incrementa el iterador de la Lista.
// void operator +( size_t n );            // Incrementa al iterador de la Lista n veces.
// void operator --();                     // Decrementa el iterador de la Lista.
// void operator -( size_t n );            // Decrementa al iterador de la Lista n veces.
// bool inicio();         // Retorna true cuando sea el inicio de la Lista.
// bool segundo();        // Retorna true cuando sea el segundo elemento de la Lista.
// bool penultimo();      // Retorna true cuando sea el penúltimo elemento de la Lista.
// bool final();          // Retorna true cuando sea el final  de la Lista.
// bool noInicio();      // Retorna true mientras no sea el inicio de la Lista.
// bool noSegundo();     // Retorna true mientras no sea al segundo elemento de la Lista.
// bool noPenultimo();   // Retorna true mientras no sea el penúltimo elemento de la Lista.
// bool noFinal();       // Retorna true mientras no sea el final de la Lista.
// -----------------------------------------------------------------------------
// Retornos especiales para Bucles FOR:
// for( it.alInicio();    it.noFinal();     ++it ) printf( "%d\n", it.ver() ); // Recorre del primer al último elemento.
// for( it.alFinal();     it.noInicio();    --it ) printf( "%d\n", it.ver() ); // Recorre del último al primer elemento.
// for( it.alSegundo();   it.noPenultimo(); ++it ) printf( "%d\n", it.ver() ); // Recorre del segundo al penúltimo elemento.
// for( it.alPenultimo(); it.noSegundo();   --it ) printf( "%d\n", it.ver() ); // Recorre del penultimo al segundo elemento.    
// -----------------------------------------------------------------------------

