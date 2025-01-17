// Finger HeartBeat
//
// File:    main.h
// Authors: Luca Mazza

#include <p32xxxx.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#ifndef MAIN_H
#define MAIN_H

#define SYSCLK         80000000
#define PBCLK          40000000
#define READINGS_LEN   1000
#define PRESC          1/256
#define READ_BUFSIZE   10
#define BPM_BUFSIZE    9
#define BPM_BUF_MEDIAN (int)(BPM_BUFSIZE/2)
#define MAX_BPM_FADDR  0x00

void start_menu(void);
int  current_bpm(void);
void heart_beat(void);
void max_bpm(void);
void reset_max_bpm(void);
void invalid(void);

#endif
