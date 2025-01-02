// Universal Asynchronous Receiver/Transmitter
//
// File:    uart.h
// Authors: Luca Mazza

#include <p32xxxx.h>

#ifndef UART_H
#define UART_H

extern char flag_rx;
extern char strg[80];
extern int  j;

void  uart_init_pins(void);
void  uart_init(int baud);
void  uart_put_4(char c);
char  uart_get_4(void);
void  uart_puts_4(char *sz_data);
void  uart_gets_4(void);
char *uart_get_string(void);

#endif
