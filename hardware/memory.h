#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef ProjetChipEight
#define ProjetChipEight

struct pile {
  unsigned int value;
  struct pile* next;
};

struct RandomAccessMemory {
  uint8_t* memory;
};

struct RandomAccessMemory* initMemory();

unsigned int readMemory(const unsigned int* memory, const unsigned int addr);

int writeMemory(struct RandomAccessMemory* memory, const unsigned int addr, const uint8_t  newVal);

int freeMemory(struct RandomAccessMemory* memory);

#endif