#include "memory.h"

struct RandomAccessMemory* RAM_init(){
  struct RandomAccessMemory* memory = malloc(sizeof(struct RandomAccessMemory));
  if (memory == NULL){
    return NULL;
  }

  for(int i = 0; i < RAM_max; i++){
    memory->memory[i] = 0;
  }

  return memory;
}

uint8_t RAM_read(struct RandomAccessMemory* memory, uint16_t addr){
  if (addr < RAM_max){
    return memory->memory[addr];
  }
  return -1;
}

int RAM_write(struct RandomAccessMemory* memory, uint16_t addr, const uint8_t newVal) {
  if (addr <= 4096) {
    memory->memory[addr] = newVal;
  }
  return 0;
}

int RAM_destroy(struct RandomAccessMemory* memory){
  free(memory->memory);
  return 0;
}