#include "processor.h"

struct Processor* Proc_init() {
    struct Processor* processor = malloc(sizeof(struct Processor));
    if (processor == NULL) {
        return NULL;
    }
    processor->stack = malloc(16 * 16);
    if (processor->stack == NULL) {
        free(processor);
        return NULL;
    }

    processor->RAM = RAM_init();
    if (processor->RAM == NULL) {
        free(processor->stack);
        free(processor);
        return NULL;
    }

    return processor;
}

void Proc_delete(struct Processor* processor) {
    free(processor->stack);
    free(processor);
}

// 35 instructions

void processor_0nnn_sys(struct Processor* processor, uint16_t addr) {
    processor->programCounter = addr;
}

void processor_00e0_cls(struct Processor* processor) {
    // TODO
}

void processor_00ee_ret(struct Processor* processor) {
    processor->programCounter = processor->stack[processor->SP-1];
}

void processor_1nnn_jp(struct Processor* processor, uint16_t addr) {
    processor->programCounter = addr;
}

void processor_2nnn_call(struct Processor* processor, uint16_t addr) {
    // TODO
}

void processor_3xkk_se(struct Processor* processor, uint8_t reg, uint8_t val) {
    if (processor->regV[reg] == val) {
        processor->programCounter += 2;
    }
}

void processor_4xkk_sne(struct Processor* processor, uint8_t reg, uint8_t val) {
    if (processor->regV[reg] != val) {
        processor->programCounter += 2;
    }
}

void processor_5xy0_sereg(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    if (processor->regV[reg1] == processor->regV[reg2]) {
        processor->programCounter += 2;
    }
}


void processor_6xkk_ldval(struct Processor* processor, uint8_t reg, uint8_t val) {
    processor->regV[reg] = val;
}

void processor_7xkk_add(struct Processor* processor, uint8_t reg, uint8_t val) {
    processor->regV[reg] = processor->regV[reg] + val;
}

void processor_8xy0_ldreg(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    processor->regV[reg1] = processor->regV[reg2];
}

void processor_8xy1_or(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    // Ou bit à bit, pas logique
    // TODO
    processor->regV[reg1] = processor->regV[reg1] || processor->regV[reg2];
}

void processor_8xy2_and(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    // Et bit à bit, pas logique
    // TODO
    processor->regV[reg1] = processor->regV[reg1] && processor->regV[reg2];

}

void processor_8xy3_xor(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    processor->regV[reg1] = (processor->regV[reg1] || processor->regV[reg2]) &&
            !(processor->regV[reg1] && processor->regV[reg2]);
}

void processor_8xy4_addc(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    if ((int)processor->regV[reg1] + (int)processor->regV[reg2] > 255) {
        // TODO
    } else {
        processor_7xkk_add(processor, reg1, processor->regV[reg2]);
    }
}

void processor_8xy5_sub(struct Processor* processor, uint8_t reg1, uint8_t reg2){
    if ((int)processor->regV[reg1] > (int)processor->regV[reg2]){
        processor->regV[15] = (uint8_t)1;
    }
    else {
        processor->regV[15] = (uint8_t)0;
    }
    processor->regV[reg1] = (uint8_t)((int)processor->regV[reg2] - (int)processor->regV[reg1]);
}

void processor_8xy6_shr(struct Processor* processor, uint8_t reg) {
    if ((int)processor->regV[reg] % 2 == 0){
        processor->regV[15] = (uint8_t)0;
    }
    else{
        processor->regV[15] = (uint8_t)1;
    }
    processor->regV[reg] = (uint8_t)((int)processor->regV[reg]/2)
}

void processor_8xy7_subn(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    // TODO
}

void processor_8xyE_shl(struct Processor* processor, uint8_t reg) {
    if ((int)processor->regV[reg] == 0){
        processor->regV[15] = (uint8_t)0;
    }
    else{
        processor->regV[15] = (uint8_t)1;
    }
    processor->regV[reg] = (uint8_t)((int)processor->regV[reg]*2)
}

void processor_9xy0_sne_reg(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    if (processor->regV[reg1] != processor->regV[reg2]) {
        processor->programCounter += 2;
    }
}

void processor_Annn_ldi(struct Processor* processor, uint16_t addr) {
    processor->I = addr;
}

void processor_Bnnn_jpv0(struct Processor* processor, uint16_t addr) {
    processor->programCounter = addr + processor->regV[0];
}

void processor_Cxkk_rnd(struct Processor* processor, uint8_t reg, uint8_t val) {
    // TODO
    // Utiliser random de C.
}

void processor_Dxyn_drw(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    // TODO pas d'écran
}

void processor_Ex9E_skp(struct Processor* processor, uint8_t reg) {
    // TODO pas de clavier
}

void processor_ExA1_sknp(struct Processor* processor, uint8_t reg) {
    // TODO pas de clavier
}

void processor_Fx07_lddt(struct Processor* processor, uint8_t reg) {
    processor->regV[reg] = processor->DT;
}

void processor_Fx0A_ldvk(struct Processor* processor, uint8_t reg) {
    // TODO pas de clavier
}

void processor_Fx15_lddt2(struct Processor* processor, uint8_t reg) {
    processor->DT = processor->regV[reg];
}

void processor_Fx18ldst(struct Processor* processor, uint8_t reg) {
    processor->ST = processor->regV[reg];
}

void processor_Fx1E_addi(struct Processor* processor, uint8_t reg) {
    processor->I += processor->regV[reg];
}

void processor_Fx29_ldf(struct Processor* processor, uint8_t reg) {
    // TODO pas d'écran
}

void processor_Fx33_ldb(struct Processor* processor, uint8_t reg) {
    // TODO
}

void processor_Fx55_ldw(struct Processor* processor, uint8_t reg) {
    // TODO
}

void processor_Fx65_ldr(struct Processor* processor, uint8_t reg) {
    // TODO
}
