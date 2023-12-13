#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>
#include <stdlib.h>

#include "stack.h"
#include "register.h"

struct Processor {
    struct Register* reg;
    struct Stack* stack;
};

struct Processor* Proc_init();
void Proc_delete(struct Processor* processor);


void ret(struct Processor* processor);
void jmp(struct Processor* processor, const uint16_t addr);
void call(struct Processor* processor, const uint16_t addr);

#endif