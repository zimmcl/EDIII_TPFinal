/*
 * activacion.c
 *
 *  Created on: 3 nov. 2018
 *      Author: Ezequiel
 */

#include "activacion.h"
#include "leds.h"
#include "simon.h"
#include <stdio.h>

#define TONO_A	51685
#define TONO_B	83702
#define TONO_C	127272
#define TONO_D	178379

uint8_t SEL_MODE = 0;
uint8_t PULSADOR = 0;
uint8_t SECUENCIA = 1;
uint8_t CONEXION = 1;
uint8_t ENABLE = 0;
/*Para indicar si ya se inicio comunicacion*/
uint8_t START_COM = 0;
/*Para indicar si recibi tono del otro jugador*/
uint8_t RCV_TONO = 0;
/*Para indicar si esta en curso el juego*/
uint8_t GAME_ON = 0;



char byte_to_send = 0;
char read_byte = 0;

char *buffer_recep;

/*
 * Empleado para seleccion del nivel de dificultad y el modo
 * de juego. Lanza el inicio del tono para el LED Verde y
 * activa el TIMER0 para la duracion del Tono.
 */

void
EINT0_IRQHandler (void)
{
  NVIC_DisableIRQ (EINT0_IRQn);
  while (debounce (LPC_GPIO2->FIOPIN >> 10 & 1))
    {
      retardo (100000);
    }

  LPC_SC->EXTINT |= 1;				//Bajo banderas de EINT0
  NVIC_EnableIRQ (EINT0_IRQn);
  if (SEL_MODE == 0)
    {
      PULSADOR = 1;
      SEL_MODE = 1;
      return;
    }
  if (SEL_MODE == 1)
    {
      PULSADOR = 1;
      SEL_MODE = 2;
      return;
    }
  if (SEL_MODE == 10)
    {
      NVIC_DisableIRQ (EINT1_IRQn);
      NVIC_DisableIRQ (EINT2_IRQn);
      NVIC_DisableIRQ (EINT3_IRQn);
      setTono (TONO_D);
      LPC_TIM0->TCR |= 1;			//Habilito cuenta del TIMER0
      //LPC_GPIO2->FIOSET |= (1 << 0);
    }
}

/*
 * Empleado para seleccion del nivel de dificultad y el modo
 * de juego. Lanza el inicio del tono para el LED Rojo y
 * activa el TIMER0 para la duracion del Tono.
 */

void
EINT1_IRQHandler (void)
{
  NVIC_DisableIRQ (EINT1_IRQn);
  while (debounce (LPC_GPIO2->FIOPIN >> 11 & 1))
    {
      retardo (100000);
    }

  NVIC_EnableIRQ (EINT1_IRQn);
  LPC_SC->EXTINT |= (1 << 1);			//Bajo banderas de EINT1
  if (SEL_MODE == 0)
    {
      PULSADOR = 2;
      SEL_MODE = 1;
      return;
    }
  if (SEL_MODE == 1)
    {
      PULSADOR = 2;
      SEL_MODE = 2;
      return;
    }

  if (SEL_MODE == 10)
    {
      NVIC_DisableIRQ (EINT0_IRQn);
      NVIC_DisableIRQ (EINT2_IRQn);
      NVIC_DisableIRQ (EINT3_IRQn);
      setTono (TONO_C);
      LPC_TIM0->TCR |= 1;			//Habilito cuenta del TIMER0
      //LPC_GPIO2->FIOSET |= (1 << 1);
    }
}

/*
 * Empleado para seleccion del nivel de dificultad y el modo
 * de juego. Lanza el inicio del tono para el LED Azul y
 * activa el TIMER0 para la duracion del Tono.
 */

void
EINT2_IRQHandler (void)
{
  NVIC_DisableIRQ (EINT2_IRQn);
  while (debounce (LPC_GPIO2->FIOPIN >> 12 & 1))
    {
      retardo (100000);
    }

  NVIC_EnableIRQ (EINT2_IRQn);
  LPC_SC->EXTINT |= (1 << 2);			//Bajo banderas de EINT2
  if (SEL_MODE == 0)
    {
      PULSADOR = 3;
      SEL_MODE = 1;
      return;
    }
  if (SEL_MODE == 1)
    {
      PULSADOR = 3;
      SEL_MODE = 2;
      return;
    }

  if (SEL_MODE == 10)
    {
      NVIC_DisableIRQ (EINT0_IRQn);
      NVIC_DisableIRQ (EINT1_IRQn);
      NVIC_DisableIRQ (EINT3_IRQn);
      setTono (TONO_B);
      LPC_TIM0->TCR |= 1;			//Habilito cuenta del TIMER0
      //LPC_GPIO2->FIOSET |= (1 << 2);
    }
}

