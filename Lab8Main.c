// Lab8Main.c
// Runs on MSPM0G3507
// Lab 8 
// Your name
// Last Modified: 12/29/2023

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/FIFO1.h"
#include "UART1.h"
#include "UART2.h"
// ****note to students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}
// UART0 routines implemented in TExaS.c
void UART_Init(void);         // 11520 baud
void UART_OutChar(char data); // busy-wait
void UART_OutString(char *pt){
  while(*pt){
    UART_OutChar(*pt);
    pt++;
  }
}

uint32_t ReceiveCount,TransmitCount;
uint32_t Position; // fixed point 0.001cm, in receiver
uint32_t Data;     // 12-bit raw ADC data in transmitter
// *****************************
// use main1 to test your FIFO1
// the object code myFifo.o has my solution
uint32_t M;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}
// these are grader implementations, hidden from students
void myFifo_Init(uint32_t size);
uint32_t myFifo_Put(char data);
char myFifo_Get(void);
uint32_t FifoError;
int main1 (void){ // use main1 to test your FIFO1
  char me,you;
  char data;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  UART_Init();
  UART_OutString("main1: Test of FIFO1\n\r");
  // your FIFO must be larger than 7 and less than 127
  Fifo1_Init();     // your FIFO
  M = 4; // seed
  FifoError = 0;
  // myFifo_Init(16) means FIFO can store up to 15 elements
  myFifo_Init(8); // change 16 to match your FIFO size
  for(uint32_t i = 0; i<10000; i++){
    uint32_t k = Random(4);
    for(uint32_t l=0; l<k ;l++){
      data = Random(255)+1; // 1 to 255
      you = Fifo1_Put(data);
      me = myFifo_Put(data);
      if(me != you){
        FifoError++; // put a breakpoint here
      }
    }
    uint32_t j = Random(40);
    for(uint32_t l=0; l<j ;l++){
      data = Random(255)+1; // 1 to 255
      you = Fifo1_Put(data);
      me = myFifo_Put(data);
      if(me != you){
        FifoError++; // put a breakpoint here
      }
    }
    for(uint32_t l=0; l<j ;l++){
      you = Fifo1_Get();
      me = myFifo_Get();
      if(me != you){
        FifoError++; // put a breakpoint here
      }
    }
    for(uint32_t l=0; l<k ;l++){
      you = Fifo1_Get();
      me = myFifo_Get();
      if(me != you){
        FifoError++; // put a breakpoint here
      }
    }
  }
  if(FifoError == 0){
    UART_OutString("Success\n\r");
    for(;;){ // success
      GPIOB->DOUTTGL31_0 = GREEN; // green means good
      for(int i=0; i<50000; i++){
      }
    }
  }else{
    UART_OutString("Failure\n\r");
    for(;;){ // failure
      GPIOB->DOUTTGL31_0 = RED; // red
      for(int i=0; i<50000; i++){
      }
    }
  }
}

// use main2 to test UART1 blind transmission
// no interrupts, just PA8 U1Tx output
int main2 (void){ // main2
  __disable_irq();
  PLL_Init();   // set bus speed
  LaunchPad_Init();
  UART1_Init(); // just transmit, PA8, blind synchronization
  TExaS_Init(ADC0,6,0); //PB20 = ADC0 channel 6, connected to PA8
  __enable_irq();       // interrupts for TExaS, not UART1
  while(1){ // one frame
    UART1_OutChar(0x80|'1'); // 0xB1
    Clock_Delay1ms(10); // about 100Hz
  }
}

