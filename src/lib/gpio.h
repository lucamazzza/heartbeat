// General Purpose Input/Output
//
// File:    gpio.h
// Authors: Luca Mazza

#include <p32xxxx.h>

#ifndef GPIO_H
#define GPIO_H

#define LED_R   LATBbits.LATB1
#define LED_G   LATBbits.LATB2
#define LED_B   LATBbits.LATB3
#define BTNC    PORTBbits.RB4
#define SPEAKER LATBbits.LATB14

void rgb_pins_init(void);
void btn_pins_init(void);
void speaker_pins_init(void);
void rgb_set_color(unsigned r, unsigned g, unsigned b);
void rgb_clear();

#endif
