/*
 * XPT2046.c
 *
 *  Created on: Sep 21, 2025
 *      Author: sap
 */

#include "debug.h"

#include "XPT2046.h"

#define SPI_HW



#define TS_CS_Clr()   GPIO_ResetBits(GPIOB, GPIO_Pin_12)//CS
#define TS_CS_Set()   GPIO_SetBits(GPIOB, GPIO_Pin_12)


//ts_config_t	ts_config ;


int TS_GPIO_init()
{
#if defined(SPI_GPIO)
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TS_SCLK_Set();

#elif defined(SPI_HW)
    GPIO_InitTypeDef GPIO_InitStructure ;
    SPI_InitTypeDef  SPI_InitStructure ;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /*  CS  PB12  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    /*  TS_INTERRUPT */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; //GPIO_Mode_IN_FLOATING;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* SPI_CLK */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* SPI_MISO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* SPI_MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; /* APB1_CLK (72MHz) */
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);

#endif


    TS_CS_Set();

    return 0;
}



void TS_Writ_Bus(u8 dat)
{
#if defined(SPI_GPIO)
    u8 i;
    TS_CS_Clr();
    for(i=0;i<8;i++)
    {
        TS_SCLK_Clr();
        if(dat&0x80)
        {
           TS_MOSI_Set();
        }
        else
        {
           TS_MOSI_Clr();
        }
        TS_SCLK_Set();
        dat<<=1;
    }
  TS_CS_Set();
#elif defined(SPI_HW)

    SPI_I2S_SendData(SPI2,dat);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);


#endif
}


uint16_t TS_Read_Bus()
{
#if defined(SPI_GPIO)
    u8 i;
    TS_CS_Clr();
    for(i=0;i<8;i++)
    {
        TS_SCLK_Clr();
        if(dat&0x80)
        {
           TS_MOSI_Set();
        }
        else
        {
           TS_MOSI_Clr();
        }
        TS_SCLK_Set();
        dat<<=1;
    }
  TS_CS_Set();
#elif defined(SPI_HW)

    uint16_t data = SPI_I2S_ReceiveData(SPI2) ;


#endif

    return data ;
}


void TS2046_SetCalibration(int16_t x0, int16_t x1, int16_t y0,int16_t y1,
		int16_t rx0, int16_t rx1, int16_t ry0,int16_t ry1)
{
	  /*ts_config.RawX0 = x0 ;
	  ts_config.RawX1 = x1 ;
	  ts_config.RawY0 = y0 ;
	  ts_config.RawY1 = y1 ;
	  ts_config.ReperX0 = rx0 ;
	  ts_config.ReperX1 = rx1 ;
	  ts_config.ReperY0 = ry0 ;
	  ts_config.ReperY1 = ry1 ;*/
}


#define	TS_STARTCONV	0x80
#define	TS_CHANNEL_MASK		0x70
#define	TS_MODE8		0x08
#define	TS_MODE12		0x00
#define	TS_SER		0x00
#define	TS_DFR		0x04
#define	TS_PD1		0x02
#define	TS_PD0		0x01


int TS2046_Init()
{


	  //ts_config._width = XPT2046_WIDTH ;
	  //ts_config._height = XPT2046_HEIGHT ;


	  TS_GPIO_init();

	  //TS2046_SetCalibration(168, 1871, 147, 1896,
	  //		0, XPT2046_WIDTH, 0, XPT2046_HEIGHT);

		/*TS_CS_Clr() ;
		Delay_Us(10);
		TS_Writ_Bus(TS_STARTCONV|TS_DFR) ;
		TS_Read_Bus() ;
		TS_Writ_Bus(0) ;
		TS_Read_Bus() ;
		TS_Writ_Bus(0) ;
		TS_Read_Bus() ;
		TS_CS_Set() ;*/


	return 0;
}






int8_t touch_Pressed()
{
	return  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) ;
	/*uint8_t res = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) ;

	if(res == 1)
		return 0 ;
	else
		return 1 ;*/
}


