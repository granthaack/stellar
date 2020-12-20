/*
 * riot.c
 *
 *  Created on: Dec 10, 2020
 *      Author: ghaack
 */

// TODO: Where does timer get reset?
// TODO: How does the timer decide which divide by to use? Can more than one be set?

#include "riot.h"

#define A0 (1 << 0)
#define A1 (1 << 1)
#define A2 (1 << 2)
#define A3 (1 << 3)
#define A4 (1 << 4)
#define A5 (1 << 5)
#define A6 (1 << 6)
#define A9 (1 << 9)

uint8_t reg_ddra = 0;
uint8_t reg_ddrb = 0;

uint8_t reg_outa = 0;
uint8_t reg_outb = 0;

uint8_t ram[128] = {0};

// Bit 7 is timer flag. Cleared when register is written or read
// Bit 6 is PA7 flag. Cleared when register is read
uint8_t interrupt_flag = 0;

// Divide by flags
uint8_t t1_flag = 0;
uint8_t t8_flag = 0;
uint8_t t64_flag = 0;
uint8_t t1024_flag = 0;

// PA7 edge detection is probably never used, but it technically could be used
uint8_t pa7_flag = 0;
// 0 for negative edge detect, 1 for positive edge detect
uint8_t pa7_edge_detect = 0;

uint8_t timer = 0;

void riot_write(uint16_t addr, uint8_t data){
	if(in_ram(addr)){
		addr = addr & (A0 | A1 | A2 | A3 | A4 | A5 | A6);
		ram[addr] = data;
		return;
	}
	else if(in_timer(addr)){
		write_timer(addr, data);
		return;
	}
	else{
		if(in_ddra(addr)){
			reg_ddra = data;
			return;
		}
		else if(in_ddrb(addr)){
			ret_ddrb = data;
			return;
		}
		// There are no outputs on the 2600, do not write to OUTA, OUTB
	}
}
uint8_t riot_read(uint16_t addr){
	if(in_ram(addr)){
		addr = addr & (A0 | A1 | A2 | A3 | A4 | A5 | A6);
		return ram[addr];
	}
	else if(in_timer(addr)){
		return read_timer(addr, data);
	}
	else{
		if(in_ddra(addr)){
			return reg_ddra ;
		}
		else if(in_ddrb(addr)){
			return ret_ddrb;
		}
		else if(in_outa(addr)){
			return reg_outa;
		}
		else if(in_outb(addr)){
			return reg_outb;
		}
	}
	return 0xFF;
}

// RAM is selected when A9/RS is 0
uint8_t in_ram(uint16_t addr){
	if(addr & a9){
		return 0;
	}
	return 1;
}

uint8_t in_outa(uint16_t addr){
	uint16_t mask = addr & (A0 | A1 | A2);
	if(mask){
		return 0;
	}
	return 1;
}

uint8_t in_ddra(uint16_t addr){
	uint16_t mask = addr & (A0 | A1 | A2);
	if(mask == 0x1){
		return 1;
	}
	return 0;
}

uint8_t in_outb(uint16_t addr){
	uint16_t mask = addr & (A0 | A1 | A2);
	if(mask == 0x2){
		return 1;
	}
	return 0;
}

uint8_t in_ddrb(uint16_t addr){
	uint16_t mask = addr & (A0 | A1 | A2);
	if(mask == 0x3){
		return 1;
	}
	return 0;
}

uint8_t in_timer(uint16_t addr){
	uint16_t mask = addr & (A0 | A1 | A2);
	if(mask & A2){
		return 1;
	}
	return 0;
}

uint8_t read_timer(uint16_t addr, uint8_t data){
	if(addr & A0){
		uint8_t ret = interrupt_flag;
		// Clear PA7 flag
		interrupt_flag = ~(0x40) & interrupt_flag;
		return ret;
	}
	else{
		// Clear Interrupt flag
		interrupt_flag = ~(0x80) & interrupt_flag;
		return timer;
	}
}

void write_timer(uint16_t addr, uint8_t data){
	uint8_t mask = addr & (A1 | A0);
	// Don't check for interrupt flag to IRQ since it's not used in the 2600
	if(addr & A4){
		if(mask == 0x0){
			t1_flag = data;
			// Clear Interrupt flag
			interrupt_flag = ~(0x80) & interrupt_flag;
			return;
		}
		else if(mask == 0x1){
			t8_flag = data;
			// Clear Interrupt flag
			interrupt_flag = ~(0x80) & interrupt_flag;
			return;
		}
		else if(mask == 0x2){
			t64_flag = data;
			// Clear Interrupt flag
			interrupt_flag = ~(0x80) & interrupt_flag;
			return;
		}
		else if(mask == 0x3){
			t1024_flag = data;
			// Clear Interrupt flag
			interrupt_flag = ~(0x80) & interrupt_flag;
			return;
		}
	}
	else{
		// Don't check for PA7 to IRQ since it's not used in the 2600
		if(mask & A0){
			if(data){
				pa7_edge_detect = 1;
				return;
			}
		}
		else{
			if(data){
				pa7_edge_detect = 0;
				return;
			}
		}
	}
}
