// Timer
//
// File:    timer.h
// Authors: Luca Mazza

#ifndef TIMER_H
#define TIMER_H

#define enable_interrupts()                     \
{                                               \
    unsigned int val = 0;                       \
    asm volatile("mfc0 %0,$13" : "=r"(val));    \
    val |= 0x00800000;                          \
    asm volatile("mtc0 %0,$13" : "+r"(val));    \
    INTCONbits.MVEC = 1;                        \
    _builtin_enable_interrupts();               \
}

void timer_init_pins();
void timer_init(unsigned tnum, int T, int f_clk, int presc, int ei);
void sleep(unsigned t, unsigned tnum);

#endif
