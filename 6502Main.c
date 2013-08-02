/*System Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*Local Includes*/
#include "Error.h"
#include "6502Emu.h"
#include "GenericTypes.h"

void Usage( char** argv )
{
	/*THIS WILL NOT RETURN!*/
	printf( "Usage: %s -f [file] -mc [size] -dh\n", argv[0] );
	printf( "	-f:	File name of rom/image\n"
		"	-m:	Size of RAM\n"
		"	-c:	Number of CPUs\n"
		"	-d:	Debug program\n"
		"	-h:	Display this help and exit\n" );
	exit( 0 );
}

int main( int argc, char** argv )
{
	struct arg_s args;
	char ch, *filename;
	int 	i, 
		debug = 0, 
		nCpu = 0, 
		memSize = 0;
	Emulator_t *em;

	/*Check/Get Args and all that jive ass bullshit*/
	if( argc < 2 ) 
		Usage( argv );

	// parse all dem fine command line options
	while (( ch = getopt( argc, argv, "f:m:c:dh" )) != -1 && i++ < argc ){
		switch( ch ){
			case 'f':
				filename = argv[++i];
				break;
			case 'm':
				memSize = atoi( argv[++i] );
				break;
			case 'c':
				nCpu = atoi( argv[++i] );
				break;
			case 'd':
				debug = 1;
				break;
			case 'h':
				Usage( argv );
				break;
			default:
				exit( 1 );
		}
	}

	// aw yeah gurrl
	memset( &args, 0, sizeof( args ) );
	em = new( Emulator_t );

	args.ramSize = memSize;
	args.debugEnable = debug;
	args.cpuNo = nCpu;

	/*Parse args and shiz*/
	/*Emulation Init*/
	if( EmulationInit( &args, em ) != ERROR_SUCCESS )
	{
		/*Does not return!*/
		ErrorExit();
	}

	/*Emulation Begin*/
	if ( em->Debugging )
		bf_debugger( em );
	else
		EmulationStart( em );

	/*Emulation Cleanup*/
	if( EmulationCleanup( em ) != ERROR_SUCCESS ) 
	{
		ErrorExit();
	}

	return 0;
}
