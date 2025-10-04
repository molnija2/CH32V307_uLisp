/***************************************************
  STM32 Support added by Jaret Burkett at OSHlab.com

  This is our library for the Adafruit ILI9488 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "ILI9488.h"
#include "debug.h"

#include <stdint.h>
#include <string.h>
#include <math.h>


#define FONT_6X8

#ifdef FONT_6X8
	//#include "font6x8.h"
	#include "5x5_font.h"
	#define		DEF_CHAR_WIDTH	6
	#define		FONT_Basic	font6x8_basic
	#define		DEF_BasicFontDIR	1
#else
	#include "font8x8_basic.h"
	#define DEF_CHAR_WIDTH	8
	#define FONT_Basic	font8x8_basic
	#define		DEF_BasicFontDIR	0
#endif
//#define LCD_CONTROL_SPI


#define FONT_STYLE_NORMAL		0
#define FONT_STYLE_ITALIC		1
#define FONT_STYLE_BOLD			2
#define FONT_STYLE_BOLD_ITALIC	3

#define DEF_FONTS_MAXNUMBER		50

static int16_t iCurrentX, iCurrentY ;
static u8 iCurrentCharMultSize, iTextBkMode ;
static uint16_t iCurrentColor, iCurrentTextColor ;
static uint16_t iCurrentBkColor ;
static uint16_t _height, _width ;
static uint8_t rotation, iTextWrap ;

static const FONT_INFO	FontBasic = {8,DEF_CHAR_WIDTH,0,DEF_BasicFontDIR,"Basic", (uint8_t *)FONT_Basic} ;
static FONT_INFO	FontInfo[DEF_FONTS_MAXNUMBER] ;

FONT_INFO	*FontCurrent = FontInfo ;

uint8_t  iFontsNumber ;

uint8_t  tabcolor;

static int16_t MinX[ILI9488_TFTHEIGHT],MaxX[ILI9488_TFTHEIGHT] ;
static int iFillMinY, iFillMaxY ;
static u8  iFillAreaDetectMode ;


#define	true	1

uint8_t  hwSPI = 1;





#ifndef LCD_CONTROL_SPI


#define DEF_GPIO_DATA8		(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)

#define LCD_CS_Clr()   GPIO_ResetBits(GPIOC, GPIO_Pin_13)//CS
#define LCD_CS_Set()   GPIO_SetBits(GPIOC, GPIO_Pin_13)

#define LCD_DC_Clr() GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define LCD_DC_Set() GPIO_SetBits(GPIOE, GPIO_Pin_5)

#define LCD_RES_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_4)//RES
#define LCD_RES_Set() GPIO_SetBits(GPIOE,GPIO_Pin_4)

#define LCD_RD_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define LCD_RD_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_6)

#define LCD_WR_Clr()   GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define LCD_WR_Set()   GPIO_SetBits(GPIOB,GPIO_Pin_2)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_8)



GPIO_InitTypeDef GPIO_DATA_InitStructure/*={0}*/;

void lcd_SET_BUS_INPUT(void)
{
	GPIO_DATA_InitStructure.GPIO_Pin = DEF_GPIO_DATA8 ;
    GPIO_DATA_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init( GPIOC, &GPIO_DATA_InitStructure );
}

void lcd_SET_BUS_OUTPUT(void)
{
	GPIO_DATA_InitStructure.GPIO_Pin = DEF_GPIO_DATA8 ;
    GPIO_DATA_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_DATA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_DATA_InitStructure );
}


void lcd_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure/*={0}*/;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);

    // RD  DC  RST
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOE, &GPIO_InitStructure );

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;  // WR
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;  // BACKLIGHT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );


    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;  // CS
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    lcd_SET_BUS_OUTPUT();

    //LCD_RES_Set() ;
    //LCD_DC_Set() ;
    LCD_CS_Set() ;
    LCD_RD_Set() ;
    LCD_WR_Set() ;
}




void LCD_Writ_Bus(uint8_t byte)
{
	//uint16_t data = byte ;
    LCD_CS_Clr() ;
 	GPIO_ResetBits(GPIOC, DEF_GPIO_DATA8);
	GPIO_SetBits(GPIOC, byte);
    LCD_WR_Clr() ;
    LCD_WR_Set() ;
    LCD_CS_Set();
}



void LCD_WR_DATA8(uint8_t byte)
{
    LCD_DC_Set() ;
    LCD_Writ_Bus(byte) ;
}

void LCD_WR_REG(uint16_t byte)
{
    LCD_DC_Clr() ;
    LCD_Writ_Bus(byte) ;
    LCD_DC_Set() ;
}


#else

#define LCD_CS_Clr()   GPIO_ResetBits(GPIOC, GPIO_Pin_13)//CS
#define LCD_CS_Set()   GPIO_SetBits(GPIOC, GPIO_Pin_13)


#define LCD_RES_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_4)//RES
#define LCD_RES_Set() GPIO_SetBits(GPIOE,GPIO_Pin_4)


#define LCD_DC_Clr()   GPIO_ResetBits(GPIOE,GPIO_Pin_5)
#define LCD_DC_Set()   GPIO_SetBits(GPIOE,GPIO_Pin_5)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_8)


#define SPI_HW



