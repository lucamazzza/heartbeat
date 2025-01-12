// Analog to Digital Converter
//
// File:    adc.h
// Authors: Luca Mazza

#include "adc.h"
#include "timer.h"

void adc_init(void) {
    AD1CON1           = 0;
    AD1CON1bits.ASAM  = 1;
    AD1CON1bits.SSRC  = 2;
    AD1CHS            = 0;
    AD1CHSbits.CH0SA  = 8;
    AD1CSSL           = 0;
    AD1CSSLbits.CSSL8 = 1;
    AD1CON3           = 0;
    AD1CON3bits.ADCS  = 2;
    AD1CON2           = 0;
    AD1CON1bits.ADON  = 1;
}

int adc_measure(void) {
    sleep(10);
    AD1CON1CLR = 0x0002;    
    while(!(AD1CON1 & 0x0001));
    return ADC1BUF0;
}