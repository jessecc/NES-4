#ifndef __GENERIC_TYPES_H_
#define __GENERIC_TYPES_H_

#include <stdint.h>

#include <MemoryInterface.h>

/*
typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef unsigned int FLAG;
*/
typedef enum { false, true } bool;

#define BYTE_MAX  0xff
#define WORD_MAX  0xffff
#define DWORD_MAX 0xffffffff

#define new( n ) calloc( 1, sizeof( n ) );

/* Memory interface structure.
 * This needs to be constructed externally and passed into the e6502_t structure somehow.
 * It's used by the primitizes in MemoryInterface.c.
 * The memory array provides a buffer of arbitrary size; the map function maps a 64k address space onto that buffer.
 * The map function takes a uint8_t array (usually the memory array) and an address. It is permitted to return NULL,
 * if the address requested isn't readable or writable. This could be expanded a little by maybe having a
 * readMap AND a writeMap function, which would allow one to emulate true ROM. That can be dealth with later though.
 * -sigkill */

typedef struct e6502 {
    uint8_t	accumulator;
    uint8_t	xIndex;
    uint8_t	yIndex;
    uint16_t	pCounter;
    uint8_t	sPointer;

    unsigned int	statusCarry		:1;
    unsigned int	statusZero		:1;
    unsigned int	statusInterrupt	:1;
    unsigned int	statusDec		:1;
    unsigned int	statusBreak		:1; //Not real. When pushed to the stack: true for a BRK or PHP instruction, false for an IRQ or NMI
    //unsigned int	statusUnk		:1; //Always true
    unsigned int	statusOverflow	:1;
    unsigned int	statusNeg		:1;

    minterface_t 	*memory;
} e6502_t;


#endif
