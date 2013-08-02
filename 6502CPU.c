/*System Includes*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Local Includes*/
#include "6502CPU.h"
//#include "6502Opcodes.h"
#include "Error.h"
#include "MemoryInterface.h"

// Dunno what this is for
// TODO:
// 	find out what it's for
#define AC 0xff

static int clockCount;
struct e6502_t *CurrentCpu = NULL;
//void (*FunctionTable[0xFF])();	/*256 function pointers*/

static void XorAccMemBase( e6502_t *cpu, BYTE data );
static void LoadXBase( e6502_t *cpu, BYTE data );
static void LoadYBase( e6502_t *cpu, BYTE data );
static void OrAccMemBase( e6502_t *cpu, BYTE data );
static void RotateLeftBase( e6502_t *cpu, WORD dataAddr, BYTE data );
static void RotateRightBase( e6502_t *cpu, WORD dataAddr, BYTE data );
static void SubtractAccMemBase( e6502_t *cpu, BYTE data );
static void TransferBase( e6502_t *cpu, BYTE* dataDest, BYTE data );
static void PushProgamCounter( e6502_t *cpu );

static void Jam( e6502_t *cpu )
{
	/*Do something with this please*/
}

static void UndocOp( e6502_t *cpu )
{
	/*Do something here*/
	cpu->pCounter++;
	//ProgramCounter++;
}

static void AddCarryImm( e6502_t *cpu )
{
	BYTE data = ReadByte( cpu->memory, cpu->pCounter++ );		/*Post inc says PC now points to next opcode*/
	AddCarryBase( cpu, data );
}

static void AddCarryZp( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AddCarryBase( cpu, data );
}

static void AddCarryZpIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex;
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AddCarryBase( cpu, data );
}

static void AddCarryAbs( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AddCarryBase( cpu, data );
	cpu->pCounter += 2;
}

static void AddCarryAbsIndexX( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex;
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AddCarryBase( cpu, data );
	cpu->pCounter += 2;
}

static void AddCarryAbsIndexY( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->yIndex;
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AddCarryBase( cpu, data );
	cpu->pCounter +=2 ;
}

static void AddCarryIndexInd( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex );
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AddCarryBase( cpu, data );
}

static void AddCarryIndIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) );
	BYTE data = ReadByte( cpu->memory, dataAddr + cpu->yIndex );
	AddCarryBase( cpu, data );
}

static void AddCarryBase( e6502_t *cpu, BYTE data )
{
	DWORD tmp = data + cpu->accumulator + cpu->statusCarry;
	cpu->statusZero = tmp && 0xFF;
	if( cpu->statusDec )
	{
		/*Ignore decimal mode.  It's fucking gay*/
	}
	else
	{
		cpu->statusNeg = tmp && 0x80;
		cpu->statusOverflow = ( !((cpu->accumulator ^ data ) & 0x80) && ((cpu->accumulator ^ tmp) & 0x80) ) ? 1 : 0;
		cpu->statusCarry = ( tmp > 0xFF );
	}
	cpu->accumulator = tmp & 0xFF;
}

static void AndImm( e6502_t *cpu )
{
	BYTE data = ReadByte( cpu->memory, cpu->pCounter++ );
	AndBase( cpu, data );
}

static void AndZp( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AndBase( cpu, data );
}

static void AndZpIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex;
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AndBase( cpu, data );
}

static void AndAbs( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AndBase( cpu, data );
	cpu->pCounter += 2;
}

static void AndAbsIndexX( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex;
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AndBase( cpu, data );
	cpu->pCounter += 2;
}

static void AndAbsIndexY( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->yIndex;
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AndBase( cpu, data );
	cpu->pCounter += 2;
}

static void AndIndexInd( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex );
	BYTE data = ReadByte( cpu->memory, dataAddr );
	AndBase( cpu, data );
}

static void AndIndIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) );
	BYTE data = ReadByte( cpu->memory, dataAddr + cpu->yIndex );
	AndBase( cpu, data );
}

static void AndBase( e6502_t *cpu, BYTE data )
{
	data &= cpu->accumulator;
	cpu->statusNeg = data && 0x80;
	cpu->statusZero = data && 0xFF;
	cpu->accumulator = data;
}

static void ShlAcc( e6502_t *cpu )
{
	cpu->accumulator = cpu->accumulator << 1;
}