int lcd_GPIO_init()
{
#if defined(SPI_GPIO)
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    LCD_SCLK_Set();

#elif defined(SPI_HW)
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    /*  CS  PC13  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    /*  RESET  PE4   DC (PE5) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
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

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; /* APB1_CLK (72MHz) */
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);
#endif


    LCD_CS_Set();

    return 0;
}



void LCD_Writ_Bus(u8 dat)
{
#if defined(SPI_GPIO)
    u8 i;
    LCD_CS_Clr();
    for(i=0;i<8;i++)
    {
        LCD_SCLK_Clr();
        if(dat&0x80)
        {
           LCD_MOSI_Set();
        }
        else
        {
           LCD_MOSI_Clr();
        }
        LCD_SCLK_Set();
        dat<<=1;
    }
  LCD_CS_Set();
#elif defined(SPI_HW)
    LCD_CS_Clr();
    //Delay_Us(1) ;
    SPI_I2S_SendData(SPI2,dat);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    Delay_Us(1);
    LCD_CS_Set();
    //Delay_Us(1);
#endif
}



void LCD_SendDataForFast(uint16_t color)
{
	 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	            SPI_I2S_SendData(SPI2,color >> 8);
	 //Delay_Us(1);
	 while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	            SPI_I2S_SendData(SPI2,color);
	 //Delay_Us(1);
}


void LCD_Send_Start()
{
    Delay_Us(1);
	LCD_CS_Clr();

}

void LCD_Send_Stop()
{
    Delay_Us(1);
    LCD_CS_Set();
}



uint16_t  LCD_Read_Bus(uint8_t dat)
{
	LCD_Writ_Bus(dat);

	return SPI_I2S_ReceiveData(SPI2) ;
}

#define lcd_SendData(a)		LCD_WR_DATA8(a)

void LCD_WR_DATA8(u8 dat)
{
	LCD_DC_Set();
    LCD_Writ_Bus(dat);
}

void LCD_WR_DATA8_DC_CS(u8 dat)
{
    LCD_DC_Set();
    LCD_CS_Clr();
    LCD_Writ_Bus(dat);
    LCD_CS_Set();
}

void LCD_WR_DATA(u16 dat)
{
    LCD_Writ_Bus(dat>>8);
    LCD_Writ_Bus(dat);
}

void LCD_WR_DATA24(u32 dat)
{
	dat>>=8 ;
    LCD_Writ_Bus(dat>>8);
    LCD_Writ_Bus(dat>>8);
    LCD_Writ_Bus(dat);
}

#define lcd_SendData24(a)		LCD_WR_DATA24(a)

#define lcd_SendCommand(a)	LCD_WR_REG(a)

void LCD_WR_REG(u8 dat)
{
    LCD_DC_Clr();
    LCD_Writ_Bus(dat);
    LCD_DC_Set();
}


#endif




void lcd_SendDataN(const uint8_t *data, int N)
{
	int i ;

	LCD_CS_Clr();
	LCD_DC_Set();
	for(i=0;i<N;i++) LCD_Writ_Bus(*data++);
	LCD_DC_Clr();
	LCD_CS_Set();
}




uint16_t  LCD_Read_Bus(uint8_t dat)
{
	return GPIO_ReadInputData(GPIOC) & 0xff;
}





void spi_begin() {} ;
void spi_end() {} ;

#define boolean		uint8_t
#define delay(a)	Delay_Ms(a)
//#define LCD_WR_REG(c)	LCD_WR_REG(c)
//#define LCD_WR_DATA8(c)	LCD_WR_DATA8(c)



#define SPI.transfer(a)	LCD_Read_Bus(a)

// Rather than a bazillion LCD_WR_REG() and LCD_WR_DATA8() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80


/*uint8_t mySPCR;
  volatile uint32_t *mosiport, *clkport, *dcport, *rsport, *csport;
  int32_t  _cs, _dc, _DCt, _mosi, _miso, _sclk;
  uint32_t  mosipinmask, clkpinmask, cspinmask, dcpinmask;
*/

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
/*void ILI9488_commandList(uint8_t *addr) {

  uint8_t  numCommands, numArgs;
  uint16_t ms;

  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while(numCommands--) {                 // For each command...
    LCD_WR_REG(pgm_read_byte(addr++)); //   Read, issue command
    numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--) {                   //   For each argument...
      LCD_WR_DATA8(pgm_read_byte(addr++));  //     Read, issue argument
    }

    if(ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      delay(ms);
    }
  }
}*/


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04


const uint8_t  gammaP[] ={	0x00,0x03,0x09,0x08,0x16,0x0A,
		0x3F,0x78,0x4C,0x09,0x0A,0x08,0x16,0x1A,0x0F
};

const uint8_t  gammaN[] ={	0x00,0x16,0x19,0x03,0x0F,0x05,
		0x32,0x45,0x46,0x04,0x0E,0x0D,0x35,0x37,0x0F
};



#define ST_CMD_DELAY 0x80 // special signifier for command lists


