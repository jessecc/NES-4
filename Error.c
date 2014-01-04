/*System Includes*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Local includes*/
#include "Error.h"

unsigned int globalError;
unsigned int errorLevel;

char *errorMessages[] = {
    "Success!",
    "No Memory!",
    "Invalid Pointer!",
    "No File"
};

char *levelString[] = {
    "Warning",
    "Error",
    "Fatal",
    "No File"
};


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
    //SetErrorLevel( errLvl );
    errorLevel = errLvl;
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
