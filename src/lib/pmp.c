// Parallel Master Port
//
// File:    pmp.c
// Authors: Luca Mazza

#include "pmp.h"
#include "timer.h"

void lcd_init() {
    ANSELE = 0x0000;        //RE0:7 as digital
    TRISE = 0x00FF;         // RE0:7 as digital input , or 0x0000 as out is the same
    TRISDbits.TRISD4 = 0;   // RD4 as digital output ENpin
    TRISDbits.TRISD5 = 0;   // RD5 as digital output RWpin
    ANSELBbits.ANSB15 = 0;  //RB15 ad digital
    TRISBbits.TRISB15 = 0;  // RB15 as digital output RSpin
    // PMP initialization
    PMCON = 0x83BF;         // Enable the PMP, long waits
    PMMODE = 0x3FF;         // Master Mode 1
    PMAEN = 0x0001;         // PMA0 enabled
    // wait for >30ms
    sleep(30);              // Timer configured before externally (i.e. you can use T1 for 1ms interrupt reset)
    PMADDR = LCD_CMD;       // command register (ADDR = 0)
    PMDATA = LCD_C_HOME;  // set: 8-bit interface, 2 lines, 5x7
    sleep(1);               //>48us
    PMDATA = 0x0c;          // ON, no cursor, no blink
    sleep(1);               //>48us
    PMDATA = LCD_CLR;       // clear display
    sleep(2);               //>1.6ms
    PMDATA = LCD_EM_SET;    // increment cursor, no shift
    sleep(2);               //>1.6ms
}

char read_lcd(int addr) {
    int dummy;
    while (PMMODEbits.BUSY);// wait for PMP to be available
    PMADDR = addr;          // select the command address
    dummy  = PMDATA;        // init read cycle, dummy read
    while (PMMODEbits.BUSY);// wait for PMP to be available
    return(PMDATA);         // read the status register
}

void write_lcd(int addr, char c) {
    while(busy_lcd());      // wait for LCD driver (KSUU06) to be available
    while(PMMODEbits.BUSY);// wait for PMP to be available
    PMADDR = addr;
    PMDATA = c;
}

void puts_lcd(char *s) {
    while(*s) {
        put_lcd(*s++);
    }
}

void set_cursor_lcd(unsigned dir, unsigned steps) {
    int cmd = (dir == 0) ? 0x10 : 0x14;
    cmd_lcd(cmd);
}