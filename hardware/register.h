#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include <stdlib.h>

#define REGV_SIZE 16;


struct Register {
    uint8_t* regV;
    uint16_t I;
    uint8_t DT;
    uint8_t ST;
    uint16_t programCounter;
};

struct Register* Reg_Init();
void Reg_destroy(struct Register* reg);

#endif
