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
volatile unsigned short timer_led_comm = 0;

// ------- Externals de los Timers -------
volatile unsigned short wait_ms_var = 0;
volatile unsigned short prog_timer = 0;
volatile unsigned char switches_timer = 0;
volatile unsigned short buzzer_timer = 0;



// ------- Externals de los modos -------




//--- VARIABLES GLOBALES ---//


// ------- de los timers -------
volatile unsigned short timer_standby;
volatile unsigned char filter_timer;
volatile unsigned char take_sample;

//volatile unsigned char door_filter;
//volatile unsigned char take_sample;
//volatile unsigned char move_relay;
volatile unsigned short secs = 0;
volatile unsigned short minutes = 0;

// ------- del display -------
unsigned char v_opt [10];


// ------- de los filtros DMX -------
#define LARGO_F		32
#define DIVISOR_F	5
unsigned char vd0 [LARGO_F + 1];


#define IDLE	0
#define LOOK_FOR_BREAK	1
#define LOOK_FOR_MARK	2
#define LOOK_FOR_START	3

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
	unsigned short ii;
	unsigned char sample_ready = 0;
	enum var_main_state main_state;

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
					LEDG_OFF;
					main_state++;
				}
				break;

			case MAIN_STANDBY:
				if (CheckS1() > S_NO)
				{
					main_state = MAIN_GEN;
					BuzzerCommands(BUZZER_MULTIPLE_SHORT, 1);
					LEDR_ON;
					timer_standby = 3000;
				}

				break;

			case MAIN_GEN:
				if (!timer_standby)
					main_state = MAIN_INIT;




				break;

			case MAIN_PAUSE:
				break;

			case MAIN_FINISH:
				break;

			case MAIN_ERROR:
				break;

			default:
				main_state = MAIN_INIT;
				break;
		}
//		ii = ReadADC1_SameSampleTime (ADC_Channel_0);
//		LED1_ON;


		UpdateSync();

		UpdateVpeak();

		UpdateBuzzer();

		UpdateSwitches();


	}
	//--- Fin Loop Principal ---//
	return 0;
}
//--- End of Main ---//

void UpdateSync (void)
{

}

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

	//cuenta de a 1 minuto
	if (secs > 59999)	//pasaron 1 min
	{
		minutes++;
		secs = 0;
	}
	else
		secs++;

}


