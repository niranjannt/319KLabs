/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * ECE319K students can use indices or pointers for next state
 * Put your names here or look silly
  */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two lines
const char EID1[] = "DMA3227"; //  ;replace abc123 with your EID
const char EID2[] = "NNT479"; //  ;replace abc123 with your EID


// initialize 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ // assumes LaunchPad_Init resets and powers A and B
      IOMUX->SECCFG.PINCM[PB15INDEX] = (uint32_t) 0x00040081; // input
      IOMUX->SECCFG.PINCM[PB16INDEX] = (uint32_t) 0x00040081; // input
      IOMUX->SECCFG.PINCM[PB17INDEX] = (uint32_t) 0x00040081; // input
      IOMUX->SECCFG.PINCM[PB27INDEX] = (uint32_t)0x00000081;
      IOMUX->SECCFG.PINCM[PB26INDEX] = (uint32_t)0x00000081;
      IOMUX->SECCFG.PINCM[PB22INDEX] = (uint32_t)0x00000081;
      IOMUX->SECCFG.PINCM[PB8INDEX] = (uint32_t) 0x00000081;
      IOMUX->SECCFG.PINCM[PB7INDEX] = (uint32_t) 0x00000081;
      IOMUX->SECCFG.PINCM[PB6INDEX] = (uint32_t) 0x00000081;
      IOMUX->SECCFG.PINCM[PB2INDEX] = (uint32_t) 0x00000081;
      IOMUX->SECCFG.PINCM[PB1INDEX] = (uint32_t) 0x00000081;
      IOMUX->SECCFG.PINCM[PB0INDEX] = (uint32_t) 0x00000081;
      GPIOB->DOE31_0 |= 0x0C4001C7;  // enable outputs
}
/* Activate LEDs
* Inputs: data1,data2,data3
*   specify what these means
* Output: none
* Feel free to change this. But, if you change the way it works, change the test programs too
* Be friendly*/
void Traffic_Out(void){
uint32_t out = GPIOB->DOUT31_0&~((1<<1)|(1<<2)|(1<<0)|(1<<6)|(1<<7)|(1<<8)|(1<<26)|(1<<22)|(1<<27)); // PB0-PB2 and PB6-PB8 outputs
uint32_t data=0;
GPIOB->DOUT31_0= out|(1<<0);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(1<<0);
Debug_Dump(data);
GPIOB->DOUT31_0= out|(1<<1);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(1<<1);
Debug_Dump(data);
GPIOB->DOUT31_0= out|(1<<2);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(1<<2);
Debug_Dump(data);
Clock_Delay(40000000);
GPIOB->DOUT31_0= out|(1<<6);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(1<<6);
Debug_Dump(data);
GPIOB->DOUT31_0= out|(1<<7);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(1<<7);
Clock_Delay(40000000);
GPIOB->DOUT31_0= out|(1<<7);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(1<<7);
Debug_Dump(data);
GPIOB->DOUT31_0= out|(1<<8);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(1<<8);
Debug_Dump(data);
GPIOB->DOUT31_0= out|(0xC400000);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(0xC400000);
Debug_Dump(data);
GPIOB->DOUT31_0= out|(0x0400000);
data=GPIOB->DOUT31_0;
GPIOB->DOUT31_0= GPIOB->DOUT31_0 &~(0x040000);





}
/* Read sensors
 * Input: none
 * Output: sensor values
*   specify what these means
* Feel free to change this. But, if you change the way it works, change the test programs too
 */
