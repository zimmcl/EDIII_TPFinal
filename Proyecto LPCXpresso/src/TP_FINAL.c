/*
 ===============================================================================
 Name        : TP_FINAL.c
 Author      : $(Ezequiel J. Zimmel)
 Version     :
 Copyright   : $(copyright)
 Description : SIMON GAME
 ===============================================================================
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

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


#define zero_X 1.42
#define zero_Y 1.42
#define zero_Z 1.42
#define PI 3.1415926535

#define X_MAX 2450.0
#define X_MIN 1821.0
#define Y_MAX 2480.0
#define Y_MIN 1700.0
/*
 #define X_MAX 2164
 #define X_MIN 1821
 #define Y_MAX 2200
 #define Y_MIN 1700
 #define Z_MAX 2315
 #define Z_MIN 2000
 #define X_MAX_f 2164.0
 #define X_MIN_f 1821.0
 #define Y_MAX_f 2200.0
 #define Y_MIN_f 1700.0
 #define Z_MAX_f 2315.0
 #define Z_MIN_f 2000.0
 */

#define AddrSTCTRL	0xE000E010
#define AddrSTRELOAD	0xE000E014

#define VREF	3.3 	//Tensión de referencia ADC
#define VS	3.3 	//Voltaje aplicado a ADXL335
#define ZERO_G (VS/2) 	//Zero g bias
#define NUM_SAMPLES 50

uint32_t volatile * const STCTRL = (uint32_t *) AddrSTCTRL;
uint32_t volatile * const STRELOAD = (uint32_t *) AddrSTRELOAD;

void
gpioConfig (void);
void
TIMERxConfig (void);
void
EINTxConfig (void);
void
UARTxConfig (void);
void
adcConfig (void);
void
SysTickConfig (void);
void
nvic_sysConfig (void);
//int
//debounce (unsigned int);
void
retardo (int);
void
initGame (uint8_t);
void
user_game (void);
void
check_sec (void);
void
lcd_Init (void);
void
Lcd_CmdWrite (char);
void
Lcd_DataWrite (char);
void
enviarNibble (char);
void
enviar(uint8_t);

void
SysTick_Handler (void);
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
check_ang(uint8_t, uint8_t);
uint16_t const
constrain (uint16_t const, uint16_t const, uint16_t const);
float
mapf (float x_, float, float, float, float);
//303370
//367404
//454545
//606759
//Variables globales
uint32_t RELOAD[4] =
  { 103370, 167404, 254545, 356759 };	//{Amarillo, Verde, Rojo, Azul}
uint8_t static GAME_MODE;
uint8_t static EJEC_SECUENCIA;
uint32_t const tiempo = 1500000;	//Para CPU clock 100Mhz -> 15ms retardo

/**
 * @brief      { function_description }
 *
 * @return     { description_of_the_return_value }
 */
