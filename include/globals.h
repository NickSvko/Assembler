
#ifndef UNTITLED_GLOBALS_H
#define UNTITLED_GLOBALS_H


/*
   This file defines global constants, data types, and enums used across the assembler program.
   It establishes various characteristics essential for proper functioning.
*/


/* Maximum line length from source file - not including '\n' */
#define maxLineLength 80

/* Initial the dataType of IC(instruction counter) */
#define ICInitialValue 100

#define maxLabelLength 31
#define maxRegisterLength 2

/* Characteristics of a signed integer consisting of one byte (including +/- sign) */
#define max1ByteIntLength 4
#define min1ByteIntVal (-128)
#define max1ByteIntVal 127

/* Characteristics of a signed integer consisting of two byte (including +/- sign) */
#define max2BytesIntLength 6
#define min2BytesIntVal (-32768)
#define max2BytesIntVal 32767

/* Characteristics of a signed integer consisting of four byte (including +/- sign) */
#define max4BytesIntLength 11
#define min4BytesIntVal (-2147483648)     /* (-2)^31 */
#define max4BytesIntVal 2147483647      /* 2^31 - 1 */

/* Characteristics of a signed integer consisting of 25bits (including +/- sign) */
#define max25bitsIntVal 16777215
#define min25BitsIntVal (-16777216)

#define maxInstructionLength 5
#define maxDirectiveName 7

#define minRegister 0
#define maxRegister 31

/* true/false definition */
typedef enum bool{FALSE,TRUE} bool;

/* valid/invalid definition */
typedef enum state{INVALID = 0, VALID = 1} state;


#endif