void CommonInit(const uint8_t *cmdlist)
{
	int i ;
	int N, Nargs ;

	N = *cmdlist ++ ;

	for(i=0; i<N; i++)
	{
		LCD_WR_REG(*cmdlist++ ) ;

		Nargs = *cmdlist++ ;
		if(Nargs >= ST_CMD_DELAY)
		{
			Nargs -= ST_CMD_DELAY ;
			lcd_SendDataN(cmdlist, Nargs ) ;
			cmdlist += Nargs ;
			Delay_Ms(*cmdlist++ );
		}
		else
		{
			lcd_SendDataN(cmdlist, Nargs ) ;
			cmdlist += Nargs ;
		}
	}
}


const uint8_t  Start_commands[] ={
		12,		//  Commands quantity

		0xC0, 2,    //Power Control 1
		0x17,    	//Vreg1out
		0x15,  		//Verg2out

		0xC1, 1,    //Power Control 2
		0x41,		//VGH,VGL

		0xC5, 3,    //Power Control 3
		0x00,
		0x12,    	//Vcom
		0x80,

	    ILI9488_SLPOUT, ST_CMD_DELAY,    //Exit Sleep
		120,

		0x3A, 1,		// Interface Pixel Format
		0x66,			//24 bit

		0XB0, 1,		// Interface Mode Control
		0x80,			//SDO NOT USE

		0xB1, 1,		//Frame rate
		0xA0,			//60Hz

		0xB4, 1,		//Display Inversion Control
		0x02,			//2-dot

		0xB6, 2,		//Display Function Control  RGB/MCU Interface Control
		0x02,			//MCU
		0x02,			//Source,Gate scan dieection

		0xE9, 1,		// Set Image Functio
		0x00,			// Disable 24 bit data

		0xF7, 4,		// Adjust Control
		0xA9,
		0x51,
		0x2C,
		0x82,			// D7 stream, loose

		ILI9488_DISPON, 0    //Display on
};


void ILI9488_begin (void)
{
	lcd_GPIO_init();

	LCD_RES_Clr();



    LCD_CS_Set();
    LCD_DC_Set();

    // toggle RST low to reset
    LCD_RES_Set();
    delay(1);
    LCD_RES_Clr();
    delay(2);
    LCD_RES_Set();
    delay(15);


    LCD_WR_REG(0xE0);
    lcd_SendDataN(gammaP,15);

	LCD_WR_REG(0XE1);
	lcd_SendDataN(gammaN,15);

	CommonInit(Start_commands) ;



  LCD_BLK_Set();

  ILI9488_setRotation(2) ;


  iCurrentX = 0 ;
  iCurrentY = 0 ;
  iCurrentCharMultSize = 1 ;
  iCurrentColor = ILI9488_WHITE ;
  iCurrentTextColor = ILI9488_WHITE ;
  iTextBkMode = 0 ;

  iTextWrap = 1 ;

  int i ;
  for(i=0; i<DEF_FONTS_MAXNUMBER; i++)  FontInfo[i].DATA = 0 ;
  FontInfo[0] = FontBasic ;
  iFontsNumber = 1 ;
  FontCurrent = &FontInfo[0] ;
  //if(LoadFont("/FONTS/NIN22H14.BIN", 1))
  //{
	  //SetFont(1) ;
  //}
  iFillAreaDetectMode = 0 ;

  ILI9488_fillScreen(ILI9488_BLUE) ;
  //iCurrentBkColor = ILI9488_BLACK ;
  //LCDTextXY(10,10,"Hello World !", ILI9488_GREEN, 1) ;


  //ILI9488_fillRect(10, 10,  300, 300, ILI9488_BLACK);

  //void LCD_Fill_Fast(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
  //LCD_Fill_Fast(10, 10,  300, 300, ILI9488_BLACK);

  //ILI9488_drawPixel(100, 100, ILI9488_WHITE);
}




void ILI9488_setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea){

  if (hwSPI) spi_begin();
  LCD_WR_REG(0x33); // Vertical scroll definition
  LCD_WR_DATA8(topFixedArea >> 8);
  LCD_WR_DATA8(topFixedArea);
  LCD_WR_DATA8((_height - topFixedArea - bottomFixedArea) >> 8);
  LCD_WR_DATA8(_height - topFixedArea - bottomFixedArea);
  LCD_WR_DATA8(bottomFixedArea >> 8);
  LCD_WR_DATA8(bottomFixedArea);
  if (hwSPI) spi_end();
}
void ILI9488_scroll(uint16_t pixels){
  if (hwSPI) spi_begin();
  LCD_WR_REG(0x37); // Vertical scrolling start address
  LCD_WR_DATA8(pixels >> 8);
  LCD_WR_DATA8(pixels);
  if (hwSPI) spi_end();
}

void ILI9488_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
 uint16_t y1)
{

	uint8_t data[4] = { (uint8_t)(x0>>8), (uint8_t)(x0&0xff), (uint8_t)(x1>>8), (uint8_t)(x1&0xff) };
	uint8_t data2[4] = { (uint8_t)(y0>>8), (uint8_t)(y0&0xff), (uint8_t)(y1>>8), (uint8_t)(y1&0xff) };

	//y0 = 10 ;
  LCD_WR_REG(ILI9488_CASET); // Column addr set
  lcd_SendDataN((uint8_t*)data, 4);

  LCD_WR_REG(ILI9488_PASET); // Row addr set
  lcd_SendDataN((uint8_t*)data2, 4);


  LCD_WR_REG(ILI9488_RAMWR); // write to RAM
}

