#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RAM_max 4096

struct RandomAccessMemory {
  uint8_t* memory;
};

struct RandomAccessMemory* RAM_init();
unsigned int RAM_read(const unsigned int* memory, uint16_t addr);
int RAM_write(struct RandomAccessMemory* memory, uint16_t addr, const uint8_t newVal);
int RAM_destroy(struct RandomAccessMemory* memory);

#endif