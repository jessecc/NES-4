#ifndef __6502_EMU_H_
#define __6502_EMU_H_

#include "GenericTypes.h"
//BYTE* ram = NULL;
//DWORD ramSize;
//static DWORD debugEnable;
struct device;

typedef struct arg_s
{
	/*Do stuff here*/
	unsigned int ramSize;
	bool debugEnable;
	unsigned int cpuNo;
	unsigned int offset;
	char *filename;

	DWORD debugFlags;
} arg_s_t;

typedef struct Emulator { 
	e6502_t *Cpus;

	BYTE *ram;
	unsigned int ramSize;
	unsigned int cpuNo;

	struct device *Devices;
	bool Debugging;
	bool running;

	char *filename;
	FILE *fp;
} Emulator_t;

unsigned int EmulationInit( struct arg_s *args, Emulator_t *em );
void EmulationStart( Emulator_t *em );
unsigned int EmulationCleanup( Emulator_t *em );

void CpuStep( e6502_t *cpu );
void EmulationStep( Emulator_t *em );

#endif
