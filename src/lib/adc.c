// Analog to Digital Converter
//
// File:    adc.h
// Authors: Luca Mazza

#include "adc.h"
#include "timer.h"

void adc_init(void) {
    ANSELBbits.ANSB2 = 1;
    AD1CON1          = 0x0004;
    AD1CHS           = 0x00020000;
    AD1CSSL          = 0;
    AD1CON3          = 0x0002;
    AD1CON2          = 0;
    AD1CON1SET       = 0x8000;
}

int adc_measure(void) {
    sleep(100);
    AD1CON1CLR = 0x0002;    
    while(!(AD1CON1 & 0x0001));
    return ADC1BUF0;
}