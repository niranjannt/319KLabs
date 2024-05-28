//****************** ECE319K_Lab2.s ***************
// Your solution to Lab 2 in assembly code
// Author: Niranjan Telkikar
// Last Modified: Your date
// ECE319K Spring 2024 (ECE319H students do Lab2H)
   .equ GPIOB_RSTCTL,   0x400A2804
   .equ GPIOB_PWREN,    0x400A2800
   .equ GPIOB_DOE31_0,  0x400A32C0
   .equ GPIOB_DOUT31_0, 0x400A3280
   .equ GPIOB_DIN31_0,  0x400A3380
   .equ IOMUXPB0,       0x40428004+4*11
   .equ IOMUXPB1,       0x40428004+4*12
   .equ IOMUXPB2,       0x40428004+4*14
   .equ IOMUXPB3,       0x40428004+4*15
   .equ IOMUXPB16,      0x40428004+4*32
   .equ IOMUXPB17,      0x40428004+4*42
   .equ IOMUXPB18,      0x40428004+4*43
   .equ GPIOB_DOUTSET31_0, 0x400A3290
   .equ GPIOB_DOUTTGL31_0, 0x400A32B0
   .equ GPIOB_DOUTCLR31_0, 0x400A32A0


        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive
//Heavily borrowed with the permission of Dr. Valvano

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "NNT479" // replace ZZZ123 with your EID here
        .align 2

// this allow your Lab2 programs to the Lab2 grader
        .global Lab2Grader
// this allow the Lab2 grader to call your Lab2 program
        .global Lab2
// these two allow your Lab2 programs to all your Lab3 solutions
        .global Debug_Init
        .global Dump

// Switch input: PB2 PB1 or PB0, depending on EID
// LED output:   PB18 PB17 or PB16, depending on EID
// logic analyzer pins PB18 PB17 PB16 PB2 PB1 PB0
// analog scope pin PB20
Lab2:
// Initially the main program will
//   set bus clock at 80 MHz,
//   reset and power enable both Port A and Port B
// Lab2Grader will
//   configure interrupts  on TIMERG0 for grader or TIMERG7 for TExaS
//   initialize ADC0 PB20 for scope,
//   initialize UART0 for grader or TExaS
     MOVS R0,#10  // 0 for info, 1 debug with logic analyzer, 2 debug with scope, 10 for grade
     BL   Lab2Grader
     BL   Debug_Init // your Lab3
     BL   Lab2Init // initialize input and output pins
// other initializations
loop:
     
     B    loop


// make switch the input, LED the output
// PortB is already reset and powered
// Set IOMUX for your input and output
// Set GPIOB_DOE31_0 for your output (be friendly)
Lab2Init:
  MOVS R1,#0x81
  LDR  R0,=IOMUXPB17
  STR  R1,[R0]  // PB17 is initialized as output pin
  LDR  R1,=0x00040081
  LDR  R0,=IOMUXPB3
  STR  R1,[R0]  // PB3 is initialized as input pin
  LDR  R0,=GPIOB_DOE31_0
  MOVS R1,#1
  LSLS R1, R1, #17 // left shifting is used as a mask to enable PB17
  LDR R0, [R0]
  ORRS R1, R0, R1
  LDR  R0,=GPIOB_DOE31_0
  STR  R1,[R0]  // PB17 is enabled out
  LDR R1,=320000
  restart:
  LDR R4,=1,360,000 //counter for cycles led is off
  LDR R5, =240,000 //counter for cycles led is on (total period is 20 ms)
  delay_loop:
  LDR R6, =GPIOB_DOUTSET31_0
  MOVS R7, #1
  LSLS R7, R7, #17 // left shifting is used as a mask to enable PB17
  STR  R7,[R6]  // set the pin 17 bit to a 1 (turns led on)
  PUSH {R0-R7}
  BL  Dump
  POP {R0-R7}
  MOVS R0, R5 //moves led on counter and starts delay subroutine
  BL Delay
  LDR R6, =GPIOB_DOUTCLR31_0
  MOVS R7, #1
  LSLS R7, R7, #17 // left shifting is used as a mask to clear PB17
  STR  R7,[R6]  // clear the pin 17 bit (turns led off)
  PUSH {R0-R7}
  BL  Dump
  POP {R0-R7}
  MOVS R0, R4 //moves led off counter and starts delay subroutine
  BL Delay
  LDR R3,=GPIOB_DIN31_0 //loads address of DIN-register
  LDR R3, [R3]
  MOVS R2, #8 // 2^3 is mask for third bit
  ANDS R3, R3, R2 // selects the PB3 input (using a bitmask)
  BNE RELEASE //branches if switch is pushed pushed
  B delay_loop
  RELEASE:
  LDR R3,=GPIOB_DIN31_0 //checks if released
  LDR R3, [R3]
  MOVS R2, #8 // 2^3 is mask for third bit
  ANDS R3, R3, R2 // selects the PB3 input (using a bitmask)
  BEQ ChangeCounter  //changes the counter for cycles led is off and on (changing the duty cycle) when the switch is released
  BNE RELEASE //keeps checking until it is released

Delay:
   SUBS R0,R0,#2
dloop:
   SUBS R0,R0,#4 // C=1 if no overflow
   NOP           // C=0 on pass through 0
   BHS  dloop
   BX LR



ChangeCounter:
LDR R2, =1200000 //checks if R5 is =1200000 (meaning 75 percent duty cycle has been reached)
SUBS R5, R5, R2 //subtracts 1201200000 for checking
BEQ restart //if 75 percent duty cycle has been reached there is a restart to 15 percent duty cycle
ADDS R5, R5, R2 //adds 1200000 back if duty cycle is not 75 percent
ADDS R5, R5, R1 //adds 320000 to high counter (to reach next duty cycle for proportionality of high-to-low)
SUBS R4, R4, R1 //subtracts 320000 to low counter (to reach next duty cycle for proportionality of high-to-low)
B delay_loop

   .end
