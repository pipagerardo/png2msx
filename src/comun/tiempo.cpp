#include "tiempo.h"

// ----------------------------------------------------------------------------
// |                                T I E M P O                               |
// ----------------------------------------------------------------------------

Tiempo::Tiempo()  { inicia(); }
Tiempo::~Tiempo() { quita();  }

void Tiempo::inicia() { 
	time( &m_time ); 
	struct tm *p_tm = localtime( &m_time );
	m_anio = p_tm->tm_year + 1900;
	m_mes  = p_tm->tm_mon  + 1;
	m_dia  = p_tm->tm_mday;
	m_diaSemana = ( p_tm->tm_wday == 0 ) ? 7 : p_tm->tm_wday;
	m_diaAnio   = p_tm->tm_yday;
	m_verano    = ( p_tm->tm_isdst == 1 ) ? true : false;
	m_hora    = p_tm->tm_hour;
	m_minuto  = p_tm->tm_min;
	m_segundo = p_tm->tm_sec;	
}

void Tiempo::inicia( const Tiempo& tiempo ) { 
	m_time = tiempo.m_time;
	m_anio = tiempo.m_anio;
	m_mes  = tiempo.m_mes;
	m_dia  = tiempo.m_dia;
	m_diaSemana = tiempo.m_diaSemana;
	m_diaAnio   = tiempo.m_diaAnio;
	m_verano    = tiempo.m_verano;
	m_hora    = tiempo.m_hora;
	m_minuto  = tiempo.m_minuto;
	m_segundo = tiempo.m_segundo;
}

bool Tiempo::inicia( int anio, int mes, int dia, int hora, int minuto, int segundo ) {
	if( !tomaFecha( anio, mes, dia ) ) return false;
	if( !tomaHora( hora, minuto, segundo ) ) return false;
	m_verano = esVerano( m_anio, m_mes, m_dia, m_hora );
	return true;
}
	
void Tiempo::quita() {
	m_time = 0;
	m_anio = 1;
	m_mes  = 1;
	m_dia  = 1;
	m_diaSemana = 1;
	m_diaAnio   = 1;
	m_verano    = false;
	m_hora    = 0;
	m_minuto  = 0;
	m_segundo = 0;
}

bool Tiempo::tomaFecha( int anio, int mes, int dia ) {
	bool retorno = true;
	if      ( anio < 1    ) { m_anio = 1;		retorno = false; }
	else if ( anio > 4882 ) { m_anio = 4882;	retorno = false; }
	else					  m_anio = anio;
	if      ( mes  < 1    ) { m_mes  = 1;		retorno = false; }
	else if ( mes  > 12   ) { m_mes  = 12;		retorno = false; }
	else					  m_mes  = mes;
	int p_dia = diaMaximoMes();
	if      ( dia < 1     ) { m_dia = 1;		retorno = false; }
	else if ( dia > p_dia ) { m_dia = p_dia;	retorno = false; }
	else					  m_dia = dia;
	m_diaSemana = dameDiaSemana();
	m_diaAnio   = dameDiaAnio();
	return retorno;
}

bool Tiempo::tomaHora( int hora, int minuto, int segundo ) {
	bool retorno = true;
	if      ( hora    < 0  ) { m_hora    = 0;		retorno = false; }
	else if ( hora    > 23 ) { m_hora    = 23;		retorno = false; }
	else				       m_hora    = hora;
	if      ( minuto  < 0  ) { m_minuto  = 0;		retorno = false; }
	else if ( minuto  > 59 ) { m_minuto  = 59;		retorno = false; }
	else					   m_minuto  = minuto;	
	if      ( segundo < 0  ) { m_segundo = 0;		retorno = false; }
	else if ( segundo > 59 ) { m_segundo = 59;		retorno = false; }
	else					   m_segundo = segundo;
	return retorno;
}
bool Tiempo::calculaSiEsVerano() {
	m_verano = esVerano( m_anio, m_mes, m_dia, m_hora );
	return m_verano;
}
	
void Tiempo::dameFecha( int& anio, int& mes, int& dia ) {
	anio = m_anio;
	mes  = m_mes;
	dia  = m_dia;
}
void Tiempo::dameHora(	int& hora, int& minuto, int& segundo ) {
	hora    = m_hora;
	minuto  = m_minuto;
	segundo = m_segundo;
}
int  Tiempo::dameDiaSemana() const { return m_diaSemana; }
int  Tiempo::dameDiaAnio()   const { return m_diaAnio; }
bool Tiempo::esVerano()      const { return m_verano; }
	
int Tiempo::dameAnio()     const { return m_anio; }
int Tiempo::dameMes()      const { return m_mes;  }
int Tiempo::dameDia()      const { return m_dia; }
int Tiempo::dameHora()     const { return m_hora; }
int Tiempo::dameMinuto()   const { return m_minuto;  }
int Tiempo::dameSegundo()  const { return m_segundo;  }
    