static void ShlZp( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
	WriteByte( cpu->memory, dataAddr, ReadByte( cpu->memory, dataAddr ) << 1 );
}

static void ShlZpIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex;
	WriteByte( cpu->memory, dataAddr, ReadByte( cpu->memory, dataAddr ) << 1 );
}

static void ShlAbs( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
	WriteByte( cpu->memory, dataAddr, ReadByte( cpu->memory, dataAddr ) << 1 );
	cpu->pCounter += 2;
}

static void ShlAbsIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
	WriteByte( cpu->memory, dataAddr, ReadByte( cpu->memory, dataAddr ) << 1 );
	cpu->pCounter += 2;
}

static void BranchCarryClear( e6502_t *cpu )
{
	if( cpu->statusCarry )
	{
		cpu->pCounter += 2;
	}
	else
	{
		cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
	}
}

static void BranchCarrySet( e6502_t *cpu )
{
	if( cpu->statusCarry )
	{
		cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
	}
	else
	{
		cpu->pCounter += 2;
	}
}

static void BranchZero( e6502_t *cpu )
{
	if( cpu->statusZero )
	{
		cpu->pCounter += 2;
	}
	else
	{
		cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
	}
}

static void BitTestZp( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
	BYTE data = ReadByte( cpu->memory, dataAddr );
	BitTestBase( cpu, data );
}

static void BitTestAbs( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
	BYTE data = ReadByte( cpu->memory, dataAddr );
	BitTestBase( cpu, data );
}

static void BitTestBase( e6502_t *cpu, BYTE data )
{
	cpu->statusNeg = data & 0x80;
	cpu->statusOverflow = data & 0x40;
	cpu->statusZero = data && cpu->accumulator;
}

static void BranchNegative( e6502_t *cpu )
{
	if( cpu->statusNeg )
	{
		cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
	}
	else
	{
		cpu->pCounter += 2;
	}
}

static void BranchNotZero( e6502_t *cpu )
{
	if( cpu->statusZero )
	{
		cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
	}
	else
	{
		cpu->pCounter += 2;
	}
}

static void BranchNotNegative( e6502_t *cpu )
{
	if( cpu->statusNeg )
	{
		cpu->pCounter += 2;
	}
	else
	{
		cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
	}
}

static void Break( e6502_t *cpu )
{
	StackPush( cpu, ( cpu->pCounter >> 8 ) & 0xFF );
	StackPush( cpu, cpu->pCounter & 0xFF );
	cpu->broke = 1;
	StackPush( cpu, ( cpu->statusNeg << 7 ) | ( cpu->statusOverflow << 6 ) | ( 1 << 5 ) | ( cpu->broke << 4 ) | ( cpu->statusDec << 3 ) | ( cpu->hasInterrupts << 2 ) | ( cpu->statusZero << 1 ) | cpu->statusCarry );
	cpu->pCounter = ReadWord( cpu->memory, 0xFFFE );
	cpu->statusInterrupt = 1;
}

static void BranchNoOverflow( e6502_t *cpu )
{
	if( cpu->statusOverflow )
	{
		cpu->pCounter += 2;
	}
	else
	{
		cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
	}
}

static void BranchOverflow( e6502_t *cpu )
{
	if( cpu->statusOverflow )
	{
		cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
	}
	else
	{
		cpu->pCounter += 2;
	}
}

static void ClearCarry( e6502_t *cpu )
{
	cpu->statusCarry = 0;
}

static void ClearDecimal( e6502_t *cpu )
{
	cpu->statusDec = 0;
}

static void ClearInterrupts( e6502_t *cpu )
{
	cpu->statusInterrupt = 0;
}

static void ClearOverflow( e6502_t *cpu )
{
	cpu->statusOverflow = 0;
}

static void CmpAccImm( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ), cpu->accumulator );
}

static void CmpAccZp( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ), cpu->accumulator);
}

static void CmpAccZpIndex( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ), cpu->accumulator );
}

static void CmpAccAbs( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) ), cpu->accumulator );
	cpu->pCounter += 2;
}

static void CmpAccAbsIndexX( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex ), cpu->accumulator );
	cpu->pCounter += 2;
}

static void CmpAccAbsIndexY( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->yIndex ), cpu->accumulator );
	cpu->pCounter += 2;
}

static void CmpAccIndexInd( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ) ), cpu->accumulator );
}

static void CmpAccIndIndex( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) + cpu->yIndex ), cpu->accumulator );
}