/*void ILI9488_drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h){

    // rudimentary clipping (drawChar w/big text requires this)
    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width  - x;
    if((y + h - 1) >= _height) h = _height - y;

    if (hwSPI) spi_begin();
    ILI9488_setAddrWindow(x, y, x+w-1, y+h-1);

    // uint8_t hi = color >> 8, lo = color;

  #if defined(USE_FAST_PINIO) && !defined (_VARIANT_ARDUINO_STM32_)
    *dcport |=  dcpinmask;
    *csport &= ~cspinmask;
  #else
    LCD_DC_Set();
    LCD_CS_Clr();
  #endif
  uint8_t linebuff[w*3+1];
  uint16_t pixels = w*h;
  // uint16_t count = 0;
  uint32_t count = 0;
  for (uint16_t i = 0; i < h; i++) {
    uint16_t pixcount = 0;
    for (uint16_t o = 0; o <  w; o++) {
      uint8_t b1 = img[count];
      count++;
      uint8_t b2 = img[count];
      count++;
      uint16_t color = b1 << 8 | b2;
      linebuff[pixcount] = (((color & 0xF800) >> 11)* 255) / 31;
      pixcount++;
      linebuff[pixcount] = (((color & 0x07E0) >> 5) * 255) / 63;
      pixcount++;
      linebuff[pixcount] = ((color & 0x001F)* 255) / 31;
      pixcount++;
    } // for row
    #if defined (__STM32F1__)
      SPI.dmaSend(linebuff, w*3);
    #else
      for(uint16_t b = 0; b < w*3; b++){
        LCD_Writ_Bus(linebuff[b]);
      }
    #endif

  }// for col
  #if defined(USE_FAST_PINIO) && !defined (_VARIANT_ARDUINO_STM32_)
    *csport |= cspinmask;
  #else
    LCD_CS_Set();
  #endif

    if (hwSPI) spi_end();
}
*/

void pushColor(uint16_t color) {
	LCD_DC_Set();
	LCD_CS_Clr();

  write16BitColor(color);
  LCD_CS_Set();
}

void pushColors(uint16_t *data, uint8_t len, boolean first) {
  uint16_t color;
  uint8_t  buff[len*3+1];
  uint16_t count = 0;
  uint8_t lencount = len;

    LCD_CS_Clr();

  if(first == true) { // Issue GRAM write command only on first call
      LCD_DC_Set();
  }
  while(lencount--) {
    color = *data++;
    buff[count] = (((color & 0xF800) >> 11)* 255) / 31;
    count++;
    buff[count] = (((color & 0x07E0) >> 5) * 255) / 63;
    count++;
    buff[count] = ((color & 0x001F)* 255) / 31;
    count++;
  }

    for(uint16_t b = 0; b < len*3; b++){
      LCD_Writ_Bus(buff[b]);
    }
}

void write16BitColor(uint16_t color)
{
	uint8_t r = (color & 0xF800) >> 8;
	uint8_t g = (color & 0x07E0) >> 3;
	uint8_t b = (color & 0x001F) << 3;

  LCD_Writ_Bus(r);
  LCD_Writ_Bus(g);
  LCD_Writ_Bus(b);
  // #endif
}

void ILI9488_drawPixel (int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  if(iFillAreaDetectMode)
	  TestXYAreaPoint(x,y) ;

  if (hwSPI) spi_begin();
  ILI9488_setAddrWindow(x,y,x+1,y+1);

#if defined(USE_FAST_PINIO) && !defined (_VARIANT_ARDUINO_STM32_)
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#else
  LCD_DC_Set();
  LCD_CS_Clr();
#endif

  // added for 24 bit
  write16BitColor(color);

#if defined(USE_FAST_PINIO) && !defined (_VARIANT_ARDUINO_STM32_)
  *csport |= cspinmask;
#else
  LCD_CS_Set();
#endif

  if (hwSPI) spi_end();
}


void ILI9488_drawFastVLine(int16_t x, int16_t y, int16_t h,
 uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;

  if((y+h-1) >= _height)
    h = _height-y;

  if (hwSPI) spi_begin();

  if(iFillAreaDetectMode)
  {
		for(int y2 = y; y2<y+h-1;y2++) TestXYAreaPoint( x, y2);
  }

  ILI9488_setAddrWindow(x, y, x, y+h-1);

//  uint8_t hi = color >> 8, lo = color;

#if defined(USE_FAST_PINIO) && !defined (_VARIANT_ARDUINO_STM32_)
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#else
  LCD_DC_Set();
  LCD_CS_Clr();
#endif

  while (h--) {
    // added for 24 bit
    write16BitColor(color);

  }

#if defined(USE_FAST_PINIO) && !defined (_VARIANT_ARDUINO_STM32_)
  *csport |= cspinmask;
#else
  LCD_CS_Set();
#endif

  if (hwSPI) spi_end();
}


