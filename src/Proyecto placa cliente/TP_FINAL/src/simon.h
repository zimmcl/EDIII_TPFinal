/*
 * simon.h
 *
 *  Created on: 3 nov. 2018
 *      Author: Ezequiel
 */

#ifndef SIMON_H_
#define SIMON_H_

#include <stdlib.h>
#include <stdint.h>

#include "leds.h"
#include "activacion.h"
#include "configuracion.h"

#define MAX_TURNOS 20





void
simon_Init (uint32_t);

void
simon_SUCCESS (void);

void
simon_MultiSUCCESS (void);

void
simon_FAIL (void);

void
modo_1 (int nivel, int* exito);

void
modo_2 (int nivel, int* exito);

void
modo_3 (int nivel, int* exito);

void
modo_4 (int nivel, int* exito);

void
cambiar_Tiempo (uint8_t);

void
retardo (uint32_t);

#endif /* SIMON_H_ */