static void CmpBase( e6502_t *cpu, BYTE data, BYTE reg )
{
	/*Compare is always a subtraction of cpu->accumulator minus data*/
	cpu->statusCarry = ( data > reg  );
	data = reg - data;
	cpu->statusNeg = data & 0x80;
	cpu->statusZero = (data && 0xFF);
}

static void CmpXImm( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ), cpu->xIndex );
}

static void CmpXZp( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ), cpu->xIndex );
}

static void CmpXAbs( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter) ), cpu->xIndex );
	cpu->pCounter += 2;
}

static void CmpYImm( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ), cpu->yIndex );
}

static void CmpYZp( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ), cpu->yIndex );
}

static void CmpYAbs( e6502_t *cpu )
{
	CmpBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) ), cpu->yIndex );
	cpu->pCounter += 2;
}

static void DecrementMemZp( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
   BYTE data = ReadByte( cpu->memory, dataAddr );
	WriteByte( cpu->memory, dataAddr, --data );
}

static void DecrementMemZpIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex;
   BYTE data = ReadByte( cpu->memory, dataAddr );
	WriteByte( cpu->memory, dataAddr, --data );
}

static void DecrementMemAbs( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
   BYTE data = ReadByte( cpu->memory, dataAddr );
   WriteByte( cpu->memory, dataAddr, --data );
   cpu->pCounter += 2;
}

static void DecrementMemAbsIndex( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex;
   BYTE data = ReadByte( cpu->memory, dataAddr );
   WriteByte( cpu->memory, dataAddr, --data );
   cpu->pCounter += 2;
}

static void DecrementX( e6502_t *cpu )
{
   cpu->xIndex--;
}

static void DecrementY( e6502_t *cpu )
{
   cpu->yIndex--;
}

static void XorAccMemImm( e6502_t *cpu )
{
   XorAccMemBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ) );
}

static void XorAccMemZp( e6502_t *cpu )
{
   XorAccMemBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) );
}

static void XorAccMemZpIndex( e6502_t *cpu )
{
   XorAccMemBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ) );
}

static void XorAccMemAbs( e6502_t *cpu )
{
   XorAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) ) );
   cpu->pCounter += 2;
}

static void XorAccMemAbsIndexX( e6502_t *cpu )
{
   XorAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex ) );
   cpu->pCounter += 2;
}

static void XorAccMemAbsIndexY( e6502_t *cpu )
{
   XorAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->yIndex ) );
   cpu->pCounter += 2;
}

static void XorAccMemIndexInd( e6502_t *cpu )
{
   XorAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ) ) );
}

static void XorAccMemIndIndex( e6502_t *cpu )
{
   XorAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) + cpu->yIndex ) );
}

static void XorAccMemBase( e6502_t *cpu, BYTE data )
{
   data ^= cpu->accumulator;
   cpu->statusNeg = data & 0x80;
   cpu->statusZero = data && 0xFF;
   cpu->accumulator = data;
}

static void IncrementMemZp( e6502_t *cpu ) 
{
   WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
   BYTE data = ReadByte( cpu->memory, dataAddr );
   WriteByte( cpu->memory, dataAddr, data++ );
}

static void IncrementMemZpIndex( e6502_t *cpu )
{
   WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex;
   BYTE data = ReadByte( cpu->memory, dataAddr );
   WriteByte( cpu->memory, dataAddr, data++ );
}

static void IncrementMemAbs( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
   BYTE data = ReadByte( cpu->memory, dataAddr );
   WriteByte( cpu->memory, dataAddr, data++ );
}

static void IncrementMemAbsIndex( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
   BYTE data = ReadByte( cpu->memory, dataAddr );
   WriteByte( cpu->memory, dataAddr, data++ );
}

static void IncrementX( e6502_t *cpu )
{
   cpu->xIndex++;
}

static void IncrementY( e6502_t *cpu )
{
   cpu->yIndex++;
}

static void JumpAbs( e6502_t *cpu )
{
   cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
}

static void JumpInd( e6502_t *cpu )
{
   cpu->pCounter = ReadWord( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) );
}

static void JumpSubroutine( e6502_t *cpu )
{
   StackPush( cpu, ( cpu->pCounter >> 8 ) & 0xFF );
   StackPush( cpu, cpu->pCounter & 0xFF );
   cpu->pCounter = ReadWord( cpu->memory, cpu->pCounter );
}

