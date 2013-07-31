#ifndef __MEMORY_INTERFACE_H_
#define __MEMORY_INTERFACE_H_
#include <stdint.h>
/*These are functions that will allow us to read from and write to RAM*/
BYTE		ReadByte( WORD addr );
BYTE		WriteByte( WORD addr, BYTE data );
WORD		ReadWord( WORD addr );
WORD		WriteWord( WORD addr, WORD data );

BYTE		ReadBytePtr( WORD addr );
BYTE		WriteBytePtr( WORD addr, BYTE data );
WORD		ReadWordPtr( WORD addr );
WORD		WriteWordPtr( WORD addr, WORD data );

#endif