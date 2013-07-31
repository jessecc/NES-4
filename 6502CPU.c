/*System Includes*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Local Includes*/
#include "6502CPU.h"
#include "6502Opcodes.h"
#include "Error.h"
#include "MemoryInterface.h"

static int clockCount;

void (*FunctionTable[0xFF])();	/*256 function pointers*/
void (*FunctionTable[0xFF]) = 
{
	Break, Jam, 
};

unsigned char cycles[0xFF] = 
{
};

unsigned int 6502CpuInit( unsigned char* ramPtr )
{
	return ERROR_SUCCESS;
}

void 6502CPUFetch( unsigned char* opcode ) 
{
	*opcode = ReadByte( ProgramCounter++ );		/*Should get opcode at PC and then increment either pointing to an argument or the next opcode*/
}

void 6502CPUDecode( unsigned char opcode, short* funcIndex )
{

}

void 6502CPUExecute( unsigned char opcode )
{
	(*FunctionTable[opcode])( );
}

void 6502Interrupt( )
{
	if( InterruptType )
	{
		/*Find out what kind of interrupt it is either NMI or IRQ*/
		switch( InterruptType )
		{
		case NMI_LEVEL:
			//ProgramCounter = ( ram[0xFFFA] ) + ( ram[0xFFFB] << 8 );
			ProgramCounter = ReadWord( 0xFFFA );
			break;
		case IRQ_LEVEL:
			//ProgramCounter = ( ram[0xFFFE] ) + ( ram[0xFFFF] << 8 );
			ProgramCounter = ReadWord( 0xFFFE );
			break;
		default:
			break;
		}
	}
}

static void AddCarryImm( )
{
	BYTE data = ReadByte ( ProgramCounter++ );		/*Post inc says PC now points to next opcode*/
	AddCarryBase( data );
}

static void AddCarryZp( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ );
	BYTE data = ReadByte( dataAddr );
	AddCarryBase( data );
}

static void AddCarryZpIndex( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ ) + XReg;
	BYTE data = ReadByte( dataAddr );
	AddCarryBase( data );
}

static void AddCarryAbs( )
{
	WORD dataAddr = ReadWord( ProgramCounter );
	BYTE data = ReadByte( dataAddr );
	AddCarryBase( data );
	ProgramCounter += 2;
}

static void AddCarryAbsIndexX( )
{
	WORD dataAddr = ReadWord( ProgramCounter ) + XReg;
	BYTE data = ReadByte( dataAddr );
	AddCarryBase( data );
	ProgramCounter += 2;
}

static void AddCarryAbsIndexY( )
{
	WORD dataAddr = ReadWord( ProgramCounter ) + YReg;
	BYTE data = ReadByte( dataAddr );
	AddCarryBase( data );
	ProgramCounter +=2 ;
}

static void AddCarryIndexInd( )
{
	WORD dataAddr = ReadWord( ReadByte( ProgramCounter++ ) + XReg );
	BYTE data = ReadByte( dataAddr );
	AddCarryBase( data );
}

static void AddCarryIndIndex( )
{
	WORD dataAddr = ReadWord( ReadByte( ProgramCounter++ ) );
	BYTE data = ReadByte( dataAddr + YReg );
	AddCarryBase( data );
}

static void AddCarryBase( BYTE data )
{
	DWORD tmp = data + Accumulator + StatusCarry;
	StatusZero = tmp && 0xFF;
	if( StatusDec )
	{
		/*Ignore decimal mode.  It's fucking gay*/
	}
	else
	{
		StatusNeg = tmp && 0x80;
		StatusOverflow = ( !((Accumulator ^ data ) & 0x80) && ((Accumulator ^ tmp) & 0x80) ) ? 1 : 0;
		StatusCarry = ( tmp > 0xFF );
	}
	Accumulator = tmp & 0xFF;
}

static void AndImm( )
{
	BYTE data = ReadByte( ProgramCounter++ );
	AndBase( data );
}

static void AndZp( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ );
	BYTE data = ReadByte( dataAddr );
	AndBase( data );
}

static void AndZpIndex( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ ) + XReg;
	BYTE data = ReadByte( dataAddr );
	AndBase( data );
}

