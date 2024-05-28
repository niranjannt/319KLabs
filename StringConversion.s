// StringConversion.s
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Runs on any Cortex M0
// ECE319K lab 6 number to string conversion
//
// You write udivby10 and Dec2String
     .data
     .align 2
// no globals allowed for Lab 6
    .global OutChar    // virtual output device
    .global OutDec     // your Lab 6 function
    .global Test_udivby10

    .text
    .align 2
// **test of udivby10**
// since udivby10 is not AAPCS compliant, we must test it in assembly
Test_udivby10:
    PUSH {LR}

    MOVS R0,#123
    BL   udivby10
// put a breakpoint here
// R0 should equal 12 (0x0C)
// R1 should equal 3

    LDR R0,=12345
    BL   udivby10
// put a breakpoint here
// R0 should equal 1234 (0x4D2)
// R1 should equal 5

    MOVS R0,#0
    BL   udivby10
// put a breakpoint here
// R0 should equal 0
// R1 should equal 0
    POP {PC}

//****************************************************
// divisor=10
// Inputs: R0 is 16-bit dividend
// quotient*10 + remainder = dividend
// Output: R0 is 16-bit quotient=dividend/10
//         R1 is 16-bit remainder=dividend%10 (modulus)
// not AAPCS compliant because it returns two values
udivby10:
   PUSH {LR}
    MOVS R1, #10
    LDR R4, =0x00010000
    MOVS R3, #0
    MOVS R2, #16
    LSLS R1, #15
    AGAIN:
    LSRS R4, R4, #1
    CMP R0, R1
    BLO next
    SUBS R0, R0, R1
    ORRS R3, R3, R4
next:
    LSRS R1, R1, #1
    SUBS R2, R2, #1
    BNE AGAIN
    MOVS R1, R0
    MOVS R0, R3
   POP  {PC}

  
//-----------------------OutDec-----------------------
// Convert a 16-bit number into unsigned decimal format
// ECE319H students must use recursion
// Call the function OutChar to output each character
// OutChar does not do actual output, OutChar does virtual output used by the grader
// You will call OutChar 1 to 5 times
// Input: R0 (call by value) 16-bit unsigned number
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11
//binding
.equ counter, 4
OutDec:
    PUSH {R4-R7, LR}
    //allocation
    SUB SP, SP, #16
    MOV R7, SP
    MOVS R5, #0
    COMPARE: CMP R0, #9
    BLS DONE
    BL udivby10
    //store local variables on stack
    STR R1, [R7, R5]
    ADDS R5, R5, #counter
    B COMPARE

    DONE:
    ADDS R0, R0, #48
    BL OutChar
    READ: SUBS R5, R5, #4
    BMI LAST
    //read local variables from stack
    LDR R0, [R7, R5]
    ADDS R0, R0, #48
    BL OutChar
    B READ
    LAST:
    //deallocation
    ADD SP, SP, #16
    POP  {R4-R7, PC}
//* * * * * * * * End of OutDec * * * * * * * *



     .end
