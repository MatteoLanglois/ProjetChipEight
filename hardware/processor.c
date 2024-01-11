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

void Proc_destroy(struct Processor* processor) {
    free(processor->stack);
    free(processor);
}

int puiss(int num, int exp){
    int ans = 1;
    for (int i = 0; i < exp; i ++){
        ans = num * ans;
    }
    return ans;
}

int hexa_to_int(char* instruc) {
    int num = 0;
    int len = strlen(instruc);
    int i = len - 1;
    while (i >= 0) {
        switch (instruc[i]) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
            case '8': case '9':
                num = num + (instruc[i] - '0') * (puiss(16, len - 1 - i));
                break;
            case 'A':
                num = num + (10) * (puiss(16, len - 1 - i));
                break;
            case 'B':
                num = num + (11) * (puiss(16, len - 1 - i));
                break;
            case 'C':
                num = num + (12) * (puiss(16, len - 1 - i));
                break;
            case 'D':
                num = num + (13) * (puiss(16, len - 1 - i));
                break;
            case 'E':
                num = num + (14) * (puiss(16, len - 1 - i));
                break;
            case 'F':
                num = num + (15) * (puiss(16, len - 1 - i));
                break;
        }
        i--;
    }
    return num;
}

// Fetch Decode execute

void processor_fetch_decode_execute(struct Processor* processor) {
    // fetch
    char* instruc = (char *) &processor->RAM->memory[processor->programCounter];
    strcat(instruc, (char *) &processor->RAM->memory[processor->programCounter + 1]);
    // decode & execute
    uint8_t* val;
    uint16_t* addr;
    switch (instruc[0]) {
        case '0':
            processor_instruc_0(processor, instruc);
            break;
        case '1':
            processor_1nnn_jp(processor, hexa_to_int(instruc+1));
            break;
        case '2':
            processor_2nnn_call(processor, hexa_to_int(instruc+1));
            break;
        case '3':
            processor_3xkk_se(processor, (uint8_t)hexa_to_int(&instruc[1]), hexa_to_int(instruc+2));
            break;
        case '4':
            processor_4xkk_sne(processor, (uint8_t)hexa_to_int(&instruc[1]), hexa_to_int(instruc+2));
            break;
        case '5':
            processor_5xy0_sereg(processor, (uint8_t)hexa_to_int(&instruc[1]), (uint8_t)hexa_to_int(&instruc[2]));
            break;
        case '6':
            processor_6xkk_ldval(processor, (uint8_t)hexa_to_int(&instruc[1]), hexa_to_int(instruc+2));
            break;
        case '7':
            processor_7xkk_add(processor, (uint8_t)hexa_to_int(&instruc[1]), hexa_to_int(instruc+2));
            break;
        case '8':
            processor_instruc_8(processor, instruc);
            break;
        case '9':
            processor_9xy0_sne_reg(processor, (uint8_t)hexa_to_int(&instruc[1]), (uint8_t)hexa_to_int(&instruc[2]));
            break;
        case 'A':
            processor_Annn_ldi(processor, hexa_to_int(instruc+1));
            break;
        case 'B':
            processor_Bnnn_jpv0(processor, hexa_to_int(instruc+1));
            break;
        case 'C':
            processor_Cxkk_rnd(processor, (uint8_t)hexa_to_int(&instruc[1]), hexa_to_int(instruc+2));
            break;
        case 'D':
            processor_Dxyn_drw(processor, (uint8_t)hexa_to_int(&instruc[1]), (uint8_t)hexa_to_int(&instruc[2]), (uint8_t)hexa_to_int(&instruc[3]));
            break;
        case 'E':
            processor_instruc_E(processor, instruc);
            break;
        case 'F':
            processor_instruc_F(processor, instruc);
        default:
            processor->programCounter++;
    }
}

void processor_instruc_0(struct Processor* processor, char* instruc) {
    uint8_t* val;
    uint16_t* addr;
    if (strcmp(instruc, "00E0") != 0) {
        processor_00e0_cls(processor);
    } else if (strcmp(instruc, "00EE") != 0) {
        processor_00ee_ret(processor);
    } else {
        processor_0nnn_sys(processor, hexa_to_int(instruc+1));
    }
}

void processor_instruc_8(struct Processor* processor, char* instruc) {
    uint8_t reg1 = hexa_to_int(&instruc[1]);
    uint8_t reg2 = hexa_to_int(&instruc[2]);
    switch (instruc[3]) {
        case '0':
            processor_8xy0_ldreg(processor, reg1, reg2);
            break;
        case '1':
            processor_8xy1_or(processor, reg1, reg2);
            break;
        case '2':
            processor_8xy2_and(processor, reg1, reg2);
            break;
        case '3':
            processor_8xy3_xor(processor, reg1, reg2);
            break;
        case '4':
            processor_8xy4_addc(processor, reg1, reg2);
            break;
        case '5':
            processor_8xy5_sub(processor, reg1, reg2);
            break;
        case '6':
            processor_8xy6_shr(processor, reg1);
            break;
        case '7':
            processor_8xy7_subn(processor, reg1, reg2);
            break;
        case 'E':
            processor_8xyE_shl(processor, reg1);
            break;
    }
}

void processor_instruc_E(struct Processor* processor, char* instruc) {
    if (strcmp(instruc+2, "9E") != 0) {
        processor_Ex9E_skp(processor, (uint8_t)hexa_to_int(&instruc[1]));
    } else {
        processor_ExA1_sknp(processor, (uint8_t)hexa_to_int(&instruc[1]));
    }
}

void processor_instruc_F(struct Processor* processor, char* instruc) {
    uint8_t reg = hexa_to_int(&instruc[1]);
    switch (instruc[3]) {
        case '5':
            if (instruc[2] == '1') {
                processor_Fx15_lddt2(processor, reg);
            }
            else if (instruc[2] == '5'){
                processor_Fx55_ldw(processor, reg);
            }
            else{
                processor_Fx65_ldr(processor, reg);
            }
            break;
        case '7':
            processor_Fx07_lddt(processor, reg);
            break;
        case 'A':
            processor_Fx0A_ldvk(processor, reg);
            break;
        case '8':
            processor_Fx18_ldst(processor, reg);
            break;
        case 'E':
            processor_Fx1E_addi(processor, reg);
            break;
        case '9':
            processor_Fx29_ldf(processor, reg);
            break;
        case '3':
            processor_Fx33_ldb(processor, reg);
            break;
        }
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