static void AndAbs( )
{
	WORD dataAddr = ReadWord( ProgramCounter );
	BYTE data = ReadByte( dataAddr );
	AndBase( data );
	ProgramCounter += 2;
}

static void AndAbsIndexX( )
{
	WORD dataAddr = ReadWord( ProgramCounter ) + XReg;
	BYTE data = ReadByte( dataAddr );
	AndBase( data );
	ProgramCounter += 2;
}

static void AndAbsIndexY( )
{
	WORD dataAddr = ReadWord( ProgramCounter ) + YReg;
	BYTE data = ReadByte( dataAddr );
	AndBase( data );
	ProgramCounter += 2;
}

static void AndIndexInd( )
{
	WORD dataAddr = ReadWord( ReadByte( ProgramCounter++ ) + XReg );
	BYTE data = ReadByte( dataAddr );
	AndBase( data );
}

static void AndIndIndex( )
{
	WORD dataAddr = ReadWord( ReadByte( ProgramCounter++ ) );
	BYTE data = ReadByte( dataAddr + YReg );
	AndBase( data );
}

static void AndBase( BYTE data )
{
	data &= Accumulator;
	StatusNeg = data && 0x80;
	StatusZero = data && 0xFF;
	Accumulator = data;
}

static void ShlAcc( )
{
	Accumulator = Accumulator << 1;
}

static void ShlZp( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ );
	WriteByte( dataAddr, ReadByte( dataAddr ) << 1 );
}

static void ShlZpIndex( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ ) + XReg;
	WriteByte( dataAddr, ReadByte( dataAddr ) << 1 );
}

static void ShlAbs( )
{
	WORD dataAddr = ReadWord( ProgramCounter );
	WriteByte( dataAddr, ReadByte( dataAddr ) << 1 );
	ProgramCounter += 2;
}

static void ShlAbsIndex( )
{
	WORD dataAddr = ReadWord( ProgramCounter );
	WriteByte( dataAddr, ReadByte( dataAddr ) << 1 );
	ProgramCounter += 2;
}

static void BranchCarryClear( )
{
	if( StatusCarry )
	{
		ProgramCounter += 2;
	}
	else
	{
		ProgramCounter = ReadWord( ProgramCounter );
	}
}

static void BranchCarrySet( )
{
	if( StatusCarry )
	{
		ProgramCounter = ReadWord( ProgramCounter );
	}
	else
	{
		ProgramCounter += 2;
	}
}

static void BranchZero( )
{
	if( StatusZero )
	{
		ProgramCounter += 2;
	}
	else
	{
		ProgramCounter = ReadWord( ProgramCounter );
	}
}

static void BitTestZp( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ );
	BYTE data = ReadByte( dataAddr );
	BitTestBase( data );
}

static void BitTestAbs( )
{
	WORD dataAddr = ReadWord( ProgramCounter );
	BYTE data = ReadByte( dataAddr );
	BitTestBase( data );
}

static void BitTestBase( BYTE data )
{
	StatusNeg = data & 0x80;
	StatusOverflow = data & 0x40;
	StatusZero = data && Accumulator;
}

static void BranchNegative( )
{
	if( StatusNeg )
	{
		ProgramCounter = ReadWord( ProgramCounter );
	}
	else
	{
		ProgramCounter += 2;
	}
}

static void BranchNotZero( )
{
	if( StatusZero )
	{
		ProgramCounter = ReadWord( ProgramCounter );
	}
	else
	{
		ProgramCounter += 2;
	}
}

static void BranchNotNegative( )
{
	if( StatusNeg )
	{
		ProgramCounter += 2;
	}
	else
	{
		ProgramCounter = ReadWord( ProgramCounter );
	}
}

static void Break( )
{
	ProgramCounter = ReadWord( 0xFFFE );
	StatusB = 1;
	StatusInt = 1;
}

static void BranchNoOverflow( )
{
	if( StatusOverflow )
	{
		ProgramCounter += 2;
	}
	else
	{
		ProgramCounter = ReadWord( ProgramCounter );
	}
}

static void BranchOverflow( )
{
	if( StatusOverflow )
	{
		ProgramCounter = ReadWord( ProgramCounter );
	}
	else
	{
		ProgramCounter += 2;
	}
}

static void ClearCarry( )
{
	StatusCarry = 0;
}

