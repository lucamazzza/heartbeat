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

volatile int      bpm              = 0;
volatile int      bpm_temp         = 0;
volatile unsigned max_bpm_m        = 0;
volatile int      busy_measure     = 0;
volatile float    previous_lowpass = 0;
volatile float    reading_buf[READ_BUFSIZE];
volatile int      read_buf_index   = 0;
volatile float    buf_sum          = 0.0;
volatile float    filtered_sig     = 0.0;

float    readings[READINGS_LEN];
int      cur_reading_idx;

extern char flag_rx;
extern char strg[80];
extern int  j;

extern int  stop_lcd;

// END Global Variables ////////////////////////////////////////////////////////

// START ISR Routines //////////////////////////////////////////////////////////

// Timer2 Interrupt Handler
void __attribute__((interrupt(ipl1), vector(8))) timer2_interrupt() {
    TMR2 = 0;
    IFS0bits.T2IF = 0;
}

// BTNC Interrupt Handler
void __attribute__((interrupt(ipl1), vector(19))) int4_btnc_interrupt() {
    busy_measure = 0;
    IFS0bits.INT4IF = 0;
}

// UART Interrupt Handler
void __attribute__((interrupt(ipl6), vector(39))) uart4_rx_interrupt() {
    char c = U4RXREG;
    strg[j++] = c;
    if (j > 0 && (c == '\r' || c == '\n')) {
        strg[j - 1] = 0;
        j = 0;
        flag_rx  = 1;
        stop_lcd = 1;
    }
    IFS2bits.U4RXIF = 0;
}

// END ISR Routines ////////////////////////////////////////////////////////////

// START Functions /////////////////////////////////////////////////////////////

void start_menu(void) {
    clr_lcd();
    uart_puts_4("\033[2J\033[0;0HSelect...\r\n");
    uart_puts_4("1. Heart Beat\r\n");
    uart_puts_4("2. Max BPM\r\n");
    uart_puts_4("3. Reset\r\n");
    puts_lcd("Select...       ");
    scroll_text_lcd(" 1. HeartBeat - 2. Max BPM - 3. Reset ", 1000);
    nl_lcd();
}

int current_bpm(float data[READINGS_LEN]){
    int cnt = 0;
    for (int i = 50; i < READINGS_LEN - 50; i++) {
        if (data[i] > data[i - 50] && data[i] < data[i + 50]) {
            cnt++;
            i += 50;
        }
    }
    return cnt * 6;
}

void heart_beat(void) {
    rgb_set_color(0, 0, 1);
    clr_lcd();
    uart_puts_4("Recording Heart Beat...\r\n");
    puts_lcd("Recording...   ");
    nl_lcd();
    puts_lcd("################");
    busy_measure = 1;
    beep(10);
    t3_start();
    while(busy_measure) {
        char str_bpm[10];
        int read = ky39_read();
        
        float tmp = (0.1 * read) + ((1.0 - 0.1) * previous_lowpass);
        previous_lowpass = tmp;
        
        buf_sum -= reading_buf[read_buf_index];
        reading_buf[read_buf_index] = tmp;
        buf_sum += reading_buf[read_buf_index];
        read_buf_index = (read_buf_index + 1) % READ_BUFSIZE;
        filtered_sig = buf_sum / READ_BUFSIZE;
        
        if (cur_reading_idx < READINGS_LEN) 
            readings[cur_reading_idx++] = filtered_sig;
        else {
            bpm = current_bpm(readings);
            cur_reading_idx = 0;
        }

        sprintf(str_bpm, "%f\r\n", filtered_sig);
        uart_puts_4(str_bpm);
        
        char *ts_bpm_str;
        sprintf(ts_bpm_str, "BPM: %11d", bpm);
        clr_lcd();
        puts_lcd(ts_bpm_str);
        nl_lcd();
        puts_lcd("################");
        if (max_bpm_m < bpm) max_bpm_m = bpm;
        
    }
    write_flash(MAX_BPM_FADDR, max_bpm_m);
    t3_stop();
    clr_lcd();
    puts_lcd("Stopping...    ");
    nl_lcd();
    puts_lcd("################");
    sleep(1000);
    clr_lcd();
    home_lcd();
    stop_lcd = 0;
    rgb_set_color(0, 1, 0);
}

void max_bpm(void) {
    rgb_set_color(0, 0, 1);
    int max_bpm = read_flash(MAX_BPM_FADDR);
    char str_bpm[16];
    
    if (max_bpm == 255) sprintf(str_bpm, "Max BPM:      --");
    else sprintf(str_bpm, "Max BPM: %7d", max_bpm);
    busy_measure = 1;
    while (busy_measure) {
        clr_lcd();
        puts_lcd(str_bpm);
        nl_lcd();
        puts_lcd("################");
        sleep(1000);
    }
    clr_lcd();
    home_lcd();
    stop_lcd = 0;
    rgb_set_color(0, 1, 0);

}

void reset_max_bpm(void) {
    rgb_set_color(0, 0, 1);
    clr_lcd();
    uart_puts_4("Max BPM Reading Reset\r\n");
    puts_lcd("Max BPM Reset");
    nl_lcd();
    puts_lcd("################");
    max_bpm_m = 0;
    erase_flash();
    sleep(2000);
    clr_lcd();
    home_lcd();
    stop_lcd = 0;
    rgb_set_color(0, 1, 0);
}

void invalid(void) {
    rgb_set_color(1, 0, 0);
    clr_lcd();
    home_lcd();
    uart_puts_4("Invalid Selection\r\n");
    puts_lcd("Invalid...      ");
    nl_lcd();
    puts_lcd("################");
    sleep(2000);
    clr_lcd();
    stop_lcd = 0;
    rgb_set_color(0, 1, 0);
} 
// END Functions ///////////////////////////////////////////////////////////////

// MAIN ////////////////////////////////////////////////////////////////////////

int main(void) {
    // GPIO Initialization
    rgb_pins_init();
    btn_pins_init();
    btn_interrupt_init();
    speaker_pins_init();
    
    // Timer Initialization
    timer_init();
    
    // UART Initialization
    uart_init_pins();
    uart_init(9600);
    
    // PMP Initialization
    lcd_init();
    
    // KY-039 Initialization
    ky39_init();
    
    // ADC Initialization
    adc_init();
    
    // Output Compare Initialization
    oc_init();
    
    // SPI Initialization
    init_spi1();
    
    rgb_set_color(0, 1, 0);
    stop_lcd = 0;
    for (int i = 0; i < READ_BUFSIZE; i++) reading_buf[i] = 0.0;
    while (1) {
        char *hb = "1";
        char *mb = "2";
        char *rs = "3";        
        start_menu();
        if (flag_rx) {
            char *str = get_strg();
            if (strcmp(str, hb) == 0) {
                heart_beat();
            } else if (strcmp(str, mb) == 0) {
                max_bpm();
            } else if (strcmp(str, rs) == 0) {
                reset_max_bpm();
            } else {
                invalid();
            }
        }
        flag_rx = 0;
        sleep(20);
    }
}

// MAIN ////////////////////////////////////////////////////////////////////////