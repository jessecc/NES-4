/*System Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Local Includes*/
#include <6502Emu.h>
#include <6502CPU.h>
#include <6502Cycles.h>
#include <GenericTypes.h>
#include <Error.h>
#include <Debug.h>
#include <MemoryInterface.h>

/*Defines and defaults*/
#define DEFAULT_RAM_SIZE	0xffff	//4K of RAM
#define RAM_SIZE_MAX		0xFFFF	//64K RAM
#define NUM_CYCLES		100	//This is just a guess.  I have no idea how many I should actually execute

unsigned int EmulationInit( struct arg_s *args, Emulator_t *em )
{
    //BYTE *ram;
    minterface_t *ram;

    int i;
    long int romSize;
    FILE *fp = NULL;

    /*Allocate various structures and the like*/
    if( args->ramSize >= RAM_SIZE_MAX || args->ramSize <= ( DEFAULT_RAM_SIZE / 2 ) ) {
        DbgPrint( "RAM sized to default due to illegal size" );
        args->ramSize = DEFAULT_RAM_SIZE;
    }

    //ram = new( BYTE[ args->ramSize ]);
    ram = new( minterface_t );

    if( !ram ) {
        SetError( FATAL_LEVEL, ERROR_NOMEM );
        return 1;
    }
    ram->memory = new( BYTE[ args->ramSize ]);
    ram->map = map_nes;
    //printf( "[EmulationInit] ram->memory: 0x%x\n", ram->memory );

    if( CPUInit( ram ) != ERROR_SUCCESS ) {
        /*We don't need to set an error here because the 6502CPUInit function will have done this already*/
        return 1;
    }

    if (( fp = fopen( args->filename, "r" )) == NULL ) {
        SetError( FATAL_LEVEL, ERROR_NOFILE );
        return 1;
    }

    // read input file into emulator's ram
    fread( ram->memory + args->offset, args->ramSize - args->offset, 1, fp );

    // Initialize emulator structure
    em->ramSize = args->ramSize;
    //em->ram = ram;
    em->filename = strdup( args->filename );
    em->fp = fp;

    em->Cpus = new( e6502_t[args->cpuNo] );
    if( !(em->Cpus) ) {
        return ERROR_NOMEM;
    }
    em->cpuNo = args->cpuNo;
    for ( i = 0; i < args->cpuNo; i++ ) {
        em->Cpus[i].memory = ram;
        /*
        printf( "[EmulationInit] cpu %d->memory: 0x%x, cpu %d->memory->memory: 0x%x\n",
        		i, em->Cpus[i].memory, i, em->Cpus[i].memory->memory );
        		*/
    }

    em->Devices = NULL;
    em->Debugging = args->debugEnable;

    /*In case of Video or other peripherial devices do a start up for them too*/
    /*return on error*/
    return ERROR_SUCCESS;
}

void CpuStep( e6502_t *cpu )
{
    BYTE opcode = 0x00;
    opcode = ReadByte( cpu->memory, cpu->pCounter++ );

    CPUExecute( cpu, opcode );

    if ( cpu->statusInterrupt )
        CPUInterrupt( cpu );
}

void EmulationStep( Emulator_t *em )
{
    int i;
    for ( i = 0; i < em->cpuNo; i++ )
        CpuStep( &em->Cpus[i] );

}

void EmulationStart( Emulator_t *em )
{
    //int cyclesLeft = 0;		/*Should be calcuated or something.  I don't know...*/
    em->running = true;

    while( em->running ) {
        EmulationStep( em );
    }
}

unsigned int EmulationCleanup( Emulator_t *em )
{
    /*Free dat rams and such*/

    //free( em->ram );
    free( em );

    /*Do other cleanup*/
}