static void ClearDecimal( )
{
	StatusDec = 0;
}

static void ClearInterrupts( )
{
	StatusInt = 0;
}

static void ClearOverflow( )
{
	StatusOverflow = 0;
}

static void CmpAccImm( )
{
	CmpBase( ReadByte( ProgramCounter++ ), Accumulator );
}

static void CmpAccZp( )
{
	CmpBase( ReadByte( ReadByte( ProgramCounter++ ) ), Accumulator);
}

static void CmpAccZpIndex( )
{
	CmpBase( ReadByte( ReadByte( ProgramCounter++ ) + XReg ), Accumulator );
}

static void CmpAccAbs( )
{
	CmpBase( ReadByte( ReadWord( ProgramCounter ) ), Accumulator );
	ProgramCounter += 2;
}

static void CmpAccAbsIndexX( )
{
	CmpBase( ReadByte( ReadWord( ProgramCounter ) + Xreg ), Accumulator );
	ProgramCounter += 2;
}

static void CmpAccAbsIndexY( )
{
	CmpBase( ReadByte( ReadWord( ProgramCounter ) + YReg ), Accumulator );
	ProgramCounter += 2;
}

static void CmpAccIndexInd( )
{
	CmpBase( ReadByte( ReadWord( ReadByte( ProgramCounter++ ) + XReg ) ), Accumulator );
}

static void CmpAccIndIndex( )
{
	CmpBase( ReadByte( ReadWord( ReadByte( ProgramCounter++ ) ) + YReg ), Accumulator );
}

static void CmpBase( BYTE data, 6502Reg8 reg )
{
	/*Compare is always a subtraction of Accumulator minus data*/
	StatusCarry = ( data > reg  );
	data = reg - data;
	StatusNeg = data & 0x80;
	StatusZero = (data && 0xFF);
}

static void CmpXImm( )
{
	CmpBase( ReadByte( ProgramCounter++ ), XReg );
}

static void CmpXZp( )
{
	CmpBase( ReadByte( ReadByte( ProgramCounter++ ) ), XReg );
}

static void CmpXAbs( )
{
	CmpBase( ReadByte( ReadWord( ProgramCounter) ), XReg );
	ProgramCounter += 2;
}

static void CmpYImm( )
{
	CmpBase( ReadByte( ProgramCounter++ ), YReg );
}

static void CmpYZp( )
{
	CmpBase( ReadByte( ReadByte( ProgramCounter++ ) ), YReg );
}

static void CmpYAbs( )
{
	CmpBase( ReadByte( ReadWord( ProgramCounter ) ), YReg );
	ProgramCounter += 2;
}

static void DecrementMemZp( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ );
   BYTE data = ReadByte( dataAddr );
	WriteByte( dataAddr, --data );
}

static void DecrementMemZpIndex( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ ) + XReg;
   BYTE data = ReadByte( dataAddr );
	WriteByte( dataAddr, --data );
}

static void DecrementMemAbs( )
{
   WORD dataAddr = ReadWord( ProgramCounter );
   BYTE data = ReadByte( dataAddr );
   WriteByte( dataAddr, --data );
   ProgramCounter += 2;
}

static void DecrementMemAbsIndex( )
{
   WORD dataAddr = ReadWord( ProgramCounter ) + XReg;
   BYTE data = ReadByte( dataAddr );
   WriteByte( dataAddr, --data );
   ProgramCounter += 2;
}

static void DecrementX( )
{
   XReg--;
}

static void DecrementY( )
{
   YReg--;
}

static void XorAccMemImm( )
{
   XorAccMemBase( ReadByte( ProgramCounter++ ) );
}

static void XorAccMemZp( )
{
   XorAccMemBase( ReadByte( ReadByte( ProgramCounter++ ) ) );
}

static void XorAccMemZpIndex( )
{
   XorAccMemBase( ReadByte( ReadByte( ProgramCounter++ ) + XReg ) );
}

static void XorAccMemAbs( )
{
   XorAccMemBase( ReadByte( ReadWord( ProgramCounter ) ) );
   ProgramCounter += 2;
}

static void XorAccMemAbsIndexX( )
{
   XorAccMemBase( ReadByte( ReadWord( ProgramCounter ) + XReg ) );
   ProgramCounter += 2;
}

