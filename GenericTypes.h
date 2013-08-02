#ifndef __GENERIC_TYPES_H_
#define __GENERIC_TYPES_H_

#include <stdint.h>

typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;

#define BYTE_MAX  0xff
#define WORD_MAX  0xffff
#define DWORD_MAX 0xffffffff

struct e6502_t
{
    BYTE	accumulator;
	BYTE	xIndex;
	BYTE	yIndex;
	BYTE	status;
	WORD	pCounter;
	BYTE	sPointer;
	
	BYTE	memory[ WORD_MAX ];
};

#endif