int
main (void)
{
  nvic_sysConfig ();
  gpioConfig ();
  EINTxConfig ();
  TIMERxConfig ();
  UARTxConfig ();
  adcConfig ();
  lcd_Init ();
  SysTickConfig ();
  nvic_sysConfig ();
  enviar(4);
  int static X, Y;
  float X_vol, Y_vol;
  while (1)
    {
      X = (LPC_ADC->ADDR0 >> 4 & 0xFFF);
      Y = (LPC_ADC->ADDR1 >> 4 & 0xFFF);
      X_vol = ((X*3.3/4095.0)*1000);
      Y_vol = ((Y*3.3/4095.0)*1000);
      printf ("X: %d\n", X);
      printf ("Y: %d\n", Y);
      printf ("X_vol: %f\n", X_vol);
      printf ("Y_vol: %f\n", Y_vol);
      //Z = (LPC_ADC->ADDR2 >> 4 & 0xFFF);
      //printf ("X: %d\n", constrain (mapf (X, X_MIN, X_MAX, 0.0, 90.0), 0, 90));
      //printf ("Y: %d\n", constrain (mapf (Y, Y_MIN, Y_MAX, 0.0, 90.0), 0, 90));
      //check_ang (X, Y);
      retardo (10000000);
      //LPC_GPIO2->FIOCLR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    }
  while (1)
    {
      if (GAME_MODE != 1)
	{
	  initGame (8);
	  //while (estado)
	    {
	      user_game ();
	      check_sec ();
	    }
	}
    }
  return 0;
}

/**
 * @brief      { function_description }
 */
void
gpioConfig (void)
{
  LPC_PINCON->PINSEL0 |= (1 << 4);		//Funcion del Pin0.2	TX
  LPC_PINCON->PINSEL0 |= (1 << 6);		//FUncion del Pin0.3	RX
  LPC_PINCON->PINSEL1 |= (1 << 14);		//Función del Pin0.23  AD0.0
  LPC_PINCON->PINSEL1 |= (1 << 16);		//Función del Pin0.24  AD0.1
  LPC_PINCON->PINSEL1 |= (1 << 18);		//Función del Pin0.25  AD0.2
  LPC_PINCON->PINSEL4 |= (0b00 << 0);//Funcion del Pin2.0 ESTA COMO GPIO, para PWM1.1 poner 0b10
  LPC_PINCON->PINSEL4 |= (0b00 << 2);//Funcion del Pin2.1 ESTA COMO GPIO, para PWM1.2 poner 0b10
  LPC_PINCON->PINSEL4 |= (0b00 << 4);//Funcion del Pin2.2 ESTA COMO GPIO, para PWM1.3 poner 0b10
  LPC_PINCON->PINSEL4 |= (0b00 << 6);//Funcion del Pin2.3 ESTA COMO GPIO, para PWM1.4 poner 0b10
  LPC_PINCON->PINSEL4 |= (1 << 20);		//Función del Pin2.10: EINT0
  LPC_PINCON->PINSEL4 |= (1 << 22);		//Función del Pin2.11: EINT1
  LPC_PINCON->PINSEL4 |= (1 << 24);		//Función del Pin2.12: EINT2
  LPC_PINCON->PINSEL4 |= (1 << 26);		//Función del Pin2.12: EINT3
  LPC_PINCON->PINMODE1 |= (1 << 15);		//Pin0.23 entrada analogica
  LPC_PINCON->PINMODE1 |= (1 << 17);		//Pin0.24 entrada analogica
  LPC_PINCON->PINMODE1 |= (1 << 19);		//Pin0.25 entrada analogica
  LPC_PINCON->PINMODE4 |= (0b11 << 20);		//Pin2.10 Pull-down
  LPC_PINCON->PINMODE4 |= (0b11 << 22);		//Pin2.11 Pull-down
  LPC_PINCON->PINMODE4 |= (0b11 << 24);		//Pin2.12 Pull-down
  LPC_PINCON->PINMODE4 |= (0b11 << 26);		//Pin2.12 Pull-down
  //LPC_GPIO2->FIOMASK2 |= ();
  LPC_GPIO0->FIODIR |= (1 << 9);			//Pin0.9 como salida
  LPC_GPIO2->FIODIR |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);	//Pines 2.0/2.1/2.2/2.3 como salida
  LPC_GPIO0->FIOMASK = 0;
  LPC_GPIO0->FIOCLR |= (1 << 9);			//Bajamos salida del tono
}

/**
 * @brief      { function_description }
 */
void
TIMERxConfig (void)
{
  //--------------TIMER0---------------
  LPC_TIM0->MR0 = 6250000;		//Match para interrupcion cada 500ms
  LPC_TIM0->MCR |= 0b11;		//Match0 interrupcion y reinicio del TC
  LPC_TIM0->TCR &= ~1;				//Deshabilitamos el contador
  LPC_TIM0->TCR |= (1 << 1);			//Reseteamos el TC y PC
  LPC_TIM0->TCR &= ~(1 << 1);			//Salimos del estado Reset
  LPC_TIM0->IR |= ~0;		//Bajamos banderas de interrupcion pendientes
  //--------------TIMER1---------------
  //--------------TIMER2---------------
  //--------------TIMER3---------------
}

