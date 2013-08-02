#ifndef __ERROR_H_
#define __ERROR_H_

#define ERROR_SUCCESS		0
#define ERROR_NOMEM			1
#define ERROR_POINTER		2

#define WARN_LEVEL			0
#define ERROR_LEVEL			1
#define FATAL_LEVEL			2
#define NONE_LEVEL			3

char *errorMessages[] = {
	"Success!",
	"No Memory!",
	"Invalid Pointer!"
};

char *levelString[] = {
	"Warning",
	"Error", 
	"Fatal",
	"None"
};

/*Global error value*/
unsigned int globalError;
unsigned int errorLevel;

void ErrorExit();
void ShowLastError();
void SetError( unsigned int, unsigned int );
void SetErrorLevel( unsigned int );
unsigned int GetErrorLevel();
void ClearError();
#endif
