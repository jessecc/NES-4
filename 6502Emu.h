#ifndef __6502_EMU_H_
#define __6502_EMU_H_

BYTE* ram = NULL;
DWORD ramSize;
static DWORD debugEnable;

unsigned int EmulationInit( struct *arg_s );
void EmulationStart();
unsigned int EmulationCleanup( );
#endif