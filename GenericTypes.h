#ifndef __GENERIC_TYPES_H_
#define __GENERIC_TYPES_H_

#include <stdint.h>

typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef enum { false, true } bool;

#define BYTE_MAX  0xff
#define WORD_MAX  0xffff
#define DWORD_MAX 0xffffffff

#define new( n ) (memset( malloc( sizeof( n )), 0, sizeof( n )))

typedef struct e6502
{
	BYTE	accumulator;
	BYTE	xIndex;
	BYTE	yIndex;
	WORD	pCounter;
	BYTE	sPointer;

	BYTE	statusCarry;
	BYTE	statusDec;
	BYTE	statusNeg;
	BYTE	statusOverflow;
	BYTE	statusZero;
	BYTE	statusInterrupt;
	BYTE	statusUnk;
	
	BYTE	*memory;
	BYTE	hasInterrupts;

	bool	interruptDisable;
	bool	broke;
} e6502_t;

#endif
