/*
 * simon.c
 *
 *  Created on: 3 nov. 2018
 *      Author: Ezequiel
 */

/*
 *	Funcion que ejecuta una secuencia de colores y sonidos para indicar el inicio del juego
 */

#include "simon.h"
#include "configuracion.h"
#include "activacion.h"
#include "time.h"
#include "lcd.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Variable empleados en los modos de juego
char *mensaje;
char buffer[10];
uint32_t tiempos[4] =
  { 6250000, 5000000, 3750000, 2000000 };

/*
 * Funcion que ejecuta una secuencia de colores para indicar el inicio del juego
 */

void
simon_Init (uint32_t tiempo)
{
  int i;

  for (i = 0; i < 3; ++i)
    {
      leds_ON (1, 0, 0, 0);
      retardo (tiempo);
      leds_OFF (1, 0, 0, 0);

      leds_ON (0, 1, 0, 0);
      retardo (tiempo);
      leds_OFF (0, 1, 0, 0);

      leds_ON (0, 0, 1, 0);
      retardo (tiempo);
      leds_OFF (0, 0, 1, 0);

      leds_ON (0, 0, 0, 1);
      retardo (tiempo);
      leds_OFF (0, 0, 0, 1);
    }

  for (i = 0; i < 2; ++i)
    {
      leds_ON (1, 0, 0, 1);
      retardo (tiempo * 5);
      leds_OFF (1, 0, 0, 1);

      leds_ON (0, 1, 1, 0);
      retardo (tiempo * 5);
      leds_OFF (0, 1, 1, 0);
    }

  leds_ON (1, 0, 1, 0);
  retardo (tiempo * 5);
  leds_OFF (1, 0, 1, 0);

  leds_ON (0, 1, 0, 1);
  retardo (tiempo * 5);
  leds_OFF (0, 1, 0, 1);

  leds_ON (1, 1, 0, 0);
  retardo (tiempo * 5);
  leds_OFF (1, 1, 0, 0);

  leds_ON (0, 0, 1, 1);
  retardo (tiempo * 5);
  leds_OFF (0, 0, 1, 1);

  leds_ON (1, 1, 1, 1);
  retardo (tiempo * 5);
  leds_OFF (1, 1, 1, 1);

}

/*
 * Funcion que ejecuta una secuencia de colores para indicar el exito del jugador
 * en el juego.
 */

void
simon_SUCCESS (void)
{
  int i;
  for (i = 0; i < 4; i++)
    {
      set_SELMODE (0);
      leds_ON (1, 0, 0, 0);
      retardo (1500000);
      leds_ON (0, 1, 0, 0);
      retardo (1500000);
      leds_ON (0, 0, 0, 1);
      retardo (1500000);
      leds_ON (0, 0, 1, 0);
      retardo (1500000);

      leds_OFF (1, 0, 0, 0);
      retardo (1500000);
      leds_OFF (0, 1, 0, 0);
      retardo (1500000);
      leds_OFF (0, 0, 0, 1);
      retardo (1500000);
      leds_OFF (0, 0, 1, 0);
      retardo (1500000);
    }
}



void
simon_MultiSUCCESS (void)
{
  int i;
  for (i = 0; i < 2; i++)
    {
      set_SELMODE (0);
      leds_ON (1, 1, 1, 1);
      retardo (2500000);
      leds_OFF (1, 1, 1, 1);

    }
}

/*
 * Funcion que ejecuta una secuencia de colores para indicar el fracaso del jugador
 * en el juego
 */
void
simon_FAIL (void)
{
  int i;
  set_SELMODE (0);
  for (i = 0; i < 3; i++)
    {
      leds_ON (1, 1, 1, 1);
      retardo (3000000);
      leds_OFF (1, 1, 1, 1);
      retardo (3000000);
    }
}

/*
 * MODO 1: Modo Clásico de Simon. Repetir secuencia hasta un numero determinado
 * 	   de colores. Numero de turnos/colores en secuencia:
 *				- Nivel 1: 4 turnos
 *       		 	- Nivel 2: 8 turnos
 *				- Nivel 3: 12 turnos
 *				- Nivel 4: 16 turnos
 * 	Cada 4 turnos, disminuye el tiempo que se muestran los colores.
 *	La funcion devuelve si el jugador ha tenido exito o no en *exito.
 */
