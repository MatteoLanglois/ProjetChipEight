#include "memory.h"

struct RandomAccessMemory* initMemory(){
  struct RandomAccessMemory* memory = malloc(sizeof(struct RandomAccessMemory));
  memory->memory = malloc(4096 * sizeof(uint8_t));
  return memory;
}

uint8_t readMemory(const struct RandomAccessMemory* memory, uint16_t addr){
  if (addr < 4096){
    return memory->memory[addr];
  }
  return -1;
}

int writeMemory(struct RandomAccessMemory* memory, uint16_t addr, const uint8_t newVal) {
  if (addr <= 4096) {
    memory->memory[addr] = newVal;
  }
  return 0;
}

int freeMemory(struct RandomAccessMemory* memory){
  free(memory->memory);
  return 0;
}