#define STACK_ADDR( reg )        0x100 + reg

static void StackPush( e6502_t *cpu, BYTE data )
{
   WriteByte( cpu->memory, STACK_ADDR( cpu->sPointer ), data );
   cpu->sPointer--;
}

static void StackPop( e6502_t *cpu, BYTE* data )
{
   *data = ReadByte( cpu->memory, STACK_ADDR( cpu->sPointer ) );
   cpu->sPointer++;
}

static void LoadAccImm( e6502_t *cpu )
{
   LoadAccBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ) );
}

static void LoadAccZp( e6502_t *cpu )
{
   WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
   LoadAccBase( cpu, ReadByte( cpu->memory, dataAddr ) );
}

static void LoadAccZpIndex( e6502_t *cpu )
{
   WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex;
   LoadAccBase( cpu, ReadByte( cpu->memory, dataAddr ) );
}

static void LoadAccAbs( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
   LoadAccBase( cpu, ReadByte( cpu->memory, dataAddr ) );
   cpu->pCounter += 2;
}

static void LoadAccAbsIndexX( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex;
   LoadAccBase( cpu, ReadByte( cpu->memory, dataAddr ) );
   cpu->pCounter += 2;
}

static void LoadAccAbsIndexY( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->yIndex;
   LoadAccBase( cpu, ReadByte( cpu->memory, dataAddr ) );
   cpu->pCounter += 2;
}

static void LoadAccIndexInd( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex );
   LoadAccBase( cpu, ReadByte( cpu->memory, dataAddr ) );
}

static void LoadAccIndIndex( e6502_t *cpu )
{
   WORD dataAddr = ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) + cpu->yIndex;
   LoadAccBase( cpu, ReadByte( cpu->memory, dataAddr ) );
}

static void LoadAccBase( e6502_t *cpu, BYTE data )
{
   cpu->statusZero = ( data ) ? 0 : 1;
   cpu->statusNeg = ( data & 0x80 ) ? 1 : 0;
   cpu->accumulator = data;
}

static void LoadXImm( e6502_t *cpu )
{
	LoadXBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ) );
}

static void LoadXZp( e6502_t *cpu )
{
	LoadXBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) );
}

static void LoadXZpIndex( e6502_t *cpu ) 
{
	LoadXBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->yIndex ) );
}

static void LoadXAbs( e6502_t *cpu )
{
	LoadXBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) ) );
	cpu->pCounter += 2;
}

static void LoadXAbsIndex( e6502_t *cpu )
{
	LoadXBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->yIndex ) );
	cpu->pCounter += 2;
}

static void LoadXBase( e6502_t *cpu, BYTE data )
{
	cpu->statusZero = ( data ) ? 0 : 1;
	cpu->statusNeg = ( data & 0x80 ) ? 1 : 0;
	cpu->xIndex = data;
}

static void LoadYImm( e6502_t *cpu )
{
	LoadYBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ) );
}

static void LoadYZp( e6502_t *cpu )
{
	LoadYBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) );
}

static void LoadYZpIndex( e6502_t *cpu )
{
	LoadYBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ) );
}

static void LoadYAbs( e6502_t *cpu )
{
	LoadYBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) ) );
	cpu->pCounter += 2;
}

static void LoadYAbsIndex( e6502_t *cpu )
{
	LoadYBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex ) );
	cpu->pCounter += 2;
}

static void LoadYBase( e6502_t *cpu, BYTE data )
{
	cpu->statusZero = ( data ) ? 0 : 1;
	cpu->statusNeg = ( data & 0x80 ) ? 1 : 0;
	cpu->yIndex = data;
}

static void NoOperation( e6502_t *cpu )
{
	/*All this does is increment the program counter*/
	cpu->pCounter++;
}

static void OrAccMemImm( e6502_t *cpu )
{
	OrAccMemBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ) );
}

static void OrAccMemZp( e6502_t *cpu )
{
	OrAccMemBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) );
}

static void OrAccMemZpIndex( e6502_t *cpu )
{
	OrAccMemBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ) );
}

static void OrAccMemAbs( e6502_t *cpu )
{
	OrAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) ) );
	cpu->pCounter += 2;
}

static void OrAccMemAbsIndex( e6502_t *cpu )
{
	OrAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex ) );
	cpu->pCounter += 2;
}

static void OrAccMemIndexInd( e6502_t *cpu )
{
	OrAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ) ) );
}

