#include "register.h"

struct Register* Reg_Init() {================
    struct Register* reg = malloc(sizeof(struct Register));
    reg->regV = malloc(((int) REGV_SIZE) * sizeof(uint8_t));
    
    return reg;
}

void Reg_destroy(struct Register* reg) {
    free(reg->regV);
    free(reg);
}


