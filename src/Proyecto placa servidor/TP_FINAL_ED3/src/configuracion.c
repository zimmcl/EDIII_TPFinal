/*
 * configuracion.c
 *
 *  Created on: 3 nov. 2018
 *      Author: Ezequiel
 */
#include "configuracion.h"

uint32_t volatile * const STCTRL = (uint32_t *) AddrSTCTRL;
uint32_t volatile * const STRELOAD = (uint32_t *) AddrSTRELOAD;

uint32_t RELOAD[4] =
  { 103370, 167404, 254545, 356759 };	//{Amarillo, Verde, Rojo, Azul}

/**
 * @brief  Configuracion de los pines a ser utilizados
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
  LPC_GPIO2->FIODIR |= (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);//Pines 2.0/2.1/2.2/2.3 como salida
  LPC_GPIO0->FIOMASK = 0;
  LPC_GPIO0->FIOCLR |= (1 << 9);		//Bajamos salida del tono
}

/**
 * @brief   Configura los TIMERs
 */
void
timerConfig (void)
{
  //--------------TIMER0---------------
  LPC_TIM0->MR0 = 6250000;		//Match para interrupcion cada 625ms
  LPC_TIM0->MCR |= 0b11;		//Match0 interrupcion y reinicio del TC
  LPC_TIM0->TCR &= ~1;				//Deshabilitamos el contador
  LPC_TIM0->TCR |= (1 << 1);			//Reseteamos el TC y PC
  LPC_TIM0->TCR &= ~(1 << 1);			//Salimos del estado Reset
  LPC_TIM0->IR |= ~0;		//Bajamos banderas de interrupcion pendientes
  //--------------TIMER1---------------
  LPC_TIM1->MR0 = 10000000;		//Match para interrupcion cada 1s
  LPC_TIM1->MCR |= 0b11;		//Match0 interrupcion y reinicio del TC
  LPC_TIM1->TCR &= ~1;				//Deshabilitamos el contador
  LPC_TIM1->TCR |= (1 << 1);			//Reseteamos el TC y PC
  LPC_TIM1->TCR &= ~(1 << 1);			//Salimos del estado Reset
  LPC_TIM1->IR |= ~0;		//Bajamos banderas de interrupcion pendientes
  //--------------TIMER2---------------
  //--------------TIMER3---------------
}

/**
 * @brief   Configura las interrupciones Externas
 */
void
eintConfig (void)
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

/**
 * @brief   Configura la comunicacion Serie UART
 */

void
uartConfig (void)
{
  LPC_UART0->LCR |= (1 << 7);
  LPC_UART0->DLL = 130;			//PCLK 20Mhz -> 9600BR
  LPC_UART0->DLM = 0;
  LPC_UART0->LCR &= ~(1 << 7);
  LPC_UART0->FCR |= 1;
  LPC_UART0->LCR |= (0b11);		//8bits
  LPC_UART0->IER |= 1;			//Interrupcion por recepcion de datos
}

/**
 * @brief   Configura el ADC para el uso del STICK
 */
void
adcConfig (void)
{
  LPC_ADC->ADCR |= 0b11;		//Canales a muestrear AD0.0, AD0.1
  //LPC_ADC->ADCR |= (0b0 << 15);	//Divisor para el clock de periferico
  LPC_ADC->ADCR &= ~(1 << 16);		//OFF Burst
  LPC_ADC->ADCR |= (1 << 21);		//ADC operacional
  LPC_ADC->ADINTEN &= ~(1 << 8);	//Deshabilitamos interrupcion por ADC
}

