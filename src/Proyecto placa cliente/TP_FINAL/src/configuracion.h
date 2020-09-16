/*
 * configuracion.h
 *
 *  Created on: 3 nov. 2018
 *      Author: Ezequiel
 */
#include "LPC17xx.h"

#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

//----------LCD-----------
#define LCD_D4	5		//PUERTO 2
#define LCD_D5	6		//PUERTO 2
#define LCD_D6	7		//PUERTO 2
#define LCD_D7	8		//PUERTO 2
#define LCD_RS	0		//PUERTO 0
#define LCD_RW	1		//PUERTO 0
#define LCD_EN	4		//PUERTO 0
//------------------------
#define BUZZ	9		//PUERTO 0
#define	LED_Ve	0		//PUERTO 2
#define	LED_Ro	1		//PUERTO 2
#define	LED_Az	2		//PUERTO 2
#define	LED_Am	3		//PUERTO 2
#define TX	2		//PUERTO 0
#define RX	3		//PUERTO 0

#define AddrSTCTRL	0xE000E010
#define AddrSTRELOAD	0xE000E014

void
nvic_sysConfig (uint8_t);
void
adcConfig (void);
void
eintConfig (void);
void
timerConfig (void);
void
gpioConfig (void);
void
pwmConfig (void);
void
uartConfig (void);
void
setTono (uint32_t);
void
sysTick_OFF (void);
void
setTiempo (uint8_t, uint32_t);
void
perifericos_Init (void);

#endif /* CONFIGURACION_H_ */
