//****************** ECE319K_Lab1.s ***************
// Your solution to Lab 1 in assembly code
// Author: Niranjan Telkikar
// Last Modified: Your date
// Spring 2024
        .data //all code below goes to ROM
        .align 2
// Declare global variables here if needed
// with the .space assembly directive
        count: .space 4
        .text
        .thumb
        .align 2
        .global EID
EID:    .string "NNT479" // replace ZZZ123 with your EID here
        .align 2
        .global Phase
Phase:  .long  10// 0 for info, 1-6 for debug, 10 for grade

        .global Lab1
// Input: R0 points to the list
// Return: R0 as specified in Lab 1 assignment and terminal window
// According to AAPCS, you must save/restore R4-R7
// If your function calls another function, you must save/restore LR
Lab1: PUSH {R4-R7,LR}
      MOVS R4, #0 //initializes index counter variable
      LDR R2, =EID// get address of EID in register
      LDRB R2, [R2] //get first character of string (1 byte) into register
      LDR R1, [R0] //get address of pointer
      LDRB R1, [R1] //get first character of pointed EID
      CMP R1, R2 //compare the two letters
      BEQ SAMECHAR //branches below are to check if characters are the same or different
      DIFFERENTCHAR:
      LDR R2, =EID //R2 was clobbered so the address of EID is loaded again
	  ADDS R4, R4, #1 //add to index counter
      ADDS R0, R0, #8 //changes address of pointer to point to next EID
      LDR R1, [R0]  // compare EID with first letter of program
      CMP R1, #0 //sets condition codes to check if null pointer (pointing to another memory location is reached)
      BEQ NOEID //if pointer is null EID is not in list
      LDRB R7, [R1] //loads character of next pointer EID
      LDRB R5, [R2] //load next character of EID
      CMP R5, R7 //compares the two characters
      BNE DIFFERENTCHAR //branches below are to check if characters are the same or different
      SAMECHAR:
      LDR R2, =EID //R2 was clobbered so the address of EID is loaded again
      LDR R1, [R0]  // compare EID with first letter of program
      SAMENOLOADCHAR:
      ADDS R1, R1, #1 //add 1 to compare next letter of linked list
      ADDS R2, R2, #1 //add 1 to compare next letter of EID variable
      LDRB R7, [R1] //get character from EID array
      CMP R7, #0  //gets condition codes for array null character
      BEQ EIDINDEX //take branch if array null is reached
      LDRB R6, [R2] //loads character from address
      CMP R6, R7 //compares the two characters
      BNE DIFFERENTCHAR //branches below are to check if characters are the same or different
      BEQ SAMENOLOADCHAR
      EIDINDEX:
      MOVS R0, R4 //puts index counter into return register R0
      POP  {R4-R7,PC} // return

      NOEID:
      MOVS R0, #0 //initialized R0 (return paramater)
      SUBS R0, R0, #1 // makes R0 (-1) (indicating EID is not in list)
      POP  {R4-R7,PC} // return

        .end
