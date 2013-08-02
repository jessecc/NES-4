#include <stdio.h>
#include "nesdb.h"

int dbg_printmem( Emulator_t *dem, int argc, char **argv ){
	if ( argc < 3 ){
		printf( "usage: %s [address] [length] [format]\n", argv[0] );
		return 1;
	}

	unsigned int i, j;
	unsigned int addr = atoi( argv[1] );
	unsigned int len = atoi( argv[2] );
	char *fmt;

	if ( argc > 3 )
		fmt = argv[3];
	else
		fmt = "%02x ";

	if ( addr > 0xffff ){
		printf( "Inconcievable!\n" );
		return 2;
	}

	for ( i = addr; i < len; i++ ){
		if ( !(( i - addr ) % 16 )){
			if ( i - addr ) printf( "\n" );
			printf( "[%04x]\t", i );
		}

		if ( !(( i - addr ) % 8 ))
			printf( "  " );

		printf( fmt, dem->ram[ i ] );
	}

	printf( "\n" );

	return 0;
}
	

void DbgPrint( char *str ){
	printf( "%s\n", str );
}
