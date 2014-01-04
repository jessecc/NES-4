#ifndef _nesdbg_header_h
#define _nesdbg_header_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "6502Emu.h"
#include "GenericTypes.h"

enum {
    BRK_IP,
    BRK_MEM,
    BRK_INSTR
};

typedef struct bfid_cmd_hook {
    char *cmd;
    struct bfid_cmd_hook *next;
} bfid_cmd_hook_t;

typedef struct bfid_breakpoint {
    int val;
    int i;
    struct bfid_breakpoint *next;
    struct bfid_breakpoint *prev;
    int type;
    struct bfid_cmd_hook *hooks;
} bfid_brkp_t;

typedef int (*bfid_func_t)( Emulator_t *dem, int argc, char **argv );

typedef struct bfid_command {
    char *name;
    char *help;
    char *usage;
    bfid_func_t func;
    struct bfid_command *next;
} bfid_cmd_t;

typedef struct bfid_var {
    char *name;
    char *val;

    struct bfid_var *next;
} bfid_var_t;

int debug_signal( int s );
int set_variable( char *name, char *val );
char *get_variable( char *name );

//int bfid_step( Emulator_t *dem, int argc, char **argv );
//int bfid_dump( Emulator_t *dem, int argc, char **argv );
int bfid_exit( Emulator_t *em, int argc, char **argv );
int bfid_set( Emulator_t *dem, int argc, char **argv );
int bfid_help( Emulator_t *dem, int argc, char **argv );
int bfid_hook( Emulator_t *dem, int argc, char **argv );
//int bfid_disas( Emulator_t *dem, int argc, char **argv );
//int bfid_trace( Emulator_t *dem, int argc, char **argv );
int bfid_break( Emulator_t *dem, int argc, char **argv );
int bfid_clear( Emulator_t *dem, int argc, char **argv );
int bfid_script( Emulator_t *dem, int argc, char **argv );
int bfid_alias( Emulator_t *dem, int argc, char **argv );
int bfid_poke( Emulator_t *dem, int argc, char **argv );
int bfid_peek( Emulator_t *dem, int argc, char **argv );
//int bfid_exec( Emulator_t *dem, int argc, char **argv );
int bfid_echo( Emulator_t *dem, int argc, char **argv );
int dbg_printmem( Emulator_t *dem, int argc, char **argv );
int dbg_cont( Emulator_t *dem, int argc, char **argv );
int dbg_regs( Emulator_t *dem, int argc, char **argv );

#endif
