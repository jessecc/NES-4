#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "GenericTypes.h"
#include "6502Types.h"

/*Strings*/
#define CONSOLE_STRING			" > "
/*Commands*/
#define CMD_ENABLE_BP			1
#define CMD_DISABLE_BP			2
#define CMD_PRNT_REG			3
#define CMD_PRNT_MEM			4
#define CMD_EDIT_REG			5
#define CMD_EDIT_MEM			6
#define CMD_HELP				7
#define CMD_CONT				8
#define CMD_QUIT				9
#define CMD_NEXT_INS			10
/*Command Strings*/
#define CMD_STR_1				"brk"
#define CMD_STR_2				"rmbrk"
#define CMD_STR_3				"preg"
#define CMD_STR_4				"pmem"
#define CMD_STR_5				"regedit"
#define CMD_STR_6				"memedit"
#define CMD_STR_7				"help"
#define CMD_STR_8				"c"
#define CMD_STR_9				"quit"
#define CMD_STR_10				"ni"

typedef struct
{
	struct breakpoint_t* next;
	WORD address;
	BYTE number;
}breakpoint_t;

breakpoint_t* breakpointList;
breakpoint_t* lastbreakpoint;

/*This is the main function that will be called by the emulator*/
void Debug( );
/*EnableBreakpoint takes a memory location and return a breakpoint number*/
static DWORD EnableBreakpoint( char* memoryLocation	);
/*DisableBreakpoint takes a breakpoint number and removes it from the breakpoint list*/
static void DisableBreakpoint( DWORD breakpointNumber );
/*Prints all registers to stdout*/
static void PrintRegisters( );
/*Prints length number of bytes at address in the style of format*/
static void PrintMemory( DWORD address, DWORD length, BYTE format );
/*Reads a line from the user and returns that line*/
static char* _ReadLine( );
/*Parses the user line and returns an index to determine the type of the command*/
static DWORD _ParseLine( );
static DWORD EditRegister( BYTE reg, WORD val );
static DWORD EditMemory( WORD addr, BYTE data );
static breakpoint_t* _MakeBreakPoint( breakpoint_t* next, WORD addr, BYTE num );
static void _DeleteBreakPoint( breakpoint_t* bp );
#endif