/*
 * riot.h
 *
 *  Created on: Dec 9, 2020
 *      Author: ghaack
 */

#ifndef SRC_RIOT_H_
#define SRC_RIOT_H_

#include "memory.h"

void riot_write(uint16_t addr, uint8_t data);
uint8_t riot_read(uint16_t addr);
void riot_tick(uint8_t count);

#endif /* SRC_RIOT_H_ */