// -----------------------------------------------------------------------------
//                           FUNCIONES ÚTILES 
// -----------------------------------------------------------------------------
// Calcula si un año es bisiesto.
// El calendario juliano trás muchas reformas en el año 44  tiene  bisiestos 
// cada catro años. Antes de esta fecha los años bisiestos y los meses  eran
// diferentes y sufrían contínuos cambios para adecuarse al año trópico.
// Hasta 1582 los años bisiestos eran cada cuatro años  pero esto acumula un 
// error de 1 día cada 150 años aprox. Para  el  Jueves 4 de Octubre de 1582 
// había 11 días  de desfase  y  se saltó  al Viernes 15 de Octubre de 1582.
// Entonces empiezan los años bisiestos cada cuatro años,  no los divisibles
// por 100 y si los divisibles por 400, siendo esto válido hasta el año 4882
// en el cual nuevamente se acumulará un día de desfase. 
// Para el año 4882 habrá que volver a modificar el calendario y como no soy
// adivino hasta aquí puedo leer.
bool Tiempo::esBisiesto( int anio ) {
	if( anio < 1 || anio > 4882 ) return false; // No puedo predecir el futuro.
    if( anio > 1582 ) {             // Bisiestos cada 4 años menos los acabados en 100 excepto los divisibles por 400.
        if( (anio % 4 == 0) && ((anio % 100 != 0) ||  (anio % 400 == 0)) ) return true;
        return false;
    }
    if( anio > 0 ) {                // Bisiestos cada 4 años. No existe el año cero.
        if(anio % 4 == 0) return true;
        return false;
    }
    return false;
}
bool Tiempo::esBisiesto() { return esBisiesto( m_anio ); }
// -----------------------------------------------------------------------------
// Calcular el día máximo de un mes determinado.
// Febrero 28 días en año no bisiesto y 29 días en años bisiestos.
// Abril, Junio, Septiembre y Noviembre 30 días.
// Enero, Marzo, Mayo, Julio, Agosto, Octubre y Diciembre 31 días.
int Tiempo::diaMaximoMes( int anio, int mes ) {
	if( anio < 1 || anio > 4882 ) return 0; 	
    if( mes == 0 ) { mes = 12; }
    if( mes > 12 ) { anio += (mes % 12); mes -= 12; }
    if( mes == 2 ) {
      if( esBisiesto( anio ) ) return 29;
      return 28;
    } else if( (mes == 4) || (mes == 6) || (mes == 9) || (mes == 11) ) return 30;
    return 31;
}
int Tiempo::diaMaximoMes() { return diaMaximoMes( m_anio, m_mes ); }
// -----------------------------------------------------------------------------
// Calcula el día de la semana, de Lunes 1, Martes 2, ... a Domingo 7.
// No es válido para fechas anteriores a 1583 ni superiores a 4882.
int Tiempo::diaDeLaSemanaZeller( int anio, int mes, int dia ) {
	if( mes <= 2 ) {
		mes = mes + 10;
		anio = anio - 1;
    } else {
		mes = mes - 2;
    }
	int a = anio % 100;
	int b = anio / 100;
	int resultado = ( 700 + ((26 * mes - 2) / 10) + dia + a + a / 4 + b / 4 - 2 * b ) % 7;
	return ( resultado == 0 ) ? 7 : resultado;
}
// -----------------------------------------------------------------------------
// Calcula el día de la semana, de Lunes 1, Martes 2, ... a Domingo 7.
// Empezamos el sabado 01 de enero del año 1, no existe el año cero.
// El lunes 1 de enero de 1582 debió ser viernes pero fué lunes.
// A partir del 15 de Octubre de 1582 se considera que el año empezó el viernes 1 de enero.
// Del jueves 04 de octubre de 1582 se salta al viernes 15 de octubre.     
// Los días del 5 al 14 de octubre de 1582 no existieron.
// El siguiente año es sábado 01 de enero de 1583.
// Lunes   01 de enero   de 1590.
// Lunes   01 de enero   de 1601.
// ...   ...   ...   ...   ...
// ...   ...   ...   ...   ...
// Lunes   01 de enero   de 3500.
// Y así hasta el año 4882 que habrá un día de error ???...
// Y todo esto es un resumen sólo válido para España, Portugal e Italia...
// Puesto que cada país fué a su bola y calcular fechas antiguas es muy complicado.
int Tiempo::diaDeLaSemana( int anio, int mes, int dia ) {
	if( anio < 1 || anio > 4882 ) return 0; 
	if( anio > 1582 ) return diaDeLaSemanaZeller( anio, mes, dia );
    int p_anio, p_dias;
    if( anio == 1582 ) { 
		p_anio = 1582;               					  // Ojo con el año 1582...
		if( (mes > 9) && (dia > 14) )       p_dias = 4;   // viernes  1 de enero   de 1582.
		else                                p_dias = 0; } // lunes    1 de enero   de 1582.
    else if( anio > 1402 ) { p_anio = 1403; p_dias = 0; } // lunes   01 de enero   de 1403.
    else if( anio >  999 ) { p_anio = 1000; p_dias = 0; } // lunes   01 de enero   de 1000.
    else if( anio >  500 ) { p_anio =  501; p_dias = 0; } // lunes   01 de enero   de  501.
    else if( anio >  321 ) { p_anio =  322; p_dias = 0; } // lunes   01 de enero   de  322.    
    else if( anio >  153 ) { p_anio =  154; p_dias = 0; } // lunes   01 de enero   de  154.    
    else                   { p_anio =    1; p_dias = 5; } // sabado  01 de enero   de    1.
    for( int i = p_anio; i < anio; ++i ) {    
        if( esBisiesto( i ) ) p_dias += 366;
        else                  p_dias += 365;
    }
    for( int i = 1; i < mes; ++i ) p_dias += diaMaximoMes( anio, i );
    p_dias += dia;
    p_dias %= 7; 
	if( p_dias == 0 ) p_dias = 7;
    return p_dias;
}
int Tiempo::diaDeLaSemana() { return diaDeLaSemana( m_anio, m_mes, m_dia ); }
// -----------------------------------------------------------------------------
// Calcular los días que han pasado desde el 1 de enero del año dado.
int Tiempo::diaDelAnio( int anio, int mes, int dia ) {
	if( anio < 1 || anio > 4882 ) return 0; 
    int  p_dias = 0;    
    for( int i = 1; i < mes; ++i ) {
        p_dias += diaMaximoMes( anio, i );
    }
    p_dias += dia;
    // Corregimos el año 1582 por su singularidad.
    if( anio == 1582 ) {
       if     ( mes >  10 )      p_dias -= 10;
       else if( mes == 10 ) {
            if     ( dia > 14 )  p_dias -= 10;
            else if( dia >  4 )  p_dias -= ( dia - 4 );
       }
    }
    return p_dias;
}
int Tiempo::diaDelAnio() { return diaDelAnio( m_anio, m_mes, m_dia ); }
// -----------------------------------------------------------------------------
// Calcular los días que han pasado desde el 1 de enero del año 1.
int Tiempo::diasPasados( int anio, int mes, int dia ) {
	if( anio < 1 || anio > 4882 ) return 0; 
    int  p_dias = 0;    
    for( int i = 1; i < anio; ++i ) {  
        if( esBisiesto( i ) ) p_dias += 366;
        else                  p_dias += 365;
    }
    for( int i = 1; i < mes; ++i ) {
        p_dias += diaMaximoMes( anio, i );
    }
    p_dias += dia;
    // Corregimos el año 1582 por su singularidad.
    if     ( anio >  1582 )      p_dias -= 10;
    else if( anio == 1582 ) {
       if     ( mes >  10 )      p_dias -= 10;
       else if( mes == 10 ) {
            if     ( dia > 14 )  p_dias -= 10;
            else if( dia >  4 )  p_dias -= ( dia - 4 );
       }
    }
    return p_dias;
}
int Tiempo::diasPasados() { return diasPasados( m_anio, m_mes, m_dia ); }

