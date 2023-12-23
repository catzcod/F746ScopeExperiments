#include "gfx.h"
#include "st7789.h"
#include "fonts.h"
#include "string.h"

uint8_t _colstart = 0, _rowstart = 0;

uint8_t tabcolor;

int16_t _width = ST7789_WIDTH;	 ///< Display width as modified by current rotation
int16_t _height = ST7789_HEIGHT; ///< Display height as modified by current rotation

int16_t _xstart = 0; ///< Internal framebuffer X offset
int16_t _ystart = 0; ///< Internal framebuffer Y offset

uint8_t rotation;

uint16_t *frameBuffer = NULL;

int16_t cursor_x = 0;				 ///< x location to start print()ing text
int16_t cursor_y = 0;				 ///< y location to start print()ing text
uint16_t textcolor = WHITE;	 ///< 16-bit background color for print()
uint16_t textbgcolor = BLACK; ///< 16-bit text color for print()
uint16_t textsize_x = 1;
uint16_t textsize_y = 1;

void drawBitmap(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint16_t *data)
{
	ST7789_DrawImage(x0, y0, w, h, data);
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	ST7789_DrawPixel_BUF(x, y, color);
}

void clearDisplay()
{
	ST7789_Fill_Color_BUF(BLACK);
}

void flushDisplay()
{
	ST7789_Display_BUF();
}

void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
			  uint16_t bg, uint8_t size_x, uint8_t size_y)
{
	ST7789_WriteChar_BUF(x, y, c, Font_7x10, color, bg);
}

void setCursor(int16_t x, int16_t y)
{
	cursor_x = x;
	cursor_y = y;
}

void setTextColor(uint16_t c, uint16_t bg)
{
	textcolor = c;
	textbgcolor = bg;
}

void setTextSize(uint16_t s)
{
	textsize_x = s;
	textsize_y = s;
}

void writeChar(char c)
{
	if (c == '\n')
	{								// Newline?
		cursor_x = 0;				// Reset x to zero,
		cursor_y += textsize_y * 8; // advance y one line
	}
	else if (c != '\r')
	{ // Ignore carriage returns
		if ((cursor_x + textsize_x * 6) > _width)
		{								// Off right?
			cursor_x = 0;				// Reset x to zero,
			cursor_y += textsize_y * 8; // advance y one line
		}
		drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor,
				 textsize_x, textsize_y);
		cursor_x += textsize_x * 6; // Advance x one char
	}
}

void printString(char s[])
{
	uint8_t n = strlen(s);
	for (int i = 0; i < n; i++)
		writeChar(s[i]);
}

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
	{                       \
		int16_t t = a;      \
		a = b;              \
		b = t;              \
	}
#endif

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	for (int i = y; i <= y + h; i++)
		drawPixel(x, i, color);
}

void drawFastHLine(int16_t x, int16_t y, int16_t l, uint16_t color)
{
	for (int i = x; i <= x + l; i++)
		drawPixel(i, y, color);
}
