/*
 * board_pins.h
 *
 *  Created on: Sep 13, 2025
 *      Author: sap
 */

#ifndef USER_BOARD_PINS_H_
#define USER_BOARD_PINS_H_


#define LED_BUILTIN 13

#define OUTPUT			1
#define INPUT			2
#define INPUT_PULLUP	3
#define INPUT_PULLDOWN	4

#define MODE_INPUT_ANALOG	5
#define MODE_OUTPUT_ANALOG	6

#define NOTAVAIL	0x00
#define REMAP0		0x20
#define REMAP1		0x40
#define REMAP2		0x80


#include "ch32v30x_gpio.h"

typedef struct {
  GPIO_TypeDef* GPIOx ;
  uint16_t	iBit;
  uint8_t	iADCChannel;
  uint8_t	iDACChannel;
} pindesc_t;



int pinMode(int pin, int mode) ;

unsigned int digitalRead(int pin);
void digitalWrite(int pin, int mode);
unsigned int analogRead(int pin) ;
int analogWrite(int pin, uint16_t Data);

#endif /* USER_BOARD_PINS_H_ */
