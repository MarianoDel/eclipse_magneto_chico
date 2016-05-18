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
#define SYNCI ((GPIOA->ODR & 0x0004) != 0)
#define SYNCI_ON	GPIOA->BSRR = 0x00000004
#define SYNCI_OFF GPIOA->BSRR = 0x00040000

//GPIOA pin3

//GPIOA pin4
#define BUZZER ((GPIOA->ODR & 0x0010) != 0)
#define BUZZER_ON	GPIOA->BSRR = 0x00000010
#define BUZZER_OFF GPIOA->BSRR = 0x00100000

//GPIOA pin5

//GPIOA pin6
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

#define LEDG 		LED1
#define LEDG_ON		LED1_ON
#define LEDG_OFF 	LED1_OFF

//GPIOA pin11
#define LED2 ((GPIOA->ODR & 0x0800) != 0)
#define LED2_ON	GPIOA->BSRR = 0x00000800
#define LED2_OFF GPIOA->BSRR = 0x08000000

#define LEDR 		LED2
#define LEDR_ON		LED2_ON
#define LEDR_OFF 	LED2_OFF

//GPIOA pin12
#define S_START ((GPIOA->IDR & 0x1000) == 0)

//GPIOA pin13
//GPIOA pin14

//GPIOA pin15
#define SYNC ((GPIOA->ODR & 0x8000) != 0)
#define SYNC_ON	GPIOA->BSRR = 0x00008000
#define SYNC_OFF GPIOA->BSRR = 0x80000000


//GPIOB pin0
//GPIOB pin1
//GPIOB pin3
//GPIOB pin4
//GPIOB pin5
//GPIOB pin6
//GPIOB pin7

#define S1_PIN		S_START


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


#define SWITCHES_TIMER_RELOAD	10

#define SWITCHES_THRESHOLD_FULL	300		//3 segundos
#define SWITCHES_THRESHOLD_HALF	100		//1 segundo
#define SWITCHES_THRESHOLD_MIN	5		//50 ms

#define S_FULL		10
#define S_HALF		3
#define S_MIN		1
#define S_NO		0

//ESTADOS DEL PROGRAMA PRINCIPAL
//estados del MAIN MENU
enum var_main_state
{
	MAIN_INIT = 0,
	MAIN_STANDBY,
	MAIN_STANDBY_1,
	MAIN_STANDBY_2,
	MAIN_GEN,
	MAIN_CHECK_PAUSE_OR_STOP,
	MAIN_CHECK_PAUSE_OR_STOP_1,
	MAIN_PAUSE,
	MAIN_PAUSE_1,
	MAIN_PAUSE_2,
	MAIN_TO_STOP,
	MAIN_FINISH,
	MAIN_ERROR

};

 //estados de ERRORES
enum var_error_states
{
	ERROR_NO = 0,
	ERROR_IPEAK,
	ERROR_VIN,
	ERROR_VBAT,
	ERROR_VBAT_REVERSAL,
	ERROR_TEMP,
	ERROR_RUN,
	ERROR_RUN_A,
	ERROR_RUN_B

};

#define IPEAK	3071	//1.5A
//#define VOLTAGE_ZERO		2048	//---- Valores de tension del ADC
//#define VOLTAGE_ZERO		1950	//zero corregido 1.76V
//#define VOLTAGE_MIN			2250
//#define VOLTAGE_MIN			(VOLTAGE_ZERO + 150)
#define VOLTAGE_SYNC_ON		100
#define VOLTAGE_SYNC_OFF	50

#define CURRENT_SYNCI_ON	100
#define CURRENT_SYNCI_OFF	50


//timeout de los ERRORES
#define TT_BIP_LONG		1000
#define TT_BIP_SHORT	300


//ESTADOS DEL BUZZER
#define BUZZER_INIT		0
#define BUZZER_TO_STOP		10

#define BUZZER_MULTIPLE_LONG			40
#define BUZZER_MULTIPLE_LONGA			41
#define BUZZER_MULTIPLE_LONGB			42

#define BUZZER_MULTIPLE_HALF			50
#define BUZZER_MULTIPLE_HALFA			51
#define BUZZER_MULTIPLE_HALFB			52

#define BUZZER_MULTIPLE_SHORT			60
#define BUZZER_MULTIPLE_SHORTA			61
#define BUZZER_MULTIPLE_SHORTB			62

//COMANDOS DEL BUZZER	(tienen que ser los del estado de arriba)
#define BUZZER_STOP_CMD		10
#define BUZZER_LONG_CMD		40
#define BUZZER_HALF_CMD		50
#define BUZZER_SHORT_CMD	60

#define TIM_BIP_SHORT		50
#define TIM_BIP_SHORT_WAIT		100
#define TIM_BIP_HALF		200
#define TIM_BIP_HALF_WAIT		500
#define TIM_BIP_LONG		1200
#define TIM_BIP_LONG_WAIT		1500


// ------- de los switches -------
void UpdateSwitches (void);
unsigned char CheckS1 (void);
void BuzzerCommands(unsigned char , unsigned char);
void UpdateBuzzer (void);
void UpdateErrors (void);
void ErrorCommands(unsigned char);



#endif /* HARD_H_ */
