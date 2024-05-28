// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"


void SysTick_IntArm(uint32_t period, uint32_t priority){
     SysTick->CTRL = 0x00; //disable during initialization
     SysTick->LOAD = period-1; //set reload register
     SCB->SHP[1] = SCB->SHP[1] & (~0xC0000000) | priority<<30 ; //
     SysTick->VAL = 0;
     SysTick->CTRL = 0x07;
   }

// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
uint32_t size;
const uint8_t *sound;
void Sound_Init(void){
   size=0;
   SysTick_IntArm(7272,0);
   DAC5_Init();
   //SysTick->LOAD = 0;
}
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    if(size){
      DAC5_Out(*sound);
      sound++;
      size--;
    }
   else{
       SysTick->LOAD = 0;
    }


}

//******* Sound_Start ************
// This function does not output to the DAC.
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement

void Sound_Start(const uint8_t *pt, uint32_t count){
   size = count;
   sound = pt;
   SysTick->LOAD = (7272);
   SysTick->VAL =1;
}
void Sound_Buzzer2(void);
void Ding(void);


void Sound_Buzzer2(void){
    Sound_Start(buzzer2,3649);
}
void Sound_Shoot(void){
  Sound_Start(buzzer2,3377);

}
void Sound_Killed(void){
  Sound_Start(invaderkilled,3377);

}
void Sound_Explosion(void){
  Sound_Start(explosion,2000);

}

void Sound_Fastinvader1(void){
    Sound_Start(fastinvader1,982);
}
void Sound_Fastinvader2(void){

}
void Sound_Fastinvader3(void){

}
void Sound_Fastinvader4(void){

}
void Sound_Highpitch(void){
}
void Ding(void){
    Sound_Start(ding, 6624);
}