/*
 * Empleado para seleccion del nivel de dificultad y el modo
 * de juego. Lanza el inicio del tono para el LED Amarillo y
 * activa el TIMER0 para la duracion del Tono.
 */

void
EINT3_IRQHandler (void)
{
  NVIC_DisableIRQ (EINT3_IRQn);
  while (debounce (LPC_GPIO2->FIOPIN >> 13 & 1))
    {
      retardo (100000);
    }

  NVIC_EnableIRQ (EINT3_IRQn);
  LPC_SC->EXTINT |= (1 << 3);			//Bajo banderas de EINT3
  if (SEL_MODE == 0)
    {
      PULSADOR = 4;
      SEL_MODE = 1;
      return;
    }
  if (SEL_MODE == 1)
    {
      PULSADOR = 4;
      SEL_MODE = 2;
      return;
    }
  if (SEL_MODE == 10)
    {
      NVIC_DisableIRQ (EINT0_IRQn);
      NVIC_DisableIRQ (EINT1_IRQn);
      NVIC_DisableIRQ (EINT2_IRQn);
      setTono (TONO_A);
      LPC_TIM0->TCR |= 1;			//Habilito cuenta del TIMER0
      //LPC_GPIO2->FIOSET |= (1 << 3);
    }
}

/**
 * Cuando se cumple el tiempo del Tono, pone el bajo la salida y apaga
 * los Leds. Desactiva el SysTick y el contador del TIMER0.
 *
 */
void
TIMER0_IRQHandler (void)
{
  sysTick_OFF ();
  LPC_TIM0->IR |= 1;		//Bajo bandera de interrupcion por match0
  LPC_GPIO0->FIOSET |= 1;		//Bajamos salida del tono
  LPC_TIM0->TCR &= ~1;			//Deshabilitamos el contador
  LPC_TIM0->TCR |= (1 << 1);		//Reseteamos el TC y PC
  LPC_TIM0->TCR &= ~(1 << 1);		//Salimos del estado Reset
  LPC_TIM0->IR |= ~0;
  LPC_GPIO2->FIOCLR |= (1 << LED_Ve | 1 << LED_Ro | 1 << LED_Az | 1 << LED_Am);
  NVIC_EnableIRQ (EINT0_IRQn);
  NVIC_EnableIRQ (EINT1_IRQn);
  NVIC_EnableIRQ (EINT2_IRQn);
  NVIC_EnableIRQ (EINT3_IRQn);
  SECUENCIA = 0;
}

/**
 * Se emplea para controlar el tiempo de espera para conexion multijugador (1s).
 * Desactiva el contador del TIMER1.
 *
 */

void
TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR |= 1;		//Bajo bandera de interrupcion por match0
  LPC_TIM1->TCR &= ~1;			//Deshabilitamos el contador
  LPC_TIM1->TCR |= (1 << 1);		//Reseteamos el TC y PC
  LPC_TIM1->TCR &= ~(1 << 1);		//Salimos del estado Reset
  LPC_TIM1->IR |= ~0;
  CONEXION = 0;
}

/*
 *
 */

void
UART0_IRQHandler (void)
{


  if (START_COM  == 0 ){

	  read_byte = LPC_UART0->RBR;
	  //printf("buffer: %d",read_byte);
	  //printf("\n");

	  if( read_byte == 1 ){

		  printf("Inicio de comunicacion multijugador \n");
	  	  byte_to_send = 2;
	  	  while( ( (LPC_UART0->LSR >> 5) & 1 ) ==0 );
	  	  LPC_UART0 -> THR = byte_to_send;
	  	  ENABLE = 1;
	  	  START_COM = 1;
	  	  GAME_ON = 1;

	  }else{
		  GAME_ON = 0;
	  	  ENABLE = 0;
	  	  START_COM = 0;
	  	  printf("Aun no se inicio comunicacion con otro jugador\n");

	  }

  } else if ( START_COM  == 1 ){

	  read_byte = LPC_UART0->RBR;
	  //printf("Recibi el byte: %d ",read_byte);
	  //printf("\n");

	  if (read_byte != 0xFF){

		  /*Lo tomo como dato valido*/
		  RCV_TONO = 1;
		  GAME_ON  = 1;


	  } else{

		  GAME_ON = 0; //Fin de juego

	  }


  }

  //printf("RCV_TONO %d \n ", RCV_TONO);

  //printf("Sali de la interrupcion por uart \n");

}

