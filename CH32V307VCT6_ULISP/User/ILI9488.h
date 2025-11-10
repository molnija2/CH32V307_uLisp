/***************************************************
  STM32 Support added by Jaret Burkett at OSHlab.com

  This is our library for the Adafruit  ILI9488 Breakout and Shield
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

#ifndef _ILI9488H_
#define _ILI9488H_

#include <inttypes.h>

//#define LCD_ILI9488H_TYPE
#define LCD_NT35510_TYPE


#ifdef	LCD_ILI9488H_TYPE
	#define LCD_TFTWIDTH  320
	#define LCD_TFTHEIGHT 480
#else
	#ifdef	LCD_NT35510_TYPE

		#define LCD_TFTWIDTH	480
		#define LCD_TFTHEIGHT	800


		#define NT35510_NOP        0x0000   ///< No-op
		#define NT35510_SWRESET    0x0100   ///< Software reset
		#define NT35510_RDDID      0x0400   ///< Read display ID (0x0400 - 0x0402)
		#define NT35510_RDNUMED    0x0500   ///< Read number of errors (DSI only)
		#define NT35510_RDDPM      0x0A00   ///< Read Display Power Mode
		#define NT35510_RDDMADCTL  0x0B00   ///< Read Display MADCTL
		#define NT35510_RDDCOLMOD  0x0C00   ///< Read Display Pixel Format
		#define NT35510_RDDIM      0x0D00   ///< Read Display Image Mode
		#define NT35510_RDDSM      0x0E00   ///< Read Display Signal Mode
		#define NT35510_RDDSDR     0x0F00   ///< Read Display Self-Diagnostic Result
		#define NT35510_SLPIN      0x1000   ///< Enter Sleep Mode
		#define NT35510_SLPOUT     0x1100   ///< Sleep Out
		#define NT35510_PTLON      0x1200   ///< Partial Mode ON
		#define NT35510_NORON      0x1300   ///< Normal Display Mode ON
		#define NT35510_INVOFF     0x2000   ///< Display Inversion OFF
		#define NT35510_INVON      0x2100   ///< Display Inversion ON
		#define NT35510_ALLPOFF    0x2200   ///< All pixels off
		#define NT35510_ALLPON     0x2300   ///< All pixels on
		#define NT35510_GAMSET     0x2600   ///< Gamma Set
		#define NT35510_DISPOFF    0x2800   ///< Display OFF
		#define NT35510_DISPON     0x2900   ///< Display ON
		#define NT35510_CASET      0x2A00   ///< Column Address Set (0x2A00 - 0x2A03)
		#define NT35510_RASET      0x2B00   ///< Row Address Set (0x2B00 - 0x2B03)
		#define NT35510_RAMWR      0x2C00   ///< Memory Write
		#define NT35510_RAMRD      0x2E00   ///< Memory Read
		#define NT35510_PTLAR      0x3000   ///< Partial Area (0x3000 - 0x3003)
		#define NT35510_TEOFF      0x3400   ///< Tearing effect line off
		#define NT35510_TEON       0x3500   ///< Tearing effect line on
		#define NT35510_MADCTL     0x3600   ///< Memory Access Control
		#define NT35510_IDMOFF     0x3800   ///< Idle mode off
		#define NT35510_IDMON      0x3900   ///< Idle mode on
		#define NT35510_COLMOD     0x3A00   ///< Interface pixel format
		#define NT35510_RAMWRC     0x3C00   ///< Memory write continue
		#define NT35510_RAMRDC     0x3E00   ///< Memory read continue
		#define NT35510_STESL      0x4400   ///< Set tearing effect line (0x4400-4401)
		#define NT35510_GSL        0x4500   ///< Get scan line (0x4500 - 0x4501)
		#define NT35510_DPCKRGB    0x4A00   ///< Display clock in RGB interface
		#define NT35510_DSTBON     0x4F00   ///< Deep standby mode on
		#define NT35510_WRPFD      0x5000   ///< Write profile value for display
		#define NT35510_WRDISBV    0x5100   ///< Write display brightness
		#define NT35510_RDDISBV    0x5200   ///< Read display brightness
		#define NT35510_WRCTRLD    0x5300   ///< Write CTRL display
		#define NT35510_RDCTRLD    0x5400   ///< Read CTRL display
		#define NT35510_WRCABC     0x5500   ///< Write content adaptive brightness
		#define NT35510_RDCABC     0x5600   ///< Read content adaptive brightness
		#define NT35510_WRHYSTE    0x5700   ///< Write hysteresis (0x5700 - 0x573F)
		#define NT35510_WRGAMMASET 0x5800   ///< Write gamma setting (0x5800 - 0x5807)
		#define NT35510_RDFSVM     0x5A00   ///< Read FS value MSBs
		#define NT35510_RDFSVL     0x5B00   ///< Read FS value LSBs
		#define NT35510_RDMFFSVM   0x5C00   ///< Read median filter FS value MSBs
		#define NT35510_RDMFFSVL   0x5D00   ///< Read median filter FS value LSBs
		#define NT35510_WRCABCMB   0x5E00   ///< Write CABC minimum brightness
		#define NT35510_RDCABCMB   0x5F00   ///< Read CABC minimum brightness
		#define NT35510_WRLSCC     0x6500   ///< Write light sensor comp (0x6500-6501)
		#define NT35510_RDLSCCM    0x6600   ///< Read light sensor value MSBs
		#define NT35510_RDLSCCL    0x6700   ///< Read light sensor value LSBs
		#define NT35510_RDBWLB     0x7000   ///< Read black/white low bits
		#define NT35510_RDBkx      0x7100   ///< Read Bkx
		#define NT35510_RDBky      0x7200   ///< Read Bky
		#define NT35510_RDWx       0x7300   ///< Read Wx
		#define NT35510_RDWy       0x7400   ///< Read Wy
		#define NT35510_RDRGLB     0x7500   ///< Read red/green low bits
		#define NT35510_RDRx       0x7600   ///< Read Rx
		#define NT35510_RDRy       0x7700   ///< Read Ry
		#define NT35510_RDGx       0x7800   ///< Read Gx
		#define NT35510_RDGy       0x7900   ///< Read Gy
		#define NT35510_RDBALB     0x7A00   ///< Read blue/acolor low bits
		#define NT35510_RDBx       0x7B00   ///< Read Bx
		#define NT35510_RDBy       0x7C00   ///< Read By
		#define NT35510_RDAx       0x7D00   ///< Read Ax
		#define NT35510_RDAy       0x7E00   ///< Read Ay
		#define NT35510_RDDDBS     0xA100   ///< Read DDB start (0xA100 - 0xA104)
		#define NT35510_RDDDBC     0xA800   ///< Read DDB continue (0xA800 - 0xA804)
		#define NT35510_RDFCS      0xAA00   ///< Read first checksum
		#define NT35510_RDCCS      0xAF00   ///< Read continue checksum
		#define NT35510_RDID1      0xDA00   ///< Read ID1 value
		#define NT35510_RDID2      0xDB00   ///< Read ID2 value
		#define NT35510_RDID3      0xDC00   ///< Read ID3 value

	#endif
#endif



#define ILI9488H_NOP     0x00
#define ILI9488H_SWRESET 0x01
#define ILI9488H_RDDID   0x04
#define ILI9488H_RDDST   0x09

#define ILI9488H_SLPIN   0x10
#define ILI9488H_SLPOUT  0x11
#define ILI9488H_PTLON   0x12
#define ILI9488H_NORON   0x13

#define ILI9488H_RDMODE  0x0A
#define ILI9488H_RDMADCTL  0x0B
#define ILI9488H_RDPIXFMT  0x0C
#define ILI9488H_RDIMGFMT  0x0D
#define ILI9488H_RDSELFDIAG  0x0F

#define ILI9488H_INVOFF  0x20
#define ILI9488H_INVON   0x21
#define ILI9488H_GAMMASET 0x26
#define ILI9488H_DISPOFF 0x28
#define ILI9488H_DISPON  0x29

#define ILI9488H_CASET   0x2A
#define ILI9488H_PASET   0x2B
#define ILI9488H_RAMWR   0x2C
#define ILI9488H_RAMRD   0x2E

#define ILI9488H_PTLAR   0x30
#define ILI9488H_MADCTL  0x36
#define ILI9488H_PIXFMT  0x3A

#define ILI9488H_FRMCTR1 0xB1
#define ILI9488H_FRMCTR2 0xB2
#define ILI9488H_FRMCTR3 0xB3
#define ILI9488H_INVCTR  0xB4
#define ILI9488H_DFUNCTR 0xB6

#define ILI9488H_PWCTR1  0xC0
#define ILI9488H_PWCTR2  0xC1
#define ILI9488H_PWCTR3  0xC2
#define ILI9488H_PWCTR4  0xC3
#define ILI9488H_PWCTR5  0xC4
#define ILI9488H_VMCTR1  0xC5
#define ILI9488H_VMCTR2  0xC7

#define ILI9488H_RDID1   0xDA
#define ILI9488H_RDID2   0xDB
#define ILI9488H_RDID3   0xDC
#define ILI9488H_RDID4   0xDD

#define ILI9488H_GMCTRP1 0xE0
#define ILI9488H_GMCTRN1 0xE1
/*
#define LCD_PWCTR6  0xFC

*/

