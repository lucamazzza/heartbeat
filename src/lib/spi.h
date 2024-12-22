// Serial Peripheral Interface
//
// File:    spi.h
// Authors: Luca Mazza

#ifndef SPI_H
#define SPI_H

#define CS  LATFbits.LATF8;
#define TCS TRISFbits.TRISF8;

void init_spi1(void);
void erase_flash(void);
int  write_spi1(int i);
int  get_flash_id(void);
void write_flash(int addr, short byte);
int  read_flash(int addr);

#endif
