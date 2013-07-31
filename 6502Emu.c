/*System Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Local Includes*/
#include "6502Emu.h"
#include "6502CPU.h"
#include "6502Cycles.h"
#include "GenericTypes.h"
#include "Error.h"
#include "Debug.h"

/*Defines and defaults*/
#define DEFAULT_RAM_SIZE		0x1000	//4K of RAM
#define RAM_SIZE_MAX			   0xFFFF	//64K RAM
#define NUM_CYCLES            100      //This is just a guess.  I have no idea how many I should actually execute

unsigned int EmulationInit( struct *arg_s args )
{
	DWORD ramSize;
	/*Allocate various structures and the like*/
	if( args->ramSize >= RAM_SIZE_MAX || args->ramSize <= ( DEFAUL_RAM_SIZE / 2 ) )
	{
		DbgPrint( "RAM sized to default due to illegal size" );
		ramSize = DEFAULT_RAM_SIZE;
	}
	ram = malloc( ramSize );
	if( !ram )
	{
		SetError( FATAL_LEVEL, ERROR_NOMEM );
		return 1;
	}
	if( 6502CPUInit( ram ) != ERROR_SUCCESS )
	{
		/*We don't need to set an error here because the 6502CPUInit function will have done this already*/
		return 1;
	}
	/*In case of Video or other peripherial devices do a start up for them too*/
	/*return on error*/
	return ERROR_SUCCESS;
}

void EmulationStart( )
{
	BYTE opcode = 0x00;
	int cyclesLeft = 0;		/*Should be calcuated or something.  I don't know...*/
	for(;;)
	{
      cyclesLeft += 100;
		/*I can't really get an answer about how to calculate the number the number of cycles to execute so fuck it man*/

		while( cyclesLeft > 0 )
		{
			6502CPUFetch( &opcode );
			/*Get the number of cycles this opcode requires*/
			/*Subtract this from cycles left*/
			cyclesLeft -= cycleTable[opcode];
			6502CPUExecute( opcode );
			/*Check to see if the opcode executed was CLI.  If so then break out of this loop*/
         if( opcode == 0x58 )
         {
            break;
         }
		}
		/*This is where we will check for interupts and the like*/
		/*What they are and when they will occur I have no idea on either account.  But something may happen with them here*/
		6502Interrupt( );
	}
}

unsigned int EmulationCleanup( ) 
{
	/*Free dat rams and such*/

	free( ram );

	/*Do other cleanup*/
}
