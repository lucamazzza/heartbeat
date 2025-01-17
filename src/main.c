// Finger HeartBeat
//
// File:    main.c
// Authors: Luca Mazza
//          Filippo De Simoni
//          Vasco Silva
//          Niccolò Pasqualetti
//          Julian Sprugasci

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

// Main global variables
volatile int      bpm_temp         = 0;
volatile float    previous_lowpass = 0;
volatile float    reading_buf[READ_BUFSIZE];
volatile int      read_buf_index   = 0;
volatile float    buf_sum          = 0.0;
volatile float    filtered_sig     = 0.0;
int               busy_measure     = 0;
int               bpm              = 0;
int               max_bpm_m        = 0;
float             readings[READINGS_LEN];
int               cur_reading_idx;

// UART global variables
extern char flag_rx;
extern char strg[80];
extern int  j;

// LCD globl variables
extern int  stop_lcd;

// END Global Variables ////////////////////////////////////////////////////////

// START ISR Routines //////////////////////////////////////////////////////////

// Timer2 Interrupt Handler
void __attribute__((interrupt(ipl1), vector(8))) timer2_interrupt() {
    TMR2 = 0;
    IFS0bits.T2IF = 0;
}

// BTNC Interrupt Handler
// 
// Used to exit from blocking function (heart_beat and max_bpm).
// When said function are running, press the BTNC to exit their procedure.
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

// Utility menu procedure
//
// Displays a start menu on the UART serial console and a scrolling menu
// (styled after old VHS players) on the LCD.
void start_menu(void) {
    clr_lcd();
    // The escape codes \033[2J sends the console a `clear` command; the 
    // \033[0;0H brings the cursor to "Home" (r:0, c:0)
    uart_puts_4("\033[2J\033[0;0HSelect...\r\n");
    uart_puts_4("1. Heart Beat\r\n");
    uart_puts_4("2. Max BPM\r\n");
    uart_puts_4("3. Reset\r\n");
    puts_lcd("Select...       ");
    scroll_text_lcd(" 1. HeartBeat - 2. Max BPM - 3. Reset ", 1000);
    nl_lcd();
}

// Computes a BPM reading given 1000 sensor readings.
// Approximates looking for a peak every 40 entry.
int current_bpm(void){
    int cnt = 0;
    for (int i = 40; i < READINGS_LEN - 40; i++) {
        if (readings[i] > readings[i - 40] && readings[i] < readings[i + 40]) {
            cnt++;
            i += 40;
        }
    }
    return cnt * 6;
}

// This function starts the acquisition of the signal from the `KY-039` sensor
// notifying with a *beep* the start.
//
// Once it is operating, the RGB LED turns Blue and the LCD is populated
// with the current BPM reading (which updates every 10s).
//
// The function, given the sensor is not precise without the perfect lighting
// condition takes some time to get to an acceptable reading (~30s).
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
        
        // The signal received from the sensor is passed through a Low-Pass 
        // filter which trims off high frequencies
        float tmp = (0.1 * read) + ((1.0 - 0.1) * previous_lowpass);
        previous_lowpass = tmp;
        
        buf_sum -= reading_buf[read_buf_index];
        reading_buf[read_buf_index] = tmp;
        buf_sum += reading_buf[read_buf_index];
        read_buf_index = (read_buf_index + 1) % READ_BUFSIZE;
        filtered_sig = buf_sum / READ_BUFSIZE;
        
        // Every 1000 reading (10 seconds) are saved in a vector, which 
        // is then used to compute the BPM through an approximated peak count.
        if (cur_reading_idx < READINGS_LEN) 
            readings[cur_reading_idx++] = filtered_sig;
        else {
            bpm = current_bpm();
            cur_reading_idx = 0;
            if (max_bpm_m <= bpm) max_bpm_m = bpm;
        }
        
        // To show a graph on the serial plotter be sure to transmit a 
        // "carriage return" after every data entry.
        sprintf(str_bpm, "%f\r\n", filtered_sig);
        uart_puts_4(str_bpm);
        
        char ts_bpm_str[16];
        sprintf(ts_bpm_str, "BPM: %11d", bpm);
        clr_lcd();
        puts_lcd(ts_bpm_str);
        nl_lcd();
        puts_lcd("################");
        
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

// Reads from the flash memory (address 0x00) the stored "Max BPM" reading.
// When the reading is `255` (reset state), on the LCD "--" is shown.
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

// Sends the flash memory the command to clear the whole memory.
// Being that only one address is always written, clearing the whole memory
// assures that no unwanted data is stored.
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

// Procedure for an invalid selection through UART console.
// 
// When the user does not select any correct option the red is turned red 
// and a message is shown to notify the invalid choice.
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
    // Initialize the sensor readings buffer
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
    return 0;
}

// MAIN ////////////////////////////////////////////////////////////////////////