void Tiempo::diferencia( const Tiempo& tiempo, int& anios, int& meses, int& dias ) {
	int segundos = m_segundo - tiempo.m_segundo;
	int minutos  = m_minuto  - tiempo.m_minuto;
	int horas    = m_hora - tiempo.m_hora;
	    dias     = m_dia  - tiempo.m_dia;
	    meses    = m_mes  - tiempo.m_mes;
	    anios    = m_anio - tiempo.m_anio;
	if( segundos  < 0 ) { segundos +=   60; --minutos;  }
	if( minutos   < 0 ) { minutos  +=   60; --horas;    }
	if( horas     < 0 ) { horas    +=   24; --dias;     }
	if( dias      < 0 ) { dias     +=   diaMaximoMes( m_anio, m_mes - 1 ); --meses; }
	if( meses     < 0 ) { meses    +=   12; --anios; }
	if( anios     < 0 ) { anios    *=   -1;          }
}

int Tiempo::primerDomingo( int anio, int mes ) {
	int diaSemana = diaDeLaSemana( anio, mes, 1 );
	return 8 - diaSemana;
}

int Tiempo::ultimoDomingo( int anio, int mes ) {
	int dia = diaMaximoMes( anio, mes );
	int diaSemana = diaDeLaSemana( anio, mes, dia );
	if( diaSemana == 7 ) diaSemana = 0;
	return dia - diaSemana;
}

// En primavera el último domingo de marzo a las 2:00 son las 3:00
// En otoño el último domingo de octubre a las 3:00 son las 2:00
bool Tiempo::esVerano( int& anio, int& mes, int& dia, int& hora ) {
	if( ( mes < 3 ) || ( mes > 10 ) ) return false; // invierno seguro.
	if( ( mes > 3 ) && ( mes < 10 ) ) return true;  // verano seguro.	
	if( mes == 3 ) {
		int domingo = ultimoDomingo( anio, 3 );
		if( dia  < domingo ) return false;
		if( dia  > domingo ) return true;
		if( hora < 2       ) return false;
		return true;
	} else {
		int domingo = ultimoDomingo( anio, 10 );
		if( dia  > domingo ) return false;
		if( dia  < domingo ) return true;
		if( hora > 2       ) return false;
		return true;
	}
}

// -----------------------------------------------------------------------------
// |                           F I N   T I E M P O                             |
// -----------------------------------------------------------------------------




