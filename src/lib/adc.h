// Analog to Digital Converter
//
// File:    adc.h
// Authors: Luca Mazza

#include <p32xxxx.h>

#ifndef ADC_H
#define ADC_H

void adc_init(void);
int  adc_measure(void);

#endif
