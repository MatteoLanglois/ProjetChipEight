#ifndef PROJETCHIPEIGHT_CHIP8_H
#define PROJETCHIPEIGHT_CHIP8_H

void chip8_init();
void chip8_destroy();
void chip8_load(const char* path);

// Viser 500Hz
void chip8_cycle();

void chip8_dec_timers();

// Viser 30/60Hz
void chip8_refresh_screen();

#endif //PROJETCHIPEIGHT_CHIP8_H
