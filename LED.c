/*
 * LED.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
/*
 * LED.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table

// initialize your LEDs
void LED_Init(void){
    IOMUX->SECCFG.PINCM[PB12INDEX] = (uint32_t) 0x00000081;
    IOMUX->SECCFG.PINCM[PB17INDEX] = (uint32_t) 0x00000081;
    IOMUX->SECCFG.PINCM[PB13INDEX] = (uint32_t) 0x00000081;
    GPIOB->DOE31_0 |= 0x00023000;  // enable outputs

}
// data specifies which LED to turn on
void LED_On(uint32_t data){
    if(data == 12){
      GPIOB->DOUTSET31_0 = (1<<12);
    }
    if(data == 17){
      GPIOB->DOUTSET31_0 = (1<<17);
    }
    if(data == 13){
      GPIOB->DOUTSET31_0 = (1<<13);
    }
}

// data specifies which LED to turn off
void LED_Off(uint32_t data){
    if(data == 12){
      GPIOB->DOUTCLR31_0 = (1<<12);
    }
    if(data == 17){
      GPIOB->DOUTCLR31_0 = (1<<17);
    }
    if(data == 13){
      GPIOB->DOUTCLR31_0 = (1<<13);
    }

}

// data specifies which LED to toggle
void LED_Toggle(uint32_t data){
    if(data == 12){
      GPIOB->DOUTTGL31_0 = (1<<12);
    }
    if(data == 17){
      GPIOB->DOUTTGL31_0= (1<<17);
    }
    if(data == 13){
      GPIOB->DOUTTGL31_0= (1<<13);
    }

}