int16_t touch_Ch(uint8_t ch)
{
	int16_t res;

	TS_CS_Clr() ;
	Delay_Us(10);

	TS_Writ_Bus(TS_STARTCONV|TS_DFR|(ch<<4)|0) ;
	TS_Writ_Bus(0) ;
	res = (TS_Read_Bus()&0xff) ;
	TS_Writ_Bus(0) ;
	res |= (TS_Read_Bus()&0xff)<<8 ;

	TS_CS_Set() ;

	return res >>4;
}

int16_t touch_Xs()
{
	int16_t res;

	TS_CS_Clr() ;
	Delay_Us(40);

	TS_Writ_Bus(TS_STARTCONV|TS_DFR|0x50|0) ;
	TS_Writ_Bus(0) ;
	res = (TS_Read_Bus()&0xff) ;
	TS_Writ_Bus(0) ;
	res |= (TS_Read_Bus()&0xff)<<8 ;

	TS_CS_Set() ;

	res >>=4 ;

	/*res -= ts_config.RawX0 ;

	res = (res*(ts_config.ReperX1-ts_config.ReperX0))/(ts_config.RawX1-ts_config.RawX0)+ts_config.ReperX0;*/

	return res;
}





int16_t touch_Ys()
{
	int16_t res;

	TS_CS_Clr() ;
	Delay_Us(40);

	TS_Writ_Bus(TS_STARTCONV|TS_DFR|(0x10)|0) ;
	TS_Writ_Bus(0) ;
	res = (TS_Read_Bus()&0xff) ;
	TS_Writ_Bus(0) ;
	res |= (TS_Read_Bus()&0xff)<<8 ;

	TS_CS_Set() ;

	res >>=4 ;

	/*res -= ts_config.RawY0 ;

	res = (res*(ts_config.ReperY1-ts_config.ReperY0))/(ts_config.RawY1-ts_config.RawY0)+ts_config.ReperY0;*/

	return res;
}

int16_t touch_X()
{
	/*if(ts_config._xyswap == 0)
	{
		if(ts_config._xflip) return  ts_config._width - touch_Xs() ;
		return touch_Xs();
	}
	else
	{
		if(ts_config._xflip) return  ts_config._height - touch_Ys() ;
		return touch_Ys();
	}*/
}


int16_t touch_Y()
{
	/*if(ts_config._xyswap == 0)
	{
		if(ts_config._yflip) return  ts_config._height - touch_Ys() ;
		return touch_Ys();
	}
	else
	{
		if(ts_config._yflip) return  ts_config._width - touch_Xs() ;
		return touch_Xs();
	}*/
}




void TS_setRotation(uint8_t m) {
	/*uint8_t rotation ;
	ts_config._xyswap = 0 ;
	ts_config._xflip = 0 ;
	ts_config._yflip = 0 ;


  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     break;
   case 1:
     ts_config._yflip = 1 ;
     ts_config._xyswap = 1 ;
     break;
  case 2:
    ts_config._yflip = 1 ;
    ts_config._xflip = 1 ;
    break;
   case 3:
     ts_config._xyswap = 1 ;
     ts_config._xflip = 1 ;
     break;
   default : ;
  }*/
}



int16_t touch_Xraw()
{
	int16_t res;

	TS_CS_Clr() ;
	Delay_Us(4);

	TS_Writ_Bus(TS_STARTCONV|TS_DFR|0x50|0) ;
	TS_Writ_Bus(0) ;
	res = (TS_Read_Bus()&0xff) ;
	TS_Writ_Bus(0) ;
	res |= (TS_Read_Bus()&0xff)<<8 ;

	TS_CS_Set() ;

	res >>=4 ;

	return res;
}





int16_t touch_Yraw()
{
	int16_t res;

	TS_CS_Clr() ;
	Delay_Us(4);

	TS_Writ_Bus(TS_STARTCONV|TS_DFR|(0x10)|0) ;
	TS_Writ_Bus(0) ;
	res = (TS_Read_Bus()&0xff) ;
	TS_Writ_Bus(0) ;
	res |= (TS_Read_Bus()&0xff)<<8 ;

	TS_CS_Set() ;

	res >>=4 ;

	return res;
}
