/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"

#include "global.h"

#include "ff.h"

#include "ch32v30x_fsmc.h"
#include "ch32v30x_adc.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_pwr.h"

#include "rtc.h"

#include "ILI9488.h"
#include "XPT2046.h"

/* Global typedef */

/* Global define */
#define Bank1_SRAM1_ADDR    ((u32)(0x60000000))



//FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTimingStruct = {0} ;
//FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStruct = {0} ;


#define	GPIO_DATA_D		(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15)
#define	GPIO_DATA_E		(GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)

#define	GPIO_ADDR_D		(GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13)
#define	GPIO_ADDR_E		(GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13)

#define	GPIO_nOE_D		GPIO_Pin_4
#define	GPIO_nWE_D		GPIO_Pin_5
#define	GPIO_nE1_D		GPIO_Pin_7

#define	GPIO_nBL0_E		GPIO_Pin_0
#define	GPIO_nBL1_E		GPIO_Pin_1

#define GPIO_NADV_B		GPIO_Pin_7


//GPIO_InitTypeDef  GPIO_Ddata_Inittructure ;
//GPIO_InitTypeDef  GPIO_Edata_Inittructure ;



/* Global Variable */

void	setup();

void	loop();




//const char file_name[] = "main_program.l" ;



//const char string_mount_error[17] = "SD mount error\n";




void SRAM_gpio_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);

	//   DATA pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1//|GPIO_Pin_3
    		|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7 //|GPIO_Pin_6
			|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11
			|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3 //|GPIO_Pin_2
    		|GPIO_Pin_4|GPIO_Pin_7 // |GPIO_Pin_5|GPIO_Pin_6
			|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11
			|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SRAM_init()
{
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTimingStruct = {0} ;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStruct = {0} ;

	SRAM_gpio_init();

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);

	FSMC_NORSRAMInitStruct.FSMC_Bank = FSMC_Bank1_NORSRAM1 ;
	FSMC_NORSRAMInitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable ;
	FSMC_NORSRAMInitStruct.FSMC_MemoryType = FSMC_MemoryType_SRAM ;
	FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b ;
	FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable ;
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable ;
	FSMC_NORSRAMInitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable ;
	FSMC_NORSRAMInitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTimingStruct ;
	FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct = &FSMC_ReadWriteTimingStruct ;

	FSMC_ReadWriteTimingStruct.FSMC_AccessMode = FSMC_AccessMode_A ;
	FSMC_ReadWriteTimingStruct.FSMC_AddressSetupTime = 2 ; //14;//14 ; //14 ; //11
	FSMC_ReadWriteTimingStruct.FSMC_AddressHoldTime = 2 ; //10;//7 ; //14 ; //10
	FSMC_ReadWriteTimingStruct.FSMC_DataSetupTime = 8 ;   //63;//94 ; //88 ;  //83 /* 1-255 */
	FSMC_ReadWriteTimingStruct.FSMC_BusTurnAroundDuration = 0 ;


	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

	Delay_Ms(1) ;

	//RAM_ext = (char*)Bank1_SRAM1_ADDR ;
}




int SRAM_Test2 (int iN)
{
	int i, errors = 0 ;

	// FIRST TEST

	for(i=0; i<64*iN; i++){
		uint32_t i1 = i*4 ;
		i1 = 0xffffffff ;
      *(u32*)(Bank1_SRAM1_ADDR + 4*i) = i1 ;
	}


	for(i=0; i<64*iN; i++){
		uint32_t i1 = i*4 ;
		i1 = 0xffffffff ;
		u32 i2 = *(u32*)(Bank1_SRAM1_ADDR + 4*i) ;
		if(i1!=i2)
		{
    		errors ++ ;
		}
    }

	//  SECOND TEST
	for(i=0; i<64*iN; i++){
		uint32_t i1 = i*4 ;
		i1 = 0 ;
      *(u32*)(Bank1_SRAM1_ADDR + 4*i) = i1 ;
	}


	for(i=0; i<64*iN; i++){
		uint32_t i1 = i*4 ;
		i1 = 0 ;
		u32 i2 = *(u32*)(Bank1_SRAM1_ADDR + 4*i) ;
		if(i1!=i2)
		{
    		errors ++ ;
		}
    }

	return errors ;
}






/* ********************  USART1  Rx Getchar ******************** */

void USART_Getch_Init (uint32_t baudrate)
{
	//GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure = { 0 };
	//USART_InitTypeDef USART_InitStructure;


	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE) ;

	//   RX - input pin
    /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);*/


    /*USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);*/


    u32 temp ;

    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE) ;

    temp = USART1->CTLR1 ;
    temp |= (1<<5) ;  //  USART_IT_RXNE
    //temp |= (1<<4) ;  //  USART_IT_IDLE
    USART1->CTLR1 = temp ;

    /*temp = USART1->CTLR2 ;
    temp |= (1<<6) ;  //  USART_IT_LBD
    USART1->CTLR2 = temp ;

    temp = USART1->CTLR3 ;
    temp &= 0xfffffff3 ;
    USART1->CTLR3 = temp | 0x0001; //  USART_IT_ERR*/

    //USART_Cmd(USART1, DISABLE);

   //USART_ReceiverWakeUpCmd(USART1, ENABLE) ;

    /* Configure USART interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init( &NVIC_InitStructure );

    /*USART_ClearFlag( USART1, USART_FLAG_RXNE );
    USART_ClearFlag( USART1, USART_FLAG_IDLE );
    USART_ClearFlag( USART1, USART_FLAG_FE );*/


    /*RxRecvPtr = cRxBuffer ;
    RxRecvReadPtr = cRxBuffer ;
    uiRecvIndex = 0 ;
    uiRecvReadIndex = 0 ;
    USART_Cmd(USART1, ENABLE);*/
    //NVIC_EnableIRQ( USART1_IRQn );
}





