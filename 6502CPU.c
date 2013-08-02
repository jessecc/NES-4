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
Break, OrAccMemIndexInd, UndocOp, UndocOp, UndocOp, OrAccMemZp, ShlZp, UndoOp, PushProgramCounter, OrAccMemImm, ShlAcc, UndocOp, UndocOp, OrAccMemAbs, ShlAbs, UndocOp,
BranchNotNegative, OrAccMemIndIndex, UndocOp, UndocOp, UndocOp, OrAccMemZpIndex, ShlZpIndex, UndocOp, ClearCarry, OrAccMemAbsIndexY, UndocOp, UndocOp, UndocOp, OrAccMemAbsIndex, ShlAbsIndex, UndocOp,
JumpSubroutine, AndIndexInd, UndocOp, UndocOp, BitTestZp, AndZp, RotateLeftZp, UndocOp, PopProgramCounter, AndImm, RotateLeftAcc, UndocOp, BitTestAbs, AndAbs, RotateLeftAbs, UndocOp,
BranchNegative, AndIndIndex, UndocOp, UndocOp, UndocOp, AndZpIndex, RotateLeftZpIndex, UndocOp, SetCarry, AndAbsIndexY, UndocOp, UndocOp, UndocOp, AndAbsIndexX, RotateLeftAbsIndexX, UndocOp
ReturnInterrupt, XorAccMemIndexInd, UndocOp, UndocOp, UndocOp, XorAccMemZp, ShrZp, UndocOp, PushAcc, XorAccMemImm, ShrAcc, UndocOp, JumpAbs, XorAccMemAbs, ShrAbs, UndocOp,
BranchNoOveflow, XorAccMemIndIndex, UndocOp, UndocOp, UndocOp, XorAccMemZpIndex, ShrZpIndex, UndocOp, ClearInterrupts, XorAccMemAbsIndexY, UndocOp, UndocOp, UndocOp, XorAccMemAbsIndexX, ShrAbsIndexX, UndocOp,
ReturnSubroutine, AddCarryIndexInd, UndocOp, UndocOp, UndocOp, AddCarryZp, RotateRightZp, UndocOp, PopAccumulator, AddCarrImm, RotateRightAcc, UndocOp, JumpInd, AddCarryAbs, RotateRightAbs, UndocOp,
BranchOverflow, AddCarryIndIndex, UndocOp, UndocOp, UndocOp, AddCarryZpIndex, RotateRightZpIndex, UndocOp, SetInterruptDisable, AddCarrAbsIndexY, UndocOp, UndocOp, UndocOp, AddCarryAbsIndexX, RotateRightAbsIndexX, UndocOp,
UndocOp, StoreAccIndexInd, UndocOp, UndocOp, StoreYZp, StoreAccZp, StoreXZp, UndocOp, DecrementY, UndocOp, TransferXToAcc, UndocOp, StoreYAbs, StoreAccAbs, StoreXAbs, UndocOp,
BranchCarryClear, StoreAccIndIndex, UndocOp, UndocOp, StoreYZpIndex, StoreAccZpIndex, StoreXZpIndex, UndocOp, TransferYToAcc, StoreAccAbsIndexY, TransferXToStack, UndocOp, UndocOp, StoreAccAbsIndexX, UndocOp, UndocOp,
LoadYImm, LoadAccIndexInd, LoadXImm, UndocOp, LoadYZp, LoadAccZp, LoadXZp, UndocOp, TransferAccToY, LoadAccImm, TransferAccToX, UndocOp, LoadYAbs, LoadAccAbs, LoadXAbs, UndocOp,
BranchCarrySet, LoadAccIndIndex, UndocOp, UndocOp, LoadYZpIndex, LoadAccZpIndex, LoadXZpIndex, UndocOp, ClearOverflow, LoadAccAbsIndexY, TransferStackToX, UndocOp, LoadYAbsIndex, LoadAccAbsIndexX, LoadXAbsIndex, UndocOp,
CmpYImm, CmpAccIndexInd, UndocOp, UndocOp, CmpYZp, CmpAccZp, DecrementZp, UndocOp, IncrementY, CmpAccImm, DecrementX, UndocOp, CmpYAbs, CmpAccAbs, DecrementAbs, UndocOp,
BranchNotZero, CmpAccIndIndex, UndocOp, UndocOp, UndocOp, CmpAccZpIndex, DecrementMemZpIndex, UndocOp, ClearDecimal, CmpAccAbsIndexY, UndocOp, UndocOp, UndocOp, CmpAccAbsIndexX, DecrementMemAbsIndex, UndocOp,
CmpXImm, SubtractAccMemIndexInd, UndocOp, UndocOp, CmpXZp, SubtractAccMemZp, IncrementMemZp, UndocOp, IncrementX, SubtractAccMemImm, NoOperation, UndocOp, CmpXAbs, SubtractAccMemAbs, IncrementMemAbs, UndocOp,
BranchZero, SubtractAccMemIndIndex, UndocOp, UndocOp, UndocOp, SubtractAccMemZpIndex, IncrementMemZpIndex, UndocOp, SetDecimal, SubtractAccMemAbsIndexY, UndocOp, UndocOp, UndocOp, SubtractAccMemAbsIndexX, IncrementMemAbsIndex, UndocOp
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