static void OrAccMemIndIndex( e6502_t *cpu )
{
	OrAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) + cpu->yIndex ) );
}

static void OrAccMemBase( e6502_t *cpu, BYTE data )
{
	BYTE tmp = data | cpu->accumulator;
	cpu->statusNeg = ( tmp & 0x80 ) ? 1 : 0;
	cpu->statusZero = ( tmp ) ? 0 : 1;
	cpu->accumulator = tmp;
}

static void PushAcc( e6502_t *cpu )
{
	StackPush( cpu, cpu->accumulator );
	cpu->pCounter++;
}

static void PushProgamCounter( e6502_t *cpu )
{
	StackPush( cpu, ( cpu->pCounter >> 8 ) & 0xFF );
	StackPush( cpu, cpu->pCounter & 0xFF );
	cpu->pCounter++;
}

static void PopAcc( e6502_t *cpu )
{
	StackPop( cpu, &cpu->accumulator );
	cpu->pCounter++;
}

static void PopProgramCounter( e6502_t *cpu )
{
	StackPop( cpu, (BYTE*)&cpu->pCounter );
	StackPop( cpu, (BYTE*)&cpu->pCounter+1 );
}

static void RotateLeftAcc( e6502_t *cpu )
{
	WORD tmp = cpu->accumulator << 1;
	tmp |= ( cpu->statusCarry ) ? 1 : 0;
	cpu->statusCarry = ( tmp > 0xFF ) ? 1 : 0;
	tmp &= 0xFF;
	cpu->statusNeg = ( tmp & 0x80 ) ? 1 : 0;
	cpu->statusZero = ( tmp ) ? 0 : 1;
	cpu->accumulator = tmp;
	cpu->pCounter++;
}

static void RotateLeftZp( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
	RotateLeftBase( cpu, dataAddr, ReadByte( cpu->memory, dataAddr ) );
}

static void RotateLeftZpIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex;
	RotateLeftBase( cpu, dataAddr, ReadByte( cpu->memory, dataAddr ) );
}

static void RotateLeftAbs( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
	RotateLeftBase( cpu, dataAddr, ReadByte( cpu->memory, dataAddr ) );
	cpu->pCounter += 2;
}

static void RotateLeftAbsIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex;
	RotateLeftBase( cpu, dataAddr, ReadByte( cpu->memory, dataAddr ) );
	cpu->pCounter += 2;
}

static void RotateLeftBase( e6502_t *cpu, WORD dataAddr, BYTE data )
{
	WORD tmp = data << 1;
	tmp |= ( cpu->statusCarry ) ? 1 : 0;
	cpu->statusCarry = ( tmp > 0xFF ) ? 1 : 0;
	tmp &= 0xFF;
	cpu->statusNeg = ( tmp & 0x80 ) ? 1 : 0;
	cpu->statusZero = ( tmp ) ? 0 : 1;
	WriteByte( cpu->memory, dataAddr, tmp );
}

static void RotateRightAcc( e6502_t *cpu ) 
{
	WORD tmp = cpu->accumulator | ( cpu->statusCarry ) ? 0x100 : 0;
	cpu->statusCarry = ( tmp & 0x1 ) ? 1 : 0;
	tmp >>= 1;
	cpu->statusNeg = ( tmp & 0x80 ) ? 1 : 0;
	cpu->statusZero = ( tmp ) ? 0 : 1;
	cpu->accumulator = tmp;
	cpu->pCounter++;
}

static void RotateRightZp( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ );
	RotateRightBase( cpu, dataAddr, ReadByte( cpu->memory, dataAddr ) );
}

static void RotateRightZpIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex;
	RotateRightBase( cpu, dataAddr, ReadByte( cpu->memory, dataAddr ) );
}

static void RotateRightAbs( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter );
	RotateRightBase( cpu, dataAddr, ReadByte( cpu->memory, dataAddr ) );
	cpu->pCounter += 2;
}

static void RotateRightAbsIndex( e6502_t *cpu )
{
	WORD dataAddr = ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex;
	RotateRightBase( cpu, dataAddr, ReadByte( cpu->memory, dataAddr ) );
	cpu->pCounter += 2;
}

