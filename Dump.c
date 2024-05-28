// Dump.c
// Your solution to ECE319K Lab 3 Spring 2024
// Author: Niranjan Telkikar
// Last Modified: Your date


#include <ti/devices/msp/msp.h>
#include "../inc/Timer.h"
#define MAXBUF 50

uint32_t DataBuffer[MAXBUF];
uint32_t TimeBuffer[MAXBUF];
uint32_t DebugCnt; // 0 to MAXBUF (0 is empty, MAXBUF is full)
uint32_t rising_edge_time_buffer_array[100];
uint32_t duty_cycle_array[100];
// *****Debug_Init******
// Initializes your index or pointer.
// Input: none
// Output:none
void Debug_Init(void){
// students write this for Lab 3
// This function should also initialize Timer G12, call TimerG12_Init.
    DebugCnt=0; // 0 to MAXBUF (0 is empty, MAXBUF is full) //initializes index
    TimerG12_Init(); //calls TimerG12_Init

}

// *****Debug_Dump******
// Records one data and one time into the two arrays.
// Input: data is value to store in DataBuffer
// Output: 1 for success, 0 for failure (buffers full)
uint32_t Debug_Dump(uint32_t data){
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.

 if (DebugCnt<=MAXBUF-1){
     uint32_t timer= TIMG12->COUNTERREGS.CTR; //timer variable is equal to current value of (decrementing) hardware counter
     DataBuffer[DebugCnt]= data; //data is being put into DataBuffer array
     TimeBuffer[DebugCnt]=timer;//time is being put into DataBuffer array
     DebugCnt++; //array index is incremented (to point to the next element of the array)



     return 1; // when buffers are full return 1

 }
 else{

     return 0; //incomplete data and/or time
 }
}

// *****Debug_Dump2******
// Always record data and time on the first call to Debug_Dump2
// However, after the first call
//    Records one data and one time into the two arrays, only if the data is different from the previous call.
//    Do not record data or time if the data is the same as the data from the previous call
// Input: data is value to store in DataBuffer
// Output: 1 for success (saved or skipped), 0 for failure (buffers full)
uint32_t Debug_Dump2(uint32_t data){
// optional for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
    if (DebugCnt<=MAXBUF-1){
        uint32_t timer= TIMG12->COUNTERREGS.CTR;
        DataBuffer[DebugCnt]= data;
        TimeBuffer[DebugCnt]=timer;
        DebugCnt++;



        return 1; // success

    }
    else{

        return 0;
    }
}
// *****Debug_Period******
// Calculate period of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in bus cycles
// Period is defined as rising edge (low to high) to the next rising edge.
// Return 0 if there is not enough collected data to calculate period .
uint32_t Debug_Period(uint32_t mask){
// students write this for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
    DebugCnt=0;
    uint32_t data_buffer_value=0; //current data buffer value (from DataBuffer array)
    uint32_t next_data_buffer_value=0; //next data buffer value (from DataBuffer array)
    uint32_t data_mask=0; //data buffer value anded with mask
    uint32_t next_data_mask=0;//next data buffer value anded with mask
    uint32_t time_buffer_variable=0; //time buffer variable
    uint32_t next_time_buffer_variable=0; //next time buffer variable
    uint32_t rising_edge_counter=0; //counter that collects rising edges
    uint32_t period_counter=0; //counter that collects periods
    uint32_t time_array_counter=0; //array that collects time of rising edges
    uint32_t time_array_index_first=0;//first index of array that collects time of rising edges
    uint32_t time_array_index_last=0;//last index of array that collects time of rising edges
    uint32_t period=0; //subtraction between first and last index
    Restart:
    if (DebugCnt<49){
    data_buffer_value=DataBuffer[DebugCnt]; //loads data buffer value
    time_buffer_variable=TimeBuffer[DebugCnt]; //loads time buffer value
    DebugCnt++;
    data_mask=data_buffer_value & mask; //bitwise ands mask and data buffer value
    next_data_buffer_value=DataBuffer[DebugCnt];
    next_data_mask=next_data_buffer_value & mask;//bitwise ands mask and next data buffer value
    next_time_buffer_variable=TimeBuffer[DebugCnt];
    goto maskcheck;
    }
   else{
       if(rising_edge_counter<2){ //not enough rising edges for calculation if <2
        return 0;
       }else{
            if(rising_edge_counter>=2){
            period_counter=rising_edge_counter-1; //period=rising edges-1
            time_array_index_first=rising_edge_time_buffer_array[0];
            time_array_index_last=rising_edge_time_buffer_array[time_array_counter-1];
            period= ~((time_array_index_last)-(time_array_index_first)); //calculates period from two points on array
            return period/period_counter; //returns average period


            }
        }
       }



    maskcheck:
    if(next_data_mask>data_mask && data_mask==0){ //checks for rising edge
    rising_edge_time_buffer_array[time_array_counter]=next_time_buffer_variable; //puts next time buffer in array
    time_array_counter++; //increments rising edge array counter
    rising_edge_counter++; //increments rising edge counter
    goto Restart;
    }
    else{
    goto Restart;
    }
}





