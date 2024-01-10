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

    if (Display_init(processor->display, 0) == 1) {
        free(processor->stack);
        free(processor->RAM);
        free(processor);
        return NULL;
    }

    load_sprite(processor);

    processor->programCounter = 200;

    return processor;
}

void Proc_delete(struct Processor* processor) {
    free(processor->stack);
    free(processor);
}

// Fetch Decode execute

void processor_fetch_decode_execute(struct Processor* processor) {
    // fetch
    uint8_t instruc = processor->RAM->memory[processor->programCounter];

    // decode


    // execute


}


// Load Sprite in memory

void load_sprite(struct Processor* processor) {
    struct Sprite* sprite;
    Sprite_init(sprite, 40);
    int spriteList[16][5] = {{240, 144, 144, 144, 240}, // 0
                             {32, 96, 32, 32, 112}, // 1
                             {240, 16, 240, 128, 240}, // 2
                             {240, 16, 240, 16, 240}, // 3
                             {144, 144, 240, 16, 16}, // 4
                             {240, 128, 240, 16, 240}, // 5
                             {240, 128, 240, 144, 240}, // 6
                             {240, 16, 32, 64, 64}, // 7
                             {240, 144, 240, 144, 240}, // 8
                             {240, 144, 240, 16, 240}, // 9
                             {240, 144, 240, 144, 144}, // A
                             {224, 144, 224, 144, 224}, // B
                             {240, 128, 128, 128, 240}, // C
                             {224, 144, 144, 144, 224}, //D
                             {240, 128, 240, 128, 240}, // E
                             {240, 128, 240, 128, 128}}; // F

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 5; j++) {
            processor->RAM->memory[431 + i * 5 + j] = spriteList[i][j];
        }
    }

}

// 35 instructions

void processor_0nnn_sys(struct Processor* processor, uint16_t addr) {
    processor->programCounter = addr;
}

void processor_00e0_cls(struct Processor* processor) {
    Display_CLS(processor->display);
}

void processor_00ee_ret(struct Processor* processor) {
    processor->programCounter = processor->stack[processor->SP-1];
}

void processor_1nnn_jp(struct Processor* processor, uint16_t addr) {
    processor->programCounter = addr;
}

void processor_2nnn_call(struct Processor* processor, uint16_t addr) {
    processor->SP++;
    processor->stack[processor->SP] = processor->programCounter;
    processor->SP--;
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
    processor->regV[reg1] = processor->regV[reg1] | processor->regV[reg2];
}

void processor_8xy2_and(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    processor->regV[reg1] = processor->regV[reg1] & processor->regV[reg2];

}

void processor_8xy3_xor(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    processor->regV[reg1] = (processor->regV[reg1] | processor->regV[reg2]) &
            !(processor->regV[reg1] & processor->regV[reg2]);
}

void processor_8xy4_addc(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    if ((int)processor->regV[reg1] + (int)processor->regV[reg2] > 255) {
        processor->regV[reg1] = (processor->regV[reg1] + processor->regV[reg2])
                % 255;
        processor->regV[15] = 1;
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
    processor->regV[reg] = (uint8_t)((int)processor->regV[reg]/2);
}

void processor_8xy7_subn(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    if ((int)processor->regV[reg2] > (int)processor->regV[reg1]){
        processor->regV[15] = (uint8_t)1;
    }
    else {
        processor->regV[15] = (uint8_t)0;
    }
    processor->regV[reg1] = (uint8_t)((int)processor->regV[reg1] - (int)processor->regV[reg2]);
}

void processor_8xyE_shl(struct Processor* processor, uint8_t reg) {
    if ((int)processor->regV[reg] == 0){
        processor->regV[15] = (uint8_t)0;
    }
    else{
        processor->regV[15] = (uint8_t)1;
    }
    processor->regV[reg] = (uint8_t)((int)processor->regV[reg]*2);
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
    long int y = random() / 255;
    processor->regV[reg] = y && val;
}

void processor_Dxyn_drw(struct Processor* processor, uint8_t reg1, uint8_t reg2,
        uint8_t nibble) {
    // Init a sprite
    struct Sprite* sprite;
    Sprite_init(sprite, 0);
    // On reconstruit le sprite
    for (uint16_t i = processor->I; i < processor->I + nibble; i++) {
        Sprite_add(sprite, processor->RAM->memory[i]);
    }
    // On appelle la fonction DRW
    Display_DRW(processor->display, sprite,
                processor->regV[reg1], processor->regV[reg2],
                &processor->regV[15]);
}

void processor_Ex9E_skp(struct Processor* processor, uint8_t reg) {
    if (Keyboard_get(processor->keyboard, reg)==1){
        processor->programCounter += 2;
    }
}

void processor_ExA1_sknp(struct Processor* processor, uint8_t reg) {
    if (Keyboard_get(processor->keyboard, reg)==0){
        processor->programCounter += 2;
    }
}

void processor_Fx07_lddt(struct Processor* processor, uint8_t reg) {
    processor->regV[reg] = processor->DT;
}

void processor_Fx0A_ldvk(struct Processor* processor, uint8_t reg) {
    Keyboard_wait(processor->keyboard, &processor->regV[reg]);
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
    processor->I = 431 + reg * 5;
}

void processor_Fx33_ldb(struct Processor* processor, uint8_t reg) {
    int carry = 0;
    for (int i = 0; i < 3; i++) {
        int nb = (processor->regV[reg] - carry) / 10^(3-i);
        processor->RAM->memory[processor->I+i] = nb;
        carry = nb * 10^(3-i);
    }
}

void processor_Fx55_ldw(struct Processor* processor, uint8_t reg) {
    for(int i = 0; i < reg; i++) {
        processor->RAM->memory[processor->I+i] = processor->regV[i];
    }
}

void processor_Fx65_ldr(struct Processor* processor, uint8_t reg) {
    for(int i = 0; i < reg; i++) {
        processor->regV[i] = processor->RAM->memory[processor->I+i];
    }
}
