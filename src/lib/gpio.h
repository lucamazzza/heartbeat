// General Purpose Input/Output
//
// File:    gpio.h
// Authors: Luca Mazza

#include <p32xxxx.h>

#ifndef GPIO_H
#define GPIO_H

#define LED_R   LATDbits.LATD2
#define LED_G   LATDbits.LATD12
#define LED_B   LATDbits.LATD3
#define BTNC    PORTFbits.RF0
#define SPEAKER LATBbits.LATB14

void rgb_pins_init(void);
void btn_pins_init(void);
void btn_interrupt_init(void);
void speaker_pins_init(void);
void rgb_set_color(unsigned r, unsigned g, unsigned b);
void rgb_clear();

#endif
