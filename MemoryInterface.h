#ifndef __MEMORY_INTERFACE_H_
#define __MEMORY_INTERFACE_H_
#include <stdint.h>
#include "GenericTypes.h"
/*These are functions that will allow us to read from and write to RAM*/
BYTE		ReadByte( BYTE *ram, WORD addr );
BYTE		WriteByte( BYTE *ram, WORD addr, BYTE data );
WORD		ReadWord( BYTE *ram, WORD addr );
WORD		WriteWord( BYTE *ram, WORD addr, WORD data );

BYTE		ReadBytePtr( BYTE *ram, WORD addr );
BYTE		WriteBytePtr( BYTE *ram, WORD addr, BYTE data );
WORD		ReadWordPtr( BYTE *ram, WORD addr );
WORD		WriteWordPtr( BYTE *ram, WORD addr, WORD data );

#endif
