#include "processor.h"

struct Processor* Proc_init(struct Display* display, struct Keyboard* keyboard,
        struct RandomAccessMemory* RAM, struct Speaker* speaker) {
    struct Processor* processor = malloc(sizeof(struct Processor));
    if (processor == NULL) {
        printf("Erreur lors de l'initialisation du processeur");
        return NULL;
    }
    // On initialise la pile
    for (int i = 0; i < 16; i++) {
        processor->stack[i] = 0;
    }
    // On initialise les registres
    for (int i = 0; i < 16; i++) {
        processor->regV[i] = 0;
    }

    // On initialise les pointeurs
    processor->RAM = RAM;
    processor->display = display;
    processor->keyboard = keyboard;
    processor->speaker = speaker;
    // On charge les sprites dans la mémoire
    load_sprite(processor);
    // On initialise le PC, le SP, le I, le ST et le DT
    processor->programCounter = 512;
    processor->SP = 0;
    processor->I = 0;
    processor->ST = 0;
    processor->DT = 0;

    return processor;
}

void Proc_destroy(struct Processor* processor) {
    free(processor);
}

// Fetch Decode execute

int processor_fetch_decode_execute(struct Processor* processor) {
    // fetch
    // Get the instruction in the memory

    uint8_t part1 = RAM_read(processor->RAM, processor->programCounter);
    uint8_t part2 = RAM_read(processor->RAM, processor->programCounter + 1);
    uint16_t instruction = part1 << 8;
    instruction += part2;
    // Increment the program counter
    processor->programCounter += 2;
    // decode & execute
    if (instruction == 0x00E0) {
        errcode = processor_00e0_cls(processor);
    } else if (instruction == 0x00EE) {
        errcode = processor_00ee_ret(processor);
    } else if ((instruction & 0xF000) == 0) {
        errcode = processor_0nnn_sys(processor, instruction);
    } else if ((instruction & 0xF000) == 0x1000) {
        errcode = processor_1nnn_jp(processor, instruction & 0x0FFF);
    } else if ((instruction & 0xF000) == 0x2000) {
        errcode = processor_2nnn_call(processor, instruction & 0x0FFF);
    } else if ((instruction & 0xF000) == 0x3000) {
        errcode = processor_3xkk_se(processor, (instruction & 0x0F00) >> 8,
                          instruction & 0x00FF);
    } else if ((instruction & 0xF000) == 0x4000) {
        errcode = processor_4xkk_sne(processor, (instruction & 0x0F00) >> 8,
                           instruction & 0x00FF);
    } else if ((instruction & 0xF000) == 0x5000) {
        errcode = processor_5xy0_sereg(processor, (instruction & 0x0F00) >> 8,
                             (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF000) == 0x6000) {
        errcode = processor_6xkk_ldval(processor, (instruction & 0x0F00) >> 8,
                             instruction & 0x00FF);
    } else if ((instruction & 0xF000) == 0x7000) {
        errcode = processor_7xkk_add(processor, (instruction & 0x0F00) >> 8,
                           instruction & 0x00FF);
    } else if ((instruction & 0xF00F) == 0x8000) {
        errcode = processor_8xy0_ldreg(processor, (instruction & 0x0F00) >> 8,
                             (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8001) {
        errcode = processor_8xy1_or(processor, (instruction & 0x0F00) >> 8,
                          (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8002) {
        errcode = processor_8xy2_and(processor, (instruction & 0x0F00) >> 8,
                           (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8003) {
        errcode = processor_8xy3_xor(processor, (instruction & 0x0F00) >> 8,
                           (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8004) {
        errcode = processor_8xy4_addc(processor, (instruction & 0x0F00) >> 8,
                            (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8005) {
        errcode = processor_8xy5_sub(processor, (instruction & 0x0F00) >> 8,
                           (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8006) {
        errcode = processor_8xy6_shr(processor, (instruction & 0x0F00) >> 8,
            (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x8007) {
        errcode = processor_8xy7_subn(processor, (instruction & 0x0F00) >> 8,
                            (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F) == 0x800E) {
        errcode = processor_8xyE_shl(processor, (instruction & 0x0F00) >> 8,
            (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF00F ) == 0x9000) {
        errcode = processor_9xy0_sne_reg(processor, (instruction & 0x0F00) >> 8,
                               (instruction & 0x00F0) >> 4);
    } else if ((instruction & 0xF000) == 0xA000) {
        errcode = processor_Annn_ldi(processor, instruction & 0x0FFF);
    } else if ((instruction & 0xF000) == 0xB000) {
        errcode = processor_Bnnn_jpv0(processor, instruction & 0x0FFF);
    } else if ((instruction & 0xF000) == 0xC000) {
        errcode = processor_Cxkk_rnd(processor, (instruction & 0x0F00) >> 8,
                           instruction & 0x00FF);
    } else if ((instruction & 0xF000) == 0xD000) {
        errcode = processor_Dxyn_drw(processor, (instruction & 0x0F00) >> 8,
                           (instruction & 0x00F0) >> 4, instruction & 0x000F);
    } else if ((instruction & 0xF0FF) == 0xE09E) {
        errcode = processor_Ex9E_skp(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xE0A1) {
        errcode = processor_ExA1_sknp(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF015) {
        errcode = processor_Fx15_lddt(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF055) {
        errcode = processor_Fx55_ldw(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF065) {
        errcode = processor_Fx65_ldr(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF007) {
        errcode = processor_Fx07_lddt(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF00A) {
        errcode = processor_Fx0A_ldvk(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF018) {
        errcode = processor_Fx18_ldst(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF01E) {
        errcode = processor_Fx1E_addi(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF029) {
        errcode = processor_Fx29_ldf(processor, (instruction & 0x0F00) >> 8);
    } else if ((instruction & 0xF0FF) == 0xF033) {
        errcode = processor_Fx33_ldb(processor, (instruction & 0x0F00) >> 8);
    } else {
        return OPCODE;
    }
    return 0;
}

// Load Sprite in memory

void load_sprite(struct Processor* processor) {
    // On initialise les sprites
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
    //On place les sprites dans la mémoire
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 5; j++) {
            processor->RAM->memory[431 + i * 5 + j] = spriteList[i][j];
        }
    }

}

// 35 instructions

int processor_0nnn_sys(struct Processor* processor, uint16_t addr) {
    //On vérifie que l'adresse donné est valide avant de la mettre dans le PC
    if (addr <= 4095 && addr >= 512) {
        processor->programCounter = addr;
    } else {
        return RANGE;
    }
    return 0;
}

int processor_00e0_cls(struct Processor* processor) {
    // On appelle la fonction CLS
    if (Display_CLS(processor->display)) {
        return SDL;
    }
    return 0;
}

int processor_00ee_ret(struct Processor* processor) {
    //On vérifie que le SP donné est valide avant de le mettre dans le PC
    if (*processor->stack > 15) {
        return SEGFAULT;
    }
    processor->programCounter = processor->stack[processor->SP];
    processor->SP--;
    return 0;
}

int processor_1nnn_jp(struct Processor* processor, uint16_t addr) {
    //On place simplement l'adresse dans le PC
    processor->programCounter = addr;
    return 0;
}

int processor_2nnn_call(struct Processor* processor, uint16_t addr) {
    //On vérifie que le SP donné est valide avant de le mettre dans le PC
    if (processor->SP > 15) {
        return SEGFAULT;
    }
    //On incrémente le SP, on passe le PC actuel dans la pile, et on met l'adresse dans le PC
    processor->SP++;
    processor->stack[processor->SP] = processor->programCounter;
    processor->programCounter = addr;
    return 0;
}

int processor_3xkk_se(struct Processor* processor, uint8_t reg, uint8_t val) {
    //Si la condition est remplie, on saute la prochaine étape en augmentant le PC de 2
    if (processor->regV[reg] == val) {
        processor->programCounter += 2;
    }
    return 0;
}

int processor_4xkk_sne(struct Processor* processor, uint8_t reg, uint8_t val) {
    //Si la condition n'est pas remplie, on saute la prochaine étape en augmentant le PC de 2
    if (processor->regV[reg] != val) {
        processor->programCounter += 2;
    }
    return 0;
}

int processor_5xy0_sereg(struct Processor* processor, uint8_t reg1,
        uint8_t reg2) {
    //Si les deux valeurs de Vx et Vy sont égales, on saute la prochaine étape en augmentant le PC de 2
    if (processor->regV[reg1] == processor->regV[reg2]) {
        processor->programCounter += 2;
    }
    return 0;
}


int processor_6xkk_ldval(struct Processor* processor, uint8_t reg,
        uint8_t val) {
    //On place la valeur dans le registre (si ce dernier est valide)
    if (reg < 16) {
        processor->regV[reg] = val;
    } else {
        return SEGFAULT;
    }
    return 0;
}

int processor_7xkk_add(struct Processor* processor, uint8_t reg, uint8_t val) {
    //On ajoute la valeur au registre (si ce dernier est valide)
    processor->regV[reg] = processor->regV[reg] + val;
    return 0;
}

int processor_8xy0_ldreg(struct Processor* processor, uint8_t reg1,
        uint8_t reg2) {
    //On place la valeur de Vx dans Vy
    processor->regV[reg1] = processor->regV[reg2];
    return 0;
}

int processor_8xy1_or(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    //On fait un ou logique entre Vx et Vy et on place le résultat dans Vx
    processor->regV[reg1] = processor->regV[reg1] | processor->regV[reg2];
    //On met le flag à 0
    processor->regV[15] = 0;
    return 0;
}

int processor_8xy2_and(struct Processor* processor, uint8_t reg1,
        uint8_t reg2) {
    //On fait un et logique entre Vx et Vy et on place le résultat dans Vx
    processor->regV[reg1] = processor->regV[reg1] & processor->regV[reg2];
    //On met le flag à 0
    processor->regV[15] = 0;
    return 0;
}

int processor_8xy3_xor(struct Processor* processor, uint8_t reg1,
        uint8_t reg2) {
    //On fait un ou exclusif entre Vx et Vy et on place le résultat dans Vx
    processor->regV[reg1] = processor->regV[reg1] ^ processor->regV[reg2];
    processor->regV[15] = 0;
    return 0;
}

int processor_8xy4_addc(struct Processor* processor, uint8_t reg1,
        uint8_t reg2) {
    //Si la somme dépasse 255, on doit mettre le flag à 1, pour la retenue
    if ((int)processor->regV[reg1] + (int)processor->regV[reg2] > 255) {
        processor->regV[reg1] = processor->regV[reg1] + processor->regV[reg2];
        processor->regV[15] = 1;
    } else {
        processor->regV[reg1] += processor->regV[reg2];
        processor->regV[15] = 0;
    }
    return 0;
}

int processor_8xy5_sub(struct Processor* processor, uint8_t reg1, uint8_t reg2){
    //Si Vx est plus grand que Vy, on met le flag à 1, pour indiquer la retenue
    if (processor->regV[reg1] >= processor->regV[reg2]){
        processor->regV[reg1] -= processor->regV[reg2];
        processor->regV[15] = 1;
    } else {
        processor->regV[reg1] -= processor->regV[reg2];
        processor->regV[15] = 0;
    }
    return 0;
}

int processor_8xy6_shr(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    //On place la valeur de Vx dans Vy si Vy est valide
    if (reg2 <= 15) {
        processor->regV[reg1] = processor->regV[reg2];
    }
    // Si Vx est pair, le flag vaudra 0, sinon 1
    uint8_t temp = processor->regV[reg1] % 2;
    processor->regV[reg1] /= 2;
    processor->regV[15] = temp;
    return 0;
}

int processor_8xy7_subn(struct Processor* processor, uint8_t reg1,
        uint8_t reg2) {
    // Si Vy est plus grand que Vx, on met le flag à 1, pour indiquer la retenue
    if (processor->regV[reg2] >= processor->regV[reg1]){
        processor->regV[reg1] = processor->regV[reg2] - processor->regV[reg1];
        processor->regV[15] = 1;
    } else {
        processor->regV[reg1] = processor->regV[reg2] - processor->regV[reg1];
        processor->regV[15] = 0;
    }
    return 0;
}

int processor_8xyE_shl(struct Processor* processor, uint8_t reg1, uint8_t reg2) {
    //On place la valeur de Vx dans Vy si Vy est valide
    if (reg2 <= 15) {
        processor->regV[reg1] = processor->regV[reg2];
    }
    // Si Vx est plus grand que 128, le flag vaudra 1, sinon 0
    uint8_t temp = processor->regV[reg1] / 128;
    processor->regV[reg1] *= 2;
    processor->regV[15] = temp;
    return 0;
}

int processor_9xy0_sne_reg(struct Processor* processor, uint8_t reg1,
        uint8_t reg2) {
    // Si Vx est différent de Vy, alors on saute la prochaine étape
    if (processor->regV[reg1] != processor->regV[reg2]) {
        processor->programCounter += 2;
    }
    return 0;
}

int processor_Annn_ldi(struct Processor* processor, uint16_t addr) {
    //On place l'adresse dans le registre I
    processor->I = addr;
    return 0;
}

int processor_Bnnn_jpv0(struct Processor* processor, uint16_t addr) {
    //On place l'adresse dans le PC, en lui ajoutant V0
    processor->programCounter = addr + processor->regV[0];
    return 0;
}

int processor_Cxkk_rnd(struct Processor* processor, uint8_t reg, uint8_t val) {
    // On génère un nombre aléatoire entre 0 et val
    long int y = random() / 255;
    processor->regV[reg] = y && val;
    return 0;
}

int processor_Dxyn_drw(struct Processor* processor, uint8_t reg1, uint8_t reg2,
        uint8_t nibble) {
    // Init a sprite
    struct Sprite sprite;
    if (Sprite_init(&sprite, nibble)) {
        return MALLOC;
    }

    // On reconstruit le sprite
    for (uint16_t i = processor->I; i < processor->I + nibble; i++) {
        if (i >= 4096) {
            printf("Erreur : dépassement de mémoire lors de la lecture du "
                   "sprite\n");
            Sprite_destroy(&sprite);
            return SEGFAULT;
        }
        Sprite_add(&sprite, processor->RAM->memory[i]);
    }

    // On appelle la fonction DRW
    if (Display_DRW(processor->display, &sprite,
                processor->regV[reg1], processor->regV[reg2],
                &processor->regV[15])) {
        return SDL;
    }

    Sprite_destroy(&sprite);
    return 0;
}

int processor_Ex9E_skp(struct Processor* processor, uint8_t reg) {
    // Si la clé demandé est appuyé, alors on saute la prochain
    if (Keyboard_get(processor->keyboard, processor->regV[reg]) == KEY_DOWN){
        processor->programCounter += 2;
    }
    return 0;
}

int processor_ExA1_sknp(struct Processor* processor, uint8_t reg) {
// Si la clé demandé n'est pas appuyé, alors on saute la prochain
    if (Keyboard_get(processor->keyboard, processor->regV[reg]) == KEY_UP){
        processor->programCounter += 2;
    }
    return 0;
}

int processor_Fx07_lddt(struct Processor* processor, uint8_t reg) {
    // On place la valeur du DT dans le registre
    processor->regV[reg] = processor->DT;
    return 0;
}

int processor_Fx0A_ldvk(struct Processor* processor, uint8_t reg) {
    // On attend que l'utilisateur appuie sur la touche correspondante au registre
    Keyboard_wait(processor->keyboard, &processor->regV[reg]);
    return 0;
}

int processor_Fx15_lddt(struct Processor* processor, uint8_t reg) {
    // On place la valeur du registre dans le DT
    processor->DT = processor->regV[reg];
    return 0;
}

int processor_Fx18_ldst(struct Processor* processor, uint8_t reg) {
    // On place la valeur du registre dans le ST
    processor->ST = processor->regV[reg];
    return 0;
}

int processor_Fx1E_addi(struct Processor* processor, uint8_t reg) {
    // On ajoute la valeur du registre à l'adresse I
    processor->I += processor->regV[reg];
    return 0;
}

int processor_Fx29_ldf(struct Processor* processor, uint8_t reg) {
    // On place l'adresse du sprite dans le registre I
    processor->I = 431 + reg * 5;
    return 0;
}

int processor_Fx33_ldb(struct Processor* processor, uint8_t reg) {
    // On récupère la valeur du registre
    uint8_t value = processor->regV[reg];
    // On stocke les chiffres dans la mémoire à partir de l'emplacement I
    processor->RAM->memory[processor->I] = value / 100;
    processor->RAM->memory[processor->I + 1] = (value / 10) % 10;
    processor->RAM->memory[processor->I + 2] = value % 10;
    return 0;
}

int processor_Fx55_ldw(struct Processor* processor, uint8_t reg) {
    // On stocke les valeurs des registres dans la mémoire à partir de l'emplacement I
    for(int i = 0; i <= reg; i++) {
        RAM_write(processor->RAM, processor->I+i, processor->regV[i]);
    }
    processor->I++;
    return 0;
}

int processor_Fx65_ldr(struct Processor* processor, uint8_t reg) {
    // On récupère les valeurs de la mémoire à partir de l'emplacement I
    for(int i = 0; i <= reg; i++) {
        processor->regV[i] = RAM_read(processor->RAM, processor->I + i);
    }
    processor->I++;
    return 0;
}
