/**
  ******************************************************************************
  * @file    Template_2/main.c
  * @author  Nahuel
  * @version V1.0
  * @date    22-August-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Use this template for new projects with stm32f0xx family.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>


//--- My includes ---//
#include "adc.h"
#include "hard.h"
#include "gpio.h"
#include "stm32f0x_tim.h"
#include "dsp.h"
//#include "stm32f0xx_dma.h"
#include "stm32f0xx.h"
#include "core_cm0.h"
#include "flash_program.h"


//--- VARIABLES EXTERNAS ---//


// ------- Externals de los Timers -------
volatile unsigned short wait_ms_var = 0;
volatile unsigned short prog_timer = 0;
volatile unsigned char switches_timer = 0;
volatile unsigned short buzzer_timer = 0;
volatile unsigned short timer_led_error = 0;



// ------- Externals de los modos -------




//--- VARIABLES GLOBALES ---//


// ------- de los timers -------
volatile unsigned short timer_standby;
volatile unsigned char filter_timer;
volatile unsigned char take_sample;
volatile unsigned short timer_led = 0;

//volatile unsigned char door_filter;
//volatile unsigned char take_sample;
//volatile unsigned char move_relay;
volatile unsigned short msecs = 0;
volatile unsigned short minutes = 0;

// ------- del display -------
unsigned char v_opt [10];


// ------- de los filtros DMX -------
#define LARGO_F		32
#define DIVISOR_F	5
unsigned char vd0 [LARGO_F + 1];


//--- FUNCIONES DEL MODULO ---//
void TimingDelay_Decrement(void);

void UpdateSync (void);
void UpdateVpeak (void);
unsigned short CheckVin (void);
unsigned short CheckSync (void);


//--- FILTROS DE SENSORES ---//
#define LARGO_FILTRO 16
#define DIVISOR      4   //2 elevado al divisor = largo filtro
//#define LARGO_FILTRO 32
//#define DIVISOR      5   //2 elevado al divisor = largo filtro
unsigned short vtemp [LARGO_FILTRO + 1];
unsigned short vpote [LARGO_FILTRO + 1];

//--- FIN DEFINICIONES DE FILTRO ---//


//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
	unsigned char i;
	unsigned char mosfet_edge_up = 0;
	unsigned char mosfet_edge_dwn = 0;
	enum var_main_state main_state = MAIN_INIT;
	unsigned char not_beeped = 0;
	unsigned char minutes_paused = 0;
	unsigned short vin_local = 0;
	unsigned short i_local = 0;

	//!< At this stage the microcontroller clock setting is already configured,
    //   this is done through SystemInit() function which is called from startup
    //   file (startup_stm32f0xx.s) before to branch to application main.
    //   To reconfigure the default setting of SystemInit() function, refer to
    //   system_stm32f0xx.c file

	//GPIO Configuration.
	GPIO_Config();


	//ACTIVAR SYSTICK TIMER
	if (SysTick_Config(48000))
	{
		while (1)	/* Capture error */
		{
			if (LED1)
				LED1_OFF;
			else
				LED1_ON;

			for (i = 0; i < 255; i++)
			{
				asm (	"nop \n\t"
						"nop \n\t"
						"nop \n\t" );
			}
		}
	}


	 //PRUEBA LEDS
//	 while (1)
//	 {
//		 if (LED1)
//		 {
//			 LED1_OFF;
//			 LED2_OFF;
//		 }
//		 else
//		 {
//			 LED1_ON;
//			 LED2_ON;
//		 }
//
//		 Wait_ms(150);
//	 }
	 //FIN PRUEBA LEDS

	 //PRUEBA LED, SWITCH y MOSFET
//	 while (1)
//	 {
//		 if (CheckS1() > S_NO)
//		 {
//			 MOSFET_ON;
//			 LED1_ON;
//		 }
//		 else
//		 {
//			 MOSFET_OFF;
//			 LED1_OFF;
//		 }
//
//		 UpdateSwitches();
//	 }
	 //FIN PRUEBA LED, SWITCH y MOSFET

	 //PRUEBA LED, SWITCH y BUZZER
//	 while (1)
//	 {
//		 if (CheckS1() > S_NO)
//		 {
//			 BUZZER_ON;
//			 LED1_ON;
//		 }
//		 else
//		 {
//			 BUZZER_OFF;
//			 LED1_OFF;
//		 }
//
//		 UpdateSwitches();
//	 }
	 //FIN PRUEBA LED, SWITCH y BUZZER

	 //PRUEBA LED Y SYNC
