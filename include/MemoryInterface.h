#ifndef __MEMORY_INTERFACE_H_
#define __MEMORY_INTERFACE_H_

#include <GenericTypes.h>

BYTE	ReadByte( minterface_t *ram, WORD addr );
BYTE	WriteByte( minterface_t *ram, WORD addr, BYTE data );
WORD	ReadWord( minterface_t *ram, WORD addr );
WORD	WriteWord( minterface_t *ram, WORD addr, WORD data );

BYTE	ReadBytePtr( minterface_t *ram, WORD addr );
BYTE	WriteBytePtr( minterface_t *ram, WORD addr, BYTE data );
WORD	ReadWordPtr( minterface_t *ram, WORD addr );
WORD	WriteWordPtr( minterface_t *ram, WORD addr, WORD data );

BYTE *map_nes( BYTE *, WORD );

#endif