uint32_t Traffic_In(void){
    uint32_t in = GPIOB->DIN31_0&((1<<15)|(1<<16)|(1<<17));     // PB3-PB0 inputs
    uint32_t data = in;                   // PB18-PB16, PB3-PB0
    return data;
}
// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main6(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab4Grader(0); // print assignment, no grading
  while(1){
  }
}
// use main2 to debug LED outputs
int main2(void){ // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();  // execute this line before your code
  Traffic_Init(); // your Lab 4 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  UART_OutString("Lab 4, Spring 2024, Step 1. Debug LEDs\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    //write debug code to test your Traffic_Out
// Call Traffic_Out testing all LED patterns
// Lab 3 dump to record output values
Traffic_Out();
    Clock_Delay(40000000); // 0.5s
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main3 to debug the three input switches
int main(void){ // main3
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2024, Step 2. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Traffic_In(); // Your Lab4 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// use main4 to debug using your dump
// proving your machine cycles through all states
int main4(void){// main4
uint32_t input;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2024, Step 3. Debug FSM cycle\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
// initialize your FSM
  struct State{
    char name[15];
    uint32_t Out;
    uint32_t Time;
    uint32_t Next[8];};
  typedef const struct State State_t;
  #define all_red  0
  #define go_west 1
  #define west_yellow 2
  #define west_red 3
  #define go_south  4
  #define south_yellow 5
  #define south_red 6
  #define go_walk 7
  #define hurry_up 8
  #define flash1 9
  #define flash2 10
  #define flash3 11
  #define flash4 12
  #define flash5 13
  #define walk_red 14
  State_t FSM[15]={
  // name    output   time  000 001 010 011 100 101 110 111
   {"all_red",0x04000104,200,{all_red,go_walk,go_south,go_south,go_west,go_west,go_west,go_south}},
   {"go_west",0x04000044 ,200,{west_yellow,west_yellow,west_yellow,west_yellow,go_west,west_yellow,west_yellow,west_yellow}},
   {"west_yellow",0x04000084,200,{west_red,west_red,west_red,west_red,west_red,west_red,west_red,west_red}},
   {"west_red",0x04000104,200,{all_red,all_red,all_red,all_red,all_red,go_walk,go_south,all_red}},
   {"go_south",0x04000101,200,{south_yellow,south_yellow,go_south,south_yellow,south_yellow,south_yellow,south_yellow,south_yellow}},
   {"south_yellow",0x04000102,200,{south_red,south_red,south_red,south_red,south_red,south_red,south_red,south_red}},
   {"south_red",0x04000104,200,{all_red,all_red,all_red,go_walk,all_red,all_red,all_red,go_walk}},
   {"go_walk",0x0C400104,200,{hurry_up,hurry_up,hurry_up,hurry_up,hurry_up,hurry_up,hurry_up,hurry_up}},
   {"hurry_up",0x04000104,50,{flash1,flash1,flash1,flash1,flash1,flash1,flash1,flash1}},
   {"flash1",0x00000104,50,{flash2,flash2,flash2,flash2,flash2,flash2,flash2,flash2}},
   {"flash2",0x04000104,50,{flash3,flash3,flash3,flash3,flash3,flash3,flash3,flash3}},
   {"flash3",0x00000104,50,{flash4,flash4,flash4,flash4,flash4,flash4,flash4,flash4}},
   {"flash4",0x04000104,50,{flash5,flash5,flash5,flash5,flash5,flash5,flash5,flash5}},
   {"flash5",0x00000104,50,{walk_red,walk_red,walk_red,walk_red,walk_red,walk_red,walk_red,walk_red}},
   {"walk_red",0x04000104,50,{all_red,all_red,all_red,all_red,all_red,all_red,all_red,go_west}}};
  SysTick_Init();   // Initialize SysTick for software waits
  uint32_t S = all_red;
  while(1){
      GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x0C4001C7)) | FSM[S].Out; // 1) output depending on state using Traffic_Out
      Debug_Dump((GPIOB->DOUT31_0 & 0x0C4001C7) | (S<<6)); // call your Debug_Dump logging your state number and output
      SysTick_Wait10ms(FSM[S].Time); // 2) wait depending on state
      input = 7; // 3) hard code this so input always shows all switches pressed
      S = FSM[S].Next[input]; // 4) next depends on state and input
  }
}
// use main5 to grade
int main5(void){// main5
  uint32_t input;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();  // execute this line before your code
  Traffic_Init(); // your Lab 4 initialization
  SysTick_Init(); // Initialize SysTick for software waits
  Debug_Init();
  // initialize your FSM
  struct State{
    char name[15];
    uint32_t Out;
    uint32_t Time;
    uint32_t Next[8];};
  typedef const struct State State_t;
  #define all_red  0
  #define go_west 1
  #define west_yellow 2
  #define west_red 3
  #define go_south  4
  #define south_yellow 5
  #define south_red 6
  #define go_walk 7
  #define hurry_up 8
  #define flash1 9
  #define flash2 10
  #define flash3 11
  #define flash4 12
  #define flash5 13
  #define walk_red 14
  State_t FSM[15]={
  // name    output   time  000 001 010 011 100 101 110 111
   {"all_red",0x04000104,200,{all_red,go_walk,go_south,go_south,go_west,go_west,go_west,go_south}},
   {"go_west",0x04000044 ,200,{west_yellow,west_yellow,west_yellow,west_yellow,go_west,west_yellow,west_yellow,west_yellow}},
   {"west_yellow",0x04000084,200,{west_red,west_red,west_red,west_red,west_red,west_red,west_red,west_red}},
   {"west_red",0x04000104,200,{all_red,all_red,all_red,all_red,all_red,go_walk,go_south,all_red}},
   {"go_south",0x04000101,200,{south_yellow,south_yellow,go_south,south_yellow,south_yellow,south_yellow,south_yellow,south_yellow}},
   {"south_yellow",0x04000102,200,{south_red,south_red,south_red,south_red,south_red,south_red,south_red,south_red}},
   {"south_red",0x04000104,200,{all_red,all_red,all_red,go_walk,all_red,all_red,all_red,go_walk}},
   {"go_walk",0x0C400104,200,{hurry_up,hurry_up,hurry_up,hurry_up,hurry_up,hurry_up,hurry_up,hurry_up}},
   {"hurry_up",0x04000104,50,{flash1,flash1,flash1,flash1,flash1,flash1,flash1,flash1}},
   {"flash1",0x00000104,50,{flash2,flash2,flash2,flash2,flash2,flash2,flash2,flash2}},
   {"flash2",0x04000104,50,{flash3,flash3,flash3,flash3,flash3,flash3,flash3,flash3}},
   {"flash3",0x00000104,50,{flash4,flash4,flash4,flash4,flash4,flash4,flash4,flash4}},
   {"flash4",0x04000104,50,{flash5,flash5,flash5,flash5,flash5,flash5,flash5,flash5}},
   {"flash5",0x00000104,50,{walk_red,walk_red,walk_red,walk_red,walk_red,walk_red,walk_red,walk_red}},
   {"walk_red",0x04000104,50,{all_red,all_red,all_red,all_red,all_red,all_red,all_red,go_west}}};
  Lab4Grader(1); // activate UART, grader and interrupts
 uint32_t S = all_red;
  while(1){
     GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x0C4001C7)) | FSM[S].Out; // 1) output depending on state using Traffic_Out
     Debug_Dump((GPIOB->DOUT31_0 & 0x0C4001C7) | (S<<6)); //(GPIOB->DIN31_0 & 0x1C0)| call your Debug_Dump logging your state number and output
     SysTick_Wait10ms(FSM[S].Time);  // 2) wait depending on state
     input = (GPIOB->DIN31_0 & 0x38000)>>15; // 3) input from switches
     S = FSM[S].Next[input]; // 4) next depends on state and input
  }
}

