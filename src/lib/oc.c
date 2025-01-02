// Output Compare
//
// File:    oc.h
// Authors: Luca Mazza

#include "oc.h"
#include "timer.h"
#include "main.h"

void oc_init(void) {
    TRISBbits.TRISB14   = 0;
    ANSELBbits.ANSB14   = 0;
    RPB14R              = 0x0c;
    OC1CONbits.ON       = 0;
    OC1CONbits.OCM      = 6;
    OC1CONbits.OCTSEL   = 1;
    OC1RS               = 900;
    OC1R                = 900;
}

void beep(unsigned t) {
    t3_start();
    OC1CONbits.ON = 1;
    int ticks = t * (PBCLK * PRESC);
    while (ticks > 0) ticks--;
    OC1CONbits.ON = 0;
    t3_stop();
}