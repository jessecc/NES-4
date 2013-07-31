#include "MemoryInterface.h"

extern BYTE* ram;

/*Read/Write Primitives*/
BYTE ReadByte( WORD addr )
{
	return ram[ addr ];
}

BYTE WriteByte( WORD addr, BYTE data )
{
	return ( ram[ addr ] = data );
}

WORD ReadWord( WORD addr )
{
	return ( ( ram[ addr ] ) + ( ram[ addr + 1 ] << 8 ) );
}

WORD WriteWord( WORD addr, WORD data )
{
	ram[addr] = data & 0xFF;
	ram[addr+1] = ( data >> 8 ) & 0xFF;
	return ram[addr];
}
/*Read/Write pointer*/
BYTE ReadBytePtr( WORD addr )
{
	WORD dataptr = ReadWord( addr );
	return ReadByte( dataptr );
}