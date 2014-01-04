#ifndef __6502_CPU_H_
#define __6502_CPU_H_
//#include "6502Types.h"
#include "6502Emu.h"
#include "GenericTypes.h"

#define	CLR_LEVEL			0x00
#define	NMI_LEVEL			0x01
#define IRQ_LEVEL			0x02
#define RST_LEVEL			0x03
#define ABT_LEVEL			0x04


unsigned int CPUInit( minterface_t *ramPtr );
void CPUFetch( e6502_t *cpu, unsigned char* opcode );
void CPUDecode( unsigned char opcode, short* funcIndex );
void CPUExecute( e6502_t *cpu, unsigned char opcode );
void CPUInterrupt( e6502_t *cpu );

/*
//AAC
static void AACImm( );
//AAX
static void AAXZp( );
static void AAXZpIndexY( );
static void AAXIndexInd( );
static void AAXAbs( );
//ARR
static void ARPImm( );
//ASR
static void ASRImm( );
//ATX
static void ATXImm( );
//AXA
static void AXAAbsIndexY( );
static void AXAIndIndex( );
//AXS
static void AXSImm( );
//DCP
static void DCPZp( );
static void DCPZpIndex( );
static void DCPAbs( );
static void DCPAbsIndexX( );
static void DCPAbsIndexY( );
static void DCPIndexInd( );
static void DCPIndIndex( );
//DOP
static void DOP( );
*/
static void XorAccMemBase( e6502_t *cpu, BYTE data );
static void LoadXBase( e6502_t *cpu, BYTE data );
static void LoadYBase( e6502_t *cpu, BYTE data );
static void OrAccMemBase( e6502_t *cpu, BYTE data );
static void RotateLeftBase( e6502_t *cpu, WORD dataAddr, BYTE data );
static void RotateRightBase( e6502_t *cpu, WORD dataAddr, BYTE data );
static void SubtractAccMemBase( e6502_t *cpu, BYTE data );
static void TransferBase( e6502_t *cpu, BYTE* dataDest, BYTE data );
static void UndocOp( e6502_t *cpu );
static void Jam( e6502_t *cpu );
static void AddCarryBase( e6502_t *cpu, BYTE data );
static void AndBase( e6502_t *cpu, BYTE data );
static void BitTestBase( e6502_t *cpu, BYTE data );
static void CmpBase( e6502_t *cpu, BYTE data, BYTE reg );
static void StackPush( e6502_t *cpu, BYTE data );
static void StackPop( e6502_t *cpu, BYTE* data );
static void LoadAccBase( e6502_t *cpu, BYTE data );
static void LoadYImm( e6502_t *cpu );
static void LoadYZp( e6502_t *cpu );
static void LoadYZpIndex( e6502_t *cpu );
static void LoadYAbs( e6502_t *cpu );
static void LoadYAbsIndex( e6502_t *cpu );
static void NoOperation( e6502_t *cpu );
static void OrAccMemImm( e6502_t *cpu );
static void OrAccMemZp( e6502_t *cpu );
static void OrAccMemZpIndex( e6502_t *cpu );
static void OrAccMemAbs( e6502_t *cpu );
static void OrAccMemAbsIndexX( e6502_t *cpu );
static void OrAccMemAbsIndexY( e6502_t *cpu );
static void OrAccMemIndexInd( e6502_t *cpu );
static void OrAccMemIndIndex( e6502_t *cpu );
static void PushAcc( e6502_t *cpu );
static void PushProgramCounter( e6502_t *cpu );
static void PopAcc( e6502_t *cpu );
static void PopProgramCounter( e6502_t *cpu );
static void RotateLeftAcc( e6502_t *cpu );
static void RotateLeftZp( e6502_t *cpu );
static void RotateLeftZpIndex( e6502_t *cpu );
static void RotateLeftAbs( e6502_t *cpu );
static void RotateLeftAbsIndex( e6502_t *cpu );
static void RotateRightAcc( e6502_t *cpu );
static void RotateRightZp( e6502_t *cpu );
static void RotateRightZpIndex( e6502_t *cpu );
static void RotateRightAbs( e6502_t *cpu );
static void RotateRightAbsIndex( e6502_t *cpu );
static void ReturnInterrupt( e6502_t *cpu );
static void ReturnSubroutine( e6502_t *cpu );
static void ShlAcc( e6502_t *cpu );
static void ShlZp( e6502_t *cpu );
static void ShlZpIndex( e6502_t *cpu );
static void ShlAbs( e6502_t *cpu );
static void ShlAbsIndex( e6502_t *cpu );
static void ShrAcc( e6502_t *cpu );
static void ShrZp( e6502_t *cpu );
static void ShrZpIndex( e6502_t *cpu );
static void ShrAbs( e6502_t *cpu );
static void ShrAbsIndex( e6502_t *cpu );
static void SubtractAccMemImm( e6502_t *cpu );
static void SubtractAccMemZp( e6502_t *cpu );
static void SubtractAccMemZpIndex( e6502_t *cpu );
static void SubtractAccMemAbs( e6502_t *cpu );
static void SubtractAccMemAbsIndex( e6502_t *cpu );
static void SubtractAccMemIndexInd( e6502_t *cpu );
static void SubtractAccMemIndIndex( e6502_t *cpu );
static void SetCarry( e6502_t *cpu );
static void SetDecimal( e6502_t *cpu );
static void SetInterruptDisable( e6502_t *cpu );
static void StoreAccZp( e6502_t *cpu );
static void StoreAccZpIndex( e6502_t *cpu );
static void StoreAccAbs( e6502_t *cpu );
static void StoreAccAbsIndexX( e6502_t *cpu );
static void StoreAccAbsIndexY( e6502_t *cpu );
static void StoreAccIndexInd( e6502_t *cpu );
static void StoreAccIndIndex( e6502_t *cpu );
static void StoreXZp( e6502_t *cpu );
static void StoreXZpIndex( e6502_t *cpu );
static void StoreXAbs( e6502_t *cpu );
static void StoreYZp( e6502_t *cpu );
static void StoreYZpIndex( e6502_t *cpu );
static void StoreYAbs( e6502_t *cpu );
static void TransferAccToX( e6502_t *cpu );
static void TransferAccToY( e6502_t *cpu );
static void TransferStackToX( e6502_t *cpu );
static void TransferXToAcc( e6502_t *cpu );
static void TransferXToStack( e6502_t *cpu );
static void TransferYToAcc( e6502_t *cpu );
#endif
