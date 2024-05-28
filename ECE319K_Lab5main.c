/* ECE319K_Lab5main.c
 * Digital Piano using a Digital to Analog Converter
 * December 28, 2023
 * 5-bit binary-weighted DAC connected to PB4-PB0
 * 4-bit keyboard connected to PB19-PB16
*/

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include "../inc/DAC5.h"  // student's Lab 5
#include "../inc/Key.h"   // student's Lab 5
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two lines
const char EID1[] = "dma3227"; // replace abc123 with your EID
const char EID2[] = "nnt479"; // replace abc123 with your EID
#define A7  11364   // 220 Hz (PB16) Key 0
#define DF0 9019   // 277.2 Hz (PB17) Key 1
#define E0  7584   // 329.6 Hz (PB18) Key 2
#define GF0 6757   // 370 Hz (PB19) Key 3
#define sound 756 //11 kilohertz frequency
const uint8_t SinWave[32] = {
16,19,22,24,27,28,30,31,31,
31,30,28,27,24,22,19,16,13,10,
8,5,4,2,1,1,1,2,4,5,8,10,13};
const uint8_t shoot[4080] = {16,12,12,20,26,16,4,13,25,14,7,11,17,27,18,7,10,19,22,12,9,16,25,16,6,11,15,25,20,9,10,16,24,14,8,15,25,16,8,13,17,21,13,6,15,24,16,9,13,26,29,11,5,10,20,14,4,10,18,25,22,24,23,14,11,1,0,10,20,13,15,29,31,31,14,0,0,0,0,0,11,29,31,31,31,31,23,0,0,0,2,21,20,17,31,31,31,22,4,0,0,2,0,7,20,31,31,31,31,31,7,0,0,5,7,5,16,26,31,31,31,18,6,1,0,0,1,16,24,25,29,31,31,25,0,0,0,2,2,4,16,31,31,31,31,31,17,0,0,0,9,8,9,21,31,31,31,28,17,6,0,0,1,13,9,15,25,31,31,31,18,8,1,0,0,0,16,20,19,27,31,31,31,10,0,0,0,0,0,10,26,27,28,31,31,31,14,0,0,0,9,8,6,15,25,31,31,31,27,14,6,0,0,0,14,16,12,22,31,31,31,27,16,9,0,0,0,12,12,12,21,31,31,31,31,23,14,0,0,0,5,15,14,15,27,31,31,31,29,12,6,0,0,0,2,16,25,20,24,31,31,31,24,2,0,0,0,0,5,19,25,23,25,31,31,31,18,9,2,0,0,0,14,17,14,22,31,31,31,29,26,15,0,0,0,4,10,10,15,25,31,31,31,31,31,16,0,0,0,3,4,4,14,22,31,31,30,31,31,27,4,0,0,0,7,5,7,16,26,31,30,31,31,31,27,4,0,0,0,1,4,12,21,28,27,28,31,31,31,17,5,0,0,0,0,1,12,23,23,21,29,31,31,31,25,6,0,0,0,0,0,8,18,28,23,23,30,31,31,29,17,5,0,0,0,0,3,14,25,20,20,27,31,31,31,25,17,8,1,0,0,0,11,15,14,19,22,31,31,31,28,29,27,10,0,0,0,0,3,6,16,25,24,21,29,31,31,28,25,17,9,0,0,0,0,11,13,14,22,27,25,26,31,31,31,24,14,5,1,0,0,0,11,12,14,20,26,30,28,26,31,31,27,18,8,3,0,0,0,5,12,14,17,24,26,24,27,31,31,28,22,17,10,2,0,0,0,8,9,13,19,24,29,26,25,28,31,31,25,17,9,4,0,0,0,3,12,13,16,21,24,30,27,26,28,31,31,24,14,4,1,0,0,0,3,15,16,17,21,23,27,30,27,26,28,31,27,17,6,0,0,0,0,0,8,19,21,20,21,21,26,30,27,26,27,30,25,15,6,1,0,0,0,0,9,19,20,19,21,25,26,24,24,27,30,29,23,17,13,6,0,0,0,2,8,10,16,20,25,26,21,19,23,29,28,25,24,25,25,13,2,0,0,0,0,2,11,20,21,20,20,23,26,22,21,25,26,29,26,21,19,14,8,0,0,0,0,2,6,14,18,24,26,22,19,21,24,23,21,24,26,25,20,16,10,7,0,0,0,0,7,9,13,18,22,26,24,20,21,23,24,22,22,25,26,21,18,16,11,7,0,0,0,0,8,11,13,19,24,24,22,20,21,24,21,19,22,23,26,25,19,19,17,12,4,0,0,0,1,2,9,14,20,25,22,21,22,24,20,17,18,22,25,24,21,22,22,19,16,12,7,4,0,0,0,3,9,13,15,19,22,25,21,20,20,22,21,17,19,22,24,22,20,20,21,21,16,10,5,4,2,0,0,3,7,15,17,18,21,21,24,22,20,19,19,21,18,16,20,22,23,21,19,20,20,17,13,10,7,2,0,1,3,6,7,10,16,21,22,21,22,23,22,19,18,18,19,18,16,18,20,23,21,19,19,19,19,16,13,12,8,3,0,1,3,6,9,10,13,18,20,23,23,21,21,20,21,20,17,17,18,17,16,17,19,21,21,19,19,18,18,18,16,13,11,6,5,4,2,3,5,8,10,12,15,18,22,22,22,21,21,21,20,17,17,18,18,17,15,16,17,18,19,19,20,20,18,17,16,16,16,14,13,11,9,5,3,4,5,7,7,9,11,14,18,19,20,21,22,22,20,20,19,19,19,17,17,17,17,16,16,16,17,17,17,17,18,20,19,18,17,16,16,16,14,14,14,14,11,8,6,5,6,6,7,8,10,13,13,14,16,19,21,21,21,21,21,20,19,19,19,18,17,17,17,17,17,16,16,16,17,16,16,16,16,17,17,17,18,18,18,16,16,16,15,16,14,13,14,15,14,10,5,1,0,1,6,12,17,20,24,31,31,21,10,7,7,13,27,25,10,9,8,14,29,25,10,8,9,15,29,25,10,8,8,14,29,25,9,8,8,14,29,25,9,8,7,13,30,26,9,7,7,12,30,26,10,7,7,12,28,27,10,6,7,11,27,29,12,6,7,10,25,30,14,6,7,9,22,31,17,6,7,8,20,31,20,7,6,7,17,31,23,7,6,7,14,31,26,9,6,7,12,28,30,12,5,6,9,24,31,16,6,7,8,20,31,21,6,7,7,15,31,26,8,6,6,12,29,30,12,5,7,9,23,31,18,5,6,7,17,31,24,7,6,6,12,30,30,11,5,6,8,23,31,18,5,6,7,16,31,26,8,5,6,11,27,31,14,5,6,7,20,31,23,6,5,6,12,30,30,11,5,6,8,21,31,21,6,5,6,14,31,29,10,5,6,9,21,31,20,6,6,7,13,31,30,11,5,6,9,21,31,22,6,5,6,12,29,31,13,5,6,8,18,31,25,8,5,6,11,25,31,17,5,6,7,15,31,29,10,5,6,8,20,31,23,6,5,6,11,26,31,16,5,6,7,15,31,29,11,5,6,8,19,31,25,7,5,6,10,23,31,20,5,6,7,12,27,31,15,5,6,7,15,31,30,11,4,6,8,17,31,27,8,5,6,9,20,31,24,7,5,6,10,22,31,21,6,5,7,11,24,31,19,6,6,7,12,26,31,17,5,6,7,14,27,31,15,5,6,8,14,28,31,15,5,6,8,14,29,31,15,6,6,8,14,28,31,14,5,6,8,14,28,31,16,5,6,8,14,27,31,16,6,6,8,13,25,31,19,6,6,7,12,23,31,21,6,6,7,11,21,31,24,7,5,5,9,18,31,25,8,4,6,9,17,31,28,10,5,6,9,15,28,31,14,5,6,8,13,24,31,19,6,6,7,11,21,31,24,8,5,7,10,17,31,29,11,6,6,9,14,26,31,18,6,6,8,12,20,31,24,8,6,7,10,16,30,31,13,5,7,9,13,23,31,21,7,6,7,11,18,31,30,11,6,6,9,14,23,31,20,6,6,8,11,17,30,30,12,6,6,9,13,22,31,22,7,6,6,10,16,28,31,14,6,6,8,13,20,31,26,8,6,6,10,14,24,31,19,7,6,7,11,17,29,31,13,6,6,8,12,19,31,26,9,6,6,10,14,22,31,23,7,6,6,11,15,25,31,19,6,6,7,12,16,27,31,15,6,6,8,12,17,29,29,12,6,6,9,12,18,31,28,10,6,6,9,12,19,31,27,10,6,6,10,13,19,31,27,9,6,6,9,13,19,31,27,10,6,6,9,13,18,31,28,11,6,6,9,13,18,30,29,13,6,6,9,12,17,28,30,15,6,6,8,12,16,25,31,18,6,6,7,11,15,23,31,21,7,6,7,10,14,20,31,26,9,6,6,9,13,18,30,29,13,7,7,8,13,16,25,31,19,7,6,7,12,14,21,31,25,9,6,6,10,14,17,28,31,16,7,6,8,12,15,22,31,24,8,6,7,11,13,17,28,30,15,7,7,8,12,14,21,31,24,9,6,7,10,14,16,26,31,18,7,6,7,11,14,19,31,28,12,6,7,9,12,15,22,31,24,8,6,7,10,14,16,25,31,20,7,6,7,11,14,17,27,31,17,7,6,8,11,14,19,29,29,14,6,7,8,12,14,19,30,27,12,5,6,8,12,14,19,30,27,11,6,7,8,13,15,19,30,27,12,6,7,8,13,15,19,30,28,13,7,6,8,12,15,19,28,29,16,7,7,8,12,14,17,26,31,18,7,6,8,11,14,16,24,31,22,8,6,8,10,13,15,21,31,26,11,7,7,9,13,15,19,27,30,17,7,6,8,11,14,16,23,31,23,9,6,7,10,13,15,19,28,30,16,7,7,8,11,14,16,22,31,24,9,6,7,9,13,15,18,26,31,19,7,6,8,11,14,15,20,29,27,14,7,6,8,12,15,16,22,31,25,10,6,7,9,12,15,17,24,31,22,8,6,7,10,13,15,17,24,31,21,8,6,7,10,13,15,17,25,31,20,8,6,7,10,14,16,18,24,31,21,8,6,7,10,13,15,17,23,31,22,9,6,7,10,13,15,17,21,30,25,11,7,7,9,12,14,16,20,28,29,16,7,6,8,11,14,15,18,24,31,21,8,6,7,10,13,16,16,20,30,27,12,7,6,8,12,14,16,18,24,30,21,8,6,7,10,13,15,16,20,28,28,15,7,6,8,11,14,16,17,21,30,25,11,7,6,9,12,14,16,18,23,30,23,9,6,7,10,13,14,16,17,24,31,21,8,6,7,10,13,15,17,17,24,31,21,9,6,7,10,13,14,17,17,23,30,23,9,6,8,9,12,14,16,17,21,29,25,12,6,6,9,12,14,16,17,20,27,28,16,7,6,8,11,14,15,17,18,24,30,22,9,7,7,9,13,14,16,17,20,27,28,16,8,6,8,11,14,16,17,17,23,29,24,11,7,7,9,12,14,16,17,18,24,30,22,9,6,7,8,12,14,16,16,17,24,29,20,8,6,7,9,12,14,16,17,18,24,29,21,8,7,7,10,12,14,16,17,17,23,29,23,10,6,7,9,12,14,16,17,17,21,28,26,15,7,6,8,11,14,15,16,17,19,24,29,20,9,7,7,10,13,15,16,17,17,20,27,26,16,7,6,8,11,14,15,16,17,18,21,28,25,13,7,6,9,11,14,15,17,17,17,22,28,24,12,7,6,9,11,14,16,17,17,17,21,27,25,14,7,6,9,11,14,15,16,17,17,20,25,27,18,8,6,7,10,13,15,16,17,17,18,22,28,23,11,6,6,9,12,14,16,17,17,17,19,24,28,19,8,6,7,9,13,14,16,16,17,18,20,25,27,18,8,6,8,10,13,15,16,17,17,17,19,25,27,19,8,6,7,9,12,15,16,17,17,17,19,23,27,22,11,7,6,8,11,14,15,17,17,17,17,20,25,26,17,8,6,8,10,12,14,16,16,17,17,18,21,26,24,15,7,6,8,11,14,14,16,17,17,17,17,20,25,25,16,7,6,8,10,13,15,16,17,17,17,17,19,24,26,18,8,6,7,10,12,14,16,17,17,17,17,18,21,25,24,14,7,6,9,11,13,15,16,17,17,17,18,18,21,26,22,13,7,6,8,11,13,15,16,17,17,17,17,18,21,25,24,16,7,6,8,10,13,15,16,17,17,17,17,17,18,22,25,20,11,6,6,9,12,14,16,16,17,17,17,17,17,19,22,25,19,10,7,6,9,12,14,16,17,17,17,18,17,17,18,21,24,22,13,7,6,8,11,13,15,16,17,17,17,17,17,17,19,22,25,19,10,6,6,8,11,13,15,16,16,17,17,17,16,16,17,22,24,20,12,7,6,9,11,14,15,16,17,17,17,17,17,17,16,19,23,24,17,9,7,7,10,12,14,16,17,17,17,17,17,17,16,16,19,22,24,19,10,7,7,9,12,14,16,17,17,17,17,17,16,17,17,17,20,22,22,17,9,6,8,10,12,14,16,17,17,17,17,17,17,16,16,16,18,21,23,20,12,7,6,9,12,14,15,16,17,17,17,17,17,16,17,16,16,18,21,23,19,12,7,6,9,11,13,15,16,17,17,17,17,17,16,17,16,16,16,19,21,22,18,10,7,7,9,11,14,16,16,17,17,17,17,17,16,17,16,16,16,17,20,22,22,16,9,6,7,10,12,14,16,16,17,17,17,17,16,16,17,16,16,16,16,17,20,22,20,14,8,6,8,10,13,14,16,16,17,17,17,17,17,16,16,16,16,16,15,16,18,20,22,19,13,8,7,8,11,13,15,16,16,17,17,17,17,17,16,16,16,16,15,16,16,15,18,20,21,19,14,9,7,8,10,13,14,16,17,17,17,17,17,17,16,16,16,16,16,16,16,15,16,17,19,20,21,17,11,8,7,9,11,13,15,16,17,17,17,17,17,16,17,16,16,16,16,16,16,15,16,15,16,18,20,21,20,14,10,7,8,10,12,15,16,16,17,17,17,17,17,16,16,16,16,16,16,16,15,16,16,15,15,16,17,19,20,21,18,12,9,7,9,11,13,15,16,17,17,17,17,17,16,17,16,16,16,16,16,16,16,16,15,16,15,16,16,16,16,18,19,20,20,15,11,8,8,9,11,14,15,16,17,17,17,17,17,16,16,16,16,16,15,16,16,15,15,15,15,15,15,16,15,15,15,15,16,17,19,19,19,16,12,9,7,9,11,13,15,16,17,17,17,17,17,17,17,16,16,16,16,16,16,16,16,15,16,15,15,16,16,15,15,15,16,16,16,15,16,16,17,18,18,19,18,15,12,10,8,8,11,13,15,16,17,17,17,17,17,17,17,16,16,16,16,16,16,16,16,16,16,15,15,16,16,16,16,15,16,15,15,15,15,15,15,15,16,16,16,16,16,16,17,17,18,18,17,16,13,11,10,8,9,12,13,15,16,17,17,17,17,17,17,17,16,19,21,14,12,14,14,14,16,20,15,12,16,14,16,20,14,12,16,13,16,20,14,13,16,13,16,19,15,12,16,14,16,19,15,12,16,13,16,20,14,13,16,13,16,20,14,12,16,13,16,20,14,13,16,13,16,19,14,14,16,13,15,19,15,14,16,13,15,18,16,15,15,13,15,17,18,15,14,14,14,16,19,15,13,15,14,16,19,15,12,16,13,15,20,14,13,16,13,15,19,15,14,15,13,15,17,18,15,13,15,14,16,20,14,12,16,13,15,20,14,13,16,13,15,18,16,15,14,14,15,16,19,16,13,15,13,16,20,14,13,16,13,15,18,16,16,14,14,14,16,19,15,13,16,13,16,19,15,14,15,14,15,17,18,15,13,15,14,16,20,14,14,16,13,15,17,17,16,14,14,14,16,20,15,13,15,14,15,18,17,15,14,15,14,16,20,14,13,16,14,15,17,17,16,14,14,14,16,19,14,14,15,14,15,16,19,16,13,15,14,16,19,15,15,15,14,15,16,20,15,13,15,14,15,16,17,15,13,14,14,15,19,15,14,15,14,15,16,20,14,13,15,14,15,16,18,15,14,15,14,15,18,16,15,14,14,14,16,19,14,14,15,14,15,16,19,15,13,15,14,15,17,19,15,13,15,14,15,17,17,16,14,14,14,16,18,16,15,14,14,15,16,18,16,15,15,14,14,16,19,15,15,15,14,15,16,19,15,14,15,14,15,16,19,14,14,15,14,15,16,19,14,14,15,14,15,16,19,14,14,15,14,15,16,19,14,14,15,15,15,16,19,15,15,15,14,15,16,18,15,15,15,14,15,15,17,16,15,15,14,15,15,17,17,15,14,14,15,15,16,18,15,14,14,15,15,16,19,15,14,15,14,14,16,19,14,14,15,14,15,16,18,15,15,15,14,15,15,17,17,15,14,14,15,15,16,19,15,14,15,15,15,16,19,15,14,15,14,15,15,17,17,15,14,14,15,14,16,19,14,14,15,14,15,15,17,16,15,15,14,15,15,16,19,15,14,15,14,15,16,18,15,15,15,14,15,15,16,19,14,14,15,14,15,16,17,16,15,15,14,15,15,16,19,14,14,16,14,15,15,17,17,15,15,14,15,15,16,18,15,15,15,14,16,15,16,19,15,14,15,14,16,15,16,17,16,15,14,15,15,16,17,15,15,15,14,16,14,16,18,14,15,15,14,16,15,16,19,15,15,15,14,16,16,16,18,15,15,15,15,16,15,16,17,15,15,14,15,15,16,17,17,16,15,15,15,15,15,17,17,15,15,15,15,15,16,16,17,16,15,15,15,16,16,16,17,15,15,15,15,15,15,16,17,16,15,15,15,15,15,16,17,16,14,14,14,15,15,16,17,14,14,15,14,15,15,16,17,14,15,16,14,16,15,16,17,15,15,15,14,16,15,16,16,16,15,15,15,15,16,16,16,17,15,15,15,15,16,15,15,17,15,15,15,15,16,16,16,16,16,15,15,15,15,16,15,16,17,14,15,15,15,16,16,16,16,16,15,15,15,15,16,16,16,17,14,15,15,15,16,16,16,16,17,15,15,15,15,15,15,16,17,15,15,15,15,16,15,16,16,17,14,15,15,15,15,16,16,16,17,15,15,15,15,16,16,16,16,16,15,15,15,15,15,15,15,16,15,15,15,15,15,16,16,16,17,15,15,15,15,15,16,16,16,17,15,15,15,15,15,16,16,16,16,15,15,16,14,15,16,16,16,16,15,15,16,15,15,15,15,16,16,15,15,15,14,16,16,15,16,16,16,16,15,15,15,15,15,15,16,16,15,15,15,15,15,16,16,16,17,15,15,16,15,16,16,15,15,16,15,15,15,15,15,15,15,15,15,16,15,15,15,15,15,16,16,16,16,15,15,15,15,16,15,16,16,16,16,15,15,15,15,15,16,15,16,16,15,15,15,15,16,16,16,16,16,16,15,15,15,15,15,15,15,15,16,16,15,15,15,15,15,15,15,15,16,16,15,15,15,15,15,15,15,15,16,16,15,15,15,15,15,15,15,15,16,15,15,15,15,15,15,15,15,16,16,15,16,15,15,16,15,15,15,15,16,16,15,15,15,15,15,15,15,16,16,16,15,15,15,15,15,15,16,15,16,16,15,15,16,15,15,15,16,15,16,16,15,15,16,15,16,15,15,15,16,16,16,15,15,15,15,15,15,15,15,16,16,15,15,15,15,15,15,15,16,15,16,16,16,15,16,15,16,16,16,16,16,16,16,15,16,15,15,15,15,15,15,15,16,15,15,15,15,15,15,15,16,16,16,16,15,15,15,15,15,15,15,15,15,16,16,15,15,15,15,16,15,15,16,15,16,16,15,15,15,15,15,15,15,16,15,15,16,15,15,15,15,16,15,15,15,15,16,15,16,15,16,16,15,15,16,15,16,15,16,16,15,15,15,15,16,15,15,15,15,16,16,16,15,16,15,15};
// prototypes to your low-level Lab 5 code
const uint8_t fastinvader2[1042] = {16,16,14,11,9,6,4,2,1,2,4,6,8,11,13,16,17,20,22,23,25,26,28,29,31,31,31,31,31,31,31,31,29,29,28,27,26,25,24,22,22,21,20,18,17,16,16,15,13,12,11,10,9,8,7,6,6,9,9,10,11,11,11,11,10,9,8,7,4,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,5,6,8,9,11,12,14,15,16,17,19,20,22,22,24,25,26,28,29,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,29,29,27,26,26,25,24,24,23,22,22,21,20,20,19,17,17,17,16,16,16,15,14,14,13,13,11,11,11,10,9,9,8,8,7,7,6,6,6,5,5,4,4,4,4,3,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,9,10,11,13,13,15,16,16,16,17,16,17,17,17,16,17,16,16,16,14,14,13,13,12,11,11,10,9,9,8,7,7,6,5,4,4,4,5,6,9,9,11,11,11,11,11,9,9,7,6,5,4,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,4,5,6,7,9,10,12,13,15,16,17,18,20,22,23,24,26,27,29,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,29,29,27,26,26,25,24,24,23,22,22,20,19,18,17,17,16,16,15,14,14,13,12,11,11,10,9,9,9,7,6,6,6,5,4,4,3,3,3,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,7,9,11,11,12,13,14,14,15,15,16,15,15,14,14,13,13,12,11,11,10,9,8,7,6,6,6,4,4,3,2,2,1,0,0,0,0,1,3,4,5,6,6,6,6,5,4,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,4,5,6,8,10,11,13,15,16,17,19,20,22,23,25,26,28,29,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,29,29,28,27,26,25,24,24,23,22,22,22,20,20,19,17,17,16,16,16,16,14,13,13,13,11,11,9,9,8,7,7,6,6,6,6,5,5,4,4,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,6,8,10,11,13,15,16,16,16,16,17,17,17,17,17,17,17,16,16,16,14,14,13,12,11,11,10,10,9,7,6,6,5,4,4,3,2,3,2,3,5,6,7,8,8,8,9,8,6,6,5,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,5,6,7,9,11,13,14,16,16,17,20,21,23,24,26,26,28,29,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,29,29,27,26,26,24,24,22,22,22,20,20,19,18,17,17,16,16,16,14,14,13,12,12,11,11,10,10,9,9,8,7,6,6,6,5,5,4,4,4,3,2,3,2,2,2,2,2,2,2,2,1,1,1,1,2,1,1,2,1,0,1,2,2,1,2,1,2,2,2,2,2,3,3,2,4,4,4,4,4,4,4,5,5,6,6,6,6,6,7,7,8,8,9,9,9,9,10,10,11,11,11,12,11,12,13,13,13,14,13,14,14,15,15,16,16,16,16,16,16,17,16,17,17,17,17,17,18,19,19,19,18,19,20,19,20,19,20,20,20,20,21,20,20,21,20,21,21,20,20,21,21,22,21,22,21,22,22,21,22,22,21,22,21,22,22,21,21,22,21,22,22,22,22,22,21,22,22,21,22,21,22,22,21,21,22,22,21,20,21,22,21,20,20,21,20,21,20,20,20,20,20,19,18,18,19,18,17,17,18,17,17,18,17,17,17,17,17,17,17,17,17,16,16,16,17,16,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,15,16,15,16,15,15,15,14,15,14,14,15,15,14,15,14,13,14,14,13,13,14,13,14,13,14,13,13,13,14,13,14,13,14,13,13,14,13,13,13,13,13,14};
const uint8_t fastinvader3[1054] = {16,16,16,16,16,16,16,14,11,9,6,4,2,2,2,4,6,9,11,13,16,18,20,22,24,26,27,29,29,31,31,31,31,31,31,31,31,30,30,29,28,26,26,24,24,22,21,20,19,17,17,16,15,13,13,11,10,9,9,8,6,7,8,9,10,11,11,11,11,11,9,7,6,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,5,6,7,9,11,13,14,15,16,17,19,20,22,23,24,26,26,28,29,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,29,29,29,27,26,25,24,24,22,22,21,21,20,19,18,17,16,16,16,15,15,14,13,13,12,11,11,11,9,9,8,8,7,7,6,5,4,4,3,3,2,3,2,2,2,2,2,2,1,1,2,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,9,11,13,16,16,17,19,20,21,21,22,23,23,22,23,22,22,22,21,21,20,20,19,19,17,16,16,16,15,15,13,13,11,11,11,9,9,9,9,11,12,13,15,15,16,15,14,13,13,11,10,9,7,6,4,4,4,2,2,1,1,0,0,0,1,0,2,2,2,4,4,5,6,8,9,11,12,13,15,16,17,19,20,22,23,24,26,27,28,29,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,29,28,26,26,25,24,23,23,22,20,20,19,17,17,16,16,15,14,13,12,11,11,10,9,9,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,7,9,11,13,14,16,16,16,17,17,18,18,19,18,18,17,17,17,16,16,16,15,14,13,13,12,11,11,10,9,9,8,7,6,6,5,5,5,6,8,9,11,11,12,12,11,11,9,8,6,5,4,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,6,7,9,10,11,13,15,16,17,18,20,22,23,24,26,28,29,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,29,29,28,26,26,25,24,24,22,22,21,20,19,18,17,17,16,16,16,14,13,13,13,12,11,10,10,9,9,8,7,6,6,6,5,5,4,4,4,3,3,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,2,1,2,2,2,2,2,3,4,3,4,4,5,4,5,6,6,6,6,6,7,7,8,8,9,9,9,9,10,11,11,11,11,11,12,13,13,13,13,14,15,15,14,16,16,16,16,16,16,16,17,17,17,17,17,17,18,17,18,18,19,18,19,19,20,20,20,20,20,20,21,20,21,21,22,22,22,21,22,21,22,22,22,22,22,22,22,22,23,22,22,22,22,22,23,22,23,22,23,23,22,22,23,22,23,22,22,22,22,22,22,22,22,22,22,21,22,21,21,22,21,20,20,21,20,21,21,20,21,20,20,20,20,20,20,20,19,20,19,19,19,19,18,19,18,19,18,17,17,18,17,17,17,17,17,16,16,17,16,16,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,15,15,16,15,16,15,16,15,14,14,15,14,15,14,13,14,14,13,13,14,13,13,14,13,13,13,13,13,13,13,13,13,13,13,13,12,13,12,13,12,13,12,13,13,13,13,13,13,13,13,12,13,13,13,12,13,13,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,13,14,13,14,14,13,14,13,13,14,13,14,14,14,14,15,14,14,15,14,15,15,14,14,15,14,15,14,13,14,13,14,14,14,15,15,14,15,14,15,14,15,16,15,16,16,16,15,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,17,16,17,16,17,17,17,16,17,17,17,16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17};
const uint8_t invaderkilled[3377] = {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,20,18,13,11,11,12,13,15,16,13,13,15,19,19,14,12,17,23,13,10,19,25,14,2,12,25,23,8,12,23,21,11,0,13,31,17,2,17,29,12,0,16,31,16,0,13,28,14,0,18,31,11,1,19,31,13,0,7,21,31,10,3,25,21,5,7,24,22,3,13,30,14,1,12,26,26,5,9,29,14,3,7,21,31,11,0,18,31,17,1,7,21,31,10,0,21,30,5,6,25,23,9,0,17,31,18,2,8,23,26,4,10,27,20,9,0,16,31,15,0,18,31,12,0,17,30,21,8,0,20,31,7,3,24,28,4,9,27,20,8,0,18,31,12,0,20,31,11,0,20,31,11,0,16,30,23,9,0,15,31,16,0,11,24,27,11,0,15,31,19,5,6,26,22,2,12,26,24,6,5,24,26,9,0,16,31,16,1,13,27,22,2,13,31,13,0,13,28,25,5,6,22,30,9,0,22,30,5,6,24,27,10,0,13,31,22,4,12,31,12,0,14,25,30,10,0,17,31,15,0,17,31,15,5,2,20,31,18,2,7,22,31,7,3,24,27,11,0,16,31,12,0,22,31,9,1,22,31,10,0,20,31,13,0,13,31,11,0,15,28,26,7,3,22,31,9,0,14,31,16,0,16,26,23,8,0,20,31,11,0,17,31,15,4,6,23,31,8,3,25,24,6,4,25,24,5,8,23,31,7,1,23,27,10,0,17,31,16,3,11,25,25,6,3,23,31,11,0,17,31,10,0,23,31,7,2,25,27,5,7,26,23,4,9,27,21,3,11,26,23,9,0,18,31,14,0,21,31,10,0,14,29,20,3,13,25,26,10,0,16,31,19,5,7,26,20,1,18,28,19,6,0,22,31,8,1,24,28,6,4,23,30,10,0,14,29,26,5,11,26,19,6,0,24,26,7,4,23,31,8,1,20,31,15,0,14,29,11,1,16,28,23,2,15,28,15,5,2,22,31,13,0,17,31,10,0,19,31,16,3,12,26,22,8,1,20,31,13,1,12,22,31,12,0,15,28,19,5,10,26,22,7,4,21,31,14,2,6,20,31,12,0,21,29,14,2,12,29,15,2,19,31,11,0,20,31,15,2,12,26,21,7,2,21,31,11,0,20,28,21,6,2,23,28,9,0,18,31,15,4,15,27,18,4,10,28,15,2,16,26,24,5,8,25,22,8,0,20,31,12,0,17,26,21,9,0,21,31,15,2,13,26,20,8,0,22,31,9,3,24,30,8,0,21,31,11,0,20,29,14,5,5,24,29,8,1,21,31,13,0,18,27,17,3,11,24,24,8,0,23,29,8,1,22,31,11,0,21,30,8,3,23,29,10,0,16,30,12,3,16,27,24,6,6,24,25,7,2,24,28,7,2,21,31,12,0,19,29,10,0,18,25,25,9,0,23,28,7,3,23,31,10,0,18,29,16,6,5,21,31,11,0,18,26,18,8,1,23,31,7,2,22,31,14,2,6,22,29,8,0,23,30,10,0,15,28,21,7,7,24,30,7,1,23,30,8,0,22,31,14,5,5,23,31,8,0,23,31,16,5,3,22,31,10,0,18,26,23,10,0,22,31,10,1,19,29,11,1,22,30,12,2,14,28,13,4,15,26,25,5,10,26,19,6,6,26,21,4,14,27,16,3,18,29,11,4,15,25,26,7,0,22,31,10,0,20,28,12,3,11,26,24,5,13,26,20,8,0,23,31,13,5,6,25,28,7,2,22,31,12,2,12,23,27,7,0,23,31,10,3,10,23,31,7,0,23,29,8,0,23,29,7,3,25,26,7,4,22,31,8,1,24,27,5,4,25,25,5,8,26,21,6,6,24,31,7,1,20,30,11,2,16,24,26,7,0,21,31,15,4,13,25,18,5,10,25,23,5,9,26,19,6,9,23,30,9,0,21,29,8,2,21,31,10,4,15,24,30,8,0,22,28,8,1,19,29,16,5,13,23,27,8,0,21,28,18,6,5,24,26,8,0,18,30,20,5,15,27,13,6,5,23,31,7,2,21,29,17,3,11,24,21,5,3,24,29,8,1,17,29,18,4,19,29,10,5,10,23,31,7,0,23,28,14,5,5,24,30,7,0,24,31,8,2,23,28,10,1,13,25,23,6,5,25,26,6,3,25,29,6,2,23,31,14,3,5,23,31,6,0,18,29,20,4,16,27,14,4,11,26,20,4,12,28,13,3,22,28,13,4,13,26,18,4,16,28,10,2,23,29,12,5,9,24,31,5,1,25,29,6,0,19,30,16,5,13,23,30,5,0,21,27,17,5,1,24,31,7,0,21,26,21,6,2,26,25,4,9,26,22,5,6,27,21,4,11,24,30,8,0,21,28,6,2,18,30,18,5,11,24,30,7,0,16,26,18,3,17,27,15,5,5,26,29,4,8,27,19,5,11,25,25,5,2,26,31,9,0,19,29,9,4,14,26,26,4,8,25,24,5,0,25,29,7,0,21,30,10,5,17,26,27,5,0,22,29,16,2,12,25,20,6,0,25,31,5,4,27,27,5,2,23,31,8,3,16,27,20,2,16,25,17,4,4,26,27,6,1,26,29,4,7,25,27,6,0,23,28,17,4,8,28,17,5,10,26,26,3,10,26,20,4,6,25,29,4,0,27,26,5,4,25,31,6,0,21,28,12,3,18,26,17,3,11,25,21,5,3,27,25,3,11,27,22,6,1,24,31,12,3,18,27,11,4,12,26,26,5,3,26,31,8,0,19,27,11,4,16,27,20,4,9,26,24,5,4,28,24,4,10,27,18,3,16,25,18,5,6,28,20,5,8,25,31,5,3,26,27,4,0,21,29,13,3,20,25,22,5,0,25,30,11,3,14,27,14,4,17,27,17,6,6,25,31,7,0,22,26,16,5,0,26,31,5,4,21,28,20,4,5,26,25,2,10,28,14,6,10,26,31,3,4,28,23,5,4,23,31,10,1,20,27,9,4,13,27,25,4,8,27,26,4,2,24,29,17,5,0,24,31,5,4,19,28,20,4,10,27,20,5,6,25,30,5,2,21,27,21,4,3,27,24,4,9,26,29,5,0,25,29,6,4,26,26,4,3,26,30,9,2,19,27,7,3,26,29,8,2,17,27,11,2,23,28,10,5,10,25,31,4,0,22,25,19,4,6,27,22,5,5,27,28,3,8,27,23,5,1,25,31,8,5,13,25,31,3,0,26,28,10,2,17,26,16,6,3,25,31,8,5,26,27,5,3,14,26,29,3,2,27,27,6,1,19,28,15,6,14,27,20,3,15,26,16,5,3,25,31,9,3,21,25,15,5,3,25,31,8,4,25,27,5,2,20,27,11,4,22,28,8,5,16,26,26,3,2,28,23,3,10,26,23,4,4,28,25,4,9,28,21,5,7,24,30,8,2,19,25,15,5,9,27,23,3,13,25,21,5,1,26,30,7,5,25,26,5,2,23,27,9,5,16,28,19,5,9,25,29,7,1,21,24,5,5,18,28,21,5,9,25,28,3,1,26,25,5,5,26,27,4,4,26,26,4,3,25,27,16,5,4,28,26,4,8,27,24,4,6,24,28,14,3,11,24,21,4,6,28,22,5,7,24,29,16,5,5,26,28,3,5,26,26,5,2,20,27,13,7,12,26,30,5,3,13,24,29,4,2,26,26,7,5,15,26,25,3,9,26,20,4,8,27,21,5,8,24,29,11,5,10,24,27,3,3,26,25,8,5,18,26,12,5,23,26,11,7,8,26,29,8,6,9,25,30,3,5,18,25,27,4,2,19,27,18,5,14,25,21,4,3,24,28,10,6,19,24,15,5,12,27,16,5,21,24,15,6,5,23,29,10,6,22,25,8,5,20,24,20,4,4,28,22,5,11,27,21,5,12,26,21,5,5,24,27,6,7,17,26,22,3,13,24,16,6,6,23,28,14,5,17,25,11,6,21,24,14,6,8,26,26,4,4,24,26,9,6,24,24,6,5,20,25,10,6,23,25,10,6,16,23,19,5,4,26,27,10,7,12,25,24,4,6,27,23,5,9,26,24,4,5,22,26,12,7,11,25,28,6,5,20,24,11,5,22,24,11,7,8,26,26,5,9,26,23,4,5,17,26,25,4,6,26,23,5,5,22,26,11,8,12,25,27,4,7,25,23,5,4,20,25,16,7,14,25,21,5,8,27,20,6,10,25,26,6,5,20,24,15,7,8,23,27,8,7,25,22,5,6,25,24,7,6,18,25,14,7,18,25,17,6,13,25,18,5,12,25,19,6,11,25,21,4,13,25,14,8,10,24,26,7,7,15,24,22,4,11,25,17,6,13,24,21,5,7,27,20,6,11,25,23,5,6,18,26,22,4,8,25,23,5,5,25,22,6,9,16,27,23,4,14,23,18,5,5,24,25,8,7,19,24,14,7,13,25,21,5,8,25,24,5,9,25,22,5,6,22,24,13,8,14,24,21,5,6,20,26,15,6,21,22,11,7,11,26,19,6,12,25,23,5,6,21,25,14,8,11,22,26,7,7,24,21,7,7,14,25,23,5,9,26,21,5,7,24,24,8,8,20,24,13,7,20,22,10,8,13,24,23,5,6,23,24,15,8,10,25,25,6,6,24,22,10,9,12,26,23,6,10,25,23,5,7,21,23,10,8,24,21,5,10,25,21,6,8,23,24,12,8,10,25,24,5,8,15,25,22,6,10,24,23,6,7,21,22,10,8,23,22,10,8,17,24,13,9,13,24,25,7,7,18,22,13,7,20,21,13,8,10,25,23,7,10,24,21,7,10,17,25,21,3,6,22,22,9,10,12,24,25,6,9,13,24,23,6,9,17,25,16,7,11,23,23,8,8,14,24,18,6,16,22,17,7,8,25,24,9,8,19,22,11,9,19,23,13,9,12,22,25,8,9,23,21,8,8,20,21,11,10,13,23,25,8,9,24,21,8,9,13,24,25,8,9,15,23,21,5,9,19,24,20,6,8,25,20,8,11,18,26,17,8,11,22,23,9,8,13,23,22,5,9,24,21,7,10,18,24,21,6,8,23,22,9,10,23,19,7,10,24,21,7,10,22,22,9,9,19,22,13,8,18,22,13,9,14,23,22,6,9,20,22,12,9,17,22,18,6,11,24,19,7,10,24,22,8,10,24,18,8,11,20,23,13,9,13,23,20,6,10,23,22,8,10,14,24,23,6,8,18,22,14,8,20,20,13,9,11,24,22,7,10,23,20,7,10,21,22,12,9,19,21,15,8,14,23,14,9,13,21,24,14,7,14,22,15,9,12,23,22,7,11,24,20,8,9,18,22,15,8,20,20,10,10,19,22,13,10,13,21,22,8,10,18,21,18,6,9,23,21,12,10,12,22,22,9,10,21,21,10,10,16,23,20,6,9,20,22,13,9,11,22,22,10,10,17,22,14,10,13,21,23,10,10,14,23,21,7,11,23,20,7,9,19,22,13,10,17,22,18,7,10,21,22,14,9,11,23,21,8,10,15,23,22,6,10,20,21,11,10,22,19,10,10,13,23,20,8,12,21,21,13,9,12,23,20,8,12,22,20,9,11,22,20,9,10,19,21,15,9,14,23,16,10,14,23,19,7,10,16,24,19,8,13,22,19,7,10,19,22,15,9,11,18,24,16,8,10,19,22,13,9,15,22,16,9,13,22,19,8,11,21,21,13,9,12,22,21,9,11,22,19,9,11,16,22,20,7,11,23,19,8,10,21,19,10,12,18,23,19,8,10,22,20,10,11,17,21,15,10,13,22,20,9,11,19,21,13,10,16,22,18,8,11,17,23,20,7,10,22,18,8,11,19,21,13,11,19,19,14,10,15,22,15,10,15,22,18,8,11,21,21,10,11,18,21,16,8,13,22,16,9,12,17,24,17,9,15,20,16,8,10,20,21,13,11,16,21,16,9,13,23,17,9,13,21,20,10,11,17,21,15,10,13,18,23,16,9,11,21,19,9,12,15,23,19,8,11,17,22,17,9,12,21,19,9,12,19,20,15,9,14,21,17,8,12,22,18,9,13,22,18,9,12,19,20,12,12,20,19,12,10,17,20,12,12,14,19,22,11,11,16,21,19,8,10,19,20,13,11,16,21,17,9,12,21,18,10,12,20,20,12,11,14,18,22,14,10,16,20,13,10,17,20,13,11,17,20,16,9,14,21,17,9,12,21,20,13,11,14,22,18,9,13,20,20,12,11,18,19,12,12,16,21,15,10,14,17,22,17,8,11,20,19,11,12,15,21,19,9,11,18,20,13,12,16,21,16,10,13,18,21,13,11,18,18,13,12,13,18,22,14,11,17,18,13,11,14,21,18,9,12,15,21,20,10,12,16,20,16,10,13,21,18,10,13,17,21,16,10,12,18,21,16,10,12,21,19,12,12,14,21,19,10,13,18,20,15,10,14,21,18,9,12,21,17,11,12,17,22,15,11,14,16,22,16,9,12,20,19,11,11,14,20,18,10,13,18,20,14,11,18,18,12,12,16,21,17,10,13,16,22,18,9,11,16,20,16,10,14,20,17,11,13,19,19,14,11,13,21,19,11,12,18,18,14,12,14,21,18,11,13,17,21,15,11,13,17,21,16,10,13,21,17,10,13,16,21,19,10,13,15,21,19,10,12,18,19,13,12,16,19,15,11,14,20,18,11,13,18,19,13,12,14,19,19,12,12,14,20,18,10,13,17,20,16,10,12,19,19,12,13,18,19,14,12,14,19,19,11,13,20,18,11,13,14,18,21,13,13,14,17,20,13,11,17,18,14,12,14,20,18,11,13,16,20,17,10,13,19,18,12,13,19,18,13,13,14,17,21,16,10,14,20,16,10,13,15,20,18,12,13,15,20,18,10,13,18,17,12,13,19,17,13,13,16,20,16,12,14,18,18,12,13,16,20,17,10,13,17,19,15,12,13,17,20,15,12,14,19,18,11,13,16,19,16,11,13,17,20,16,11,14,19,17,11,13,17,19,16,12,14,19,17,11,14,18,17,12,13,18,18,13,13,18,17,13,13,15,20,16,11,13,16,20,16,12,14,18,18,13,12,16,19,15,12,14,17,20,16,11,13,17,18,14,13,15,19,17,11,15,19,15,12,15,19,14,12,14,17,20,15,12,14,18,18,13,13,16,18,14,13,14,16,21,16,11,13,17,18,14,13,16,18,16,12,14,18,18,13,14,15,17,19,13,13,15,18,18,12,13,16,19,15,12,15,18,17,13,14,15,19,17,12,13,16,18,15,13,15,19,17,11,13,17,18,15,13,14,18,17,13,13,16,18,15,13,14,19,17,13,14,14,18,18,13,13,15,19,16,11,14,18,16,12,14,15,18,18,13,14,15,18,17,12,14,15,18,17,13,14,15,19,16,12,14,15,19,18,13,13,14,17,18,13,14,15,16,19,14,13,16};
void Sound_Init(uint32_t period, uint32_t priority);
void Sound_Start(uint32_t period);
void Sound_Stop(void);
// use main1 to determine Lab5 assignment
void Lab5Grader(int mode);
void Grader_Init(void);
int main1(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab5Grader(0); // print assignment, no grading
  while(1){
  }
}
const uint32_t Inputs[12]={0, 1, 7, 8, 15, 16, 17, 23, 24, 25, 30, 31};
uint32_t Testdata;