void ILI9488_drawFastHLine(int16_t x, int16_t y, int16_t w,
  uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  if (hwSPI) spi_begin();

  if(iFillAreaDetectMode)
  {
	  TestXYAreaPoint( x, y);
	  TestXYAreaPoint( x+w-1, y);
		/*if(MinX[y]>x) MinX[y] = x ;
		int x2 =(x+w-1);
		if(MaxX[y]<x2) MaxX[y] = x2 ;*/
  }

  ILI9488_setAddrWindow(x, y, x+w-1, y);

  // uint8_t hi = color >> 8, lo = color;
#if defined(USE_FAST_PINIO) && !defined (_VARIANT_ARDUINO_STM32_)
  *dcport |=  dcpinmask;
  *csport &= ~cspinmask;
#else
  LCD_DC_Set();
  LCD_CS_Clr();
#endif
  while (w--) {
    // added for 24 bit
    write16BitColor(color);
  }
#if defined(USE_FAST_PINIO) && !defined (_VARIANT_ARDUINO_STM32_)
  *csport |= cspinmask;
#else
  LCD_CS_Set();
#endif
  if (hwSPI) spi_end();
}

void ILI9488_fillScreen(uint16_t color) {
	ILI9488_fillRect(0, 0,  _width, _height, color);
	iCurrentBkColor = color ;
}



