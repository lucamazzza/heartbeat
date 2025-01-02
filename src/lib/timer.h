// Timer
//
// File:    timer.h
// Authors: Luca Mazza

#include <p32xxxx.h>

#ifndef TIMER_H
#define TIMER_H

#define t3_start() {                            \
    TMR3            = 0;                        \
    T3CONbits.ON    = 1;                        \
}
#define t3_stop() {                             \
    TMR3         = 0;                           \
    T3CONbits.ON = 0;                           \
}
#define enable_interrupts()                     \
{                                               \
    unsigned int val = 0;                       \
    asm volatile("mfc0 %0,$13" : "=r"(val));    \
    val |= 0x00800000;                          \
    asm volatile("mtc0 %0,$13" : "+r"(val));    \
    INTCONbits.MVEC = 1;                        \
    _builtin_enable_interrupts();               \
}

void timer_init(void);
void sleep(unsigned t);

#endif
