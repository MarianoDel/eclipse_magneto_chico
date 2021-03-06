/*
 * hard.c
 *
 *  Created on: 11/02/2016
 *      Author: Mariano
 */
#include "hard.h"
#include "stm32f0xx.h"

// ------- Externals de los switches -------

// ------- Externals de los timers -------
extern volatile unsigned char switches_timer;
extern volatile unsigned short buzzer_timer;
extern volatile unsigned short timer_led_error;

#define buzzer_timeout buzzer_timer

// ------- Globales de los switches -------
unsigned short s1;
//unsigned short s2;

// ------- Globales del buzzer -------
unsigned char buzzer_state = 0;
unsigned char buzz_multiple = 0;

// ------- de los led y errores -------
enum var_error_states error_state = ERROR_NO;
unsigned char error_bips = 0;
unsigned char error_bips_counter = 0;


// ------- Funciones del Modulo -------

void UpdateSwitches (void)
{
	//revisa los switches cada 10ms
	if (!switches_timer)
	{
		if (S1_PIN)
			s1++;
		else if (s1 > 50)
			s1 -= 50;
		else if (s1 > 10)
			s1 -= 5;
		else if (s1)
			s1--;


//		if (S2_PIN)
//			s2++;
//		else if (s2 > 50)
//			s2 -= 50;
//		else if (s2 > 10)
//			s2 -= 5;
//		else if (s2)
//			s2--;

		switches_timer = SWITCHES_TIMER_RELOAD;
	}
}


/*
void UpdateACSwitch (void)
{
	//revisa el switch ac cada 12ms
	if (!acswitch_timer)
	{
		//termino ventana de 12ms
		if (sac_aux)
		{
			if (sac < AC_SWITCH_THRESHOLD_ROOF)
				sac++;
			sac_aux = 0;
		}
		else if (sac > 50)
			sac -= 50;
		else if (sac > 10)
			sac -= 5;
		else if (sac)
			sac--;

		acswitch_timer = AC_SWITCH_TIMER_RELOAD;
	}
	else
	{
		if (SW_AC)
			sac_aux = 1;
	}
}
*/

/*
unsigned char CheckACSw (void)	//cada check tiene 12ms
{
	if (sac > AC_SWITCH_THRESHOLD_FULL)
		return S_FULL;

	if (sac > AC_SWITCH_THRESHOLD_HALF)
		return S_HALF;

	if (sac > AC_SWITCH_THRESHOLD_MIN)
		return S_MIN;

	return S_NO;
}
*/


unsigned char CheckS1 (void)	//cada check tiene 10ms
{
	if (s1 > SWITCHES_THRESHOLD_FULL)
		return S_FULL;

	if (s1 > SWITCHES_THRESHOLD_HALF)
		return S_HALF;

	if (s1 > SWITCHES_THRESHOLD_MIN)
		return S_MIN;

	return S_NO;
}


/*
unsigned char CheckS2 (void)
{
	if (s2 > SWITCHES_THRESHOLD_FULL)
		return S_FULL;

	if (s2 > SWITCHES_THRESHOLD_HALF)
		return S_HALF;

	if (s2 > SWITCHES_THRESHOLD_MIN)
		return S_MIN;

	return S_NO;
}
*/


void BuzzerCommands(unsigned char command, unsigned char multiple)
{
	buzzer_state = command;
	buzz_multiple = multiple;
}