static void XorAccMemAbsIndexY( )
{
   XorAccMemBase( ReadByte( ReadWord( ProgramCounter ) + YReg ) );
   ProgramCounter += 2;
}

static void XorAccMemIndexInd( )
{
   XorAccMemBase( ReadByte( ReadWord( ReadByte( ProgramCounter++ ) + XReg ) ) );
}

static void XorAccMemIndIndex( )
{
   XorAccMemBase( ReadByte( ReadWord( ReadByte( ProgramCounter++ ) ) + YReg ) );
}

static void XorAccMemBase( BYTE data )
{
   data ^= Accumulator;
   StatusNeg = data & 0x80;
   StatusZer = data && 0xFF;
   Accumulator = data;
}

static void IncrementMemZp( ) 
{
   WORD dataAddr = ReadByte( ProgramCoutner++ );
   BYTE data = ReadByte( dataAddr );
   WriteByte( dataAddr, data++ );
}

static void IncrementMemZpIndex( )
{
   WORD dataAddr = ReadByte( ProgramCounter++ ) + XReg;
   BYTE data = ReadByte( dataAddr );
   WriteByte( dataAddr, data++ );
}

static void IncrementMemAbs( )
{
   WORD dataAddr = ReadWord( ProgramCounter );
   BYTE data = ReadByte( dataAddr );
   WriteByte( dataAddr, data++ );
}

static void IncrementMemAbsIndex( )
{
   WORD dataAddr = ReadWord( ProgramCounter );
   BYTE data = ReadByte( dataAddr );
   WriteByte( dataAddr, data++ );
}

static void IncrementX( )
{
   XReg++;
}

static void IncrementY( )
{
   YReg++;
}

static void JumpAbs( )
{
   ProgramCounter = ReadWord( ProgramCounter );
}

static void JumpInd( )
{
   ProgramCounter = ReadWord( ReadWord( ProgramCounter ) );
}

static void JumpSubroutine( )
{
   StackPush( ( ProgramCounter >> 8 ) & 0xFF );
   StackPush( ProgramCounter & 0xFF );
   ProgramCounter = ReadWord( ProgramCounter );
}

#define STACK_ADDR( reg )        0x100 + reg

static void StackPush( BYTE data )
{
   WriteByte( STACK_ADDR( StackPointer ), data );
   StackPointer--;
}

static void StackPop( BYTE* data )
{
   *data = ReadByte( STACK_ADDR( StackPointer ) );
   StackPointer++;
}

static void LoadAccImm( )
{
   LoadAccBase( ReadByte( ProgramCounter++ ) );
}

static void LoadAccZp( )
{
   WORD dataAddr = ReadByte( ProgramCounter++ );
   LoadAccBase( ReadByte( dataAddr ) );
}

static void LoadAccZpIndex( )
{
   WORD dataAddr = ReadByte( ProgramCounter++ ) + XReg;
   LoadAccBase( ReadByte( dataAddr ) );
}

static void LoadAccAbs( )
{
   WORD dataAddr = ReadWord( ProgramCounter );
   LoadAccBase( ReadByte( dataAddr ) );
   ProgramCounter += 2;
}

static void LoadAccAbsIndexX( )
{
   WORD dataAddr = ReadWord( ProgramCounter ) + XReg;
   LoadAccBase( ReadByte( dataAddr ) );
   ProgramCounter += 2;
}

static void LoadAccAbsIndexY( )
{
   WORD dataAddr = ReadWord( ProgramCoutner ) + YReg;
   LoadAccBase( ReadByte( dataAddr ) );
   ProgramCounter += 2;
}

static void LoadAccIndexInd( )
{
   WORD dataAddr = ReadWord( ReadByte( ProgramCounter++ ) + XReg );
   LoadAccBase( ReadByte( dataAddr ) );
}

static void LoadAccIndIndex( )
{
   WORD dataAddr = ReadWord( ReadByte( ProgramCounter++ ) ) + YReg;
   LoadAccBase( ReadByte( dataAddr ) );
}

static void LoadAccBase( BYTE data )
{
   StatusZero = ( data ) ? 0 : 1;
   StatusNeg = ( data & 0x80 ) ? 1 : 0;
   Accumulator = data;
}
