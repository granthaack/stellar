/*
 * riot.h
 *
 *  Created on: Dec 9, 2020
 *      Author: ghaack
 */

#ifndef SRC_MEMORY_H_
#define SRC_MEMORY_H_

// TIA Addresses:
// CS3/A7 = 0, CS0/A12 = 0, A8 - A11 = X, A6 = X
// A0 - A5 Select TIA Regs
uint8_t in_tia(uint16_t addr);

// RIOT Addresses:
// CS1/A7 = 1, CS2/A12 = 0, A8 = X
// A0 - A6 Select RAM Addresses, A9/RS selects RAM or registers
uint8_t in_riot(uint16_t addr);

// Cart Addresses
// A12 = 1, A0 - A11 Read Addresses
uint8_t in_cart(uint16_t addr);

uint8_t read_memory(uint16_t addr);

void write_memory(uint16_t addr, uint8_t data);

#endif /* SRC_MEMORY_H_ */