static void Jam( )
{
	/*Do something with this please*/
}

static void UndocOp( )
{
	/*Do something here*/
	ProgramCounter++;
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
	StackPush( ( ProgramCounter >> 8 ) & 0xFF );
	StackPush( ProgramCounter & 0xFF );
	StatusB = 1;
	StackPush( ( StatusNeg << 7 ) | ( StatusOverflow << 6 ) | ( 1 << 5 ) | ( StatusB << 4 ) | ( StatusDecimal << 3 ) | ( StatusInterrupt << 2 ) | ( StatusZero << 1 ) | StatusCarry );
	ProgramCounter = ReadWord( 0xFFFE );
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

static void LoadXImm( )
{
	LoadXBase( ReadByte( ProgramCounter++ ) );
}

static void LoadXZp( )
{
	LoadXBase( ReadByte( ReadByte( ProgramCounter++ ) ) );
}

static void LoadXZpIndex( ) 
{
	LoadXBase( ReadByte( ReadByte( ProgramCounter++ ) + YReg ) );
}

static void LoadXAbs( )
{
	LoadXBase( ReadByte( ReadWord( ProgramCounter ) ) );
	ProgramCounter += 2;
}

static void LoadXAbsIndex( )
{
	LoadXBase( ReadByte( ReadWord( ProgramCounter ) + YReg ) );
	ProgramCounter += 2;
}

static void LoadXBase( BYTE data )
{
	StatusZero = ( data ) ? 0 : 1;
	StatusNeg = ( data & 0x80 ) ? 1 : 0;
	XReg = data;
}

static void LoadYImm( )
{
	LoadYBase( ReadByte( ProgramCounter++ ) );
}

static void LoadYZp( )
{
	LoadYBase( ReadByte( ReadByte( ProgramCounter++ ) ) );
}

static void LoadYZpIndex( )
{
	LoadYBase( ReadByte( ReadByte( ProgramCounter++ ) + XReg ) );
}

static void LoadYAbs( )
{
	LoadYBase( ReadByte( ReadWord( ProgramCounter ) ) );
	ProgramCounter += 2;
}

static void LoadYAbsIndex( )
{
	LoadYBase( ReadBYte( ReadWord( ProgramCounter ) + XReg ) );
	ProgramCounter += 2;
}

static void LoadYBase( BYTE data )
{
	StatusZero = ( data ) ? 0 : 1;
	StatusNeg = ( data & 0x80 ) ? 1 : 0;
	YReg = data;
}

static void NoOperation( )
{
	/*All this does is increment the program counter*/
	ProgramCounter++;
}

static void OrAccMemImm( )
{
	OrAccMemBase( ReadByte( ProgramCounter++ ) );
}

static void OrAccMemZp( )
{
	OrAccMemBase( ReadByte( ReadByte( ProgramCounter++ ) ) );
}

static void OrAccMemZpIndex( )
{
	OrAccMemBase( ReadByte( ReadByte( ProgramCounte++ ) + XReg ) );
}

static void OrAccMemAbs( )
{
	OrAccMemBase( ReadByte( ReadWord( ProgramCounter ) ) );
	ProgramCounter += 2;
}

static void OrAccMemAbsIndex( )
{
	OrAccMemBase( ReadByte( ReadWord( ProgramCounter ) + XReg ) );
	ProgramCounter += 2;
}

static void OrAccMemIndexInd( )
{
	OrAccMemBase( ReadByte( ReadWord( ReadByte( ProgramCounter++ ) + XReg ) ) );
}

static void OrAccMemIndIndex( )
{
	OrAccMemBase( ReadByte( ReadWord( ReadByte( ProgramCounter++ ) ) + YReg ) );
}

static void OrAccMemBase( BYTE data )
{
	BYTE tmp = data | Accumulator;
	StatusNeg = ( tmp & 0x80 ) ? 1 : 0;
	StatusZero = ( tmp ) ? 0 : 1;
	Accumulator = tmp;
}

static void PushAcc( )
{
	StackPush( Accumulator );
	ProgramCounter++;
}

static void PushProgamCounter( )
{
	StackPush( ( ProgramCounter >> 8 ) & 0xFF );
	StackPush( ProgramCounter & 0xFF );
	ProgramCounter++;
}

static void PopAcc( )
{
	StackPop( &Accumulator );
	ProgramCounter++;
}

static void PopProgramCounter( )
{
	StackPop( (BYTE*)&ProgramCounter );
	StackPop( (BYTE*)&(ProgramCounter+1) );
}

static void RotateLeftAcc( )
{
	WORD tmp = Accumulator << 1;
	tmp |= ( StatusCarry ) ? 1 : 0;
	StatusCarry = ( tmp > 0xFF ) ? 1 : 0;
	tmp &= 0xFF;
	StatusNeg = ( tmp & 0x80 ) ? 1 : 0;
	StatusZero = ( tmp ) ? 0 : 1;
	Accumulator = tmp;
	ProgramCounter++;
}

static void RotateLeftZp( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ );
	RotateLeftBase( dataAddr, ReadByte( dataAddr ) );
}

