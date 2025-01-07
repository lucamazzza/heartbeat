// Parallel Master Port
//
// File:    pmp.h
// Authors: Luca Mazza

#include <p32xxxx.h>

#ifndef PMP_H
#define PMP_H

#define HLCD         16     // LCD width=16 characters
#define VLCD         2      // LCD height=2 rows
#define LCD_DATAbits 0x0ff  // Port E bits 0 - 7
#define LCD_DB0     (1<<0)
#define LCD_DB1     (1<<1)
#define LCD_DB2     (1<<2)
#define LCD_DB3     (1<<3)
#define LCD_DB4     (1<<4)
#define LCD_DB5     (1<<5)
#define LCD_DB6     (1<<6)
#define LCD_DB7     (1<<7)

#define ENpin       (1<<4)      // RD4
#define LCD_HIGH    1
#define LCD_LOW     0
#define LCD_EN(a)	{ if(a) LATDSET = ENpin; else LATDCLR = ENpin; }	

#define RWpin       (1<<5)      // RD5
#define LCD_WR      0
#define LCD_RD      1
#define LCD_RW(a)	{ if(a) LATDSET = RWpin; else LATDCLR = RWpin; }

#define RSpin       (1<<15)     // RB15
#define LCDDATA     1           // address of data register
#define LCDCMD      0           // address of command register
#define LCD_RS(a)	{if(a) LATBSET = RSpin; else LATBCLR = RSpin;}

#define LCD_DATA    1
#define LCD_CMD     0
#define PMDATA      PMDIN

#define LCD_C_HOME  0x38         // Cursor Home
#define LCD_EM_SET  0x06         // Entry Mode Set
#define LCD_CLR     0x01         // Clear Display
#define LCD_CR      0xc0         // Carriage Return

#define busy_lcd()  read_lcd(LCD_CMD) & 0x80
#define put_lcd(d)  write_lcd(LCD_DATA, (d))
#define cmd_lcd(c)  write_lcd(LCD_CMD, (c))
#define home_lcd()  write_lcd(LCD_CMD, 2)
#define clr_lcd()   write_lcd(LCD_CMD, 1)
#define set_lcdg(a) write_lcd(LCDCMD, (a & 0x3F) | 0x40)
#define set_lcdc(a) write_lcd(LCDCMD, (a & 0x7F) | 0x80)
#define addr_lcd()  (read_lcd(LCDCMD) & 0x7F)
#define get_lcd()   read_lcd(LCDDATA)
#define nl_lcd()    write_lcd(LCD_CMD, LCD_CR)

void lcd_init(void);
char read_lcd(int addr);
void write_lcd(int addr, char c);
void puts_lcd(char *s);
void move_cursor_lcd(unsigned dir, unsigned steps);
void scroll_text_lcd(const char *str, int t);

#endif
