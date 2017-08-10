#ifndef _TIEMPO_
#define _TIEMPO_
#include "funciones.h"
#include <time.h>

// Pues es un calendario básicamente.
class Tiempo {
public:
	Tiempo();
	~Tiempo();
	
	void inicia();
	void inicia( const Tiempo& tiempo );
	bool inicia( int anio, int mes, int dia, int hora, int minuto, int segundo );
	void quita();
	
	bool tomaFecha( int anio, int mes, int dia );
	bool tomaHora(  int hora, int minuto, int segundo );
	bool calculaSiEsVerano(); // Si cambias la fecha o la hora.
	
	void dameFecha( int& anio, int& mes, int& dia );
	void dameHora(	int& hora, int& minuto, int& segundo );
	int  dameDiaSemana() const; // Día semana: 	1 - 7 ( Lunes - Domingo )
	int  dameDiaAnio()   const; // Día del año:	1 - 365/366 (Bisiesto 366 días.) 
    bool esVerano()      const;
	    
	int  dameAnio()    const; // Año:			0 - ????
	int  dameMes()     const; // Mes del año:	1 - 12	( Enero - Diciembre )
	int  dameDia()     const; // Día del mes:	1 - 31	
	int  dameHora()    const; // Hora:    0-23 
	int  dameMinuto()  const; // Minuto:  0-59 
	int  dameSegundo() const; // Segundo: 0-59
	
	// -----------------------------------------------------------------------------
	//                            FUNCIONES UTILES
	// ----------------------------------------------------------------------------- 
	bool esBisiesto( int anio );
	bool esBisiesto();
	int  diaMaximoMes( int anio, int mes );
	int  diaMaximoMes();
	int  diaDeLaSemanaZeller( int anio, int mes, int dia ); // Válido del año 1583 al 4882.
	int  diaDeLaSemana( int anio, int mes, int dia );		// Válido del año 1 al 4882.
	int  diaDeLaSemana();
	int  diaDelAnio( int anio, int mes, int dia ); // Días pasados desde el (01-01-anio)
	int  diaDelAnio();
	int  diasPasados( int anio, int mes, int dia ); // Días pasados desde el (01-01-01)
	int  diasPasados();
	void diferencia( const Tiempo& tiempo, int& anios, int& meses, int& dias );
	int  primerDomingo( int anio, int mes );
	int  ultimoDomingo( int anio, int mes );
	bool esVerano( int& anio, int& mes, int& dia, int& hora );
		    
private:
	time_t m_time;		// Segundos desde 01-Enero-1970.
	int m_anio;			// 1 ~ 4882
	int m_mes;			// 1 ~ 12
	int m_dia;			// 1 ~ 31
	int m_hora;			// 0 ~ 23
	int m_minuto;		// 0 ~ 59
	int m_segundo;		// 0 ~ 59
	int m_diaSemana;	// 1 = Lunes ~ 7 = Domingo
	int m_diaAnio;		// 1 ~ 365/366
	bool m_verano;		// si ~ no
};

#endif