// use main2a to perform static testing of DAC, if you have a voltmeter
int main2a(void){ // main2a
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  DAC5_Init();     // your Lab 5 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<12; i++){ //0-11
      Testdata = Inputs[i];
      DAC5_Out(Testdata);
      // put a breakpoint on the next line and use meter to measure DACout
      // place data in Table 5.3
      Debug_Dump(Testdata);
    }
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main2b to perform static testing of DAC, if you do not have a voltmeter
// attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To use the scope, there can be no breakpoints in your code
int main2b(void){ // main2b
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // Scope
  DAC5_Init();     // your Lab 5 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    while(1){}; // access to GPIOB->DOE31_0 should be friendly
  }
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<12; i++){ //0-11
      Testdata = Inputs[i];
      DAC5_Out(Testdata);
      Debug_Dump(Testdata);
        // use TExaSdisplay scope to measure DACout
        // place data in Table 5.3
        // touch and release S2 to continue
      while(LaunchPad_InS2()==0){}; // wait for S2 to be touched
      while(LaunchPad_InS2()!=0){}; // wait for S2 to be released
      if((GPIOB->DOUT31_0&0x20) == 0){
         while(1){}; // DOUT not friendly
      }
    }
  }
}
// use main3 to perform dynamic testing of DAC,
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a monotonic ramp with period 32ms,
int main3(void){ // main3
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // Scope
  DAC5_Init();     // your Lab 5 initialization
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<32; i++){ //0-31
      DAC5_Out(i);
      Debug_Dump(i);
        // scope to observe waveform
        // place data in Table 5.3
      Clock_Delay1ms(1);
    }
  }
}

