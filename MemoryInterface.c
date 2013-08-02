#include "MemoryInterface.h"

//extern BYTE* ram;

/*Read/Write Primitives*/
BYTE ReadByte( BYTE *ram, WORD addr )
{
	return ram[ addr ];
}

BYTE WriteByte( BYTE *ram, WORD addr, BYTE data )
{
	return ( ram[ addr ] = data );
}

WORD ReadWord( BYTE *ram, WORD addr )
{
	return ( ( ram[ addr ] ) + ( ram[ addr + 1 ] << 8 ) );
}

WORD WriteWord( BYTE *ram, WORD addr, WORD data )
{
	ram[addr] = data & 0xFF;
	ram[addr+1] = ( data >> 8 ) & 0xFF;
	return ram[addr];
}
/*Read/Write pointer*/
BYTE ReadBytePtr( BYTE *ram, WORD addr )
{
	WORD dataptr = ReadWord( ram, addr );
	return ReadByte( ram, dataptr );
}
