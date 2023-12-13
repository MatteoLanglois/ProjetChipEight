#include "processor.h"
#include "register.h"
#include "stack.h"

struct Processor* Proc_init() {
    struct Processor* processor = malloc(sizeof(struct Processor));
    if (processor == NULL) {
        return NULL;
    }
    processor->reg = Reg_Init();
    processor->stack = Stack_init();

    if (processor->reg == NULL
        || processor->stack == NULL) {
      return NULL;
    }

    return processor;
}

void Proc_delete(struct Processor* processor) {
    Stack_destroy(processor->stack);
    Reg_destroy(processor->reg);
    free(processor);
}

void ret(struct Processor* processor) {
    processor->reg->programCounter = processor->stack->tail->value;
}

void jmp(struct Processor* processor, const uint16_t addr) {
    processor->reg->programCounter = addr;
}

void call(struct Processor* processor, const uint16_t addr) {
    addStack(processor->stack, processor->reg->programCounter);
    processor->reg->programCounter = addr;
}