/*
 * Conmuta el estado del pin asociado al BUZZER para generar
 * la onda cuadrada del Tono.
 */

void
SysTick_Handler (void)
{
  LPC_GPIO0->FIOPIN = ~(LPC_GPIO0->FIOPIN >> 9 & 1) << 9;
}

/*
 * Comprueba en que posicion se encuentra el Stick y devuelve
 * el valor correspondiente a la posicion de este:
 * 	Superior:	- 1
 * 	Derecha: 	- 2
 * 	Inferior: 	- 3
 * 	Izquierda:	- 4
 * 	Reposo:		- 0
 */

uint8_t
movimiento_Stick (void)
{
  uint16_t digital_X, digital_Y;
  float X_vol, Y_vol;
  digital_X = (LPC_ADC->ADDR0 >> 4 & 0xFFF);
  digital_Y = (LPC_ADC->ADDR1 >> 4 & 0xFFF);
  X_vol = (digital_X * 3.3 / 4095.0);
  Y_vol = (digital_Y * 3.3 / 4095.0);
  //printf ("X_vol: %f\n", X_vol);
  //printf ("Y_vol: %f\n", Y_vol);

  if (Y_vol > 2.5)
    {
      return 1;
    }
  else if (X_vol > 2.5)
    {
      return 2;
    }
  else if (Y_vol < 0.7)
    {
      return 3;
    }
  else if (X_vol < 0.7)
    {
      return 4;
    }
  else return 0;
}

/*
 * Borra la variable de control SELMODE
 */

void
clear_SELMODE (void)
{
  SEL_MODE = 0;
}

/*
 * Devuelve el valor de la variable PULSADOR. Variable asociada
 * con los botonos, los valores son:
 * 	Superior:	-1
 * 	Derecho:	-2
 * 	Inferior:	-3
 * 	Izquierda:	-4
 * 	Default:	-0
 *
 */

uint8_t
getPulsador (void)
{
  return PULSADOR;
}

/*
 * Pone a valor por defecto la variable PULSADOR
 */

void
clearPulsador (void)
{
  PULSADOR = 0;
}

/*
 * Setea la variable de control SELMODE
 */

void
set_SELMODE (uint8_t valor)
{
  SEL_MODE = valor;
}

/*
 * Obtener el valor de la variable de control SELMODE
 */

uint8_t
get_SELMODE (void)
{
  return SEL_MODE;
}

uint8_t
get_SECUENCIA (void)
{
  return SECUENCIA;
}

void
set_SECUENCIA (uint8_t valor)
{
  SECUENCIA = valor;
}

uint8_t
get_CONEXION (void)
{
  return CONEXION;
}

void
set_CONEXION (uint8_t valor)
{
  CONEXION = valor;
}


uint8_t
get_ENABLE(void)
{
  return ENABLE;
}

void
set_ENABLE (uint8_t valor)
{
  ENABLE= valor;
}



/*
 * Devuelve el contenido del buffer de recepcion
 */

char
get_Buffer (void)
{
  return *buffer_recep;
}


/*Para manejo de variables en el modo 4 (comunicacion UART)*/
char
get_read_byte(void){

	return read_byte;
}

void
set_read_byte(char bts ){

	read_byte = bts;


}


/*Para recepcion de tonos en modo multijugador*/
uint8_t
get_user_tono(void){
    if (RCV_TONO==1){
    	return read_byte;
    } else {
    	return 6;
    }

}

/*Para saber si el juego esta en curso*/
uint8_t
get_GAME_ON(void){
	return GAME_ON;

}

void
set_GAME_ON(uint8_t value){
	GAME_ON = value;

}


void
set_START_COM(uint8_t value){
	START_COM = value;

}




/*
 * Envia por UART los datos pasados por parametro
 */

void
Uart_Send (char * dato)
{
  while( ( (LPC_UART0->LSR >> 5) & 1 ) ==0 );
  LPC_UART0->THR = *dato;
}

/*
 * Algoritmo Antirrebote
 */

int
debounce (uint8_t Sample)
{
  unsigned static int Samples[NUM_SAMPLES];
  unsigned static int index = 0;
  unsigned static int LastDebounceResult = 0;
  unsigned int andTerm = Sample;
  unsigned int orTerm = Sample;

  Samples[index++] = Sample;
  if (index >= NUM_SAMPLES) index = 0;

  for (int j = 0; j < NUM_SAMPLES; j++)
    {
      andTerm &= Samples[j];
      orTerm |= Samples[j];
    }
  LastDebounceResult = ((LastDebounceResult & orTerm) | andTerm);
  return LastDebounceResult;
}
