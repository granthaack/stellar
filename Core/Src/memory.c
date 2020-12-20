/*
 * memory.c
 *
 *  Created on: Dec 19, 2020
 *      Author: ghaack
 */

#include "memory.h"

#define A7 	(1 << 7)
#define A12 (1 << 12)
// TODO: These can probably be greatly sped up with some fancy bit masking
// TODO: Or inline ASM

// To be in TIA:
//	A7 = 0, A12 = 0
uint8_t in_tia(uint16_t addr){
	uint16_t mask = addr & (A7 | A12);
	if(mask){
		return 0;
	}
	return 1;
}

// To be in RIOT:
//	A7 = 1, A12 = 0
uint8_t in_riot(uint16_t addr){
	uint16_t mask = addr & (A7 | A9 | A12);
	if(mask & A12){
		return 0;
	}
	else if((mask & A7) == 0){
		return 0;
	}
	else{
		return 1;
	}
}

// To be in Cart:
//	A12 = 1
uint8_t in_cart(uint16_t addr){
	if(addr & A12){
		return 1;
	}
	return 0;
}