/**
 * @brief      { function_description }
 */
void
EINTxConfig (void)
{
  //--------------EINT0---------------
  LPC_SC->EXTMODE |= 1;				//EINT0 sensible por flanco
  LPC_SC->EXTPOLAR |= 1;			//Flanco ascendente
  //--------------EINT1---------------
  LPC_SC->EXTMODE |= (1 << 1);			//EINT1 sensible por flanco
  LPC_SC->EXTPOLAR |= (1 << 1);			//Flanco ascendente
  //--------------EINT2---------------
  LPC_SC->EXTMODE |= (1 << 2);			//EINT2 sensible por flanco
  LPC_SC->EXTPOLAR |= (1 << 2);			//Flanco ascendente
  //--------------EINT3---------------
  LPC_SC->EXTMODE |= (1 << 3);			//EINT3 sensible por flanco
  LPC_SC->EXTPOLAR |= (1 << 3);			//Flanco ascendente
}

void
UARTxConfig (void)
{
  LPC_UART0->LCR |= (1 << 7);
  LPC_UART0->DLL = 130;
  LPC_UART0->DLM = 0;
  LPC_UART0->LCR &= ~(1 << 7);
  LPC_UART0->FCR |= 1;
  LPC_UART0->LCR |= (0b11);		//8bits
  LPC_UART0->IER |= 1;			//Interrupcion por recepcion de datos
}

/**
 * @brief      { function_description }
 */
void
SysTickConfig (void)
{
  *STRELOAD = RELOAD[0] / 2;
}

/**
 * @brief      { function_description }
 */
void
adcConfig (void)
{
  LPC_ADC->ADCR |= 0b111;	//Canales a muestrear AD0.0, AD0.1, AD0.2
  //LPC_ADC->ADCR |= (0b0 << 15);		//Divisor para el clock de periferico
  LPC_ADC->ADCR &= ~(1 << 16);			//OFF Burst
  LPC_ADC->ADCR |= (1 << 21);			//ADC operacional
  LPC_ADC->ADINTEN &= ~(1 << 8);	//Deshabilitamos interrupcion por ADC
}

/**
 * @brief      { function_description }
 */
void
nvic_sysConfig (void)
{
  uint8_t static i;
  if (i == 0)
    {
      LPC_SC->PCONP |= (1 << 12);		//Activamos periférico ADC
      //LPC_SC->PCONP |= (1 << 1);		//Activamos TIMER0
      LPC_SC->PCLKSEL0 |= (0b11 << 24);		//ADC clock = 1/8 CPU clock
      LPC_SC->PCLKSEL0 |= (0b11 << 2);		//TIMER clock = 1/8 CPU clock
      *STCTRL |= (1 << 2);
      i = 1;
    }
  else
    {
      LPC_SC->EXTINT |= ~0;		//Bajo banderas de interrupcion EINT
      NVIC_EnableIRQ (EINT0_IRQn);
      NVIC_EnableIRQ (EINT1_IRQn);
      NVIC_EnableIRQ (EINT2_IRQn);
      NVIC_EnableIRQ (EINT3_IRQn);
      NVIC_EnableIRQ (ADC_IRQn);
      NVIC_EnableIRQ (TIMER0_IRQn);
      NVIC_EnableIRQ (UART0_IRQn);
      LPC_ADC->ADCR |= (1 << 16);		//Conversion Burst ON
      //NVIC_EnableIRQ (SysTick_IRQn);
      //*STCTRL |= (1 << 0) | (1 << 1);//Cuenta e interrupcion de SysTick habilitadas
      i = 0;
    }
}

/**
 * @brief      { function_description }
 */