void
modo_1 (int nivel, int* exito)
{
  uint8_t i, turno, fallo, turno_jugador, boton_pulsado, numero_turnos,
      colores[MAX_TURNOS]; // Declaramos el vector de colores a acertar

  for (int i = 0; i < MAX_TURNOS; i++)
    {
      colores[i] = 0;
    }

  turno = 1;
  fallo = 0;
  set_SELMODE (10);
  srand (time (NULL));   		// Inicializamos semilla

  numero_turnos = (nivel * 4);

  // Bucle que hace el juego hasta que se gana o se pierde
  while (turno <= numero_turnos && fallo == 0)
    {
      sprintf (buffer, "%d", turno);
      Lcd_Show ("Turno:", 1, 1, 1, 0);
      Lcd_Show (buffer, 0, 0, 0, 0);
      Lcd_Show ("-", 0, 0, 0, 0);
      sprintf (buffer, "%d", numero_turnos);
      set_SELMODE (10);
      colores[turno - 1] = rand () % 4; // Nuevo color
      Lcd_Show (buffer, 0, 0, 0, 2000000);

      for (i = 0; i < turno; i++)	// Imprimimos la secuencia de colores
	{
	  set_SECUENCIA (1);
	  led_color_ON (colores[i]);
	  while (get_SECUENCIA ())
	    {
	    }
	  retardo (500000);
	}

      turno_jugador = 1;
      while (turno_jugador <= turno && fallo == 0)
	{
	  clear_SELMODE ();
	  clearPulsador ();
	  boton_pulsado = getPulsador ();
	  while (!boton_pulsado)
	    {
	      boton_pulsado = getPulsador ();
	      retardo (15000);
	    }

	  set_SELMODE (10);
	  set_SECUENCIA (1);
	  led_color_ON (colores[turno_jugador - 1]);
	  while (get_SECUENCIA ())
	    {
	    }
	  retardo (500000);

	  if ((boton_pulsado - 1) == colores[turno_jugador - 1]) turno_jugador++;
	  else fallo = 1;
	}
      turno++;

      if (turno % 4 == 0)
	{
	  cambiar_Tiempo (turno);
	}

    } // while

  if (fallo == 0) *exito = 1;
  else *exito = 0;

  setTiempo (0, tiempos[0]);
}

/*
 * MODO 2: Modo Inverso de Simon. Se denera repetir la secuencia hasta un numero determinado
 * 	   de colores de manera inversa. Numero de turnos/colores en secuencia:
 *				- Nivel 1: 4 turnos
 *       		 	- Nivel 2: 8 turnos
 *				- Nivel 3: 12 turnos
 *				- Nivel 4: 16 turnos
 * 	Cada 4 turnos, disminuye el tiempo que se muestran los colores.
 *	La funcion devuelve si el jugador ha tenido exito o no en *exito.
 */

void
modo_2 (int nivel, int* exito)
{
  int i, turno, fallo, turno_jugador, boton_pulsado, numero_turnos,
      colores[MAX_TURNOS]; // Declaramos el vector de colores a acertar

  for (int i = 0; i < MAX_TURNOS; i++)
    {
      colores[i] = 0;
    }

  turno_jugador = 0;
  boton_pulsado = 0;
  numero_turnos = 0;
  turno = 1;
  fallo = 0;
  set_SELMODE (10);
  srand (time (NULL));   		// Inicializamos semilla

  numero_turnos = nivel * 4;

  // Bucle que hace el juego hasta que se gana o se pierde
  while (turno <= numero_turnos && fallo == 0)
    {
      sprintf (buffer, "%d", turno);
      Lcd_Show ("Turno:", 1, 1, 1, 0);
      Lcd_Show (buffer, 0, 0, 0, 0);
      Lcd_Show ("-", 0, 0, 0, 0);
      sprintf (buffer, "%d", numero_turnos);
      set_SELMODE (10);
      colores[turno - 1] = rand () % 4; // Nuevo color
      Lcd_Show (buffer, 0, 0, 0, 2000000);

      for (i = 0; i < turno; i++)	// Imprimimos la secuencia de colores
	{
	  set_SECUENCIA (1);
	  led_color_ON (colores[i]);
	  while (get_SECUENCIA ())
	    {
	    }
	  retardo (500000);
	}

      turno_jugador = turno - 1;
      while (turno_jugador >= 0 && fallo == 0)
	{
	  clear_SELMODE ();
	  clearPulsador ();
	  boton_pulsado = getPulsador ();
	  while (!boton_pulsado)
	    {
	      boton_pulsado = getPulsador ();
	      retardo (15000);
	    }
	  set_SELMODE (10);
	  set_SECUENCIA (1);
	  led_color_ON (colores[turno_jugador]);
	  while (get_SECUENCIA ())
	    {
	    }
	  retardo (500000);

	  if ((boton_pulsado - 1) == colores[turno_jugador]) turno_jugador--;
	  else fallo = 1;

	}
      turno++;

      if (turno % 4 == 0)
	{
	  cambiar_Tiempo (turno);
	}

    } // while
  if (fallo == 0) *exito = 1;
  else *exito = 0;
}