// Color definitions
#define LCD_BLACK       0x0000      /*   0,   0,   0 */
#define LCD_NAVY        0x000F      /*   0,   0, 128 */
#define LCD_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define LCD_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define LCD_MAROON      0x7800      /* 128,   0,   0 */
#define LCD_PURPLE      0x780F      /* 128,   0, 128 */
#define LCD_OLIVE       0x7BE0      /* 128, 128,   0 */
#define LCD_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define LCD_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define LCD_BLUE        0x001F      /*   0,   0, 255 */
#define LCD_GREEN       0x07E0      /*   0, 255,   0 */
#define LCD_CYAN        0x07FF      /*   0, 255, 255 */
#define LCD_RED         0xF800      /* 255,   0,   0 */
#define LCD_MAGENTA     0xF81F      /* 255,   0, 255 */
#define LCD_YELLOW      0xFFE0      /* 255, 255,   0 */
#define LCD_WHITE       0xFFFF      /* 255, 255, 255 */
#define LCD_ORANGE      0xFD20      /* 255, 165,   0 */
#define LCD_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define LCD_PINK        0xF81F

typedef struct{
	uint8_t Height ;
	uint8_t Width ;
	uint8_t Style ;
	uint8_t Dir ;
	char name[64] ;
	uint8_t *DATA ;
} FONT_INFO;


  void     LCD_begin(void);
  void     LCD_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),
  LCD_setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea),
  LCD_scroll(uint16_t pixels),
           pushColor(uint16_t color),
           pushColors(uint16_t *data, uint8_t len, uint8_t first),
		   LCD_drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h),
		   LCD_fillScreen(uint16_t color),
		   LCD_drawPixel(int16_t x, int16_t y, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
		   LCD_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
             uint16_t color),
			 LCD_setRotation(uint8_t r),
			 LCD_invertDisplay(uint8_t i);
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

  /* These are not for current use, 8-bit protocol only! */
  uint8_t  LCD_readdata(void);
  uint8_t  LCD_readcommand8(uint8_t reg, uint8_t index );
  void write16BitColor(uint16_t color) ;

  void LCDTextXY(int textX, int textY, char *myString, int color, int fontSize) ;
  void LCD_gotoxy(int x, int y);
  void LCD_setcolor(uint16_t color);
  void LCD_settextcolor(uint16_t color);
  void LCD_settextBkcolor(uint16_t color);
  void LCD_drawChar(uint8_t asciiCode);
  void LCD_setTextWrap(int16_t mode) ;
  void LCD_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) ;
  void LCD_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) ;
  void LCD_settextmultsize(uint16_t size) ;
  void LCD_settextBkMode(uint8_t mode);
  int LCD_drawCircle(int center_x, int center_y, int radius, int color);
  int LCD_fillCircle(int center_x, int center_y, int radius, int color);
  int LCD_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius,
    uint16_t color);
  void LCD_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius,
    uint16_t color);
  void LCD_drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
  		int16_t x3, int16_t y3,  uint16_t color);

  void LCD_fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
  		int16_t x3, int16_t y3,  uint16_t color);

  void ClearFillYAreaBorders();
  void ClearFillXAreaBorders();
  void TestYArea(int16_t y);
  void TestXYAreaPoint(int16_t x, int16_t y);

  int LoadFont(char *name, uint8_t ifont);
  int SetFont(uint8_t ifont) ;

  int LCD_getRect(int16_t x, int16_t y, int16_t w, int16_t h, char *fname) ;
  int LCD_putRect(int16_t x, int16_t y, char *fname) ;

  /*
  uint16_t readcommand16(uint8_t);
  uint32_t readcommand32(uint8_t);
  void     dummyclock(void);
  */
  /*void     spiwrite(uint8_t),
    writecommand(uint8_t c),
    ,
    writedata(uint8_t d),
    commandList(uint8_t *addr);
  uint8_t  spiread(void);*/



#endif