static void RotateLeftZpIndex( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ ) + XReg;
	RotateLeftBase( dataAddr, ReadByte( dataAddr ) );
}

static void RotateLeftAbs( )
{
	WORD dataAddr = ReadWord( ProgramCounter );
	RotateLeftBase( dataAddr, ReadByte( dataAddr ) );
	ProgramCounter += 2;
}

static void RotateLeftAbsIndex( )
{
	WORD dataAddr = ReadWord( ProgramCounter ) + XReg;
	RotateLeftBase( dataAddr, ReadByte( dataAddr ) );
	ProgramCounter += 2;
}

static void RotateLeftBase( WORD dataAddr, BYTE data )
{
	WORD tmp = data << 1;
	tmp |= ( StatusCarry ) ? 1 : 0;
	StatusCarry = ( tmp > 0xFF ) ? 1 : 0;
	tmp &= 0xFF;
	StatusNeg = ( tmp & 0x80 ) ? 1 : 0;
	StatusZero = ( tmp ) ? 0 : 1;
	WriteByte( dataAddr, tmp );
}

static void RotateRightAcc( ) 
{
	WORD tmp = Accumulator | ( StatusCarry ) ? 0x100 : 0;
	StatusCarry = ( tmp & 0x1 ) ? 1 : 0;
	tmp >>= 1;
	StatusNeg = ( tmp & 0x80 ) ? 1 : 0;
	StatusZero = ( tmp ) ? 0 : 1;
	Accumuator = tmp;
	ProgramCounter++;
}

static void RotateRightZp( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ );
	RotateRightBase( dataAddr, ReadByte( dataAddr ) );
}

static void RotateRightZpIndex( )
{
	WORD dataAddr = ReadByte( ProgramCounter++ ) + XReg;
	RotateRightBase( dataAddr, ReadByte( dataAddr ) );
}

static void RotateRightAbs( )
{
	WORD dataAddr = ReadWord( ProgramCounter );
	RotateRightBase( dataAddr, ReadByte( dataAddr ) );
	ProgramCounter += 2;
}

static void RotateRightAbsIndex( )
{
	WORD dataAddr = ReadWord( ProgramCounter ) + XReg;
	RotateRightBase( dataAddr, ReadByte( dataAddr ) );
	ProgramCounter += 2;
}

static void RotateRightBase( WORD dataAddr, BYTE data )
{
	WORD tmp = data | ( StatusCarry ) ? 0x100 : 0;
	StatusCarry = ( tmp & 0x1 ) ? 1 : 0;
	tmp >>= 1;
	StatusNeg = ( tmp & 0x80 ) ? 1 : 0;
	StatusZero = ( tmp ) ? 0 : 1;
	WriteByte( dataAddr, tmp );
}

static void ReturnInterrupt( )
{
	BYTE sr;
	StackPop( &sr );
	StatusNeg = ( sr & 0x80 ) >> 7;
	StatusOverflow = ( sr & 0x40 ) >> 6;
	StatusUnk = ( sr & 20 ) >> 5;
	StatusB = ( sr & 0x10 ) >> 4;
	StatusDec = ( sr & 0x8 ) >> 3;
	StatusInt = ( sr & 0x4 ) >> 2;
	StatusZero = ( sr & 0x2 ) >> 1;
	StatusCarry = ( sr & 0x1 );
	StackPop( (BYTE*)&(ProgramCounter+1) );
	StackPop( (BYTE*)&ProgramCounter );
}

static void ReturnSubroutine( )
{
	BYTE hi, lo;
	StackPop( &lo );
	StackPop( &hi );
	ProgramCounter = ( hi << 8 ) + lo;		/*Is adding one needed?  What did we push onto the stack?*/
}

