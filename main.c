#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Unexpected number of arguments.\n");
        fprintf(stderr, "Usage: emulator <ROM.ch8>\n");
    }
    chip8_init(argv[1]);
}
