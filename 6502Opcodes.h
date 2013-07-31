#ifndef __6502_OPCODES_H_
#define __6502_OPCODES_H_
void (*FunctionTable[0xFF])();	/*256 function pointers*/

void (*FunctionTable[0xFF]) = 
{
	BreakImm, Jam, 
};
#endif