void
SysTick_Handler (void)
{
  LPC_GPIO0->FIOPIN = ~(LPC_GPIO0->FIOPIN>>9 & 1)<<9;
}

//Boton derecho
void
EINT0_IRQHandler (void)
{
  NVIC_DisableIRQ (EINT1_IRQn);
  NVIC_DisableIRQ (EINT2_IRQn);
  NVIC_DisableIRQ (EINT3_IRQn);
  *STRELOAD = RELOAD[1] / 2;
  *STCTRL |= (1 << 0) | (1 << 1); //Habilitos cuenta e interrupcion de SysTick
  LPC_TIM0->TCR |= 1;				//Habilito cuenta del TIMER0
  /* while (debounce (LPC_GPIO2->FIOPIN >> 10 & 1))
   {
   }*/
  LPC_SC->EXTINT |= 1;				//Bajo banderas de EINT0
  LPC_GPIO2->FIOSET |= (1 << 0);
}

//Boton izquierda
void
EINT1_IRQHandler (void)
{
  NVIC_DisableIRQ (EINT0_IRQn);
  NVIC_DisableIRQ (EINT2_IRQn);
  NVIC_DisableIRQ (EINT3_IRQn);
  *STRELOAD = RELOAD[2] / 2;
  *STCTRL |= (1 << 0) | (1 << 1);//Habilito cuenta e interrupcion de SysTick
  LPC_TIM0->TCR |= 1;				//Habilito cuenta del TIMER0
  /*while (debounce (LPC_GPIO2->FIOPIN >> 11 & 1))
   {
   }*/
  LPC_SC->EXTINT |= (1 << 1);			//Bajo banderas de EINT1
  LPC_GPIO2->FIOSET |= (1 << 1);
}

//Boton inferior
void
EINT2_IRQHandler (void)
{
  NVIC_DisableIRQ (EINT0_IRQn);
  NVIC_DisableIRQ (EINT1_IRQn);
  NVIC_DisableIRQ (EINT3_IRQn);
  *STRELOAD = RELOAD[3] / 2;
  *STCTRL |= (1 << 0) | (1 << 1);//Habilitos cuenta e interrupcion de SysTick
  LPC_TIM0->TCR |= 1;				//Habilito cuenta del TIMER0
  /*while (debounce (LPC_GPIO2->FIOPIN >> 12 & 1))
   {
   }*/
  LPC_SC->EXTINT |= (1 << 2);			//Bajo banderas de EINT2
  LPC_GPIO2->FIOSET |= (1 << 2);
}

//Boton superior
void
EINT3_IRQHandler (void)
{
  NVIC_DisableIRQ (EINT0_IRQn);
  NVIC_DisableIRQ (EINT1_IRQn);
  NVIC_DisableIRQ (EINT2_IRQn);
  *STRELOAD = RELOAD[0] / 2;
  *STCTRL |= (1 << 0) | (1 << 1);//Habilitos cuenta e interrupcion de SysTick
  LPC_TIM0->TCR |= 1;				//Habilito cuenta del TIMER0
  /*while (debounce (LPC_GPIO2->FIOPIN >> 13 & 1))
   {
   }*/
  LPC_SC->EXTINT |= (1 << 3);			//Bajo banderas de EINT3
  LPC_GPIO2->FIOSET |= (1 << 3);
}

/**
 * @brief      { function_description }
 */
void
TIMER0_IRQHandler (void)
{
  *STCTRL &= ~(1 | (1 << 1));	//Deshabilitos cuenta e interrupcion de SysTick
  LPC_TIM0->IR |= 1;		//Bajo bandera de interrupcion por match0
  LPC_GPIO0->FIOSET |= 1;		//Bajamos salida del tono
  LPC_TIM0->TCR &= ~1;			//Deshabilitamos el contador
  LPC_TIM0->TCR |= (1 << 1);		//Reseteamos el TC y PC
  LPC_TIM0->TCR &= ~(1 << 1);		//Salimos del estado Reset
  LPC_TIM0->IR |= ~0;
  LPC_GPIO2->FIOCLR |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
  NVIC_EnableIRQ (EINT0_IRQn);
  NVIC_EnableIRQ (EINT1_IRQn);
  NVIC_EnableIRQ (EINT2_IRQn);
  NVIC_EnableIRQ (EINT3_IRQn);
  EJEC_SECUENCIA = 0;
}

