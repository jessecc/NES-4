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
    printf( "Usage: %s -f [file] -mco [size] -dh\n", argv[0] );
    printf( "	-f:	File name of rom/image\n"
            "	-m:	Size of RAM\n"
            "	-c:	Number of CPUs\n"
            "	-o:	Offset to load rom at\n"
            "	-d:	Debug program\n"
            "	-h:	Display this help and exit\n" );
    exit( 0 );
}

int main( int argc, char** argv )
{
    struct arg_s args;
    char ch;
    char *filename = NULL;
    unsigned int i = 0,
                 debug = 0,
                 nCpu = 1,
                 romOffset = 0,
                 memSize = 0;
    Emulator_t *em;

    /*Check/Get Args and all that jive ass bullshit*/
    if( argc < 2 )
        Usage( argv );

    // parse all dem fine command line options
    while (( ch = getopt( argc, argv, "f:m:c:o:dh" )) != -1 && i++ < argc ) {
        switch( ch ) {
        case 'f':
            filename = argv[++i];
            break;
        case 'm':
            memSize = atoi( argv[++i] );
            break;
        case 'c':
            nCpu = atoi( argv[++i] );
            break;
        case 'o':
            romOffset = atoi( argv[++i] );
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

    if ( filename == NULL ) {
        SetError( FATAL_LEVEL, ERROR_NOFILE );
        ErrorExit( );
    }

    // aw yeah gurrl
    memset( &args, 0, sizeof( args ) );
    em = new( Emulator_t );
    if( !em ) {
        ErrorExit( );
    }

    args.filename = filename;
    args.offset = romOffset;
    args.ramSize = memSize;
    args.debugEnable = debug;
    args.cpuNo = nCpu;

    /*Parse args and shiz*/
    /*Emulation Init*/
    if( EmulationInit( &args, em ) != ERROR_SUCCESS ) {
        /*Does not return!*/
        ErrorExit();
    }

    /*Emulation Begin*/
    if ( em->Debugging )
        bf_debugger( em );
    else
        EmulationStart( em );

    /*Emulation Cleanup*/
    if( EmulationCleanup( em ) != ERROR_SUCCESS ) {
        ErrorExit();
    }

    return 0;
}
