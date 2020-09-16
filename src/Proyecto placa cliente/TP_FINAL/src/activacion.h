/*
 * activacion.h
 *
 *  Created on: 3 nov. 2018
 *      Author: Ezequiel
 */

#include "configuracion.h"


#ifndef ACTIVACION_H_
#define ACTIVACION_H_

#define TONO_A	51685
#define TONO_B	83702
#define TONO_C	127272
#define TONO_D	178379
#define NUM_SAMPLES 50




void
EINT0_IRQHandler (void);
void
EINT1_IRQHandler (void);
void
EINT2_IRQHandler (void);
void
EINT3_IRQHandler (void);
void
TIMER0_IRQHandler (void);
void
TIMER1_IRQHandler (void);
void
UART0_IRQHandler (void);
void
SysTick_Handler (void);

void
clear_SELMODE (void);
void
set_SELMODE (uint8_t);
uint8_t
get_SELMODE (void);
uint8_t
getPulsador (void);
void
clearPulsador (void);
uint8_t
get_SECUENCIA (void);
void
set_SECUENCIA (uint8_t);
uint8_t
get_CONEXION (void);
void
set_CONEXION (uint8_t);
uint8_t
get_ENABLE (void);
void
set_ENABLE (uint8_t);
char
get_read_byte(void);
void
set_read_byte(char bts );

uint8_t
get_user_tono(void);
uint8_t
get_START_COM(void);
uint8_t
get_GAME_ON(void);
void
set_GAME_ON(uint8_t);
void
set_START_COM(uint8_t);



char
get_Buffer (void);
void
Uart_Send (char *);

uint8_t
movimiento_Stick (void);

int
debounce (uint8_t);

#endif /* ACTIVACION_H_ */
