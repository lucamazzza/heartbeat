// Finger HeartBeat
//
// File:    main.h
// Authors: Luca Mazza 

#include <p32xxxx.h>
#include <stdio.h>

#ifndef MAIN_H
#define MAIN_H

#define PBCLK        40000000
#define PRESC        1/256
#define HEART_BEAT   "1"
#define MAX_BPM      "2"
#define RESET        "3"
#define READ_BUFSIZE 10
#define BPM_BUFSIZE  9
#define BPM_BUF_MEDIAN (int)(BPM_BUFSIZE/2)


#include <p32xxxx.h>
#include <string.h>

#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "pmp.h"
#include "adc.h"
#include "oc.h"
#include "spi.h"
#include "ky39.h"

void start_menu(void);
void heart_beat(void);
void max_bpm(void);
void reset_max_bpm(void);

#endif
