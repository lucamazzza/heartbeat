// Universal Asynchronous Receiver/Transmitter
//
// File:    uart.c
// Authors: Luca Mazza

#include "uart.h"
#include "main.h"

char flag_rx;
char strg[80];
int  j;

char *get_strg(void) {
    return strg;
}

void uart_init_pins(void) {
    TRISFbits.TRISF12 = 0; // U4TX output
    RPF12R            = 2; // U4TX on port RPF12
    TRISFbits.TRISF13 = 1; // U4RX input
    U4RXR             = 9; // U4RX on port RPF13
}

void uart_init(int baud) {
    unsigned int uart_brg;
    // REGSET
    U4MODEbits.ON       = 0;
    U4MODEbits.SIDL     = 0;
    U4MODEbits.IREN     = 0;
    U4MODEbits.RTSMD    = 0;
    U4MODEbits.UEN0     = 0;
    U4MODEbits.UEN1     = 0;
    U4MODEbits.WAKE     = 0;
    U4MODEbits.LPBACK   = 0;
    U4MODEbits.ABAUD    = 0;
    U4MODEbits.RXINV    = 0;
    U4MODEbits.PDSEL1   = 0;
    U4MODEbits.PDSEL0   = 0;
    U4MODEbits.STSEL    = 0;
    U4MODEbits.BRGH     = 0;
    // COMPUTE BRG
    uart_brg            = (int) (((float)PBCLK / (16 * baud) - 1) + 0.5);
    U4BRG               = uart_brg;
    U4STAbits.UTXEN     = 1;
    U4STAbits.URXEN     = 1;
    // UART RX Interrupt Flag
    IPC9bits.U4IP       = 6;
    IPC9bits.U4IS       = 3;
    IFS2bits.U4RXIF     = 0;
    IFS2bits.U4TXIF     = 0;
    IEC2bits.U4RXIE     = 1;
    IEC2bits.U4TXIE     = 0;
    // Enable UART
    U4MODEbits.ON       = 1;
}

void uart_put_4(char c) {
    while (U4STAbits.UTXBF == 1);
    U4TXREG = c;
}

char uart_get_4(void) {
    while (!U4STAbits.URXDA);
    return U4RXREG;
}

void uart_puts_4(char *sz_data) {
    char *p_data = sz_data;
    while (*p_data) {
        uart_put_4((*(p_data++)));
    }
}

void uart_gets_4(void) {
    unsigned char c;
    while (U4STAbits.URXDA) {
        c = uart_get_4();
        strg[j++] = c;
        if (j > 0 && (c == '\r' || c == '\n')) {
            strg[j - 1] = 0;
            j = 0;
            flag_rx = 1;
        }
    }
}
