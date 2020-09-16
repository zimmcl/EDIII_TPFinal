/*
 * lcd.h
 *
 *  Created on: 14 oct. 2018
 *      Author: Ezequiel
 */

#ifndef LCD_H_
#define LCD_H_

//Comandos
#define CLEAR_DISPLAY 	0x01	//Borra datos desplegados
#define BACK_HOME	0x02	//Cursor en posicion (0,0)
#define SHIFT_C_LEFT	0x04	//Desplaza el cursor hacia la izquierda 1 posicion
#define SHIFT_C_RIGHT	0x06	//Desplaza el cursor hacia la derecha 1 posicion
#define	SHIFT_D_LEFT	0x05	//Desplaza display hacia la izquierda 1 posicion
#define SHIFT_D_RIGHT	0x07	//Desplaza display hacia la derecha 1 posicion
#define D_OFF_C_OFF	0x08	//Display apagado, cursor deshabilitado
#define	D_OFF_C_ON	0x0A	//Display apagado, cursor habilitado
#define D_ON_C_OFF	0x0C	//Display encendido, cursor deshabilitado
#define	D_ON_C_BLK	0x0E	//Display encendido, cursor intermitente
#define	D_ON_C_BLK	0x0F	//Display encendido, cursor intermitente
#define	SHIFT_C_P_LEFT	0x10	//Posicionar cursor a la izquierda
#define	SHIFT_C_P_RIGHT	0x14	//Posicionar cursor a la derecha
#define	SHIFT_E_D_LEFT	0x18	//Desplazamiento completo a la izquierda
#define	SHIFT_E_D_RIGHT	0x1C	//Desplazamiento completo a la derecha
#define	C_FIRST_LINE	0x80	//Cursor a la primera fila
#define	C_SECOND_LINE	0xC0	//Cursor a la segunda fila
#define	_2_LINE_MATRIX	0x38	//2 lineas y matriz 5x7




#endif /* LCD_H_ */