/*
 * MODO 3: Modo Aleatorio de Simon. Se debera repetir la secuencia hasta un numero determinado
 * 	   de colores. En cada turno se determina de forma aleatoria si el jugado debe introducir
 * 	   la secuencia de manera clásico o inversa. Se indicara mediante el LCD.
 * 	   A su vez, se debe utilziar un Stick para activar la secuencia de colores en vez de los
 * 	   clásicos botonos.
 * 	   Numero de turnos/colores en secuencia:
 *				- Nivel 1: 4 turnos
 *       		 	- Nivel 2: 8 turnos
 *				- Nivel 3: 12 turnos
 *				- Nivel 4: 16 turnos
 *	Cada 4 turnos, disminuye el tiempo que se muestran los colores.
 *	La funcion devuelve si el jugador ha tenido exito o no en *exito.
 *
 *
 */

void
modo_3 (int nivel, int* exito)
{
  int i, turno, fallo, turno_jugador, boton_pulsado, sentido, numero_turnos,
      colores[MAX_TURNOS]; // Declaramos el vector de colores a acertar

  boton_pulsado = 0;
  turno = 1;
  fallo = 0;
  srand (time (NULL));   		// Inicializamos semilla

  numero_turnos = nivel * 4;

  // Bucle que hace el juego hasta que se gana o se pierde
  while (turno <= numero_turnos && fallo == 0)
    {
      boton_pulsado = 0;
      NVIC_DisableIRQ (EINT0_IRQn);
      NVIC_DisableIRQ (EINT1_IRQn);
      NVIC_DisableIRQ (EINT2_IRQn);
      NVIC_DisableIRQ (EINT3_IRQn);

      sprintf (buffer, "%d", turno);
      Lcd_Show ("Turno:", 1, 1, 1, 0);
      Lcd_Show (buffer, 0, 0, 0, 0);
      Lcd_Show ("-", 0, 0, 0, 0);
      sprintf (buffer, "%d", numero_turnos);
      set_SELMODE (10);
      colores[turno - 1] = rand () % 4; // Nuevo color
      Lcd_Show (buffer, 0, 0, 0, 2000000);

      for (i = 0; i < turno; i++)	// Imprimimos la secuencia de colores
	{
	  set_SECUENCIA (1);
	  led_color_ON (colores[i]);
	  while (get_SECUENCIA ())
	    {
	    }
	  retardo (500000);
	}

      sentido = rand () % 2;

      if (sentido == 0)
	{ 	// Sentido directo
	  printf("Modo directo \n");
      //Lcd_Show ("DIRECTO", 2, 7, 0, 0);
	  turno_jugador = 1;
	  while (turno_jugador <= turno && fallo == 0)
	    {
	      boton_pulsado = 0;
	      while (!boton_pulsado)
		{
		  boton_pulsado = movimiento_Stick ();
		  retardo (750000);
		}
	      set_SELMODE (10);
	      set_SECUENCIA (1);
	      led_color_ON (colores[turno_jugador - 1]);
	      while (get_SECUENCIA ())
		{
		}
	      retardo (500000);

	      if ((boton_pulsado - 1) == colores[turno_jugador - 1]) turno_jugador++;
	      else fallo = 1;

	    }
	}
      else
	{		// Sentido inverso
	  //Lcd_Show ("INVERSO", 2, 7, 0, 0);
    	  printf("Modo inverso \n");
	  turno_jugador = turno - 1;
	  while (turno_jugador >= 0 && fallo == 0)
	    {
	      boton_pulsado = 0;
	      while (!boton_pulsado)
		{
		  boton_pulsado = movimiento_Stick ();
		  retardo (750000);
		}
	      set_SELMODE (10);
	      set_SECUENCIA (1);
	      led_color_ON (colores[turno_jugador]);
	      while (get_SECUENCIA ())
		{
		}
	      retardo (500000);

	      if ((boton_pulsado - 1) == colores[turno_jugador]) turno_jugador--;
	      else fallo = 1;

	    }
	}
      turno++;

      if (turno % 4 == 0)
	{
	  cambiar_Tiempo (turno);
	}

    } // while
  if (fallo == 0) *exito = 1;
  else *exito = 0;

  NVIC_ClearPendingIRQ (EINT0_IRQn);
  NVIC_ClearPendingIRQ (EINT1_IRQn);
  NVIC_ClearPendingIRQ (EINT2_IRQn);
  NVIC_ClearPendingIRQ (EINT3_IRQn);
  retardo (15000);
  NVIC_EnableIRQ (EINT0_IRQn);
  NVIC_EnableIRQ (EINT1_IRQn);
  NVIC_EnableIRQ (EINT2_IRQn);
  NVIC_EnableIRQ (EINT3_IRQn);
}

