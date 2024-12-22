// Parallel Master Port
//
// File:    pmp.h
// Authors: Luca Mazza

#ifndef PMP_H
#define PMP_H

#define LCD_DATA    1
#define LCD_CMD     0
#define PMDATA      PMDIN

#define busy_lcd()  read_lcd(LCD_CMD) & 0x80
#define put_lcd(d)  write_lcd(LCD_DATA, (d))
#define cmd_lcd(c)  write_lcd(LCD_CMD, (c))
#define home_lcd()  write_lcd(LCD_CMD, 2)
#define clr_lcd()   write_lcd(LCD_CMD, 1)

void lcd_init(void);
void read_lcd(int addr);
void write_lcd(int addr, char c);
void puts_lcd(char *s);

#endif
