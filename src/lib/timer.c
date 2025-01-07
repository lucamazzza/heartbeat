// Timer
//
// File:    timer.c
// Authors: Luca Mazza

#include "timer.h"

void timer_init(void){
    // Timer 2 (sleep)
    T2CONbits.ON    = 0;        // Disable Timer2
    T2CONbits.T32   = 0;        // not use 32-bit mode - use 16-bit mode
    T2CONbits.TCKPS = 0b111;    // or 0x7, select prescaler 256
    T2CONbits.TCS   = 0;        // select internal peripheral clock
    TMR2            = 0;        // Clear TMR2 register
    PR2             = 5000;    // Set PR2 register - Calculated to have 1s
    enable_interrupts();
    IPC2bits.T2IP   = 1;
    IPC2bits.T2IS   = 0;
    IFS0bits.T2IF   = 0;        // Reset flag interrupt
    IEC0bits.T2IE   = 1;        // Enable interrupt
    T2CONbits.ON    = 1;        // Enable Timer2
    
    // Timer 3 (PWM)
    T3CONbits.ON    = 0;        // Disable Timer3
    T3CONbits.TCKPS = 0b111;    // Select prescaler 256
    T3CONbits.TCS   = 0;        // Select internal peripheral clock
    TMR3            = 0;        // Clear TMR3 register
    PR3             = 999;      // Set PR3 register
}

void sleep(unsigned t){
    while (t--) { // t x 1ms loop
		TMR2 = 0; // manually reset TMR2 register to guarantee time elaplsing desidered
		while (TMR2 < PR2);
    }
}