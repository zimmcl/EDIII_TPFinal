/*
 * lcd.c
 *
 *  Created on: 14 oct. 2018
 *      Author: Ezequiel
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "LPC17xx.h"
#include "lcd.h"

uint8_t LCD_RS; // LOW: comando.  HIGH: dato.
uint8_t LCD_RW; // LOW: escribir en LCD.  HIGH: leer desde LCD.
uint8_t LCD_EN; // activacion por un pulso en HIGH.

uint8_t LCD_D4;
uint8_t LCD_D5;
uint8_t LCD_D6;
uint8_t LCD_D7;

/*
 * Inicialziacion de LCD. Se admite cualquier pin del GPIO0 para funciones
 * de control y cualquier pin del GPIO2 para datos.
 * */

void
Lcd_Init (uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d4, uint8_t d5,
	  uint8_t d6, uint8_t d7)
{
  LCD_RS = rs;
  LCD_RW = rw;
  LCD_EN = enable;

  LCD_D4 = d4;
  LCD_D5 = d5;
  LCD_D6 = d6;
  LCD_D7 = d7;

  // Configura todos los pines del LCD como salida
  LPC_GPIO2->FIODIR |= ((1 << d4) | (1 << d5) | (1 << d6) | (1 << d7));
  LPC_GPIO0->FIODIR |= ((1 << rs) | (1 << rw) | (1 << enable));

  Lcd_CmdWrite (CLEAR_DISPLAY);		// Borra datos previos del LCD
  Lcd_CmdWrite (BACK_HOME); 		// Inicializa el LCD en modo 4-bit
  Lcd_CmdWrite (_2_LINE_MATRIX);        // Habilitado modo 5x7 para caracteres
  Lcd_CmdWrite (D_ON_C_BLK);            // Display OFF, Cursor ON
  Lcd_CmdWrite (CLEAR_DISPLAY);         // Borra datos previos del LCD
  Lcd_CmdWrite (C_FIRST_LINE);     	// Fuerza al cursos a la posicion (1,1)

}

/*
 * Envia comandos al LCD para ajustar su funcionamiento
 */

void
Lcd_CmdWrite (char cmd)
{
  enviarNibble ((cmd >> 0x04) & 0x0F);  // Enviar el nibble más significativo
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RS); 	// Enviar pulso BAJO al pin RS  para seleccion de MODO Comandos
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RW); 	// Enviar pulso BAJO al pin RW para establecer operacion de escritura
  LPC_GPIO0->FIOPIN |= (1 << LCD_EN);  	// Generar un pulso HIGH-LOW en el pin EN
  delay (10000);
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_EN);

  delay (100000);

  enviarNibble (cmd & 0x0F);            // Enviar el nibble menos significativo
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RS); 	// Enviar pulso BAJO al pin RS  para seleccion de MODO Comandos
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RW); 	// Enviar pulso BAJO al pin RW para establecer operacion de escritura
  LPC_GPIO0->FIOPIN |= (1 << LCD_EN);  	// Generar un pulso HIGH-LOW en el pin EN
  delay (10000);
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_EN);

  delay (100000);
}

/*
 * Envia los datos a ser mostrados en el LCD
 */

void
Lcd_DataWrite (char dato)
{
  enviarNibble ((dato >> 0x04) & 0x0F); // Enviar el nibble más significativo
  LPC_GPIO0->FIOPIN |= (1 << LCD_RS); 	// Enviar pulso ALTO al pin RS  para seleccion de MODO Datos
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RW); 	// Enviar pulso BAJO al pin RW para establecer operacion de escritura
  LPC_GPIO0->FIOPIN |= (1 << LCD_EN);  	// Generar un pulso HIGH-LOW en el pin EN
  delay (10000);
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_EN);

  delay (100000);

  enviarNibble (dato & 0x0F);           // Enviar el nibble menos significativo
  LPC_GPIO0->FIOPIN |= (1 << LCD_RS); 	// Enviar pulso ALTO al pin RS  para seleccion de MODO Datos
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RW); 	// Enviar pulso BAJO al pin RW para establecer operacion de escritura
  LPC_GPIO0->FIOPIN |= (1 << LCD_EN);  	// Generar un pulso HIGH-LOW en el pin EN
  delay (10000);
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_EN);

  delay (100000);
}

/*
 * Envia por los pines de datos los 4 bits que se envian como parametro
 */

void
enviarNibble (char nibble)
{
  // Limpiar datos anteriores
  LPC_GPIO2->FIOPIN &= ~(((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6)
      | (1 << LCD_D7)));
  LPC_GPIO2->FIOPIN |= (((nibble >> 0x00) & 0x01) << LCD_D4);
  LPC_GPIO2->FIOPIN |= (((nibble >> 0x01) & 0x01) << LCD_D5);
  LPC_GPIO2->FIOPIN |= (((nibble >> 0x02) & 0x01) << LCD_D6);
  LPC_GPIO2->FIOPIN |= (((nibble >> 0x03) & 0x01) << LCD_D7);
}

/*
 * Posiciona el cursor en la posicion definida por los parametros FILA y COLUMNA
 */

void
Lcd_posCursor (uint8_t fila, uint8_t columna)
{
  uint8_t i;
  Lcd_CmdWrite (BACK_HOME);		//Posicionamos cursos en el origen
  switch (fila)
    {
    case (1):
      Lcd_CmdWrite (C_FIRST_LINE);	//Cursor en primera fila
      break;
    case (2):
      Lcd_CmdWrite (C_SECOND_LINE);	//Cursor en segunda fila
      break;
    default:
      break;
    }
  for (i = 0; i < columna - 2; i++)
    {
      Lcd_CmdWrite (SHIFT_C_P_RIGHT);	//Desplazamos el cursor a la columna indicada
    }
  //Deshabilitamos el cursor para evitar su desplazamiento cuando se envian datos al LCD
  Lcd_CmdWrite (D_ON_C_OFF);

}

/*
 * Transfiere el bloque de informacion que se pasa como parametro al LCD
 */

void
Lcd_Send (char * texto)
{
  for (int i = 0; texto[i] != 0; i++)
    {
      Lcd_DataWrite (texto[i]);
      Lcd_CmdWrite (SHIFT_C_RIGHT);
    }
}

/*
 * Borra toda infomacion existente en el LCD
 */
void
Lcd_Clear (void)
{
  Lcd_CmdWrite (CLEAR_DISPLAY);
}

/*
 * Ajusta el texto en el LCD y alternativamente puede generar un retardo.
 */
void Lcd_Show (char *mensaje, uint8_t fila, uint8_t columna, uint8_t clear, uint32_t tiempo)
{
  if(clear)
    {
      Lcd_Clear ();
    }
  if(fila != 0 && columna != 0)
    {
      Lcd_posCursor (fila, columna);
    }
  Lcd_Send (mensaje);
  if(tiempo != 0)
    {
      delay(tiempo);
    }
}


void
delay (uint32_t tiempo)
{
  for (uint32_t i = 0; i < tiempo; i++)
    ;
}
