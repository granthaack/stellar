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
uint8_t sp = 1FF;
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
#define ZERO_S(n)          		(((n) == 0) ? ZERO_F : 0)
#define CARRY_S(n, n1, n2)		((((n) < (n1)) | ((n) < (n2))) << CARRY_F)  // Not sure how this one works tbh. I stole it.
#define NEG_S(n)				((n) & 0x80) ? NEG_F : 0)
#define OVRF_S(n, n1, n2)		((((n1) == 0xFF) && ((n2) == 0x0)) ? OVRF_F : 0)

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

void decode_instruction(uint8_t instr){
	switch(instr){
		case 0x0:
			stat = stat | BRK_F;
			push_stack(pc + 2);
			push_stack(stat);
			riot_tick(7);
	}
}
