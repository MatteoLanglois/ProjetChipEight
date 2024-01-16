#include "chip8.h"

void chip8_init() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO)) {
            errcode = SDL;
    } else {
        struct chip8* chip8 = malloc(sizeof(struct chip8));

        chip8->RAM = RAM_init();
        if (chip8->RAM == NULL) {
            printf("Erreur lors de l'initialisation de la RAM");
            exit(1);
        }

        chip8->processor->RAM = chip8->RAM;

        chip8->display = malloc(sizeof(struct Display));
        if (Display_init(chip8->display, 10) == 1) {
            printf("Erreur lors de l'initialisation de l'affichage");
            exit(1);
        }

        chip8->keyboard = malloc(sizeof(struct Keyboard));
        if (Keyboard_init(chip8->keyboard) == 1) {
            printf("Erreur lors de l'initialisation du clavier");
            exit(1);
        }

        chip8->speaker = malloc(sizeof(struct Speaker));
        if (Speaker_init(chip8->speaker) == 1) {
            printf("Erreur lors de l'initialisation du haut-parleur");
            exit(1);
        }
        chip8_load(chip8, "/home/mlanglois/Documents/ProjetChipEight/rom/Airplane.ch8");
        chip8_cycle(chip8);

        chip8->processor = Proc_init(chip8->display, chip8->keyboard, chip8->RAM,
                                     chip8->speaker);
        if (chip8->processor == NULL) {
            printf("Erreur lors de l'initialisation du processeur");
            exit(1);
        }
    }
}

void chip8_destroy(struct chip8* chip8) {
    Proc_destroy(chip8->processor);
    RAM_destroy(chip8->RAM);
    Display_destroy(chip8->display);
    Keyboard_destroy(chip8->keyboard);
    Speaker_destroy(chip8->speaker);
    free(chip8);
}

void chip8_load(struct chip8* chip8, const char* path) {
    // Ouvrir le fichier
    FILE* rom = fopen(path, "r");
    if (rom == NULL) {
        printf("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    // Lire le fichier
    uint16_t address = 512;
    char* buffer = malloc(sizeof(char) * 2);
    while (fread(buffer, 2, 1, rom) == 1) {
        RAM_write(chip8->RAM, address, strtol(buffer, NULL, 16));
        address += 2;
    }
    free(buffer);

    // Fermer le fichier
    fclose(rom);

}

// Viser 500Hz
void chip8_cycle(struct chip8* chip8) {
    int cpt = 0;
    while (1) {
        processor_fetch_decode_execute(chip8->processor);
        if (cpt == 8) {
            chip8_refresh_screen(chip8);
            chip8_dec_timers(chip8);
            cpt = 0;
        }
        SDL_Delay(2);
        cpt++;
    }
    chip8_destroy(chip8);
}

void chip8_dec_timers(struct chip8* chip8) {

}

// Viser 30/60Hz
void chip8_refresh_screen(struct chip8* chip8) {
    Display_update(chip8->display);
}