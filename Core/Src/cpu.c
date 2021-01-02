/*
 * cpu.c
 *
 *  Created on: Dec 20, 2020
 *      Author: ghaack
 */

#include "riot.h"
#include "memory.h"

uint8_t a = 0;
uint8_t y = 0;
uint8_t x = 0;
uint16_t pc = 0;
uint8_t sp = 0x1FF;
uint8_t stat = 0;

// Carry flag. 1 = True
#define CARRY_F 	(1 << 0)
// Zero flag. 1 = Result was 0
#define ZERO_F 		(1 << 1)
// IRQ Disable. 1 = Disabled
#define IRQD_F 		(1 << 2)
// Decimal Mode. 1 = True
#define DECM_F 		(1 << 3)
// BRK Command
#define BRK_F 		(1 << 4)
// Overflow Flag. 1 = Overflow Occurred
#define OVRF_F 		(1 << 6)
// Negative Flag. 1 = Negative
#define NEG_F 		(1 << 7)

// Test to see if the various flags are set
#define CARRY_T(n)	(n & CARRY_F)
#define ZERO_T(n)	(n & ZERO_F)
#define IRQD_T(n)	(n & IRQD_F)
#define DECM_T(n)	(n & DECMY_F)
#define BRK_T(n)	(n & BRK_F)
#define OVRF_T(n)	(n & OVRF_F)
#define NEG_T(n)	(n & NEG_F)

// Generate flags based on some check
#define ZERO_S(n)          		(((n) == 0) ? ZERO_F : 0);
#define CARRY_S(n, n1, n2)		((((n) < (n1)) | ((n) < (n2))) << CARRY_F)  // Not sure how this one works tbh. I stole it.
#define NEG_S(n)				(((n) & 0x80) ? NEG_F : 0);
#define OVRF_S(n, n1, n2)		((((n1) == 0xFF) && ((n2) == 0x0)) ? OVRF_F : 0)


// Notes on memory:
// TODO: Look up how page boundaries are crossed. Need to tick then
// Page Zero
// Used for zero page and indirect addressing
// Zero Page Addressing:
//   CPU assumes high byte is 0x00, low byte is second byte of instruction
// Indirect Addressing
//   Second byte of instruction points to location in page zero that contains
//   the low byte of a full two byte address. The high byte is the next
//   highest byte.
//     Ex: Indirect addressing, second byte is 0x12
//         Points to byte 0x12 in page zero, which contains 0xad.
//         0x13 contains 0xde. Full address is 0xdead
//
// Page One
// Used for the stack. You can muck about here, but it's a bad idea.
// 0x1FF to 0x100
//
// Page 0xFF
// Used for interrupt vectors
// Six highest bytes are designated for interrupt vectors, arranged in 3 pairs.
// These vectors are addresses where the CPU goes on reset and on both types of
// Interrupts (NMI and INT). 2600 doesn't care about interrupts cause Atari is cheap.
void cpu_tick(){
	return;
}

uint8_t get_opcode(){
	uint8_t ret = read_memory(pc);
	pc++;
	return ret;
}

void push_stack(uint8_t data){
	write_memory(sp, data);
	sp--;
}

uint16_t get_indirect_addr(uint8_t low_byte){
	// Indirect addressing uses the low byte to get an address in the zero page
	uint16_t ret = read_memory(low_byte);
	return ret | (read_memory(low_byte + 1) << 8);
}

void decode_instruction(uint8_t instr){
	uint8_t workspace = 0;
	switch(instr){
		case 0x00:
			// Force Break. Interrupt, push PC + 2, push stat
			stat = stat | BRK_F;
			push_stack(pc + 2);
			push_stack(stat);
			riot_tick(7);
			break;
		case 0x01:
			// ORA (indirect, x)
			a = a | read_memory(get_indirect_addr(get_opcode() + x));
			flags = flags | ZERO_S(a);
			flags = flags | NEG_S(a);
			riot_tick(6);
			break;
		case 0x05:
			// ORA Zero Page
			a = a | read_memory(get_opcode());
			flags = flags | ZERO_S(a);
			flags = flags | NEG_S(a);
			riot_tick(6);
			break;
	}
}