// *****Debug_Duty******
// Calculate duty cycle of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in percent (0 to 100)
// Period is defined as rising edge (low to high) to the next rising edge.
// High is defined as rising edge (low to high) to the next falling edge.
// Duty cycle is (100*High)/Period
// Return 0 if there is not enough collected data to calculate duty cycle.
uint32_t Debug_Duty(uint32_t mask){
    // students write this for Lab 3
    // This function should not alter the recorded data.
    // AND each recorded data with mask,
    //    if nonzero the signal is considered high.
    //    if zero, the signal is considered low.
        DebugCnt=0;
        uint32_t data_buffer_value=0; //variable for value stored in data_buffer_value
        uint32_t next_data_buffer_value=0; //next_data_buffer_variable
        uint32_t data_mask=0; //
        uint32_t next_data_mask=0;
        uint32_t time_buffer_variable=0;
        uint32_t next_time_buffer_variable=0;
        uint32_t rising_edge_counter=0;
        uint32_t period_counter=0;
        uint32_t time_array_counter=0;
        uint32_t time_array_index_first=0;
        uint32_t time_array_index_last=0;
        uint32_t period=0;
        uint32_t duty_counter=0;
        uint32_t average_period=0;
        uint32_t average_pulse_width=0;
        uint32_t pulse_width=0;
        uint32_t return_value=0;
        Restart:
        if (DebugCnt<49){
        data_buffer_value=DataBuffer[DebugCnt];
        time_buffer_variable=TimeBuffer[DebugCnt];
        DebugCnt++;
        data_mask=data_buffer_value & mask;
        next_data_buffer_value=DataBuffer[DebugCnt];
        next_data_mask=next_data_buffer_value & mask;
        next_time_buffer_variable=TimeBuffer[DebugCnt];
        goto maskcheck;
        }

       else{
           if(rising_edge_counter<2){
            return 0;
           }else{
                if(rising_edge_counter>=2){
                period_counter=rising_edge_counter-1;
                time_array_index_first=rising_edge_time_buffer_array[0];
                time_array_index_last=rising_edge_time_buffer_array[time_array_counter-1];
                period= ~((time_array_index_last)-(time_array_index_first));
                average_period = (period/period_counter);
                average_pulse_width=duty_cycle_array[0]; //takes pulse width from array
                return ((100*average_pulse_width)/ (average_period)); //returns duty cycle


                }
            }
           }



        maskcheck:
        if(next_data_mask>data_mask && data_mask==0){
        rising_edge_time_buffer_array[time_array_counter]=next_time_buffer_variable;
        time_array_counter++;
        rising_edge_counter++;
        goto Restart;
        }
        else if(next_data_mask<data_mask && data_mask>=1 && next_data_mask==0){ //takes branch when there is falling edge
        duty_cycle_array[duty_counter]=time_buffer_variable-next_time_buffer_variable; //subtracts time from rising and falling edge (time high)
        duty_counter++; //goes to next in falling edge array
        goto Restart;
        }
        else{
        goto Restart;
        }
        }








// Lab2 specific debugging code
uint32_t Theperiod,Thedutycycle;
void Dump(void){
  uint32_t out = GPIOB->DOUT31_0&0x0070000; // PB18-PB16 outputs
  uint32_t in = GPIOB->DIN31_0&0x0F;        // PB3-PB0 inputs
  uint32_t data = out|in;                   // PB18-PB16, PB3-PB0
  uint32_t result = Debug_Dump(data);       // calls your Lab3 function
  if(result == 0){ // 0 means full
    Theperiod = Debug_Period(1<<17);        // calls your Lab3 function
    Thedutycycle =Debug_Duty(1<<17);
//   __asm volatile("bkpt; \n"); // breakpoint here
// observe Theperiod
  }
}