// use main4 to debug the four input switches
int main4(void){ // main4
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Key_Init(); // your Lab 5 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 5, Spring 2024, Step 4. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Key_In(); // Your Lab5 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}

// use main5 to debug your system
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a sine wave with period/frequency depending on which key is pressed
int main5(void){// main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Sound_Start(9956); // start one continuous wave
  while(1){
  }
}


// use main6 to debug/grade your final system
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a sine wave with period/frequency depending on which key is pressed
int main(void){// main6
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(1);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Debug_Init();    // Lab 3 debugging
  uint32_t last=0,now;
  while(1){
// if key goes from not pressed to pressed
//   -call Sound_Start with the appropriate period
//   -call Debug_Dump with period
// if key goes from pressed to not pressed
//   -call Sound_Stop
// I.e., if key has not changed DO NOT CALL start or stop
           now = Key_In();
           if((last == 0) && (now==0x10000)){
           Sound_Start(7272);
           Debug_Dump(7272);// change
           last = now;
           }
           if((last == 0) && (now==0x20000)){
           Sound_Start(7272);
           Debug_Dump(7272);// change
           last = now;
           }
           if((last == 0) && (now==0x40000)){
           Sound_Start(7272);
           Debug_Dump(7272);// change
           last = now;
           }
           if((last == 0) && (now==0x80000)){
           Sound_Start(7272);
           Debug_Dump(7272);// change
           last = now;
           }

           if(now==0){
           Sound_Stop();
           last = now;
           }
          Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// To grade you must connect PB20 to your DACout
// Run main6 with Lab5Grader(3);   // Grader
// Observe Terminal window

// Lab 5 low-level function implementations

// ARM SysTick period interrupts
// Input: interrupts every 12.5ns*period
//        priority is 0 (highest) to 3 (lowest)
void Sound_Init(uint32_t period, uint32_t priority){
  SysTick->CTRL = 0x00; //disable during initialization
  SysTick->LOAD = period-1; //set reload register
  SCB->SHP[1] = SCB->SHP[1] & (~0xC0000000) | priority<<30 ; //
  SysTick->VAL = 0;
  SysTick->CTRL = 0x07;
}
void Sound_Stop(void){
    SysTick->LOAD = 0;// either set LOAD to 0 or clear bit 1 in CTRL
    // write this
}


void Sound_Start(uint32_t period){
    // write this
    SysTick->LOAD = period-1;// set reload value
    SysTick->VAL = 1;// write any value to VAL, cause reload
}

// Interrupt service routine -- what happens when the interrupt is called
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
    static uint32_t i=0;
    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x1F)|fastinvader3[i]);   // output one value
    i = (i+1)&0xFF;

}