void
UART0_IRQHandler(void)
{

}

/**
 * @brief      { function_description }
 *
 * @param[in]  tonos  The tonos
 */
void
initGame (uint8_t tonos)
{
  uint8_t secuencia[tonos + 1], i;
  uint8_t static ingreso[4];
  for (uint8_t j = 0; j < tonos; j++)
    {
      secuencia[j] = 4;
    }
  srand (time (NULL));   		// Inicializamos semilla

  for (i = 0; i < 4; i++)
    {
      secuencia[i] = rand () % 4; // Asignamos un pseudo numero aleatorio entre 0 and 3.
    }
  for (uint8_t j = 0; j < i; j++)
    {
      switch (secuencia[j])
	{
	case 0:
	  if (ingreso[0]) retardo (tiempo);
	  EJEC_SECUENCIA = 1;
	  EINT3_IRQHandler ();
	  while (EJEC_SECUENCIA)
	    {
	    }
	  ingreso[0]++;
	  break;
	case 1:
	  if (ingreso[1]) retardo (tiempo);
	  EJEC_SECUENCIA = 1;
	  EINT0_IRQHandler ();
	  while (EJEC_SECUENCIA)
	    {
	    }
	  ingreso[1]++;
	  break;
	case 2:
	  if (ingreso[2]) retardo (tiempo);
	  EJEC_SECUENCIA = 1;
	  EINT1_IRQHandler ();
	  while (EJEC_SECUENCIA)
	    {
	    }
	  ingreso[2]++;
	  break;
	case 3:
	  if (ingreso[3]) retardo (tiempo);
	  EJEC_SECUENCIA = 1;
	  EINT2_IRQHandler ();
	  while (EJEC_SECUENCIA)
	    {
	    }
	  ingreso[3]++;
	  break;
	default:
	  LPC_GPIO0->FIOSET0 |= 1;		//Bajamos salida del tono
	  for (uint8_t i = 0; i < 4; i++)
	    {
	      ingreso[i] = 0;
	    }
	  break;
	}
    }
  i = 0;
  GAME_MODE = 1;
}

/**
 * @brief      { function_description }
 */
void
user_game (void)
{

}

void
check_sec (void)
{

}

void
retardo (int tiempo)
{
  for (uint32_t i = 0; i < tiempo; i++)
    {
    }
}

void
lcd_Init (void)
{
  char i, a[] =
    { "TEST LCD!" };

  LPC_GPIO2->FIODIR |= ((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6)
      | (1 << LCD_D7));  // Configure all the LCD pins as output
  LPC_GPIO0->FIODIR |= ((1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN));

  Lcd_CmdWrite (0x02);                   // Initialize Lcd in 4-bit mode
  Lcd_CmdWrite (0x28);                   // enable 5x7 mode for chars
  Lcd_CmdWrite (0x0E);                   // Display OFF, Cursor ON
  Lcd_CmdWrite (0x01);                   // Clear Display
  Lcd_CmdWrite (0x80);             // Move the cursor to beginning of first line

  for (i = 0; a[i] != 0; i++)
    {
      Lcd_DataWrite (a[i]);
    }
}

