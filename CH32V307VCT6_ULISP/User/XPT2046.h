/*
 * XPT2046.h
 *
 *  Created on: Sep 21, 2025
 *      Author: sap
 */

#ifndef USER_XPT2046_H_
#define USER_XPT2046_H_


#define XPT2046_WIDTH  LCD_TFTWIDTH
#define XPT2046_HEIGHT LCD_TFTHEIGHT



typedef struct {
  int16_t	RawX0,RawX1,RawY0,RawY1;
  uint16_t  _width, _height ;
  uint16_t  ReperX0, ReperX1, ReperY0, ReperY1;
  uint8_t	_xflip;
  uint8_t	_yflip;
  uint8_t	_xyswap;
} ts_config_t;



int TS2046_Init();
void TS_setRotation(uint8_t m);
void TS2046_SetCalibration(int16_t x0, int16_t x1, int16_t y0,int16_t y1,
		int16_t rx0, int16_t rx1, int16_t ry0,int16_t ry1);
int8_t touch_Pressed() ;
int16_t touch_X() ;
int16_t touch_Y() ;
int16_t touch_Ch(uint8_t ch) ;
int TS_GPIO_init();
int16_t touch_Xraw();
int16_t touch_Yraw();

#endif /* USER_XPT2046_H_ */
