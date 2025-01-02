// General Purpose Input/Output
//
// File:    gpio.c
// Authors: Luca Mazza

#include "gpio.h"

void rgb_pins_init(void) {
    TRISBbits.TRISB1 = 0; // LED_R output
    TRISBbits.TRISB2 = 0; // LED_G output
    TRISBbits.TRISB3 = 0; // LED_B output
}

void btn_pins_init(void) {
    TRISBbits.TRISB4 = 1; // BTNC input
}

void speaker_pins_init(void) {
    TRISBbits.TRISB14 = 0; // SPEAKER output
}

void rgb_set_color(unsigned r, unsigned g, unsigned b) {
    LED_R = r;
    LED_G = g;
    LED_B = b;
}

void rgb_clear() {
    LED_R = 0;
    LED_G = 0;
    LED_B = 0;
}
