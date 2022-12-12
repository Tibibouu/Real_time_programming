#ifndef __LCD_H__
#define __LCD_H__

//lcd driver thanks to 
// => https://stm32f4-discovery.net/2014/04/library-08-ili9341-lcd-on-stm32f429-discovery-board/
//very basic set of functions for Trampoline labs.

void lcdInit();
void lcdClear();

void lcdPrintString(char *str);
void lcdPrintChar(char c);
void lcdPrintInt(int val, int field);

//set cursor at the beginning of line lineId (from 0 to 8)
void lcdSetLine(int lineNb);

//set cursor position
//\param line from 0 to 8
//\param chr  from 0 to 19
void lcdSetPos(int line, int chr);

#endif
