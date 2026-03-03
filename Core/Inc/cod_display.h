/*
 * cod_display.h
 *
 *  Created on: Feb 28, 2026
 *      Author: Usuario
 */

#ifndef COD_DISPLAY_H_
#define COD_DISPLAY_H_
#include "stm32f1xx_hal.h"

/// Inicializa el LCD display
void LCD_Init(void);


///Sends data to data pins when a high to low pulse is given;
///Extra voltage push is required to execute the instruction and EN(enable) signal is used for this purpose. Usually,
//we set en=0, when we want to execute the instruction we make it high en=1 for some milliseconds.
//After this we again make it ground that is, en=0
void LCD_EnablePulse(void);

void LCD_Send4Bits(uint8_t data);

void LCD_SendData(uint8_t data);


///imprime un mensaje en el display
void LCD_Print(char *str);


#endif /* COD_DISPLAY_H_ */
