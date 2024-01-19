#include "processor.h"

struct Processor* Proc_init(struct Display* display, struct Keyboard* keyboard,
        struct RandomAccessMemory* RAM, struct Speaker* speaker) {
    struct Processor* processor = malloc(sizeof(struct Processor));
    if (processor == NULL) {
        printf("Erreur lors de l'initialisation du processeur");
        return NULL;
    }
    processor->stack = malloc(16 * 16);
    if (processor->stack == NULL) {
        free(processor);
        printf("Erreur lors de l'initialisation de la pile");
        return NULL;
    }
    for (int i = 0; i < 16; i++) {
        processor->stack[i] = 0;
    }

    processor->RAM = RAM;
    processor->display = display;
    processor->keyboard = keyboard;
    processor->speaker = speaker;

    load_sprite(processor);

    processor->programCounter = 512;
    processor->SP = 0;

    return processor;
}

void Proc_destroy(struct Processor* processor) {
    free(processor->stack);
    free(processor);
}

// Fetch Decode execute

void processor_fetch_decode_execute(struct Processor* processor) {
    // fetch
    // Get the instruction in the memory

    uint8_t part1 = RAM_read(processor->RAM, processor->programCounter);
    uint8_t part2 = RAM_read(processor->RAM, processor->programCounter + 1);
    uint16_t instruction = part1 << 8;
    instruction += part2;
    printf("Commande: %X\n",instruction);
    // Increment the program counter
    processor->programCounter += 2;
    // decode & execute
    if (instruction == 0x00E0) {
        processor_00e0_cls(processor);
    } else if (instruction == 0x00EE) {
        processor_00ee_ret(processor);
    } else if ((instruction & 0xF000) == 0) {
        processor_0nnn_sys(processor, instruction);
    } else if ((instruction & 0xF000) == 0x1000) {
        processor_1nnn_jp(processor, instruction & 0x0FFF);
    } else if ((instruction & 0xF000) == 0x2000) {
        processor_2nnn_call(processor, instruction & 0x0FFF);
    } else if ((instruction & 0xF000) == 0x3000) {
        processor_3xkk_se(processor, (instruction & 0x0F00) >> 8, instruction & 0x00FF);
    } else if ((instruction & 0xF000) == 0x4000) {
        processor_4xkk_sne(processor, (instruction & 0x0F00) >> 8, instruction & 0x00FF);
    } else if ((instruction & 0xF000) == 0x5000) {
        processor_5xy0_sereg(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF000) == 0x6000) {
        processor_6xkk_ldval(processor, (instruction & 0x0F00) >> 8, instruction & 0x00FF);
    } else if ((instruction & 0xF000) == 0x7000) {
        processor_7xkk_add(processor, (instruction & 0x0F00) >> 8, instruction & 0x00FF);
    } else if ((instruction & 0xF00F) == 0x8000) {
        processor_8xy0_ldreg(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8001) {
        processor_8xy1_or(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8002) {
        processor_8xy2_and(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8003) {
        processor_8xy3_xor(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8004) {
        processor_8xy4_addc(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8005) {
        processor_8xy5_sub(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8006) {
        processor_8xy6_shr(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF00F) == 0x8007) {
        processor_8xy7_subn(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x800E) {
        processor_8xyE_shl(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF00F ) == 0x9000) {
        processor_9xy0_sne_reg(processor, (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF000) == 0xA000) {
        processor_Annn_ldi(processor, instruction & 0x0FFF);
    } else if ((instruction & 0xF000) == 0xB000) {
        processor_Bnnn_jpv0(processor, instruction & 0x0FFF);
    } else if ((instruction & 0xF000) == 0xC000) {
        processor_Cxkk_rnd(processor, (instruction & 0x0F00) >> 8,
                           instruction & 0x00FF);
    } else if ((instruction & 0xF000) == 0xD000) {
        processor_Dxyn_drw(processor, (instruction & 0x0F00) >> 8,
                           (instruction & 0x00F0) >> 4, instruction & 0x000F);
    } else if ((instruction & 0xF0FF) == 0xE09E) {
        processor_Ex9E_skp(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xE0A1) {
        processor_ExA1_sknp(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF015) {
        processor_Fx15_lddt2(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF055) {
        processor_Fx55_ldw(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF065) {
        processor_Fx65_ldr(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF007) {
        processor_Fx07_lddt(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF00A) {
        processor_Fx0A_ldvk(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF018) {
        processor_Fx18_ldst(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF01E) {
        processor_Fx1E_addi(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF029) {
        processor_Fx29_ldf(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF033) {
        processor_Fx33_ldb(processor, (instruction & 0x0F00) >> 8);
    } else {
        printf("Erreur lors de l'execution de l'instruction");
        exit(1);
    }
}

// Load Sprite in memory

void load_sprite(struct Processor* processor) {
    int spriteList[16][5] = {{0x00F0, 0x0090, 0x0090, 0x0090, 0x00F0}, // 0
                             {0x0020, 0x0060, 0x0020, 0x0020, 0x0070}, // 1
                             {0x00F0, 0x0010, 0x00F0, 0x0080, 0x00F0}, // 2
                             {0x00F0, 0x0010, 0x00F0, 0x0010, 0x00F0}, // 3
                             {0x0090, 0x0090, 0x00F0, 0x0010, 0x0010}, // 4
                             {0x00F0, 0x0080, 0x00F0, 0x0010, 0x00F0}, // 5
                             {0x00F0, 0x0080, 0x00F0, 0x0090, 0x00F0}, // 6
                             {0x00F0, 0x0010, 0x0020, 0x0040, 0x0040}, // 7
                             {0x00F0, 0x0090, 0x00F0, 0x0090, 0x00F0}, // 8
                             {0x00F0, 0x0090, 0x00F0, 0x0010, 0x00F0}, // 9
                             {0x00F0, 0x0090, 0x00F0, 0x0090, 0x0090}, // A
                             {0x00E0, 0x0090, 0x00E0, 0x0090, 0x00E0}, // B
                             {0x00F0, 0x0080, 0x0080, 0x0080, 0x00F0}, // C
                             {0x00E0, 0x0090, 0x0090, 0x0090, 0x00E0}, //D
                             {0x00F0, 0x0080, 0x00F0, 0x0080, 0x00F0}, // E
                             {0x00F0, 0x0080, 0x00F0, 0x0080, 0x0080}}; // F

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 5; j++) {
            processor->RAM->memory[431 + i * 5 + j] = spriteList[i][j];
        }
    }

}

// 35 instructions

void processor_0nnn_sys(struct Processor* processor, uint16_t addr) {
    if (addr <= 4095 && addr >= 512) {
        processor->programCounter = addr;
    }
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
    printf("%d", processor->SP);
    processor->SP++;
    processor->stack[processor->SP] = processor->programCounter;
    processor->SP--;
    processor->programCounter = addr;
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
    struct Sprite sprite;
    Sprite_init(&sprite, nibble);

    // On reconstruit le sprite
    for (uint16_t i = processor->I; i < processor->I + nibble; i++) {
        if (i >= 4096) {
            printf("Erreur : dépassement de mémoire lors de la lecture du sprite\n");
            Sprite_destroy(&sprite);
            return;
        }
        Sprite_add(&sprite, processor->RAM->memory[i]);
    }

    // On appelle la fonction DRW
    if (Display_DRW(processor->display, &sprite,
                processor->regV[reg1], processor->regV[reg2],
                &processor->regV[15])) {
        printf("Erreur lors de l'affichage du sprite\n");
    }

    Sprite_destroy(&sprite);
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

void processor_Fx18_ldst(struct Processor* processor, uint8_t reg) {
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
