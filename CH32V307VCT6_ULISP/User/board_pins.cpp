/*
 * board_pins.cpp
 *
 *  Created on: Sep 13, 2025
 *      Author: sap
 */


#include "board_pins.h"




const pindesc_t pin_desc[] = {
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_0, REMAP0|0, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_1, REMAP0|1, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_2, REMAP0|2, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_3, REMAP0|3, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_4, REMAP1|4, REMAP0|0 },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_5, REMAP1|5, REMAP0|1 },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_6, REMAP0|6, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_7, REMAP0|7, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_8, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_9, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_10, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_11, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_12, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_13, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_14, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOA, GPIO_Pin_15, NOTAVAIL, NOTAVAIL },

		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_0, REMAP0|8, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_1, REMAP0|9, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_2, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_3, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_4, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_5, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_6, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_7, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_8, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_9, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_10, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_11, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_12, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_13, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_14, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOB, GPIO_Pin_15, NOTAVAIL, NOTAVAIL },

		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_0, REMAP0|10, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_1, REMAP0|11, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_2, REMAP0|12, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_3, REMAP0|13, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_4, REMAP0|14, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_5, REMAP0|15, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_6, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_7, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_8, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_9, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_10, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_11, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_12, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_13, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_14, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOC, GPIO_Pin_15, NOTAVAIL, NOTAVAIL },

		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_0, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_1, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_2, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_3, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_4, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_5, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_6, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_7, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_8, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_9, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_10, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_11, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_12, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_13, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_14, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOD, GPIO_Pin_15, NOTAVAIL, NOTAVAIL },

		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_0, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_1, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_2, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_3, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_4, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_5, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_6, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_7, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_8, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_9, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_10, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_11, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_12, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_13, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_14, NOTAVAIL, NOTAVAIL },
		{(GPIO_TypeDef*)GPIOE, GPIO_Pin_15, NOTAVAIL, NOTAVAIL },
};



static uint8_t	iPinMode[64] ;



unsigned int digitalRead(int pin)
{
	pindesc_t *pind = (pindesc_t*)(&pin_desc[pin])  ;

	return GPIO_ReadInputDataBit(pind->GPIOx, pind->iBit) ;
}


void digitalWrite(int pin, int mode)
{
	pindesc_t *pind = (pindesc_t*)(&pin_desc[pin])  ;
	if(mode)
		GPIO_SetBits(pind->GPIOx, pind->iBit);
	else
		GPIO_ResetBits(pind->GPIOx, pind->iBit);
}



unsigned int analogRead(int pin)
{
	unsigned int getADC(int ch) ;

	if(iPinMode[pin] != MODE_INPUT_ANALOG)
	{
		if(pinMode(pin, MODE_INPUT_ANALOG) == 0) return -1 ;
	}

	return getADC(pin_desc[pin].iADCChannel&0x0f);
}



int analogWrite(int pin, uint16_t Data)
{
	pindesc_t *pind = (pindesc_t*)(&pin_desc[pin])  ;

	if(iPinMode[pin] != MODE_OUTPUT_ANALOG)
	{
		if(pinMode(pin, MODE_OUTPUT_ANALOG) == 0) return 0 ;
	}

	if((pind->iDACChannel&0x0f)==0)
		DAC_SetChannel1Data(DAC_Align_12b_R, Data);
	else
		DAC_SetChannel2Data(DAC_Align_12b_R, Data);
	return 1 ;
}




int pinMode(int pin, int mode)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	pindesc_t *pind = (pindesc_t*)(&pin_desc[pin])  ;

	switch(mode)
	{
	case MODE_INPUT_ANALOG:
		if(pind->iADCChannel&(REMAP0|REMAP1|REMAP2)==0) return 0 ;
	    GPIO_InitStructure.GPIO_Pin = pind->iBit ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(pind->GPIOx, &GPIO_InitStructure);
	    //iPinMode[pin] = MODE_INPUT_ANALOG ;
	    break ;
	case MODE_OUTPUT_ANALOG:
		if(pind->iDACChannel&(REMAP0|REMAP1|REMAP2)==0) return 0 ;
	    GPIO_InitStructure.GPIO_Pin = pind->iBit ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(pind->GPIOx, &GPIO_InitStructure);
	    //iPinMode[pin] = MODE_OUTPUT_ANALOG ;
		DAC_Cmd((pind->iDACChannel&0x0f)<<4, ENABLE) ;
	    break ;
	case OUTPUT:
	    GPIO_InitStructure.GPIO_Pin = pind->iBit ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(pind->GPIOx, &GPIO_InitStructure);
	    break ;
	case INPUT_PULLUP:
	    GPIO_InitStructure.GPIO_Pin = pind->iBit ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(pind->GPIOx, &GPIO_InitStructure);
	    break ;
	case INPUT_PULLDOWN:
	    GPIO_InitStructure.GPIO_Pin = pind->iBit ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(pind->GPIOx, &GPIO_InitStructure);
	    break ;
	default :
	    GPIO_InitStructure.GPIO_Pin = pind->iBit ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(pind->GPIOx, &GPIO_InitStructure);
	}

	iPinMode[pin] = mode ;

	return 1 ;
}