//	 while (1)
//	 {
//		 if (SYNC)
//		 {
//			 SYNC_OFF;
//			 LED1_ON;
//		 }
//		 else
//		 {
//			 SYNC_ON;
//			 LED1_OFF;
//		 }
//
//		 Wait_ms (100);
//	 }
	 //FIN PRUEBA LED Y SYNC

	//TIM Configuration.
	//TIM_1_Init();
	TIM_3_Init();
	TIM_14_Init();

	LED1_ON;
	Wait_ms(1000);
	LED1_OFF;

	//ADC configuration.
	AdcConfig();
	ADC1->CR |= ADC_CR_ADSTART;


	//--- Loop Principal ---//
	LEDR_ON;
	LEDG_ON;
	timer_standby = 2000;

	while(1)
	{
		switch (main_state)
		{
			case MAIN_INIT:
				if (!timer_standby)
				{
					LEDR_OFF;
					not_beeped = 0;
					minutes_paused = 0;
					main_state++;
				}
				break;

			case MAIN_STANDBY:
				if (CheckS1() > S_NO)
				{
					main_state = MAIN_STANDBY_1;
					BuzzerCommands(BUZZER_MULTIPLE_SHORT, 1);
					LEDR_ON;
				}
				break;

			case MAIN_STANDBY_1:
				if (CheckS1() == S_NO)
				{
					//espero edge dwn o timeout
					timer_standby = 3000;
					while ((!mosfet_edge_up) && (timer_standby));
					MOSFET_ON;
					minutes = 45;
					main_state = MAIN_GEN;
					LEDR_OFF;
				}
				break;

			case MAIN_GEN:
				//Generando señal
				if (!timer_led)
				{
					if (LEDG)
						LEDG_OFF;
					else
						LEDG_ON;
					timer_led = 1500;
				}

				if ((minutes < 5) && (not_beeped))
				{
					not_beeped = 1;
					BuzzerCommands(BUZZER_MULTIPLE_SHORT, 1);
				}

				if (!minutes)
				{
					BuzzerCommands(BUZZER_MULTIPLE_SHORT, 3);
					main_state = MAIN_FINISH;
				}

				if (CheckS1() > S_NO)
				{
					//reviso por pausa o por stop
					main_state = MAIN_CHECK_PAUSE_OR_STOP;
					timer_standby = 20;
					LEDG_ON;
					//espero edge dwn o timeout
					while ((!mosfet_edge_dwn) && (timer_standby));
					MOSFET_OFF;
					timer_standby = 1500;
				}

				//Chequeo IPEAK
				i_local = ReadADC1_SameSampleTime (ADC_Channel_8);
				if (i_local > IPEAK)
				{
					main_state = MAIN_ERROR;
					ErrorCommands(ERROR_IPEAK);
					timer_standby = 10000;		//10 segundos
				}
				break;

			case MAIN_CHECK_PAUSE_OR_STOP:
				if ((timer_standby) && (CheckS1() == S_NO))		//es una pausa
				{
					main_state = MAIN_PAUSE;
					minutes_paused = minutes;
					BuzzerCommands(BUZZER_MULTIPLE_SHORT, 1);
				}

				if (CheckS1() > S_HALF)		//es un STOP
				{
					main_state = MAIN_TO_STOP;
					timer_standby = 2000;
					BuzzerCommands(BUZZER_MULTIPLE_LONG, 1);
					LEDR_ON;
					LEDG_ON;
				}
				break;

			case MAIN_PAUSE:
				if (CheckS1() > S_NO)
				{
					minutes = minutes_paused;
					main_state = MAIN_PAUSE_1;
				}

				if (!timer_led)
				{
					if (LEDG)
						LEDG_OFF;
					else
						LEDG_ON;
					timer_led = 300;
				}
				break;

			case MAIN_PAUSE_1:
				if (CheckS1() == S_NO)
				{
					timer_standby = 20;
					//espero edge dwn o timeout
					while ((!mosfet_edge_up) && (timer_standby));
					MOSFET_ON;
					BuzzerCommands(BUZZER_MULTIPLE_SHORT, 2);
					main_state = MAIN_GEN;
				}
				break;

			case MAIN_TO_STOP:
				if (!timer_standby)
				{
					if (CheckS1() == S_NO)
						main_state = MAIN_INIT;
				}
				break;

			case MAIN_FINISH:
				if (!timer_standby)
				{
					main_state = MAIN_INIT;
				}
				break;

			case MAIN_ERROR:
				if (!timer_standby)
				{
					if (CheckS1() > S_NO)
					{
						minutes = minutes_paused;
						main_state = MAIN_GEN;
						timer_standby = 20;
						//espero edge dwn o timeout
						while ((!mosfet_edge_up) && (timer_standby));
						MOSFET_ON;
					}
				}
				break;

			default:
				main_state = MAIN_INIT;
				break;
		}

		//Verifico fase con ADC
		vin_local = ReadADC1_SameSampleTime (ADC_Channel_0);

		if (vin_local > VOLTAGE_SYNC_ON)
		{
			if (!SYNC)		//si venia de no estar en SYNC pongo el flag durante una muestra
				mosfet_edge_up = 1;
			else
				mosfet_edge_up = 0;

			SYNC_ON;
		}
		else if (vin_local < VOLTAGE_SYNC_OFF)
		{
			SYNC_OFF;
		}

		if (main_state == MAIN_GEN)
		{
			if (i_local > CURRENT_SYNCI_ON)
			{
				SYNCI_ON;
			}
			else if (i_local < CURRENT_SYNCI_OFF)
			{
				if (SYNCI)		//si venia con SYNCI pongo el flag durante una muestra
					mosfet_edge_dwn = 1;
				else
					mosfet_edge_dwn = 0;

				SYNCI_OFF;
			}
		}


		UpdateVpeak();

		UpdateBuzzer();

		UpdateSwitches();

		UpdateErrors();

	}
	//--- Fin Loop Principal ---//
	return 0;
}
//--- End of Main ---//

void UpdateVpeak (void)
{

}

unsigned short CheckVin (void)
{
	return 0;
}

unsigned short CheckSync (void)
{
	return 0;
}

void TimingDelay_Decrement(void)
{
	if (wait_ms_var)
		wait_ms_var--;

	if (timer_standby)
		timer_standby--;

	if (take_sample)
		take_sample--;

	if (filter_timer)
		filter_timer--;

	if (switches_timer)
		switches_timer--;

	if (buzzer_timer)
		buzzer_timer--;

	if (timer_led)
		timer_led--;

	if (timer_led_error)
		timer_led_error--;

	//cuenta de a 1 minuto
	if (msecs > 59999)	//pasaron 1 min
	{
		minutes++;
		msecs = 0;
	}
	else
		msecs++;

}