void StartWaitUSART()
{

     RxRecvPtr = cRxBuffer ;
     //CmdBufferPtr = cRxBuffer ;
     RxRecvReadPtr = cRxBuffer ;
     //iPackageLenght = 0 ;
     //iFlagPackageEnd = 0 ;

     uiRecvIndex = 0 ;
     uiRecvReadIndex = 0 ;

     NVIC_EnableIRQ( USART1_IRQn );
}


int getch ()
{
	//while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) ;
	//return USART_ReceiveData(USART1) & 0x0ff;

	int ret ;
	while(uiRecvReadIndex==uiRecvIndex) ;

	ret = *RxRecvReadPtr ++ ;
    uiRecvReadIndex ++ ;
    if(uiRecvReadIndex >= DEF_USART_RX_SIZE)
    {
    	uiRecvReadIndex = 0 ;
    	RxRecvReadPtr = cRxBuffer ;
    }

    return ret & 0xFF ;
}


/*void MMC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

    //CS @ PA15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    GPIOA->BSHR = GPIO_Pin_15; //CS H
}*/


void SPI_SDD_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

    //CS @ PA15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    GPIOA->BSHR = GPIO_Pin_15; //CS H


    /* SPI_MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    /* SPI_MISO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    /* SPI_CLK */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE) ;

    //SPI
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI1, &SPI_InitStructure );

    SPI_Cmd( SPI1, ENABLE );

}





void putchar0(uint8_t c)
{
	  //USART_ClearFlag(USART1,USART_FLAG_TC);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, c);
}



int16_t calibration_value;

void ADC_initialization()
{


    ADC_InitTypeDef InitStruct = { 0 } ;

    // First let's give the ADC some clock
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE ); // Clock to ADC1 - both ADCs are hanging off of the APB2 bus
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); // What is PCLK2 - 144 MHz / 4 = 36 MHz?

	ADC_DeInit(ADC1) ;

    InitStruct.ADC_Mode = ADC_Pga_1 | ADC_Mode_Independent;  // No fancy injection stuff for now
    InitStruct.ADC_ScanConvMode = DISABLE;       // Just sample one
    InitStruct.ADC_ContinuousConvMode = DISABLE; // And just do it once
    InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // We will ASK when needed
    InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    InitStruct.ADC_NbrOfChannel = 1;


	ADC_Init(ADC1, &InitStruct) ;
    ADC_Cmd(ADC1, ENABLE);

    // Let's run a self calibration on the ADC
    ADC_BufferCmd(ADC1, DISABLE);   // disable buffer
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
    calibration_value = Get_CalibrationValue(ADC1);
    ADC_BufferCmd(ADC1, ENABLE);   // reenable buffer

    ADC_TempSensorVrefintCmd(ENABLE);

	/*ADC_DiscModeCmd(ADC1, ENABLE) ;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE) ;*/
}


void DAC_sound_init()
{

	DAC_InitTypeDef DAC_InitStruct = {0} ;


    RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC, ENABLE ); // Clock to ADC1 - both ADCs are hanging off of the APB2 bus

	DAC_DeInit() ;

    DAC_InitStruct.DAC_Trigger = DAC_Trigger_None ; //T2_TRGO ;
    DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	DAC_Init(DAC_Channel_1, &DAC_InitStruct);
	DAC_Init(DAC_Channel_2, &DAC_InitStruct);
	DAC_Cmd(DAC_Channel_1, DISABLE) ;
	DAC_Cmd(DAC_Channel_2, DISABLE) ;

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

unsigned int getADC(int ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_28Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    //uint32_t cnt = 0;
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE) ;
	//ADC_Cmd(ADC1, ENABLE) ;

	while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==0) // //++cnt;

	return ADC_GetConversionValue(ADC1)&0x0fff;
}



#define UART_BOUDRATE		115200


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(UART_BOUDRATE);
	USART_Getch_Init(UART_BOUDRATE) ;
	//printf("SystemClk:%d\n",SystemCoreClock);
	//printf( "ChipID:%08x\n\r\n", DBGMCU_GetCHIPID() );

	ADC_initialization() ;
	DAC_sound_init() ;

	RTC_Init() ;
	//RTC_Reset() ;

	SRAM_init() ;
	//Delay_Ms(10) ;
	//printf("External RAM test %d errors\n", SRAM_Test2(4*512));


	SysClockRestart() ;

	//MMC_GPIO_Init();
    SPI_SDD_Init();

    StartWaitUSART() ;

//    ILI9488_begin() ;

	setup();
	//LCD_fillTriangle(20, 40, 180, 120, 30,100, 0xffff) ;
	loop();

	while(1)
    {
	}

	return 0 ;
}

