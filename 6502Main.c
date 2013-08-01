/*System Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Local Includes*/
#include "Error.h"
#include "6502Emu.h"
#include "GenericTypes.h"

struct arg_s
{
	/*Do stuff here*/
	unsigned int ramSize;
	DWORD debugEnable;
};

void Usage( char** argv )
{
	/*THIS WILL NOT RETURN!*/
	printf( "Invalid Argument/Syntax\n" );
	printf( "Usage\n\t%s [options] rom/imagename\n", argv[0] );
	exit( 0 );
}

int main( int argc, char** argv )
{
	struct arg_s args;
	/*Check/Get Args and all that jive ass bullshit*/
	if( argc < 2 ) 
	{
		/*Doesn't return!*/
		Usage( argv );
	}
	memset( args, 0, sizeof( args ) );
	/*Parse args and shiz*/
	/*Emulation Init*/
	if( EmulationInit( &args ) != ERROR_SUCCESS )
	{
		/*Does not return!*/
		ErrorExit();
	}
	/*Emulation Begin*/
	EmulationStart();

	/*Emulation Cleanup*/
	if( EmulationCleanup() != ERROR_SUCCESS ) 
	{
		ErrorExit();
	}

	return 0;
}