/*System Includes*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Local includes*/
#include "Error.h"

void ErrorExit( )
{
	ShowLastError();
	exit( globalError );
}

void ShowLastError( )
{
	printf( "%s: %s\n", levelString[errorLevel], errorMessages[globalError] );
}

void SetError( unsigned int errLvl, unsigned int err )
{
	SetErrorLevel( errLvl );
	globalError = err;
}

unsigned int GetErrorLevel( )
{
	return errorLevel;
}

void ClearError( ) 
{
	SetError( NONE_LEVEL, ERROR_SUCCESS );
}