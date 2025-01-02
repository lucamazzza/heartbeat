// Finger HeartBeat
//
// File:    main.h
// Authors: Luca Mazza 

#include <p32xxxx.h>

#ifndef MAIN_H
#define MAIN_H

#define PBCLK 40000000
#define PRESC 1/256

#include <p32xxxx.h>

#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "pmp.h"
#include "adc.h"
#include "oc.h"
#include "spi.h"
#include "ky39.h"

void start_monitoring(void);
void display_menu(void);
void reset_max_bpm(void);

#endif
