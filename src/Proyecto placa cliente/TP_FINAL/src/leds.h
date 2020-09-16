/*
 * leds.h
 *
 *  Created on: 3 nov. 2018
 *      Author: Ezequiel
 */

#ifndef LEDS_H_
#define LEDS_H_


void
leds_Init (void);

void
led_verde_ON (void);

void
led_verde_OFF (void);

void
led_amarillo_ON (void);

void
led_amarillo_OFF (void);

void
led_rojo_ON (void);

void
led_rojo_OFF (void);

void
led_azul_ON (void);

void
led_azul_OFF (void);

void
leds_ON (unsigned short ledV, unsigned short ledA, unsigned short ledR, unsigned short ledAz);

void
leds_OFF (unsigned short ledV, unsigned short ledA, unsigned short ledR, unsigned short ledAz);

void
led_color_ON (unsigned short color);

void
led_color_OFF (unsigned short color);

#endif /* LEDS_H_ */
