/*
 * hard.h
 *
 *  Created on: 28/11/2013
 *      Author: Mariano
 */

#ifndef HARD_H_
#define HARD_H_


//-------- Defines For Configuration -------------


//-------- End Of Defines For Configuration ------
//GPIOA pin0	ADC FASE y SYNC

//GPIOA pin1
//GPIOA pin2
//GPIOA pin3

//GPIOA pin4
#define BUZZER ((GPIOA->ODR & 0x0010) != 0)
#define BUZZER_ON	GPIOA->BSRR = 0x00000010
#define BUZZER_OFF GPIOA->BSRR = 0x00100000

//GPIOA pin5

//GPIOA pin6 funciona negado
#define MOSFET ((GPIOA->ODR & 0x0040) == 0)
#define MOSFET_OFF	GPIOA->BSRR = 0x00000040
#define MOSFET_ON GPIOA->BSRR = 0x00400000

//GPIOA pin7

//GPIOA pin8	ADC CURRENT_SENSE

//GPIOA pin9

//GPIOA pin10
#define LED1 ((GPIOA->ODR & 0x0400) != 0)
#define LED1_ON	GPIOA->BSRR = 0x00000400
#define LED1_OFF GPIOA->BSRR = 0x04000000

//GPIOA pin11
#define LED2 ((GPIOA->ODR & 0x0800) != 0)
#define LED2_ON	GPIOA->BSRR = 0x00000800
#define LED2_OFF GPIOA->BSRR = 0x08000000

//GPIOA pin12
#define S_START ((GPIOA->IDR & 0x1000) == 0)

//GPIOA pin13
//GPIOA pin14
//GPIOA pin15

//GPIOB pin0
//GPIOB pin1
//GPIOB pin3
//GPIOB pin4
//GPIOB pin5
//GPIOB pin6
//GPIOB pin7

#define S1_PIN		S_START

//ESTADOS DEL PROGRAMA PRINCIPAL
#define MAIN_INIT				0

#define MAIN_STAND_ALONE		10
#define MAIN_GROUPED			11
#define MAIN_NETWORKED			12
#define MAIN_IN_MAIN_MENU		13

//---- Temperaturas en el LM335
//37	2,572
//40	2,600
//45	2,650
//50	2,681
//55	2,725
//60	2,765
#define TEMP_IN_30		3226
#define TEMP_IN_35		3279
#define TEMP_IN_50		3434
#define TEMP_IN_65		3591
#define TEMP_DISCONECT		4000

//ESTADOS DEL DISPLAY EN RGB_FOR_CAT
#define SHOW_CHANNELS	0
#define SHOW_NUMBERS	1

#define SWITCHES_TIMER_RELOAD	10
#define AC_SWITCH_TIMER_RELOAD	22

#define SWITCHES_THRESHOLD_FULL	300		//3 segundos
#define SWITCHES_THRESHOLD_HALF	100		//1 segundo
#define SWITCHES_THRESHOLD_MIN	5		//50 ms

#define AC_SWITCH_THRESHOLD_ROOF	255		//techo del integrador
#define AC_SWITCH_THRESHOLD_FULL	136		//3 segundos
#define AC_SWITCH_THRESHOLD_HALF	45		//1 segundo
#define AC_SWITCH_THRESHOLD_MIN		2		//50 ms

#define TTIMER_FOR_CAT_DISPLAY			2000	//tiempo entre que dice canal y el numero
#define TIMER_STANDBY_TIMEOUT_REDUCED	2000	//reduced 2 segs
#define TIMER_STANDBY_TIMEOUT			6000	//6 segundos
#define DMX_DISPLAY_SHOW_TIMEOUT		30000	//30 segundos

#define S_FULL		10
#define S_HALF		3
#define S_MIN		1
#define S_NO		0

#define FUNCTION_DMX	1
#define FUNCTION_MAN	2
#define FUNCTION_CAT	FUNCTION_MAN

// ------- de los switches -------
void UpdateSwitches (void);
unsigned char CheckS1 (void);
unsigned char CheckS2 (void);
void UpdateACSwitch (void);
unsigned char CheckACSw (void);


#endif /* HARD_H_ */
