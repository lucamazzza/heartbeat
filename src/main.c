// Finger HeartBeat
//
// File:    main.c
// Authors: Luca Mazza 

// START Includes //////////////////////////////////////////////////////////////

#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "pmp.h"
#include "adc.h"
#include "oc.h"
#include "spi.h"
#include "ky39.h"


// END Includes ////////////////////////////////////////////////////////////////

// START PRAGMA Configurations /////////////////////////////////////////////////

#pragma config JTAGEN   = OFF
#pragma config FWDTEN   = OFF
#pragma config FNOSC    = FRCPLL
#pragma config FSOSCEN  = OFF
#pragma config POSCMOD  = XT
#pragma config OSCIOFNC = ON
#pragma config FPBDIV   = DIV_2
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL  = MUL_20
#pragma config FPLLODIV = DIV_1

// END PRAGMA Configurations ///////////////////////////////////////////////////

// START Global Variables //////////////////////////////////////////////////////

volatile int bpm            = 0;
volatile int max_bpm        = 0;
volatile int busy_measure   = 0;
volatile int menu_scrolled  = 0;

// END Global Variables ////////////////////////////////////////////////////////

// START ISR Routines //////////////////////////////////////////////////////////

// BTNC Interrupt Handler
void __attribute__((interrupt(ipl1), vector(19))) int4_btnc_interrupt() {
    // TODO: Implement
}

// BTND Interrupt Handler
void __attribute__((interrupt(ipl2), vector(15))) int3_btnd_interrupt() {
    // TODO: Implement
}

// BTNU Interrupt Handler
void __attribute__((interrupt(ipl2), vector(9))) int2_btnu_interrupt() {
    // TODO: Implement
}


// END ISR Routines ////////////////////////////////////////////////////////////

// START Functions /////////////////////////////////////////////////////////////

void start_monitoring(void) {
    busy_measure = 1;
    rgb_set_color(0, 1, 0);
    beep(100);
}

void display_menu(void) {
    clr_lcd(); 
    if (menu_scrolled == 0) {
        puts_lcd("1 HeartBeat");
        nl_lcd();
    }
    puts_lcd("2 Max BPM");
    if (menu_scrolled) {
        nl_lcd();
        puts_lcd("3 Clear Readings");
    }
    sleep(10);
}

void reset_max_bpm(void) {
    max_bpm = 0;
    erase_flash();
}
// END Functions ///////////////////////////////////////////////////////////////

// MAIN ////////////////////////////////////////////////////////////////////////

int main(void) {
    // GPIO Initialization
    rgb_pins_init();
    btn_pins_init();
    speaker_pins_init();
    
    // Timer Initialization
    timer_init();
    
    // UART Initialization
    uart_init_pins();
    uart_init(9600);
    
    // PMP Initialization
    lcd_init();
    
    // ADC Initialization
    adc_init();
    
    // Output Compare Initialization
    oc_init();
    
    // SPI Initialization
    init_spi1();
    
    // KY-039 Initialization
//    ky39_init();
    
    while(1) display_menu();
    
}

// MAIN ////////////////////////////////////////////////////////////////////////