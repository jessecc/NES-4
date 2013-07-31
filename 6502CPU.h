#ifndef __6502_CPU_H_
#define __6502_CPU_H_
#include "6502Types.h"

#define	CLR_LEVEL			0x00
#define	NMI_LEVEL			0x01
#define IRQ_LEVEL			0x02
#define RST_LEVEL			0x03
#define ABT_LEVEL			0x04

static 6502Reg8		Accumulator;
static 6502Reg8		XReg;
static 6502Reg8		YReg;
static 6502Reg8		StackPointer;
static 6502Reg8		StatusZero;
static 6502Reg8      StatusCarry;
static 6502Reg8      StatusNeg;
static 6502Reg8      StatusInt;
static 6502Reg8      StatusDec;
static 6502Reg8      StatusOverflow;
static 6502Reg8      StatusB;
static 6502Reg8      StatusUnk;
static 6502Reg16	   ProgramCounter;
BYTE				InterruptType;

unsigned int 6502CPUInit( unsigned char* ramPtr );
void 6502CPUFetch( unsigned char* opcode );
void 6502CPUDecode( unsigned char opcode, short* funcIndex );
void 6502CPUExecute( unsigned char opcode );
void 6502Interrupt( );
/*ADC - Add Memory with Accumulator with Carry*/
static void AddCarryImm( );					/*ADC #$12	 	- A = A + 0x12*/
static void AddCarryZp( );				   	/*ADC 0x12		- A = A + [0x0012] Prepends an 0x00 to create a 16 bit address*/
static void AddCarryZpIndex( );				/*ADC 0x12,X	- A = A + [0x0012 + X]*/
static void AddCarryAbs( );					/*ADC 0x1234	- A = A + 0x1234*/
static void AddCarryAbsIndexX( );			/*ADC 0x1234,X	- A = A + [0x1234 + X]*/
static void AddCarryAbsIndexY( );			/*ADC 0x1234,Y	- A = A + [0x1234 + Y]*/
static void AddCarryIndexInd( );		   	/*ADC (0x12,X)	- A = A + [[0x0012 + X]] Get the address pointed to by 0x0012 + X, then get the byte pointed to by that address.*/
static void AddCarryIndIndex( );		   	/*ADC (0x12),Y	- A = A + [[0x12] + Y] Get the address pointed to by 0x0012, then add Y and then get the byte pointed to by that address*/
static void AddCarryBase( BYTE data );
/*AND - And Memory with Accumulator*/
static void AndImm( );					   	/*AND #$12		- A = A & 0x12*/
static void AndZp( );			   			/*AND 0x12		- A = A & [0x0012]*/
static void AndZpIndex( );			   		/*AND 0x12,X	- A = A & [0x0012 + X]*/
static void AndAbs( );				   		/*AND 0x1234	- A = A & [0x1234]*/
static void AndAbsIndexX( );		   		/*AND 0x1234,X	- A = A & [0x1234 + X]*/
static void AndAbsIndexY( );		   		/*AND 0x1234,Y	- A = A & [0x1234 + Y]*/
static void AndIndexInd( );					/*AND (0x12,X)	- A = A & [[0x0012 + X]]*/
static void AndIndIndex( );					/*AND (0x12),Y	- A = A & [[0x12] + Y]*/
static void AndBase( BYTE data );
/*ASL - Arithmatic Shift Left*/
static void ShlAcc( );				   		/*ASL A			- A = A << 1*/
static void ShlZp( );				   		/*ASL 0x12		- [0x0012] << 1*/
static void	ShlZpIndex( );			   		/*ASL 0x12,X	- [0x0012 + X] << 1*/
static void	ShlAbs( );				   		/*ASL 0x1234	- [0x1234] << 1*/
static void ShlAbsIndex( );					/*ASL 0x1234,X	- [0x1234 + X] << 1*/
/*BCC - Branch Carry Clear*/
static void BranchCarryClear( );		   	/*BCC 0x12		- PC = ( StatusCarry ^ 1 ) ? 0x12 : PC*/
/*BCS - Branch Carry Set*/
static void BranchCarrySet( );				/*BCS 0x12		- PC = ( !( StatusCarry ^ 1 ) ) ? 0x12 : PC*/
/*BEQ - Branch Zero*/
static void BranchZero( );			   		/*BEQ 0x12		- PC = ( StatusZero ^ 1 ) ? 0x12 : PC*/
/*BIT - Bit Test bits in memory with Accumulator*/
static void BitTestZp( );			   		/*BIT 0x12		- StatusZero = (A & [0x0012]); StatusNegative = ([0x0012] & 0x40); StatusOverflow = ([0x0012] & 0x20)*/
static void BitTestAbs( );			   		/*BIT 0x1234	- StatusZero = (A & [0x0012]); StatusNegative = ([0x0012] & 0x40); StatusOverflow = ([0x0012] & 0x20)*/
static void BitTestBase( BYTE data );
/*BMI - Branch Negative*/
static void BranchNegative( );				/*BMI 0x12		- PC = ( !( StatusNeg ^ 1 ) ) ? 0x12 : PC*/
/*BNE - Branch Not Zero*/
static void BranchNotZero( );	   			/*BNE 0x12		- PC = ( StatusZero ^ 1 ) ? 0x12 : PC*/
/*BPL - Branch Not Negative*/
static void BranchNotNegative( );			/*BPL 0x12		- PC = ( StatusNeg ^ 1 ) ? 0x12 : PC*/
/*BRK - Force Break*/
static void Break( );				   		/*BRK			- PC = [0xFFFE]; StatusB = 1; StatusInterupt = 1; Loads address pointed to by [0xFFFE] into PC also pushes current PC onto the stack*/
/*BVC - Branch No Overflow*/
static void BranchNoOverflow( );		   	/*BVC 0x12		- PC = ( StatusOverflow ^ 1 ) ? 0x12 : PC*/
/*BVS - Branch Overflow*/
static void BranchOverflow( );				/*BVS 0x12		- PC = ( !( StatusOverflow ^ 1 ) ) ? 0x12 : PC*/
/*CLC - Clear Carry*/
static void ClearCarry( );			   		/*CLC			- StatusCarry = 0*/
/*CLD - Clear Decimal Flag*/
static void ClearDecimal( );			   	/*CLD			- StatusDecimal = 0*/
/*CLI - Clear Interupts*/
static void ClearInterupts( );				/*CLI			- StatusInterupts = 0*/
/*CLV - Clear Overflow*/
static void ClearOverflow( );	   			/*CLV			- StatusOverflow = 0*/
/*CMP - Compare Memory and Accumulator*/
static void CmpAccImm( );			   		/*CMP #$12		- StatusZero = ( A - 0x12 ) ? 1 : 0; StatusCarry = ( A > 0x12 ); StatusNeg = ( A < 0x12 )*/
static void CmpAccZp( );			   		/*CMP 0x12		- StatusZero = ( A - [0x0012] ) ? 1 : 0; StatusCarry = ( A > [0x0012] ); StatusNeg = ( A < [0x0012] )*/
static void CmpAccZpIndex( );		   		/*CMP 0x12,X	- StatusZero = ( A - [0x0012 + X] ) ? 1 : 0; StatusCarry = ( A > [0x0012 + X] ); StatusNeg = ( A < [0x0012 + X] )*/
static void CmpAccAbs( );			   		/*CMP 0x0012	- StatusZero = ( A - [0x0012] ) ? 1 : 0; StatusCarry = ( A > [0x0012] ); StatusNeg = ( A < [0x0012] )*/
static void CmpAccAbsIndexX( );				/*CMP 0x0012,X	- StatusZero = ( A - [0x0012 + X] ) ? 1 : 0; StatusCarry = ( A > [0x0012 + X] ); StatusNeg = ( A < [0x0012 + X] )*/
static void CmpAccAbsIndexY( );				/*CMP 0x0012,Y	- StatusZero = ( A - [0x0012 + Y] ) ? 1 : 0; StatusCarry = ( A > [0x0012 + Y] ); StatusNeg = ( A < [0x0012 + Y] )*/
static void CmpAccIndexInd( );				/*CMP (0x12,X)	- StatusZero = ( A - [[0x0012 + X]] ) ? 1 : 0; StatusCarry = ( A > [[0x0012 + X]] ); StatusNeg = ( A < [[0x0012 + X]] )*/
static void CmpAccIndIndex( );				/*CMP (0x12),Y	- StatusZero = ( A - [[0x0012] + Y] ) ? 1 : 0; StatusCarry = ( A > [[0x0012] + Y] ); StatusNeg = ( A < [[0x0012] + Y] )*/
static void CmpBase( BYTE data, 6502Reg8 reg );
/*CPX - Compare Memory Index X*/
static void CmpXImm( );			   			/*CPX #$12		- StatusZero = ( X - 0x12 ) ? 1 : 0; StatusCarry = ( X > 0x12 ); StatusNeg = ( X < 0x12 )*/
static void CmpXZp( );				   		/*CPX 0x12		- StatusZero = ( X - [0x0012] ) ? 1 : 0; StatusCarry = ( X > [0x0012] ); StatusNeg = ( X < [0x0012] )*/
static void CmpXAbs( );				   		/*CPX 0x1234	- StatusZero = ( X - [0x1234] ) ? 1 : 0; StatusCarry = ( X > [0x1234] ); StatusNeg = ( X < [0x1234] )*/
/*CPY - Compare Memory Index Y*/
static void CmpYImm( );					   	/*CPY #$12		- StatusZero = ( Y - 0x12 ) ? 1 : 0; StatusCarry = ( Y > 0x12 ); StatusNeg = ( Y < 0x12 )*/
static void CmpYZp( );				   		/*CPY 0x12		- StatusZero = ( Y - [0x0012] ) ? 1 : 0; StatusCarry = ( Y > [0x0012] ); StatusNeg = ( Y < [0x0012] )*/
static void CmpYAbs( );				   		/*CPY 0x1234	- StatusZero = ( Y - [0x1234] ) ? 1 : 0; StatusCarry = ( Y > [0x1234] ); StatusNeg = ( Y < [0x1234] )*/
/*DEC - Decrement memory by 1*/
static void DecrementMemZp( );				/*DEC 0x12		- [0x0012]--; StatusZero = ![0x0012]; StatusNeg = [0x0012] & 0x80*/
static void DecrementMemZpIndex( );			/*DEC 0x12,X	- [0x0012 + X]--; StatusZero = ![0x0012 + X]; StatusNeg = [0x0012 + X] & 0x80*/
static void DecrementMemAbs( );				/*DEC 0x1234	- [0x1234]--; StatusZero = ![0x1234]; StatusNeg = [0x1234] & 0x80*/
static void DecrementMemAbsIndex( );		/*DEC 0x1234,X	- [0x1234 + X]--; StatusZero = ![0x1234 + X]; StatusNeg = [0x1234 + X] & 0x80*/
/*DEX - Decrement X index by 1*/
static void DecrementX( );                /*DEX */
/*DEY - Decrement Y index by 1*/
static void DecrementY( );                /*DEY */
/*EOR - XOR memory with Accumulator*/
static void XorAccMemImm( );              /*EOR #$12     - A = A ^ 0x12*/
static void XorAccMemZp( );               /*EOR 0x12     - A = A ^ [0x0012]*/
static void XorAccMemZpIndex( );          /*EOR 0x12,X   - A = A ^ [0x0012 + X]*/
static void XorAccMemAbs( );              /*EOR 0x1234   - A = A ^ [0x1234]*/
static void XorAccMemAbsIndexX( );        /*EOR 0x1234,X - A = A ^ [0x1234 + X]*/
static void XorAccMemAbsIndexY( );        /*EOR 0x1234,Y - A = A ^ [0x1234 + Y]*/
static void XorAccMemIndexInd( );         /*EOR (0x12,X) - A = A ^ [[0x0012 + X]]*/
static void XorAccMemIndIndex( );         /*EOR (0x12),Y - A = A ^ [[0x0012] + Y]*/
/*INC - Increment Memory by 1*/
static void IncrementMemZp( );            /*INC 0x12     - [0x0012]++*/
static void IncrementMemZpIndex( );       /*INC 0x12,X   - [0x0012 + X]++*/
static void IncrementMemAbs( );           /*INC 0x1234   - [0x1234]++*/
static void IncrementMemAbsIndex( );      /*INC 0x1234,X - [0x1234 + X]++*/
/*INX - Increment X index by 1*/
static void IncrementX( );                /*INX          - X++*/
/*INY - Increment Y index by 1*/
static void IncrementY( );                /*INY          - Y++*/
/*JMP - Jump*/
static void JumpAbs( );                   /*JMP 0x1234   - PC = 0x1234*/
static void JumpInd( );                   /*JMP [0x1234] - PC = [0x1234]*/
/*JSR - Jump to Subroutine*/
static void JumpSubroutine( );            /*JSR 0x1234   - SP = SP - 2; [SP] = PC; PC = 0x1234*/
static void StackPush( BYTE data );
static void StackPop( BYTE* data );
/*LDA - Load Accumulator with Memory*/
static void LoadAccImm( );                /*LDA #$12     - A = 0x12*/
static void LoadAccZp( );                 /*LDA 0x12     - A = [0x0012]*/
static void LoadAccZpIndex( );            /*LAD 0x12,X   - A = [0x0012 + X]*/
static void LoadAccAbs( );                /*LDA 0x1234   - A = [0x1234]*/
static void LoadAccAbsIndex( );           /*LDA 0x1234,X - A = [0x1234 + X]*/
static void LoadAccIndexInd( );           /*LDA (0x12,X) - A = [[0x0012 + X]]*/
static void LoadAccIndIndex( );           /*LDA (0x12),Y - A = [[0x0012] + Y]*/
static void LoadAccBase( BYTE data );
/*LDX - Load X index with Memory*/
static void LoadXImm( );                  /*LDX #$12     - X = 0x12*/
static void LoadXZp( );                   /*LDX 0x12     - X = [0x0012]*/
static void LoadXZpIndex( );              /*LDX 0x12,Y   - X = [0x0012 + Y]*/
static void LoadXAbs( );                  /*LDX 0x1234   - X = [0x1234]*/
static void LoadXAbsIndex( );             /*LDX 0x1234,Y - X = [0x1234 + Y]*/
/*LDY - Load Y index with memory*/
static void LoadYImm( );
static void LoadYZp( );
static void LoadYZpIndex( );
static void LoadYAbs( );
static void LoadYAbsIndex( );
/*NOP - No Operation ( 2 Cycles )*/
static void NoOperation( );
/*ORA - OR memory with Accumulator*/
static void OrAccMemImm( );
static void OrAccMemZp( );
static void OrAccMemZpIndex( );
static void OrAccMemAbs( );
static void OrAccMemAbsIndex( );
static void OrAccMemIndexInd( );
static void OrAccMemIndIndex( );
/*PHA - Push Accumulator to Stack*/
static void PushAcc( );
/*PHP - Push Program Counter to Stack*/
static void PushProgramCounter( );
/*PLA - Pop Accumulator from Stack*/
static void PopAcc( );
/*PLP - Pop Program Counter from Stack*/
static void PopProgramCounter( );
/*ROL - Rotate one bit left (memory or accumulator)*/
static void RotateLeftAcc( );
static void RotateLeftZp( );
static void RotateLeftZpIndex( );
static void RotateLeftAbs( );
static void RotateLeftAbsIndex( );
/*ROR - Rotate one bit right (memory or accumulator)*/
static void RotateRightAcc( );
static void RotateRightZp( );
static void RotateRightZpIndex( );
static void RotateRightAbs( );
static void RotateRightAbsIndex( );
/*RTI - Return from Interrupt*/
static void ReturnInterrupt( );
/*RTS - Return from Subroutine*/
static void ReturnSubroutine( );
/*SBC - Subtract memory from Acc with carry*/
static void SubtractAccMemImm( );
static void SubtractAccMemZp( );
static void SubtractAccMemZpIndex( );
static void SubtractAccMemAbs( );
static void SubtractAccMemAbsIndex( );
static void SubtractAccMemIndexInd( );
static void SubtractAccMemIndIndex( );
/*SEC - Set Carry flag*/
static void SetCarry( );
/*SED - Set Decimal Mode*/
static void SetDecimal( );
/*SEI - Set Interupt disable*/
static void SetInterruptDisable( );
/*STA - Store Accumulator in Memory*/
static void StoreAccZp( );
static void StoreAccZpIndex( );
static void StoreAccAbs( );
static void StoreAccAbsIndexX( );
static void StoreAccAbsIndexY( );
static void StoreAccIndexInd( );
static void StoreAccIndIndex( );
/*STX - Store X index in memory*/
static void StoreXZp( );
static void StoreXZpIndex( );
static void StoreXAbs( );
/*STY - Store Y index in memory */
static void StoreYZp( );
static void StoreYZpIndex( );
static void StoreYAbs( );
/*TAX - Transfer Accumulator to X index*/
static void TransferAccToX( );
/*TAY - Transfer Accumulator to Y index*/
static void TransferAccToY( );
/*TSX - Transfer Stack pointer to X index*/
static void TransferStackToX( );
/*TXA - Transfer X index to Accumulator*/
static void TransferXToAcc( );
/*TXS - Transfer X index to Stack pointer*/
static void TransferXToStack( );
/*TYA - Transfer Y index to Accumulator*/
static void TransferYToAcc( );
#endif