static void RotateRightBase( e6502_t *cpu, WORD dataAddr, BYTE data )
{
	WORD tmp = data | ( cpu->statusCarry ) ? 0x100 : 0;
	cpu->statusCarry = ( tmp & 0x1 ) ? 1 : 0;
	tmp >>= 1;
	cpu->statusNeg = ( tmp & 0x80 ) ? 1 : 0;
	cpu->statusZero = ( tmp ) ? 0 : 1;
	WriteByte( cpu->memory, dataAddr, tmp );
}

static void ReturnInterrupt( e6502_t *cpu )
{
	BYTE sr;
	StackPop( cpu, &sr );
	cpu->statusNeg = ( sr & 0x80 ) >> 7;
	cpu->statusOverflow = ( sr & 0x40 ) >> 6;
	cpu->statusUnk = ( sr & 20 ) >> 5;
	cpu->broke = ( sr & 0x10 ) >> 4;
	cpu->statusDec = ( sr & 0x8 ) >> 3;
	cpu->statusInterrupt = ( sr & 0x4 ) >> 2;
	cpu->statusZero = ( sr & 0x2 ) >> 1;
	cpu->statusCarry = ( sr & 0x1 );
	StackPop( cpu, (BYTE*)&cpu->pCounter+1 );
	StackPop( cpu, (BYTE*)&cpu->pCounter );
}

static void ReturnSubroutine( e6502_t *cpu )
{
	BYTE hi, lo;
	StackPop( cpu, &lo );
	StackPop( cpu, &hi );
	cpu->pCounter = ( hi << 8 ) + lo;		/*Is adding one needed?  What did we push onto the stack?*/
}

static void SubtractAccMemImm( e6502_t *cpu )
{
	SubtractAccMemBase( cpu, ReadByte( cpu->memory, cpu->pCounter++ ) );
}

static void SubtractAccMemZp( e6502_t *cpu )
{
	SubtractAccMemBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) );
}

static void SubtractAccMemZpIndex( e6502_t *cpu )
{
	SubtractAccMemBase( cpu, ReadByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ) );
}

static void SubtractAccMemAbs( e6502_t *cpu )
{
	SubtractAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) ) );
	cpu->pCounter += 2;
}

static void SubtractAccMemAbsIndex( e6502_t *cpu )
{
	SubtractAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex ) );
	cpu->pCounter += 2;
}

static void SubtractAccMemIndexInd( e6502_t *cpu )
{
	SubtractAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ) ) );
}

static void SubtractAccMemIndIndex( e6502_t *cpu )
{
	SubtractAccMemBase( cpu, ReadByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) + cpu->yIndex ) );
}

static void SubtractAccMemBase( e6502_t *cpu, BYTE data )
{
	// TODO: Replace temp
	int temp = 0;
	WORD tmp = cpu->accumulator - data - cpu->statusCarry;
	cpu->statusNeg = ( tmp & 0x80 ) ? 1 : 0;
	cpu->statusZero = ( tmp ) ? 0 : 1;
	cpu->statusOverflow = ( ((AC ^ temp) & 0x80) && ((AC ^ data) & 0x80) ) ? 1 : 0;		/*I hate this thing*/
	if( cpu->statusDec )
	{
		/*Whatever*/
	}
	cpu->statusCarry = ( tmp < 0x100 ) ? 1 : 0;
	cpu->accumulator = tmp & 0xFF;
}

static void SetCarry( e6502_t *cpu )
{
	cpu->statusCarry = 1;
	cpu->pCounter++;
}

static void SetDecimal( e6502_t *cpu )
{
	cpu->statusDec = 1;
	cpu->pCounter++;
}

static void SetInterruptDisable( e6502_t *cpu )
{
	cpu->statusInterrupt = 1;
	cpu->pCounter++;
}

static void StoreAccZp( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ), cpu->accumulator );
}

static void StoreAccZpIndex( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex, cpu->accumulator );
}

static void StoreAccAbs( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ), cpu->accumulator );
	cpu->pCounter += 2;
}

static void StoreAccAbsIndexX( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->xIndex, cpu->accumulator );
	cpu->pCounter += 2;
}

static void StoreAccAbsIndexY( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ) + cpu->yIndex, cpu->accumulator );
	cpu->pCounter += 2;
}

static void StoreAccIndexInd( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex ), cpu->accumulator );
}

static void StoreAccIndIndex( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadWord( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) ) + cpu->yIndex, cpu->accumulator );
}

static void StoreXZp( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ), cpu->xIndex );
}

static void StoreXZpIndex( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->yIndex, cpu->xIndex );
}

