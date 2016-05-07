/*
 * adc.h
 *
 *  Created on: 10/09/2014
 *      Author: Mariano
 */

#ifndef ADC_H_
#define ADC_H_

#define RCC_ADC_CLK 		(RCC->APB2ENR & 0x00000200)
#define RCC_ADC_CLK_ON 		RCC->APB2ENR |= 0x00000200
#define RCC_ADC_CLK_OFF 	RCC->APB2ENR &= ~0x00000200

#define ADC_IT_ADRDY                               ADC_IER_ADRDYIE
#define ADC_IT_EOSMP                               ADC_IER_EOSMPIE
#define ADC_IT_EOC                                 ADC_IER_EOCIE
#define ADC_IT_EOSEQ                               ADC_IER_EOSEQIE
#define ADC_IT_OVR                                 ADC_IER_OVRIE
#define ADC_IT_AWD                                 ADC_IER_AWDIE


#define ADC_CH0		0x00000001
#define ADC_CH1		0x00000002
#define ADC_CH2		0x00000004

void AdcConfig (void);
unsigned short ReadADC1 (unsigned int);
unsigned short ReadADC1_SameSampleTime (unsigned int);
void SetADC1_SampleTime (void);
unsigned short ReadADC1Check (unsigned char);

#endif /* ADC_H_ */
