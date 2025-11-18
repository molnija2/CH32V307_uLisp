/*
 * board_pins.cpp
 *
 *  Created on: Sep 13, 2025
 *      Author: sap
 */

#include "ch32v30x_rcc.h"
#include "board_pins.h"


GPIO_TypeDef* GPIOx[] = {GPIOA,GPIOB, GPIOC, GPIOD, GPIOE };

#define DEF_GPIOA 	 0
#define DEF_GPIOB 	 1
#define DEF_GPIOC 	 2
#define DEF_GPIOD 	 3
#define DEF_GPIOE 	 4

enum{ NO_TIMER=0, TIMER1, TIMER2, TIMER3, TIMER4, TIMER5, TIMER6, TIMER7, TIMER8, TIMER9, TIMER10 } ;

const pindesc_t pin_desc[] = {
		{DEF_GPIOA, 0, REMAP0|0, NOTAVAIL },
		{DEF_GPIOA, 1, REMAP0|1, NOTAVAIL },
		{DEF_GPIOA, 2, REMAP0|2, NOTAVAIL },
		{DEF_GPIOA, 3, REMAP0|3, NOTAVAIL },
		{DEF_GPIOA, 4, REMAP1|4, REMAP0|0 },
		{DEF_GPIOA, 5, REMAP1|5, REMAP0|1 },
		{DEF_GPIOA, 6, REMAP0|6, NOTAVAIL },
		{DEF_GPIOA, 7, REMAP0|7, NOTAVAIL },
		{DEF_GPIOA, 8, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOA, 9, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOA, 10, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOA, 11, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOA, 12, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOA, 13, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOA, 14, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOA, 15, NOTAVAIL, NOTAVAIL },

		{DEF_GPIOB, 0, REMAP0|8, NOTAVAIL },
		{DEF_GPIOB, 1, REMAP0|9, NOTAVAIL },
		{DEF_GPIOB, 2, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 3, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 4, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 5, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 6, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 7, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 8, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 9, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 10, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 11, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 12, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 13, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 14, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOB, 15, NOTAVAIL, NOTAVAIL },

		{DEF_GPIOC, 0, REMAP0|10, NOTAVAIL },
		{DEF_GPIOC, 1, REMAP0|11, NOTAVAIL },
		{DEF_GPIOC, 2, REMAP0|12, NOTAVAIL },
		{DEF_GPIOC, 3, REMAP0|13, NOTAVAIL },
		{DEF_GPIOC, 4, REMAP0|14, NOTAVAIL },
		{DEF_GPIOC, 5, REMAP0|15, NOTAVAIL },
		{DEF_GPIOC, 6, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 7, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 8, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 9, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 10, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 11, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 12, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 13, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 14, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOC, 15, NOTAVAIL, NOTAVAIL },

		{DEF_GPIOD, 0, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 1, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 2, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 3, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 4, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 5, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 6, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 7, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 8, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 9, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 10, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 11, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 12, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 13, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 14, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOD, 15, NOTAVAIL, NOTAVAIL },

		{DEF_GPIOE, 0, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 1, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 2, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 3, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 4, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 5, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 6, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 7, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 8, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 9, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 10, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 11, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 12, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 13, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 14, NOTAVAIL, NOTAVAIL },
		{DEF_GPIOE, 15, NOTAVAIL, NOTAVAIL },
};



static uint8_t	iPinMode[16*5] ;



unsigned int digitalRead(int pin)
{
	pindesc_t *pind = (pindesc_t*)(&pin_desc[pin])  ;

	return GPIO_ReadInputDataBit(GPIOx[pind->iPort], pind->iBit) ;
}


void digitalWrite(int pin, int mode)
{
	pindesc_t *pind = (pindesc_t*)(&pin_desc[pin])  ;
	if(mode)
		GPIO_SetBits(GPIOx[pind->iPort], pind->iBit);
	else
		GPIO_ResetBits(GPIOx[pind->iPort], pind->iBit);
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


	pindesc_t *pind = (pindesc_t*)(&pin_desc[pin])  ;

	switch(mode)
	{
	case MODE_INPUT_ANALOG:
		if(pind->iADCChannel&(REMAP0|REMAP1|REMAP2)==0) return 0 ;
	    GPIO_InitStructure.GPIO_Pin = (1<<pind->iBit) ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOx[pind->iPort], &GPIO_InitStructure);
	    //iPinMode[pin] = MODE_INPUT_ANALOG ;
	    break ;
	case MODE_OUTPUT_ANALOG:
		if(pind->iDACChannel&(REMAP0|REMAP1|REMAP2)==0) return 0 ;
	    GPIO_InitStructure.GPIO_Pin = (1<<pind->iBit) ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOx[pind->iPort], &GPIO_InitStructure);
	    //iPinMode[pin] = MODE_OUTPUT_ANALOG ;
		DAC_Cmd((pind->iDACChannel&0x0f)<<4, ENABLE) ;
	    break ;
	case OUTPUT:
	    GPIO_InitStructure.GPIO_Pin = (1<<pind->iBit) ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOx[pind->iPort], &GPIO_InitStructure);
	    break ;
	case INPUT_PULLUP:
	    GPIO_InitStructure.GPIO_Pin = (1<<pind->iBit) ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOx[pind->iPort], &GPIO_InitStructure);
	    break ;
	case INPUT_PULLDOWN:
	    GPIO_InitStructure.GPIO_Pin = (1<<pind->iBit) ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOx[pind->iPort], &GPIO_InitStructure);
	    break ;
	default :
	    GPIO_InitStructure.GPIO_Pin = (1<<pind->iBit) ;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOx[pind->iPort], &GPIO_InitStructure);
	}

	iPinMode[pin] = mode ;

	return 1 ;
}

void checkanalogread (int pin) {
	//if(pin_desc[pin].iADCChannel==NOTAVAIL) return 0 ;
	//return 1 ;
}

void checkanalogwrite (int pin) {

}

static const uint16_t spi_bitorder[2] = { SPI_FirstBit_LSB, SPI_FirstBit_MSB} ;
static const uint16_t spi_mode[4] = { SPI_CPOL_High|(SPI_CPHA_2Edge<<8),
		SPI_CPOL_Low|(SPI_CPHA_2Edge<<8),
		SPI_CPOL_High|(SPI_CPHA_1Edge<<8),
		SPI_CPOL_Low|(SPI_CPHA_1Edge<<8) } ;

int SPI_beginTransaction( unsigned long speed, uint8_t bitorder, uint8_t mode) {

    GPIO_InitTypeDef GPIO_InitStructure ;
    SPI_InitTypeDef  SPI_InitStructure ;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1, ENABLE);
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI1, ENABLE);


    /* SPI_CLK */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI_MISO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI_MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    unsigned int apbclock = RCC_ClocksStatus.PCLK1_Frequency;
    int presc = -1 ;
    do{
    	apbclock <<=1 ;
    	presc += 1 ;
    }  while((apbclock>speed)&&(presc<SPI_BaudRatePrescaler_256)) ;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b ;
    SPI_InitStructure.SPI_CPOL = spi_mode[mode]&0xff ;
    SPI_InitStructure.SPI_CPHA = spi_mode[mode]>>8 ;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = presc ; /* APB1_CLK (72MHz) */
    SPI_InitStructure.SPI_FirstBit = spi_bitorder[bitorder];
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    //GPIO_PinRemapConfig(GPIO_PartialRemap1_SPI1, ENABLE);

    SPI_Cmd(SPI1, ENABLE);

	return 0 ;
}
