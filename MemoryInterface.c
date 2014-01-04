#include <MemoryInterface.h>

/* sigkill says:
 * I've rewritten all these primitives to work with minterface_t rather than with uint8_t pointers.
 * Also, I've restructured the "higher order" functions to operate based on the more primitive ones,
 * ie, Word functions are now amalgamations of Byte functions.
 * If it were me, I'd suggest having these functions accept an minterface_t * - passing by pointer rather than by value;
 * however, I'm leaving that alone for now as it breaks existing code.
 * Speaking of breaking things, these functions can now all conceivably fail. I figured that, for the Read family,
 * returning a null byte is acceptable behaviour - as if the given byte of memory was unplugged or something.
 * The write family of functions now indicate failure by returning a value other than their parameter. That's kinda weird. */

uint8_t ReadByte( minterface_t *ram, uint16_t addr )
{
    /* If the location is readable, return its value; otherwise return a null byte */
    /*
    printf( "[ReadByte] ram: 0x%x, ram->map: 0x%x, ram->memory: 0x%x\n",
    		ram, ram->map, ram->memory );
    		*/
    uint8_t *bt = ram->map( ram->memory, addr );
    return ( bt ) ? *bt : 0x00;
}

uint8_t WriteByte( minterface_t *ram, uint16_t addr, uint8_t data )
{
    /* If the address is writable, write to it and return its value; otherwise return a byte other than data */
    uint8_t *bt = ram->map( ram-> memory, addr );
    return ( bt ) ? ( *bt ) = data : ~ data;
}

uint16_t ReadWord( minterface_t *ram, uint16_t addr )
{
    return ReadByte( ram, addr ) & ( ReadByte( ram, addr + 1 ) << 8 );
}

uint16_t WriteWord( minterface_t *ram, uint16_t addr, uint16_t data )
{
    return WriteByte( ram, addr, data & 0xff ) & ( WriteByte( ram, addr + 1, data >> 8 ) << 8 );
}

/*Read/Write pointer*/
uint8_t ReadBytePtr( minterface_t *ram, uint16_t addr )
{
    uint16_t dataptr = ReadWord( ram, addr );
    return ReadByte( ram, dataptr );
}

/* This definitely doesn't belong here.
 * It's an example map function that maps a 64k address space onto the NES's physical memory. */
uint8_t *map_nes( uint8_t *memory, uint16_t addr )
{
    /* This function maps a 64k address space onto the NES's memory - size 0xC8O8 */

    if( addr < 0x2000 ) { /* Map 2k of general RAM - mirrored 3 times (+1 original) */
        return &memory[ addr % 0x0800 ];
    } else if( addr < 0x4000 ) { /* Map 8 bytes of PPU registers - mirrored 1023 times (+1 original) */
        return &memory[ ( addr - 0x2000 ) % 8 + 0x0800 ];
    } else { /* Map the remainder of memory - APU and IO registers, and cartridge space. No mirroring */
        return &memory[ ( addr - 0x4000 ) + 0x0808 ];
    }
}
