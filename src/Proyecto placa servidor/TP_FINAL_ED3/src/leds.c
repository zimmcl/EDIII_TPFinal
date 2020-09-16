/*
 * leds.c
 *
 *  Created on: 3 nov. 2018
 *      Author: Ezequiel
 */

#include "leds.h"
#include "configuracion.h"
#include "activacion.h"


/*
 *	Funcion que inicializa registros del GPIO2 para el uso de
 *	los leds de los botones
 */
void
leds_Init (void)
{
  LPC_GPIO2->FIODIR |= (1 << LED_Ve) | (1 << LED_Ro) | (1 << LED_Az)
      | (1 << LED_Am);
}

/*
 *	Funcion que enciende el led verde. Si se desea generar tambien el
 *	tono se debe setear el valor de SELMODE a 10
 */
void
led_verde_ON (void)
{
  LPC_GPIO2->FIOSET |= (1 << LED_Ve);
  if (get_SELMODE () == 10)
    {
      EINT0_IRQHandler ();
    }
}

/*
 *	Funcion que apaga el led verde
 */
void
led_verde_OFF (void)
{
  LPC_GPIO2->FIOCLR |= (1 << LED_Ve);
}

/*
 *	Funcion que enciende el led amarillo. Si se desea generar tambien el
 *	tono se debe setear el valor de SELMODE a 10
 */
void
led_amarillo_ON (void)
{
  LPC_GPIO2->FIOSET |= (1 << LED_Am);
  if (get_SELMODE () == 10)
    {
      EINT3_IRQHandler ();
    }
}

/*
 *	Funcion que apaga el led amarillo.
 */
void
led_amarillo_OFF (void)
{
  LPC_GPIO2->FIOCLR |= (1 << LED_Am);
}

/*
 *	Funcion que enciende el led rojo. Si se desea generar tambien el
 *	tono se debe setear el valor de SELMODE a 10
 */
void
led_rojo_ON (void)
{
  LPC_GPIO2->FIOSET |= (1 << LED_Ro);
  if (get_SELMODE () == 10)
    {
      EINT1_IRQHandler ();
    }
}

/*
 *	Funcion que apaga el led rojo.
 */
void
led_rojo_OFF (void)
{
  LPC_GPIO2->FIOCLR |= (1 << LED_Ro);
}

/*
 *	Funcion que enciende el led azul. Si se desea generar tambien el
 *	tono se debe setear el valor de SELMODE a 10
 */
void
led_azul_ON (void)
{
  LPC_GPIO2->FIOSET |= (1 << LED_Az);
  if (get_SELMODE () == 10)
    {
      EINT2_IRQHandler ();
    }
}

/*
 *	Funcion que apaga el led azul
 */
void
led_azul_OFF (void)
{
  LPC_GPIO2->FIOCLR |= (1 << LED_Az);
}

/*
 *	Funcion que enciende leds segun los parametros de entrada ledV, ledA, ledR, ledAz
 *		ledV 	- Verde
 *		ledA 	- Amarillo
 *		ledR 	- Rojo
 *		ledAz 	- Azul
 */
void
leds_ON (unsigned short ledV, unsigned short ledA, unsigned short ledR, unsigned short ledAz)
{
  if (ledV == 1) led_verde_ON ();
  if (ledA == 1) led_amarillo_ON ();
  if (ledR == 1) led_rojo_ON ();
  if (ledAz == 1) led_azul_ON ();
}

/*
 *	Funcion que apaga leds segun los parametros de entrada ledV, ledA, ledR, ledAz
 *		ledV 	- Verde
 *		ledA 	- Amarillo
 *		ledR 	- Rojo
 *		ledAz 	- Azul
 */
void
leds_OFF (unsigned short ledV, unsigned short ledA, unsigned short ledR, unsigned short ledAz)
{
  if (ledV == 1) led_verde_OFF ();
  if (ledA == 1) led_amarillo_OFF ();
  if (ledR == 1) led_rojo_OFF ();
  if (ledAz == 1) led_azul_OFF ();
}

/*
 *	Funcion que enciende un led segun el parametro de entrada color
 *		0 - Verde
 *		1 - Amarillo
 *		2 - Rojo
 *		3 - Azul
 */
void
led_color_ON (unsigned short color)
{
  if (color == 0) led_verde_ON ();
  if (color == 1) led_rojo_ON ();
  if (color == 2) led_azul_ON ();
  if (color == 3) led_amarillo_ON ();
}

/*
 *	Funcion que enciende apaga un led segun el parametro de entrada color
 *		0 - Verde
 *		1 - Amarillo
 *		2 - Rojo
 *		3 - Azul
 */
void
led_color_OFF (unsigned short color)
{
  if (color == 0) led_verde_OFF ();
  if (color == 1) led_rojo_OFF ();
  if (color == 2) led_azul_OFF ();
  if (color == 3) led_amarillo_OFF ();
}