static void StoreXAbs( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ), cpu->xIndex );
	cpu->pCounter += 2;
}

static void StoreYZp( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ), cpu->yIndex );
}

static void StoreYZpIndex( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadByte( cpu->memory, cpu->pCounter++ ) + cpu->xIndex, cpu->yIndex );
}

static void StoreYAbs( e6502_t *cpu )
{
	WriteByte( cpu->memory, ReadWord( cpu->memory, cpu->pCounter ), cpu->yIndex );
	cpu->pCounter += 2;
}

static void TransferAccToX( e6502_t *cpu )
{
	TransferBase( cpu, &cpu->xIndex, cpu->accumulator );
}

static void TransferAccToY( e6502_t *cpu )
{
	TransferBase( cpu, &cpu->yIndex, cpu->accumulator );
}

static void TransferStackToX( e6502_t *cpu )
{
	TransferBase( cpu, &cpu->xIndex, cpu->sPointer );
}

static void TransferXToAcc( e6502_t *cpu )
{
	TransferBase( cpu, &cpu->accumulator, cpu->xIndex );
}

static void TransferXToStack( e6502_t *cpu )
{
	TransferBase( cpu, &cpu->sPointer, cpu->xIndex );
}

static void TransferYToAcc( e6502_t *cpu )
{
	TransferBase( cpu, &cpu->accumulator, cpu->yIndex );
}

static void TransferBase( e6502_t *cpu, BYTE* dataDest, BYTE data )
{
	cpu->statusNeg = ( data & 0x80 ) ? 1 : 0 ;
	cpu->statusZero = ( data ) ? 0 : 1;
	*dataDest = data;
	cpu->pCounter++;
}

