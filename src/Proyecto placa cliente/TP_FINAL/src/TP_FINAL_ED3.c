/*
 ===============================================================================
 Name        : TP_FINAL_ED3.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "configuracion.h"
#include "leds.h"
#include "simon.h"
#include "lcd.h"

int
main (void)
{
  int juego = 0, nivel = 0, exito = 0;

  clearPulsador ();
  clear_SELMODE ();
  perifericos_Init ();
  leds_Init ();
  Lcd_Init (LCD_RS, LCD_RW, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
  Lcd_Show ("SIMON", 1, 7, 0, 0);
  Lcd_Show ("--EDIII--", 2, 5, 0, 0);
  simon_Init (1000000);

  while (1)
    {
      Lcd_Show ("1) 4", 1, 1, 1, 0);
      Lcd_Show ("2) 8", 1, 10, 0, 0);
      Lcd_Show ("3) 12", 2, 1, 0, 0);
      Lcd_Show ("4) 16", 2, 10, 0, 0);
      while (!nivel)
	{
	  nivel = getPulsador ();
	  LPC_SC->EXTINT |= ~0;
	  retardo (500000);
	}
      clearPulsador ();
      clear_SELMODE ();
      Lcd_Show ("1)Clasi", 1, 1, 1, 0);
      Lcd_Show ("2)Inver", 1, 10, 0, 0);
      Lcd_Show ("3)Avanz", 2, 1, 0, 0);
      Lcd_Show ("4)Multi", 2, 10, 0, 0);
      juego = 0;
      while (!juego)
	{
	  juego = getPulsador ();
	  LPC_SC->EXTINT |= ~0;
	  retardo (500000);
	}

      switch (juego)
	{
	case (1):
//	  Lcd_Show ("MODO CLASICO", 1, 4, 1, 2500000);
//	  Lcd_Show ("3", 2, 9, 0, 2500000);
//	  Lcd_Show ("2", 2, 9, 0, 2500000);
//	  Lcd_Show ("1", 2, 9, 0, 2500000);
//	  Lcd_Show ("A JUGAR!", 1, 6, 1, 5000000);
	  modo_1 (nivel, &exito);
	  break;
	case (2):
//	  Lcd_Show ("MODO INVERSO", 1, 4, 1, 2500000);
//	  Lcd_Show ("3", 2, 9, 0, 2500000);
//	  Lcd_Show ("2", 2, 9, 0, 2500000);
//	  Lcd_Show ("1", 2, 9, 0, 2500000);
//	  Lcd_Show ("A JUGAR!", 1, 6, 1, 5000000);
	  modo_2 (nivel, &exito);
	  break;
	case (3):
	  printf("Modo avanzado JOYSTICK: \n");
	  //Lcd_Show ("MODO AVANZADO", 1, 3, 1, 2500000);
	  // Lcd_Show ("3", 2, 9, 0, 2500000);
	  //Lcd_Show ("2", 2, 9, 0, 2500000);
	  //Lcd_Show ("1", 2, 9, 0, 2500000);
	  //Lcd_Show ("A JUGAR!", 1, 6, 1, 5000000);
	  modo_3 (nivel, &exito);
	  break;
	case (4):
	  //Lcd_Show ("MULTIJUGADOR", 1, 4, 1, 2500000);
	  //Lcd_Show ("3", 2, 9, 0, 2500000);
	  //Lcd_Show ("2", 2, 9, 0, 2500000);
	  //Lcd_Show ("1", 2, 9, 0, 2500000);
	  printf("\n Entre en modo 4 \n");
	  modo_4 (nivel, &exito);
	  break;
	}

      if (juego != 4)
	{
	  if (exito)
	    {
	      Lcd_Show ("GANASTE!", 1, 5, 1, 0);
	      simon_SUCCESS ();
	    }
	  else
	    {
	      Lcd_Show ("PERDISTE!", 1, 5, 1 , 0);
	      simon_FAIL ();
	    }
	}
      clear_SELMODE ();
      nivel = 0;
      juego = 0;
      clearPulsador ();

      Lcd_Show ("A JUGAR!", 1, 6, 1, 0);
      simon_Init (500000);

    }

}