void
Lcd_CmdWrite (char cmd)
{
  enviarNibble ((cmd >> 0x04) & 0x0F);  //Send higher nibble
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RS); // Send LOW pulse on RS pin for selecting Command register
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RW); // Send LOW pulse on RW pin for Write operation
  LPC_GPIO0->FIOPIN |= (1 << LCD_EN);  // Generate a High-to-low pulse on EN pin
  retardo (10000);
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_EN);

  retardo (100000);

  enviarNibble (cmd & 0x0F);            //Send Lower nibble
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RS); // Send LOW pulse on RS pin for selecting Command register
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RW); // Send LOW pulse on RW pin for Write operation
  LPC_GPIO0->FIOPIN |= (1 << LCD_EN);  // Generate a High-to-low pulse on EN pin
  retardo (10000);
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_EN);

  retardo (100000);
}

void
Lcd_DataWrite (char dato)
{
  enviarNibble ((dato >> 0x04) & 0x0F);  //Send higher nibble
  LPC_GPIO0->FIOPIN |= (1 << LCD_RS); // Send HIGH pulse on RS pin for selecting data register
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RW); // Send LOW pulse on RW pin for Write operation
  LPC_GPIO0->FIOPIN |= (1 << LCD_EN);  // Generate a High-to-low pulse on EN pin
  retardo (10000);
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_EN);

  retardo (100000);

  enviarNibble (dato & 0x0F);            //Send Lower nibble
  LPC_GPIO0->FIOPIN |= (1 << LCD_RS); // Send HIGH pulse on RS pin for selecting data register
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_RW); // Send LOW pulse on RW pin for Write operation
  LPC_GPIO0->FIOPIN |= (1 << LCD_EN);  // Generate a High-to-low pulse on EN pin
  retardo (10000);
  LPC_GPIO0->FIOPIN &= ~(1 << LCD_EN);

  retardo (100000);
}

void
enviarNibble (char nibble)
{
  LPC_GPIO2->FIOPIN &= ~(((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6)
      | (1 << LCD_D7)));                   // Clear previous data
  LPC_GPIO2->FIOPIN |= (((nibble >> 0x00) & 0x01) << LCD_D4);
  LPC_GPIO2->FIOPIN |= (((nibble >> 0x01) & 0x01) << LCD_D5);
  LPC_GPIO2->FIOPIN |= (((nibble >> 0x02) & 0x01) << LCD_D6);
  LPC_GPIO2->FIOPIN |= (((nibble >> 0x03) & 0x01) << LCD_D7);
}

/*int
 debounce (unsigned int Sample)
 {
 unsigned static int Samples[NUM_SAMPLES];
 unsigned static int index = 0;
 unsigned static int LastDebounceResult = 0;
 unsigned int andTerm = Sample;
 unsigned int orTerm = Sample;

 Samples[index++] = Sample;
 if (index >= NUM_SAMPLES)
 index = 0;

 for (int j = 0; j < NUM_SAMPLES; j++)
 {
 andTerm &= Samples[j];
 orTerm |= Samples[j];
 }
 LastDebounceResult = ((LastDebounceResult & orTerm) | andTerm);
 return LastDebounceResult;
 }*/

uint16_t const
constrain (uint16_t const x, uint16_t const a, uint16_t const b)
{
  if (x < a)
    {
      return a;
    }
  else if (b < x)
    {
      return b;
    }
  else return x;
}

float
mapf (float x_, float in_min, float in_max, float out_min, float out_max)
{
  return (x_ - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void
enviar(uint8_t dato)
{
  while(!(LPC_UART0->LSR>>5 & 1))
    {

    }
  LPC_UART0->THR = dato;
}

void
check_ang(uint8_t X_ang, uint8_t Y_ang)
{
  if(Y_ang < 30 && X_ang > 65)
    {
      LPC_GPIO2->FIOSET |= (1 << 0);
    }else if(Y_ang < 30 && X_ang < 30)
      {
	LPC_GPIO2->FIOSET |= (1 << 1);
      }else if(Y_ang > 65 && X_ang < 30)
	{
	  LPC_GPIO2->FIOSET |= (1 << 2);
	}else if(Y_ang > 65 && X_ang > 65)
	  {
	    LPC_GPIO2->FIOSET |= (1 << 3);
	  }else LPC_GPIO2->FIOCLR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
}