void LCD_Fill_Fast(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{
    u16 i,j;

    if((xsta >= _width) || (ysta >= _height)) return;
    if((xend <=0) || (yend <= 0)) return;

    ILI9488_setAddrWindow(xsta,ysta,xend-1,yend-1);

    LCD_DC_Set();
    LCD_CS_Clr();
#ifdef LCD_CONTROL_SPI
    LCD_Send_Start() ;
#endif

    for(i=ysta;i<yend;i++)
     for(j=xsta;j<xend;j++)
    {
    		write16BitColor(color);
    }

#ifdef LCD_CONTROL_SPI
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
#endif

    LCD_CS_Set();

#ifdef LCD_CONTROL_SPI
    LCD_Send_Stop() ;
#endif
}


void ILI9488_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {

	LCD_Fill_Fast(x, y, x+w, y+h, color);

}



// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t ILI9488_color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}



void ILI9488_setRotation(uint8_t m) {

  if (hwSPI) spi_begin();
  LCD_WR_REG(ILI9488_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     LCD_WR_DATA8(MADCTL_MX | MADCTL_BGR);
     _width  = ILI9488_TFTWIDTH;
     _height = ILI9488_TFTHEIGHT;
     break;
   case 1:
     LCD_WR_DATA8(MADCTL_MV | MADCTL_BGR);
     _width  = ILI9488_TFTHEIGHT;
     _height = ILI9488_TFTWIDTH;
     break;
  case 2:
    LCD_WR_DATA8(MADCTL_MY | MADCTL_BGR);
     _width  = ILI9488_TFTWIDTH;
     _height = ILI9488_TFTHEIGHT;
    break;
   case 3:
     LCD_WR_DATA8(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     _width  = ILI9488_TFTHEIGHT;
     _height = ILI9488_TFTWIDTH;
     break;
  }
  if (hwSPI) spi_end();
}


void ILI9488_invertDisplay(boolean i) {
  if (hwSPI) spi_begin();
  LCD_WR_REG(i ? ILI9488_INVON : ILI9488_INVOFF);
  if (hwSPI) spi_end();
}


////////// stuff not actively being used, but kept for posterity


/*uint8_t spiread(void) {
  uint8_t r = 0;

  if (hwSPI) {
    r = LCD_Read_Bus(0x00);
  }
  //Serial.print("read: 0x"); Serial.print(r, HEX);

  return r;
}*/

 uint8_t ILI9488_readdata(void) {
   LCD_DC_Set();
   LCD_CS_Clr();
   //uint8_t r = spiread();
   uint8_t r = LCD_Read_Bus(0x00);
   LCD_CS_Set();

   return r;
}


uint8_t ILI9488_readcommand8(uint8_t c, uint8_t index) {
   if (hwSPI) spi_begin();
   LCD_DC_Clr(); // command
   LCD_CS_Clr();
   LCD_Writ_Bus(0xD9);  // woo sekret command?
   LCD_DC_Set(); // data
   LCD_Writ_Bus(0x10 + index);
   LCD_CS_Set();

   LCD_DC_Clr();

   LCD_CS_Clr();
   LCD_Writ_Bus(c);

   LCD_DC_Set();
   //uint8_t r = spiread();
   uint8_t r = LCD_Read_Bus(0) ;
   LCD_CS_Set();
   if (hwSPI) spi_end();
   return r;
}



/*

 uint16_t ILI9488::readcommand16(uint8_t c) {
 LCD_DC_Clr();
 if (_cs)
 LCD_CS_Clr();

 LCD_Writ_Bus(c);
 pinMode(_sid, INPUT); // input!
 uint16_t r = spiread();
 r <<= 8;
 r |= spiread();
 if (_cs)
 LCD_CS_Set();

 pinMode(_sid, OUTPUT); // back to output
 return r;
 }

 uint32_t ILI9488::readcommand32(uint8_t c) {
 LCD_DC_Clr();
 if (_cs)
 LCD_CS_Clr();
 LCD_Writ_Bus(c);
 pinMode(_sid, INPUT); // input!

 dummyclock();
 dummyclock();

 uint32_t r = spiread();
 r <<= 8;
 r |= spiread();
 r <<= 8;
 r |= spiread();
 r <<= 8;
 r |= spiread();
 if (_cs)
 LCD_CS_Set();

 pinMode(_sid, OUTPUT); // back to output
 return r;
 }

 */

void LCD_gotoxy(int x, int y)
{
	iCurrentX = x ;
	iCurrentY = y ;
}

void LCD_settextmultsize(uint16_t size)
{
	iCurrentCharMultSize = size ;
}

void LCD_setcolor(uint16_t color)
{
	iCurrentColor = color ;
}

void LCD_settextcolor(uint16_t color)
{
	iCurrentTextColor = color ;
}

void LCD_settextBkcolor(uint16_t color)
{
	iCurrentBkColor = color ;
}

void LCD_settextBkMode(uint8_t  mode)
{
	iTextBkMode = mode ;
}


void LCD_drawChar (u8 asciiCode)
{
    u8 fontSize = iCurrentCharMultSize ;
    uint16_t color = iCurrentTextColor ;

    u8 charH, charW ;

    charH = iCurrentCharMultSize*FontCurrent->Height ;
    charW = iCurrentCharMultSize*FontCurrent->Width ;

    if(iTextWrap)
    	if((iCurrentX+charW)>=_width)
    {
    	iCurrentX = 0 ;
    	iCurrentY += charH + /*(charH>>2)*/ 2 ;
    	if((iCurrentY+charH)>=_height)
    	{
    		iCurrentY = 0 ;
    	}
    	LCD_Fill_Fast(iCurrentX, iCurrentY, _width, iCurrentY+charH+2, iCurrentBkColor) ;
    }

    if(asciiCode=='\n')
    {
    	iCurrentX = 0 ;
    	iCurrentY += charH + /*(charH>>2)*/ 2 ;
    	if((iCurrentY+charH)>=_height)
    	{
    		iCurrentY = 0 ;
    		//ILI9488_fillScreen(iCurrentBkColor) ;
    	}
    	LCD_Fill_Fast(iCurrentX, iCurrentY, _width, iCurrentY+charH+2, iCurrentBkColor) ;
    	return ;
    }

    //if(iTextBkMode)
    //if((FontCurrent == FontInfo ) && (FontCurrent->Dir == 1))
    //	LCD_Fill_Fast(iCurrentX, iCurrentY, iCurrentX+charW, iCurrentY+charH, iCurrentBkColor);

    ILI9488_setAddrWindow(iCurrentX, iCurrentY, iCurrentX+charW-1, iCurrentY+charH-1);

    if(FontCurrent == FontInfo ) asciiCode -=0x20 ;

     if(FontCurrent->Dir == 1)
      for (int  yI=0; yI<FontCurrent->Height; yI++) {
          unsigned int fontData ;
          unsigned int ibit ;

    	  if(FontCurrent == FontInfo) ibit = 1 << yI ;
    	  else  ibit = 0x8000 >> yI ;

          for (int  xI=0; xI<FontCurrent->Width; xI++){
    		  uint16_t x = iCurrentX+(xI*fontSize) ;
    		  uint16_t y = iCurrentY+(yI*fontSize) ;
              if(FontCurrent == FontInfo)
           	    fontData = *(FontCurrent->DATA+asciiCode*FontCurrent->Width+xI);
              else
                fontData = ((uint16_t*)(FontCurrent->DATA))[asciiCode*FontCurrent->Width+xI];

        	  if ((fontData&ibit)!=0)
        	  {
        		  if(iCurrentCharMultSize>1) LCD_Fill_Fast(x, y, x+fontSize, y+fontSize, color);
        		  else	write16BitColor(color);
        	  }
        	  else
        	  {
        		  if(iCurrentCharMultSize>1) LCD_Fill_Fast(x, y, x+fontSize, y+fontSize, iCurrentBkColor);
        		  else	write16BitColor(iCurrentBkColor);
        	  }
          }
        }
    else
     for (int  yI=0; yI<FontCurrent->Height; yI++) {
      unsigned int fontData ;
      int ibit = 0x01 ;
      if(FontCurrent == FontInfo)
       	fontData = *(FontCurrent->DATA+asciiCode*FontCurrent->Height+yI);
      else
      {
         	fontData = ((uint16_t*)(FontCurrent->DATA))[asciiCode*FontCurrent->Height+yI];
         	ibit = 0x8000;
      }
      for (int  xI=0; xI<FontCurrent->Width; xI++){
		  uint16_t x = iCurrentX+(xI*fontSize) ;
		  uint16_t y = iCurrentY+(yI*fontSize) ;
    	  if (fontData&ibit)
    	  {
    		  if(iCurrentCharMultSize>1) LCD_Fill_Fast(x, y, x+fontSize, y+fontSize, color);
    		  else	write16BitColor(color);
    	  }
    	  else
    	  {
    		  if(iCurrentCharMultSize>1) LCD_Fill_Fast(x, y, x+fontSize, y+fontSize, iCurrentBkColor);
    		  else	write16BitColor(iCurrentBkColor);
    	  }

    	  if(FontCurrent == FontInfo) ibit<<=1 ;
    	  else  ibit>>=1 ;
      }
    }


    iCurrentX += charW ;
}




void LCDTextXY(int textX, int textY, char *myString, int color, int fontSize)
{
	iCurrentX = textX ;
	iCurrentY = textY ;
	iCurrentTextColor = color ;
	iCurrentCharMultSize = fontSize ;

    for (int strI=0; strI<strlen(myString); strI++)
    {
    	LCD_drawChar(myString[strI]);
    }
}


void LCD_DrawLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    //https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    int16_t dx = abs(x1 - x0);
    int32_t SX=0;
    if (x0<x1) {
        SX=1;
    }
    else {
        SX=-1;
    }
    int dy = -abs(y1 - y0);
    int SY;
    if (y0<y1) {
        SY=1;
    }
    else {
        SY=-1;
    }
    int error = dx + dy;

    for (;;){
    	ILI9488_drawPixel(x0, y0, color);

        if ((x0==x1)&&(y0==y1)) break;
        int e2 = 2 * error;
        if (e2 >= dy) {
            if (x0 == x1) break;
            error = error + dy;
            x0 = x0 + SX;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            error = error + dx;
            y0 = y0 + SY;
        }
    }
}


void LCD_setTextWrap(int16_t mode)
{
	iTextWrap = mode ;
}


// draw a rectangle
void ILI9488_drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {

	ILI9488_drawFastHLine(x, y, w, color) ;
	ILI9488_drawFastHLine(x, y+h, w, color) ;
	ILI9488_drawFastVLine(x, y, h, color) ;
	ILI9488_drawFastVLine(x+w, y, h, color) ;
}



int LCD_drawCircle(int center_x, int center_y, int radius, int color)
{
    int result = 0 ;

    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    ILI9488_drawPixel(center_x  , center_y+radius, color);
    ILI9488_drawPixel(center_x  , center_y-radius, color);
    ILI9488_drawPixel(center_x+radius, center_y, color );
    ILI9488_drawPixel(center_x-radius, center_y, color );

    while (x<y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9488_drawPixel(center_x + x, center_y + y, color );
        ILI9488_drawPixel(center_x - x, center_y + y, color );
        ILI9488_drawPixel(center_x + x, center_y - y, color );
        ILI9488_drawPixel(center_x - x, center_y - y, color );
        ILI9488_drawPixel(center_x + y, center_y + x, color );
        ILI9488_drawPixel(center_x - y, center_y + x, color );
        ILI9488_drawPixel(center_x + y, center_y - x, color );
        ILI9488_drawPixel(center_x - y, center_y - x, color );
    }
    return result;
}


int LCD_fillCircle(int center_x, int center_y, int radius, int color) {
    int y, l, rsin = radius , r2 = radius * radius ;

    for(y=center_y-radius; y<= center_y+radius;y++)
    {
    	l = sqrt(r2 - rsin*rsin) -0.25;
    	if(l>0)
         LCD_Fill_Fast(center_x-l, y, center_x+l+1, y+1, color);
    	rsin -- ;
    }
    return 0;
}


int LCD_drawCircle2(int center_x, int center_y, int radius, int color) {
    int y, l, rsin = radius , r2 = radius * radius ;

    for(y=center_y-radius; y<= center_y+radius;y++)
    {
    	l = sqrt(r2 - rsin*rsin) -0.25;
    	//LCDDrawLine(center_x-l, y, center_x+l, y, color);
    	if(l>0)
    	{
         //LCD_Fill_Fast(center_x-l, y, center_x+l+1, y+1, color);
    		ILI9488_drawPixel(center_x-l, y, color);
    		ILI9488_drawPixel(center_x+l, y, color);
    	}
    	rsin -- ;
    }
    return 0;
}






int LCD_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius,
  uint16_t color) {

	ILI9488_drawFastHLine(x+radius, y, w-(radius<<1)+1, color) ;
	ILI9488_drawFastHLine(x+radius, y+h-1, w-(radius<<1)+1, color) ;
	ILI9488_drawFastVLine(x, y+radius, h-(radius<<1)+1, color) ;
	ILI9488_drawFastVLine(x+w-1, y+radius, h-(radius<<1)+1, color) ;

    int result = 0 ;

    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int center_x1 = x+radius ;
    int center_x2 = x+w-radius-1 ;
    int center_y1 = y+radius ;
    int center_y2 = y+h-radius-1 ;

    x = 0;
    y = radius;

    /*ILI9488_drawPixel(center_x2  , center_y2+radius, color);
    ILI9488_drawPixel(center_x1  , center_y1-radius, color);
    ILI9488_drawPixel(center_x2+radius, center_y1, color );
    ILI9488_drawPixel(center_x1-radius, center_y2, color );*/

    while (x<y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        //color = ILI9488_GREEN ;

        ILI9488_drawPixel(center_x2 + x, center_y2 + y, color );
        ILI9488_drawPixel(center_x1 - x, center_y2 + y, color );
        ILI9488_drawPixel(center_x2 + x, center_y1 - y, color );
        ILI9488_drawPixel(center_x1 - x, center_y1 - y, color );

        //color = ILI9488_YELLOW ;
        ILI9488_drawPixel(center_x2 + y, center_y2 + x, color );
        ILI9488_drawPixel(center_x1 - y, center_y2 + x, color );
        ILI9488_drawPixel(center_x2 + y, center_y1 - x, color );
        ILI9488_drawPixel(center_x1 - y, center_y1 - x, color );
    }
    return result;
}