/*
 void
 pwmConfig (void)
 {
 LPC_PWM1->TCR |= (1 << 0);			//Habilitamos cuenta del PWM
 LPC_PWM1->TCR |= (1 << 1);			//Modo Reset
 LPC_PWM1->TCR |= (1 << 3);			//PWM Enable

 LPC_PWM1->PR = 0; 				//Sin prescaler
 LPC_PWM1->MCR = (1 << 1); 			//Reset TC cuando machea con MR0

 LPC_PWM1->MR0 = 100; 		// set PWM cycle(Ton+Toff)=100)
 LPC_PWM1->MR1 = 50; 		// Set 50% Duty Cycle for all four channels
 //LPC_PWM1->MR2 = 50;
 //LPC_PWM1->MR3 = 50;
 //LPC_PWM1->MR4 = 50;
 // Trigger the latch Enable Bits to load the new Match Values
 LPC_PWM1->LER = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);

 // Enable the PWM output pins for PWM_1-PWM_4(P2_0 - P2_3)
 LPC_PWM1->PCR |= (1 << 9);
 //LPC_PWM1->PCR |= (1 << 10);
 //LPC_PWM1->PCR |= (1 << 11);
 //LPC_PWM1->PCR |= (1 << 12);
 }
 */

/**
 * @brief    Realiza la Habilitacion de los perifericos, la configuracion de su
 * 	     frecuencia y luego habilita las interrupciones de los perifericos
 */
void
nvic_sysConfig (uint8_t i)
{
  if (i == 0)
    {
      *STCTRL &= ~(1 | (1 << 1)); //Deshabilitos cuenta e interrupcion de SysTick
      LPC_SC->PCONP |= (1 << 12);		//Activamos periférico ADC
      //LPC_SC->PCONP |= (1 << 1);		//Activamos TIMER0
      //LPC_SC->PCONP |= (1 << 2);		//Activamos TIMER1
      //LPC_SC->PCONP |= (1 << 6);		//Activamos PWM
      LPC_SC->PCLKSEL0 |= (0b11 << 24);		//ADC clock = 1/8 CPU clock
      LPC_SC->PCLKSEL0 |= (0b11 << 2);		//TIMER0 clock = 1/8 CPU clock
      LPC_SC->PCLKSEL0 |= (0b11 << 4);		//TIMER1 clock = 1/8 CPU clock
      LPC_SC->PCLKSEL0 |= (0b11 << 12);		//PWM clock = 1/8 CPU clock
      *STCTRL |= (1 << 2);
      *STRELOAD = RELOAD[0] / 2;
      i = 1;
    }
  else
    {
      LPC_SC->EXTINT |= ~0;		//Bajo banderas de interrupcion EINT
      //Habilitamos las interrupciones
      NVIC_EnableIRQ (EINT0_IRQn);
      NVIC_EnableIRQ (EINT1_IRQn);
      NVIC_EnableIRQ (EINT2_IRQn);
      NVIC_EnableIRQ (EINT3_IRQn);
      NVIC_EnableIRQ (ADC_IRQn);
      NVIC_EnableIRQ (TIMER0_IRQn);
      NVIC_EnableIRQ (TIMER1_IRQn);
      NVIC_EnableIRQ (UART0_IRQn);
      LPC_ADC->ADCR |= (1 << 16);		//Conversion Burst ON
      //*STCTRL |= (1 << 0) | (1 << 1);//Cuenta e interrupcion de SysTick habilitadas
      i = 0;
    }
}

/*
 * Ajusta la carga del SysTick para generar un tono en particular
 * y habilita su cuenta e interrupcion.
 */

void
setTono (uint32_t freq)
{
  *STRELOAD = freq;
  *STCTRL |= (1 << 0) | (1 << 1);
}

/*
 * Deshabilita la cuenta e interrupcion del SysTick.
 */

void
sysTick_OFF (void)
{
  *STCTRL &= ~(1 | (1 << 1));
}

/*
 * Inicializa el bloque de perifericos
 */

/*
 *
 */
void
setTiempo (uint8_t timer, uint32_t tiempo)
{
  switch (timer)
    {
      case (0):
      LPC_TIM0->MR0 = tiempo;
      break;
      case (1):
      LPC_TIM1->MR0 = tiempo;
      break;
    }
}

void
perifericos_Init (void)
{
  nvic_sysConfig (0);
  gpioConfig ();
  adcConfig ();
  timerConfig ();
  //pwmConfig ();
  eintConfig ();
  uartConfig ();
  nvic_sysConfig (1);
}
