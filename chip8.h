#ifndef PROJETCHIPEIGHT_CHIP8_H
#define PROJETCHIPEIGHT_CHIP8_H

#include <unistd.h>
#include <SDL2/SDL.h>
#include "hardware/processor.h"
#include "hardware/memory.h"
#include "libprovided/include/display/display.h"
#include "libprovided/include/display/sprite.h"
#include "libprovided/include/keyboard/keyboard.h"
#include "libprovided/include/misc/error.h"
#include "libprovided/include/speaker/speaker.h"

struct chip8 {
    struct Processor* processor;
    struct RandomAccessMemory* RAM;
    struct Display* display;
    struct Keyboard* keyboard;
    struct Speaker* speaker;
};

void chip8_init();
void chip8_destroy(struct chip8* chip8);
void chip8_load(struct chip8* chip8, const char* path);

// Viser 500Hz
void chip8_cycle(struct chip8* chip8);

void chip8_dec_timers(struct chip8* chip8);

// Viser 30/60Hz
void chip8_refresh_screen(struct chip8* chip8);

#endif //PROJETCHIPEIGHT_CHIP8_H