void UpdateBuzzer (void)
{
	switch (buzzer_state)
	{
		case BUZZER_INIT:
			break;

		case BUZZER_MULTIPLE_SHORT:
			if (buzz_multiple > 0)
			{
				BUZZER_ON;
				buzzer_state++;
				buzzer_timeout = TIM_BIP_SHORT;
			}
			else
				buzzer_state = BUZZER_TO_STOP;
			break;

		case BUZZER_MULTIPLE_SHORTA:
			if (!buzzer_timeout)
			{
				buzzer_state++;
				BUZZER_OFF;
				buzzer_timeout = TIM_BIP_SHORT_WAIT;
			}
			break;

		case BUZZER_MULTIPLE_SHORTB:
			if (!buzzer_timeout)
			{
				if (buzz_multiple)
					buzz_multiple--;

				buzzer_state = BUZZER_MULTIPLE_SHORT;
			}
			break;

		case BUZZER_MULTIPLE_HALF:
			if (buzz_multiple > 0)
			{
				BUZZER_ON;
				buzzer_state++;
				buzzer_timeout = TIM_BIP_HALF;
			}
			else
				buzzer_state = BUZZER_TO_STOP;
			break;

		case BUZZER_MULTIPLE_HALFA:
			if (!buzzer_timeout)
			{
				buzzer_state++;
				BUZZER_OFF;
				buzzer_timeout = TIM_BIP_HALF_WAIT;
			}
			break;

		case BUZZER_MULTIPLE_HALFB:
			if (!buzzer_timeout)
			{
				if (buzz_multiple)
					buzz_multiple--;

				buzzer_state = BUZZER_MULTIPLE_HALF;
			}
			break;

		case BUZZER_MULTIPLE_LONG:
			if (buzz_multiple > 0)
			{
				BUZZER_ON;
				buzzer_state++;
				buzzer_timeout = TIM_BIP_LONG;
			}
			else
				buzzer_state = BUZZER_TO_STOP;
			break;

		case BUZZER_MULTIPLE_LONGA:
			if (!buzzer_timeout)
			{
				buzzer_state++;
				BUZZER_OFF;
				buzzer_timeout = TIM_BIP_LONG_WAIT;
			}
			break;

		case BUZZER_MULTIPLE_LONGB:
			if (!buzzer_timeout)
			{
				if (buzz_multiple)
					buzz_multiple--;

				buzzer_state = BUZZER_MULTIPLE_LONG;
			}
			break;

		case BUZZER_TO_STOP:
		default:
			BUZZER_OFF;
			buzzer_state = BUZZER_INIT;
			break;
	}
}

void UpdateErrors (void)
{
	switch (error_state)
	{
		case ERROR_NO:
			break;

		case ERROR_IPEAK:
			error_bips = 1;
			error_state = ERROR_RUN;
			break;

		case ERROR_VIN:
			error_bips = 2;
			error_bips_counter = error_bips;
			error_state = ERROR_RUN;
			break;

		case ERROR_VBAT:
			error_bips = 3;
			error_bips_counter = error_bips;
			error_state = ERROR_RUN;
			break;

		case ERROR_VBAT_REVERSAL:
			error_bips = 5;
			error_bips_counter = error_bips;
			error_state = ERROR_RUN;
			break;

		case ERROR_TEMP:
			error_bips = 4;
			error_bips_counter = error_bips;
			error_state = ERROR_RUN;
			break;

		case ERROR_RUN:
			if (!timer_led_error)
			{
				error_state = ERROR_RUN_A;
				error_bips_counter = error_bips;
			}

			break;

		case ERROR_RUN_A:
			if (!timer_led_error)
			{
				if (error_bips_counter)
				{
					LEDR_ON;
					error_bips_counter--;
					error_state++;
					timer_led_error = TT_BIP_SHORT;
				}
				else
				{
					LEDR_OFF;
					timer_led_error = TT_BIP_LONG;
					error_state = ERROR_RUN;		//termina el ciclo hago la espera larga
				}
			}
			break;

		case ERROR_RUN_B:
			if (!timer_led_error)
			{
				LEDR_OFF;
				timer_led_error = TT_BIP_SHORT;
				error_state--;
			}
			break;

		default:
			LEDR_OFF;
			error_state = ERROR_NO;
			timer_led_error = 0;
			break;
	}
}

void ErrorCommands(unsigned char command)
{
	if (command == ERROR_NO)
		LEDR_OFF;

	error_state = command;
}


