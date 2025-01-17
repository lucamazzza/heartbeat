// Serial Peripheral Interface
//
// File:    spi.c
// Authors: Luca Mazza

#include "spi.h"
#include "timer.h"

void init_spi1(void) {
    TRISFbits.TRISF2 = 0;
    TRISFbits.TRISF6 = 0;
    TRISFbits.TRISF7 = 1;
    RPF2R            = 0x08;
    SDI1R            = 0x0f;
    TCS              = 0;
    CS               = 1;
    SPI1CON          = 0x8120;
    SPI1BRG          = 15;
}

void erase_flash(void) {
    // write enable
    CS = 0;
    write_spi1(0x06);
    CS = 1;
    sleep(10);
    //full erase CMD
    CS = 0;
    write_spi1(0x60);
    CS = 1;
    sleep(10);
//    // write disable
//    CS = 0;
//    write_spi1(0x04);
//    CS = 1;
}

// send one byte of data and receive one back at the same time
int write_spi1(int i) {
    while(!SPI1STATbits.SPITBE);	// wait for TX buffer to be empty
    SPI1BUF = i;                    // write to buffer for TX
    while( !SPI1STATbits.SPIRBF);   // wait for transfer complete
    return (int)SPI1BUF;            // read the received value
}

int get_flash_id(void) {
    //get ID = 15
    CS = 0;             // select the Serial EEPROM
    write_spi1(0xAB);   // get ID
    write_spi1(0);
    write_spi1(0);
    write_spi1(0);
    write_spi1(0);      // send dummy, read data  
    return SPI1BUF;
    CS = 1;
}

void write_flash(int addr, short byte) {
    short value = 0;
    int address = 0;
    value = byte;
    address = addr;
    // write enable
    CS = 0;
    write_spi1(0x06);
    CS = 1;
    // send a Write command
    CS = 0;                     // select the Serial EEPROM
    write_spi1(0x02);           // send command, ignore data Page Program
    write_spi1(address >> 16);   // send MSB of memory address
    write_spi1(address >> 8);    // send LSB of memory address
    write_spi1(address);         // send LSB of memory address
    write_spi1(value);           // send the actual data
    // send more data here to perform a page write
    CS = 1;                     // start actual EEPROM write cycle 
    // write disable
    CS = 0;
    write_spi1(0x04);
    CS = 1;
}

int read_flash(int addr) {
    int tmp=0;
     // send a read command
    CS = 0;                 // select the Serial EEPROM
    write_spi1(0x03);       // send command Read Data, ignore data
    write_spi1(addr >> 16); // send MSB of memory address
    write_spi1(addr >> 8);  // send LSB of memory address
    write_spi1(addr);       // send LSB of memory address
    tmp = write_spi1(0);    // send dummy, read data 
    CS = 1;                 // terminate the read sequence
    return tmp;
}