// use main3 to test 4-frame message transmission
// no interrupts, just PA8 U1Tx output
int main3 (void){ // main3
  __disable_irq();
  PLL_Init();   // set bus speed
  LaunchPad_Init();
  UART1_Init(); // just transmit, PA8, blind synchronization
  TExaS_Init(ADC0,6,0); //PB20 = ADC0 channel 6, connected to PA8
  __enable_irq();       // interrupts for TExaS, not UART1
  while(1){ // message is 1.234 cm
    UART1_OutChar(0x80|'1'); // 0xB1
    UART1_OutChar('2'); // 0x32
    UART1_OutChar('3'); // 0x33
    UART1_OutChar('4'); // 0x34
    Clock_Delay1ms(50); // about 20Hz
  }
}
// use main4 to test blind transmission and receive timeout interrupt
// connect PA22 U2Rx input to PA8 U1Tx output on the same board
int main4 (void){ // main4, loop back test
  char data1,data2,data3,data4;
  __disable_irq();
  PLL_Init();   // set bus speed
  LaunchPad_Init();
   __enable_irq();       // interrupts for UART1
  while(1){ // message is 1.234 cm
    UART1_OutChar(0x80|'1'); // 0xB1
    UART1_OutChar('2');   // 0x32
    UART1_OutChar('3');   // 0x33
    UART1_OutChar('4');   // 0x34
    Clock_Delay1ms(1000); // about 1Hz
    data1 = UART2_InChar(); // should be B1
    UART_OutChar(data1&0x7F);
    UART_OutChar('.');    // implied
    data2 = UART2_InChar(); // should be 32
    UART_OutChar(data2);
    data3 = UART2_InChar(); // should be 33
    UART_OutChar(data3);
    data4 = UART2_InChar(); // should be 34
    UART_OutChar(data4);
    UART_OutString("\n\r");
  }
}
// sampling frequency is 30 Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg; char newposno, newpos, newpos2no, newpos2, newpos3no, newpos3;
// complete this
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    // increment TransmitCount
    TransmitCount++;
    // sample
    Data = ADCin();  // sample 12-bit ADC0 channel 5, slidepot //sample
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    // convert to fixed point distance
    Position = Convert(Data);
    // output 4-frame message
    newposno=Position/1000;
    newposno=newposno+48+0x80;
    UART1_OutChar(newposno);
    newpos=Position%1000;
    newpos2no=newpos/100;
    newpos2no=newpos2no+48;
    UART1_OutChar(newpos2no);
    newpos2=newpos%100;
    newpos3no=newpos2/10;
    newpos3no=newpos3no+48;
    UART1_OutChar(newpos3no);
    newpos3=newpos2%10;
    newpos3=newpos3+48;
    UART1_OutChar(newpos3);





    TransmitCount++;
     GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)


    }

  }

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}
// use main5 to test entire system in loop back
// connect PA22 U2Rx input to PA8 U1Tx output on the same board
// connect slidepot pin 2 to PB18
// Data should go from 0 to 4095 in transmitter
// Position should go from 0 to 2000 in receiver
// LCD should show 0.000cm to 2.000 cm
int main (void){ // main5
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ReceiveCount=0; TransmitCount=0;
  UART1_Init(); // just transmit, PA8, blind synchronization
  UART2_Init(); // just receive, PA22, receiver timeout synchronization
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ADCinit(); //PB18 = ADC1 channel 5, slidepot
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
  ST7735_PlotClear(0,2000);
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(2666666,1);

  __enable_irq();
uint32_t firstframe,secondframe, thirdframe, fourthframe, Position2;
  while(1){
      // complete this
    // move cursor to top left
      GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
      GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)


      ST7735_SetCursor(0,0); // top left

    // wait for first frame
      firstframe=UART2_InChar();

    while((firstframe&0x80)!=0x80|firstframe==0){
        firstframe=UART2_InChar();

    }
    // increment ReceiveCount
      ReceiveCount++;
    // receive next three frames
    firstframe=firstframe&(~0x80);
    firstframe=(firstframe-48)*1000;
    secondframe=(UART2_InChar()-48)*100;
    thirdframe=(UART2_InChar()-48)*10;
    fourthframe=(UART2_InChar()-48);
    Position2=firstframe+secondframe+thirdframe+fourthframe;
      // output message to 22
      printf("d=%1u.%.3u cm", Position2/1000, Position2%1000);
      GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
    // calculate Position from message
    if((ReceiveCount%15)==0){
      ST7735_PlotPoint(Position2);
      ST7735_PlotNextErase(); // data plotted at about 2 Hz
    }
}
  }


// use main5 to test entire system in distributed mode
// needs two identical MSPM0 LaunchPads
// connect PA22 U2Rx input on one to PA8 U1Tx output on the other board
// connect PA8 U1Tx output on one to PA22 U2Rx input on the other board
// connect grounds of two boards together
// connect slidepot pin 2 to PB18 on both boards
// Data should go from 0 to 4095 in transmitter
// Position should go from 0 to 2000 in receiver
// LCD should show 0.000cm to 2.000 cm