static void SubtractAccMemImm( )
{
	SubtractAccMemBase( ReadByte( ProgramCounter++ ) );
}

static void SubtractAccMemZp( )
{
	SubtractAccMemBase( ReadByte( ReadByte( ProgramCounter++ ) ) );
}

static void SubtractAccMemZpIndex( )
{
	SubtractAccMemBase( ReadByte( ReadByte( ProgramCounter++ ) + XReg ) );
}

static void SubtractAccMemAbs( )
{
	SubtractAccMemBase( ReadByte( ReadWord( ProgramCounter ) ) );
	ProgramCounter += 2;
}

static void SubtractAccMemAbsIndex( )
{
	SubtractAccMemBase( ReadByte( ReadWord( ProgramCounter ) + XReg ) );
	ProgramCounter += 2;
}

static void SubtractAccMemIndexInd( )
{
	SubtractAccMemBase( ReadByte( ReadWord( ReadByte( ProgramCounter++ ) + XReg ) ) );
}

static void SubtractAccMemIndIndex( )
{
	SubtractAccMemBase( ReadByte( ReadWord( ReadByte( ProgramCounter++ ) ) + YReg ) );
}

static void SubtractAccMemBase( BYTE data )
{
	WORD tmp = Accumulator - data - StatusCarry;
	StatusNeg = ( tmp & 0x80 ) ? 1 : 0;
	StatusZero = ( tmp ) ? 0 : 1;
	StatusOverflow = ( ((AC ^ temp) & 0x80) && ((AC ^ data) & 0x80) ) ? 1 : 0;		/*I hate this thing*/
	if( StatusDec )
	{
		/*Whatever*/
	}
	StatusCarry = ( tmp < 0x100 ) ? 1 : 0;
	Accumulator = tmp & 0xFF;
}

static void SetCarry( )
{
	StatusCarry = 1;
	ProgramCounter++;
}

static void SetDecimal( )
{
	StatusDec = 1;
	ProgramCounter++;
}

static void SetInterruptDisable( )
{
	StatusInt = 1;
	ProgramCounter++;
}

static void StoreAccZp( )
{
	WriteByte( ReadByte( ProgramCounter++ ), Accumulator );
}

static void StoreAccZpIndex( )
{
	WriteByte( ReadByte( ProgramCounter++ ) + XReg, Accumulator );
}

static void StoreAccAbs( )
{
	WriteByte( ReadWord( ProgramCounter ), Accumulator );
	ProgramCounter += 2;
}

static void StoreAccAbsIndexX( )
{
	WriteByte( ReadWord( ProgramCounter ) + XReg, Accumulator );
	ProgramCounter += 2;
}

static void StoreAccAbsIndexY( )
{
	WriteByte( ReadWord( ProgramCounter ) + YReg, Accumulator );
	ProgramCounter += 2;
}

static void StoreAccIndexInd( )
{
	WriteByte( ReadWord( ReadByte( ProgramCounter++ ) + XReg ), Accumulator );
}

static void StoreAccIndIndex( )
{
	WriteByte( ReadWord( ReadByte( ProgramCounter++ ) ) + YReg, Accumulator );
}

static void StoreXZp( )
{
	WriteByte( ReadByte( ProgramCounter++ ), XReg );
}

static void StoreXZpIndex( )
{
	WriteByte( ReadByte( ProgramCounter++ ) + YReg, XReg );
}

static void StoreXAbs( )
{
	WriteByte( ReadWord( ProgramCounter ), XReg );
	ProgramCounter += 2;
}

static void StoreYZp( )
{
	WriteByte( ReadByte( ProgramCounter++ ), YReg );
}

static void StoreYZpIndex( )
{
	WriteByte( ReadByte( ProgramCounter++ ) + XReg, YReg );
}

static void StoreYAbs( )
{
	WriteByte( ReadWord( ProgramCounter ), YReg );
	ProgramCounter += 2;
}

static void TransferAccToX( )
{
	TransferBase( &XReg, Accumulator );
}

static void TransferAccToY( )
{
	TransferBase( &YReg, Accumulator );
}

static void TransferStackToX( )
{
	TransferBase( &XReg, StackPointer );
}

static void TransferXToAcc( )
{
	TransferBase( &Accumulator, XReg );
}

static void TransferXToStack( )
{
	TransferBase( &StackPointer, XReg );
}

static void TransferYToAcc( )
{
	TransferBase( &Accumulator, YReg );
}

static void TransferBase( BYTE* dataDest, BYTE data )
{
	StatusNeg = ( data & 0x80 ) ? 1 : 0 ;
	StatusZero = ( data ) ? 0 : 1;
	*dataDest = data;
	ProgramCounter++;
}