void FillMarkedArea(uint16_t color)
{
	for(int y=iFillMinY;y<=iFillMaxY;y++)
		if((y<_height)&&(y>=0))
	{
	       LCD_Fill_Fast(MinX[y], y, MaxX[y], y+1, color);
	}
}

void LCD_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius,
  uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  ClearFillYAreaBorders();
  TestYArea(y);
  TestYArea(y+h-1);
  ClearFillXAreaBorders();

  iFillAreaDetectMode = 1 ;

  LCD_drawRoundRect( x,  y,  w,  h,  radius, color);

  iFillAreaDetectMode = 0 ;

  FillMarkedArea(color) ;

}


// draw a TRIangle
void LCD_drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		int16_t x3, int16_t y3,  uint16_t color) {

  LCD_DrawLine( x1, y1, x2, y2, color) ;
  LCD_DrawLine( x1, y1, x3, y3, color) ;
  LCD_DrawLine( x2, y2, x3, y3, color) ;
}







void LCD_DetectLineArea (int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    //https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    int16_t dx = abs(x1 - x0);
    int32_t SX=0;
    if (x0<x1) {
        SX=1;
    }
    else {
        SX=-1;
    }
    int dy = -abs(y1 - y0);
    int SY;
    if (y0<y1) {
        SY=1;
    }
    else {
        SY=-1;
    }
    int error = dx + dy;

    for (;;){
    	//ILI9488_drawPixel(x0, y0, color);
    	if((y0<_height)&&(y0>=0))
    	{
    		if(MinX[y0]>x0) MinX[y0] = x0 ;
    		if(MaxX[y0]<x0) MaxX[y0] = x0 ;
    	}

        if ((x0==x1)&&(y0==y1)) break;
        int e2 = 2 * error;
        if (e2 >= dy) {
            if (x0 == x1) break;
            error = error + dy;
            x0 = x0 + SX;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            error = error + dx;
            y0 = y0 + SY;
        }
    }
}







