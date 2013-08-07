#include "nesdb.h"

int bfid_peek( Emulator_t *dem, int argc, char **argv ){
	if ( argc < 2 ){
		printf( "usage: %s [where]\n", argv[0] );
		return 1;
	}

	int where = atoi( argv[1] );
	if ( where >= dem->ramSize ){
		printf( "\"%d\" is out of memory range.\n", where );
		return 3;
	}

	return dem->ram[ where ]; 
}

int bfid_poke( Emulator_t *dem, int argc, char **argv ){
	if ( argc < 4 ){
		printf( "usage: %s [where] [value]\n", argv[0] );

		return 1;
	}

	int where = atoi( argv[1] );
	int val = atoi( argv[2] );

	if ( where >= dem->ramSize ){
		printf( "\"%d\" is out of memory range.\n", where );
		return 3;
	}

	dem->ram[ where ] = val;

	return 0;
}

int dbg_regs( Emulator_t *dem, int argc, char **argv ){
	int i = 0, j = dem->cpuNo;
	for ( ; i < j; i++ ){
		printf( "---===[ CPU%d\n", i );
		printf( "instr.ptr:\t%d\n", 		dem->Cpus[i].pCounter );
		printf( "stackptr:\t%d\n", 		dem->Cpus[i].sPointer );
		printf( "accumulator:\t%d\n", 	dem->Cpus[i].accumulator );
		printf( "xIndex:\t\t%d\n",	dem->Cpus[i].xIndex );
		printf( "yIndex:\t\t%d\n",	dem->Cpus[i].yIndex );
		printf( "status:\tcarry:\t%d\n", dem->Cpus[i].statusCarry );
		printf( "\tdec:\t%d\n",		dem->Cpus[i].statusDec );
		printf( "\tneg:\t%d\n",		dem->Cpus[i].statusNeg );
		printf( "\toverf:\t%d\n",	dem->Cpus[i].statusOverflow );
		printf( "\tzero:\t%d\n",	dem->Cpus[i].statusZero );
		printf( "\tinter:\t%d\n",	dem->Cpus[i].statusInterrupt );
	}

	return 0;
}

/* TODO
int dbg_disas( Emulator_t *dem, int argc, char **argv ){
}
*/

/* TODO
int bfid_trace( Emulator_t *dem, int argc, char **argv ){
	if ( argc < 2 ){
		printf( "usage: %s [type]\n", argv[0] );
		printf( "types:	loop 	- trace where loops enter/leave.\n" );
		return 1;
	}

	if ( strcmp( argv[1], "loop" ) == 0 ) {
	} else {
		printf( "Invalid type.\n" );
		return 2;
	}

	return 0;
}
*/
