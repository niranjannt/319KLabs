/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    IOMUX->SECCFG.PINCM[PA8INDEX] = (uint32_t)0x00040081;
    IOMUX->SECCFG.PINCM[PB17INDEX] = (uint32_t)0x00040081;
    IOMUX->SECCFG.PINCM[PB18INDEX] = (uint32_t)0x00040081;
    IOMUX->SECCFG.PINCM[PB19INDEX] = (uint32_t)0x00040081;
    IOMUX->SECCFG.PINCM[PA17INDEX] = (uint32_t)0x00040081;
    IOMUX->SECCFG.PINCM[PA15INDEX] = (uint32_t)0x00040081;
    IOMUX->SECCFG.PINCM[PB7INDEX] = (uint32_t)0x00040081;

}
// return current state of switches
uint32_t Switch_In(void){
    uint32_t in = GPIOB->DIN31_0 & (1<<19);     // PB3-PB0 inputs
    return in;
}

uint32_t Switch_InA(void){
    uint32_t in_a = GPIOA->DIN31_0 & ((1<<15)|(1<<17));
    return in_a;
}
