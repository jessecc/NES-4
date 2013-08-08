#ifndef __GENERIC_TYPES_H_
#define __GENERIC_TYPES_H_

#include <stdint.h>

typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef unsigned int FLAG;
typedef enum { false, true } bool;

#define BYTE_MAX  0xff
#define WORD_MAX  0xffff
#define DWORD_MAX 0xffffffff

#define new( n ) calloc( 1, sizeof( n ) );

typedef struct e6502
{
	BYTE	accumulator;
	BYTE	xIndex;
	BYTE	yIndex;
	WORD	pCounter;
	BYTE	sPointer;

	FLAG	statusCarry		:1;
	FLAG	statusZero		:1;
	FLAG	statusInterrupt	:1;
	FLAG	statusDec		:1;
	FLAG	statusBreak		:1; //Not real. When pushed to the stack: true for a BRK or PHP instruction, false for an IRQ or NMI
	//FLAG	statusUnk		:1; //Always true
	FLAG	statusOverflow	:1;
	FLAG	statusNeg		:1;
	
	BYTE	*memory;	
} e6502_t;

#endif