//void (*FunctionTable[0xFF])();	/*256 function pointers*/
void (*FunctionTable[0xFF])() = 
{
Break, OrAccMemIndexInd, UndocOp, UndocOp, UndocOp, OrAccMemZp, ShlZp, UndocOp, /* FIXME PushProgramCounter */UndocOp, OrAccMemImm, ShlAcc, UndocOp, UndocOp, OrAccMemAbs, ShlAbs, UndocOp,
BranchNotNegative, OrAccMemIndIndex, UndocOp, UndocOp, UndocOp, OrAccMemZpIndex, ShlZpIndex, UndocOp, ClearCarry, /* FIXME OrAccMemAbsIndexY*/UndocOp, UndocOp, UndocOp, UndocOp, OrAccMemAbsIndex, ShlAbsIndex, UndocOp,
JumpSubroutine, AndIndexInd, UndocOp, UndocOp, BitTestZp, AndZp, RotateLeftZp, UndocOp, PopProgramCounter, AndImm, RotateLeftAcc, UndocOp, BitTestAbs, AndAbs, RotateLeftAbs, UndocOp,
BranchNegative, AndIndIndex, UndocOp, UndocOp, UndocOp, AndZpIndex, RotateLeftZpIndex, UndocOp, SetCarry, AndAbsIndexY, UndocOp, UndocOp, UndocOp, AndAbsIndexX, /* FIXME RotateLeftAbsIndexX*/UndocOp, UndocOp,
ReturnInterrupt, XorAccMemIndexInd, UndocOp, UndocOp, UndocOp, XorAccMemZp, /* FIXME ShrZp */UndocOp, UndocOp, PushAcc, XorAccMemImm, /* FIXME ShrAcc */UndocOp, UndocOp, JumpAbs, XorAccMemAbs, /* FIXME ShrAbs */UndocOp, UndocOp,
/* FIXME BranchNoOveflow */UndocOp, XorAccMemIndIndex, UndocOp, UndocOp, UndocOp, XorAccMemZpIndex, /* FIXME ShrZpIndex*/UndocOp, UndocOp, ClearInterrupts, XorAccMemAbsIndexY, UndocOp, UndocOp, UndocOp, XorAccMemAbsIndexX, /* FIXME ShrAbsIndexX */UndocOp, UndocOp,
ReturnSubroutine, AddCarryIndexInd, UndocOp, UndocOp, UndocOp, AddCarryZp, RotateRightZp, UndocOp, /* FIXME PopAccumulator */UndocOp, /* FIXME AddCarrImm */UndocOp, RotateRightAcc, UndocOp, JumpInd, AddCarryAbs, RotateRightAbs, UndocOp,
BranchOverflow, AddCarryIndIndex, UndocOp, UndocOp, UndocOp, AddCarryZpIndex, RotateRightZpIndex, UndocOp, SetInterruptDisable, /* FIXME AddCarrAbsIndexY */UndocOp, UndocOp, UndocOp, UndocOp, AddCarryAbsIndexX, /* FIXME RotateRightAbsIndexX */UndocOp, UndocOp,
UndocOp, StoreAccIndexInd, UndocOp, UndocOp, StoreYZp, StoreAccZp, StoreXZp, UndocOp, DecrementY, UndocOp, TransferXToAcc, UndocOp, StoreYAbs, StoreAccAbs, StoreXAbs, UndocOp,
BranchCarryClear, StoreAccIndIndex, UndocOp, UndocOp, StoreYZpIndex, StoreAccZpIndex, StoreXZpIndex, UndocOp, TransferYToAcc, StoreAccAbsIndexY, TransferXToStack, UndocOp, UndocOp, StoreAccAbsIndexX, UndocOp, UndocOp,
LoadYImm, LoadAccIndexInd, LoadXImm, UndocOp, LoadYZp, LoadAccZp, LoadXZp, UndocOp, TransferAccToY, LoadAccImm, TransferAccToX, UndocOp, LoadYAbs, LoadAccAbs, LoadXAbs, UndocOp,
BranchCarrySet, LoadAccIndIndex, UndocOp, UndocOp, LoadYZpIndex, LoadAccZpIndex, LoadXZpIndex, UndocOp, ClearOverflow, LoadAccAbsIndexY, TransferStackToX, UndocOp, LoadYAbsIndex, LoadAccAbsIndexX, LoadXAbsIndex, UndocOp,
CmpYImm, CmpAccIndexInd, UndocOp, UndocOp, CmpYZp, CmpAccZp, /* FIXME DecrementZp */UndocOp, UndocOp, IncrementY, CmpAccImm, DecrementX, UndocOp, CmpYAbs, CmpAccAbs, /* FIXME DecrementAbs */UndocOp, UndocOp,
BranchNotZero, CmpAccIndIndex, UndocOp, UndocOp, UndocOp, CmpAccZpIndex, DecrementMemZpIndex, UndocOp, ClearDecimal, CmpAccAbsIndexY, UndocOp, UndocOp, UndocOp, CmpAccAbsIndexX, DecrementMemAbsIndex, UndocOp,
CmpXImm, SubtractAccMemIndexInd, UndocOp, UndocOp, CmpXZp, SubtractAccMemZp, IncrementMemZp, UndocOp, IncrementX, SubtractAccMemImm, NoOperation, UndocOp, CmpXAbs, SubtractAccMemAbs, UndocOp, IncrementMemAbs, UndocOp,
BranchZero, SubtractAccMemIndIndex, UndocOp, UndocOp, UndocOp, SubtractAccMemZpIndex, IncrementMemZpIndex, UndocOp, SetDecimal, /* FIXME SubtractAccMemAbsIndexY */UndocOp, UndocOp, UndocOp, UndocOp, /* FIXME SubtractAccMemAbsIndexX */UndocOp, IncrementMemAbsIndex, UndocOp
};

unsigned int CPUInit( unsigned char* ramPtr )
{
	return ERROR_SUCCESS;
}

void CPUFetch( e6502_t *cpu, unsigned char* opcode ) 
{
	/*Should get opcode at PC and then increment either pointing to an argument or the next opcode*/
	*opcode = ReadByte( cpu->memory, cpu->pCounter++ );		
}

void CPUDecode( unsigned char opcode, short* funcIndex )
{

}

void CPUExecute( e6502_t *cpu, unsigned char opcode )
{
	(*FunctionTable[opcode])( cpu );
}

void CPUInterrupt( e6502_t *cpu )
{
	if( cpu->statusInterrupt )
	{
		/*Find out what kind of interrupt it is either NMI or IRQ*/
		switch( cpu->statusInterrupt )
		{
		case NMI_LEVEL:
			//ProgramCounter = ( ram[0xFFFA] ) + ( ram[0xFFFB] << 8 );
			cpu->pCounter = ReadWord( cpu->memory, 0xFFFA );
			break;
		case IRQ_LEVEL:
			//ProgramCounter = ( ram[0xFFFE] ) + ( ram[0xFFFF] << 8 );
			cpu->pCounter = ReadWord( cpu->memory, 0xFFFE );
			break;
		default:
			break;
		}
	}
}

