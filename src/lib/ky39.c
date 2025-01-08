// KY-039 Heart Beat Sensor
//
// File:    ky39.c
// Authors: Luca Mazza

#include "ky39.h"
#include "adc.h"

void ky39_init(void) {
    TRISBbits.TRISB8 = 1;
    ANSELBbits.ANSB8 = 1;
}

int  ky39_read(void) {
    return adc_measure();
}