/*
 * MODO 4: Modo Multijugador de Simon. El jugador que inicia la partida selecciona
 * 	   el color de inicio. El segundo jugador debe replicar la secuencia y
 * 	   si es correcta agregara otro color a la secuencia existente.
 * 	   Cada jugador debe replicar la secuancia existente y luego agregar
 * 	   otro color.
 * 	   El primer jugador que se equivoca en la secuencia, pierde.
 * 	   Se dispone de un tiempo de 10 segundos para establecer la conexion.
 * 	   La funcion devuelve si el jugador ha tenido exito o no en *exito.
 *
 */

void
modo_4 (int nivel, int* exito)
{

	 int  num_t, tono_a_replicar, boton_pulsado,
	 colores[MAX_TURNOS*4]; // Declaramos el vector de colores a acertar

	 int j = 0; //Para almacenamiento de tonos

	 /*Arranque y cfg periferico UART (solo en modo 4 es necesario)*/
	 uartConfig ();
	 //LPC_UART0->FCR |= ((1 << 1) | (1 << 2));
	 NVIC_EnableIRQ (UART0_IRQn);


	 //printf("%d \n", get_ENABLE());
	 while (get_ENABLE()==0){
	 	printf("Esperando a iniciar juego.. \n");
	 }



	 printf("Conexion existosa, inicio SIMON MULTIJUGADOR.. \n");

	 /*Inicializamos arreglo*/
	 for (int i = 0; i < MAX_TURNOS*4; i++) {
	       colores[i] = 9; //9 ->significa libre
	  }
	 num_t = 0; //Numero de turnos que transcurrieron




	 set_SELMODE (10);

	 printf("GAME ON %d", get_GAME_ON());


	 while(get_GAME_ON()==1){

		 tono_a_replicar = 1;

		 //printf("J ES %d \n", j);



		 printf("\n Esperando tono player 1.. \n");
		 while(get_user_tono()==6){

		 }

		 if ((int) get_user_tono() ==255){
			 printf("Usted gano, el player 1 ingreso mal \n");
			 *exito=1;
			 return;

		 }

		 colores[j] = (int) get_user_tono();

		 set_read_byte(6);

		//		 printf("Colores:");
		//		 for (int m = 0; m<MAX_TURNOS*4; m++){
		//
		//			  printf("%d ", colores[m]);
		//		 }
		//		 printf("\n");

		 num_t = 0;
		 for(int w = 0; w < MAX_TURNOS * 4; w ++)
			{
			  if(colores[w] != 9)
				{
				  num_t++;
				}
			}


		 //printf("Num t es: %d", num_t);

		 // Bucle que hace el juego hasta que se gana o se pierde


		 for (int pled = 0; pled < num_t; pled++)// Imprimimos la secuencia de colores
		 {

			 set_SELMODE(10);
			 set_SECUENCIA(1);
			  led_color_ON (colores[pled]);
			  while (get_SECUENCIA())
				{
				}


		 }



		 while (tono_a_replicar <= num_t){

			 //printf("\n Entro while num_t a replicar \n");
			 //printf("tono a replicar es: %d ", tono_a_replicar);

			//			 printf("Colores:");
			//			 for (int m = 0; m<MAX_TURNOS*4; m++){
			//
			//				  printf("%d ", colores[m]);
			//			 }


			  clear_SELMODE ();
			  clearPulsador ();
			  boton_pulsado = getPulsador ();
			  while (!boton_pulsado)
				{
				  boton_pulsado = getPulsador ();
				  retardo (1500);
				}
			  set_SELMODE (10);
			  set_SECUENCIA (1);
			  led_color_ON (boton_pulsado - 1);
			  while (get_SECUENCIA ())
				{
				}


			  //printf("Boton pulsado es: %d \n", boton_pulsado);

			  if ((boton_pulsado - 1) == colores[tono_a_replicar - 1]) {
				  printf("\n Metiste bien eltono capo \n");

				  tono_a_replicar++;
			  } else{


				  /*Perdi, envio 5 */
				  char error = 0xFF;
				  while( ( (LPC_UART0->LSR >> 5) & 1 ) ==0 );
				  LPC_UART0 -> THR = error;
				  retardo(500000);
				  set_GAME_ON(0);
				  set_START_COM(0);
				  /*Deshabilito interrupciones*/
				  NVIC_DisableIRQ (UART0_IRQn);
				  /*Chau modo 4*/
				  *exito = 0;
				  return;
			  }


		 }


		 /*Indicamos al usuario que va todo OK*/
		 //simon_MultiSUCCESS();
		 //retardo (1000);

		/*Replicamos todos los tonos bien, agregamos el que querramos mediante
		 * el pulsador correspondiente*/

		 simon_MultiSUCCESS();

		 clear_SELMODE ();
	     clearPulsador ();
	     boton_pulsado = getPulsador ();
		 while (!boton_pulsado){

			 boton_pulsado = getPulsador ();
		 	 retardo (1500);
		 }



		  /*Enciendo led que pulse, indicativamente*/
		  set_SELMODE (10);
		  set_SECUENCIA (1);
	 	  led_color_ON (boton_pulsado-1);

		//	 	 printf("\n Colores:");
		//		 for (int m = 0; m<MAX_TURNOS*4; m++){
		//
		//			  printf("%d ", colores[m]);
		//		 }
		//
		//		 printf("\n Agrego boton AHORA \n");

	 	  /*Agrego tono que pulse, a la secuencia*/
	 	  j++;
	 	  colores[j] = boton_pulsado-1;



	 	  /*Envio tono al otro jugador*/
	 	 char tono_a_enviar = (char) boton_pulsado-1;

	 	 printf("\nTe mande : %d ", tono_a_enviar);
		 while( ( (LPC_UART0->LSR >> 5) & 1 ) ==0 );
		 LPC_UART0 -> THR = tono_a_enviar;

		 /*Vuelvo a aumentar j*/
	 	 j++;


	 }

	 printf("\n Chau modo4 \n");
	 NVIC_DisableIRQ (UART0_IRQn);



 }





void
retardo (uint32_t tiempo)
{
  for (int i = 0; i < tiempo; i++)
    ;
}

void
cambiar_Tiempo (uint8_t turno)
{

  if (turno > 12)
    {
      setTiempo (0, tiempos[3]);
    }
  else if (turno > 8)
    {
      setTiempo (0, tiempos[2]);
    }
  else if (turno > 4)
    {
      setTiempo (0, tiempos[1]);
    }
  else setTiempo (0, tiempos[0]);

}