void ClearFillYAreaBorders()
{
	iFillMaxY = -30000, iFillMinY = 30000 ;
}


void ClearFillXAreaBorders()
{
	int y, y0 = iFillMinY, y1 = iFillMaxY ;

	if(y0>y1)
	{
		y0 = 0 ;  y1 = _height-1 ;
	}
	//for(y=iFillMinY;y<=iFillMaxY;y++)
	//	if((y<_height)&&(y>0))
	for(y=y0;y<=y1;y++)
	{
			MinX[y] = 30000 ;
			MaxX[y] = -30000 ;
	}
}


void TestYArea(int16_t y)
{
	if(iFillMaxY<y) iFillMaxY = y ;
	if(iFillMinY>y) iFillMinY = y ;
}

void TestXYAreaPoint(int16_t x, int16_t y)
{
	if((y<_height)&&(y>=0))
	{
		if(MinX[y]>x) MinX[y] = x ;
		if(MaxX[y]<x) MaxX[y] = x ;
		TestYArea(y);
	}
}

void LCD_fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		int16_t x0, int16_t y0,  uint16_t color) {

	ClearFillYAreaBorders() ;

	TestYArea(y1) ;
	TestYArea(y2) ;
	TestYArea(y0) ;
	ClearFillXAreaBorders() ;


	iFillAreaDetectMode = 1 ;


	LCD_drawTriangle(x1, y1, x2, y2, x0, y0, color) ;

	iFillAreaDetectMode = 0 ;

	FillMarkedArea(color) ;
}



void LCD_fillPolyInit()
{
	ClearFillYAreaBorders() ;
	ClearFillXAreaBorders() ;

	iFillAreaDetectMode = 1 ;
}

void LCD_fillPoly(int color)
{
	iFillAreaDetectMode = 0 ;

	FillMarkedArea(color) ;
}



#include "ff.h"
#define BUFFERSIZE 512  // Number of bits+4



int LoadFont (char *name, uint8_t ifont)
{
	FONT_INFO	*Font = &FontInfo[ifont] ;
	  FIL File;
	  FATFS fatfs;
	  UINT uint_result;
	  FRESULT fres;
	  uint8_t res = 0 ;

	  fres = f_mount(&fatfs, "", 1);
	  if(fres != FR_OK) return -1 ;

	  delay(50);


	  fres = f_open(&File, name,  FA_OPEN_EXISTING | FA_READ);
	  if(fres != FR_OK) return -1 ;


	  f_read(&File, Font, sizeof(FONT_INFO), &uint_result);

	  //printf("Load Font '%s' \n", Font->name) ;
	  int size = Font->Height*256 * sizeof(uint16_t);

	  if(Font->DATA!=0)
	  {
		  free(Font->DATA) ;
		  iFontsNumber -- ;
	  }

	  Font->DATA = 0 ;
	  Font->DATA = (uint8_t*)(malloc(size)) ;

	  if(Font->DATA)
	  {
		  res = 1 ;
		  f_read(&File, (uint8_t*)(Font->DATA), size, &uint_result);
	  }

	  fres = f_close(&File);
	  //if(fres != FR_OK) return -1 ;

	  fres =  f_unmount("");
	  //if(fres != FR_OK) return -1 ;

	  iFontsNumber ++ ;
	  return res ;
}


int SetFont (uint8_t ifont)
{
	if(FontInfo[ifont].DATA == 0 ) return 0 ;

	FontCurrent = &FontInfo[ifont] ;

	return 1;
}
