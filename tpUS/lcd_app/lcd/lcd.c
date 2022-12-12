#include "lcd.h"
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"

int __LCDline=0;
TM_FontDef_t *lcdFont = &TM_Font_16x26;
 
//extern from tm_stm32f4_ili9341.c, to redefine putString...
extern uint16_t ILI9341_x;
extern uint16_t ILI9341_y;

void lcdInit()
{
	//init GPIO for LED: PORTD.0
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//set LCD backlight
	GPIO_SetBits(GPIOD, GPIO_Pin_0);

	//Initialize ILI9341
	TM_ILI9341_Init();

	//Landscape mode.
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
	
	//clear
	lcdClear();
	//
	lcdSetLine(0);
}

void lcdPrintChar(char c)
{
	TM_ILI9341_Putc(ILI9341_x, ILI9341_y, c, lcdFont, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
}

void lcdPrintString(char *str)
{
	while (*str) {
		if (*str == '\n') lcdSetLine(__LCDline+1);
		else lcdPrintChar(*str);
		str++;
	}
}

void lcdSetLine(int line)
{
	lcdSetPos(line,0);
}

void lcdSetPos(int line, int chr)
{
	__LCDline=(line)%9;
	ILI9341_y = (lcdFont->FontHeight + 1)*(__LCDline);
	ILI9341_x = (lcdFont->FontWidth)*(chr%20); //20 character max.
}

void lcdClear() {
	//FIll lcd with color
	TM_ILI9341_Fill(ILI9341_COLOR_BLACK);
}

//affiche un entier dans une zone, l'entier est cadré à droite de la zone.
//val: la valeur à afficher
//field: la largeur de la zone d'affichage
void lcdPrintInt(int val, int field)
{
	int negative = 0; 
	char buffer[10];
	int index = 0;
	
	if (val < 0)
	{
      negative = 1;
	  val = -val;
	}
	if (val == 0) buffer[index++] = '0';
	else
	{
		while (val != 0)
		{
			int remaining = val % 10;
			buffer[index++] = (char)remaining + '0';
			val = val / 10;
		}
	}
	if (negative) buffer[index++] = '-';
	int i;
	for (i = index; i < field; i++) lcdPrintChar(' ');
	for (i = index - 1; i >= 0; i--) lcdPrintChar(buffer[i]);
}

