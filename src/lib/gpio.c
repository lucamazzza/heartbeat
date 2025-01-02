// General Purpose Input/Output
//
// File:    gpio.c
// Authors: Luca Mazza

#include "gpio.h"

void rgb_pins_init(void) {
    TRISDbits.TRISD2 = 0; // LED_R output
    TRISDbits.TRISD12 = 0;// LED_G output
    TRISDbits.TRISD3 = 0; // LED_B output
}

void btn_pins_init(void) {
    TRISFbits.TRISF0 = 1; // BTNC  input
}

void btn_interrupt_init(void) {
    // Enable Interrupt INT4 (BTNC)
    INT4R           = 4;
    IPC4bits.INT4IP = 1;
    IPC4bits.INT4IS = 2;
    IFS0bits.INT4IF = 0;
    IEC0bits.INT4IE = 1;
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
    rgb_set_color(0, 0, 0);
}
