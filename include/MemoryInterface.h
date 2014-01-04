#ifndef __MEMORY_INTERFACE_H_
#define __MEMORY_INTERFACE_H_
#include <stdint.h>

typedef struct minterface {
    uint8_t *memory;
    uint8_t *(*map)(uint8_t *, uint16_t);
} minterface_t;

uint8_t		ReadByte( minterface_t *ram, uint16_t addr );
uint8_t 	WriteByte( minterface_t *ram, uint16_t addr, uint8_t data );
uint16_t	ReadWord( minterface_t *ram, uint16_t addr );
uint16_t	WriteWord( minterface_t *ram, uint16_t addr, uint16_t data );

uint8_t		ReadBytePtr( minterface_t *ram, uint16_t addr );
uint8_t		WriteBytePtr( minterface_t *ram, uint16_t addr, uint8_t data );
uint16_t	ReadWordPtr( minterface_t *ram, uint16_t addr );
uint16_t	WriteWordPtr( minterface_t *ram, uint16_t addr, uint16_t data );

uint8_t 	*map_nes( uint8_t *, uint16_t );

#endif
