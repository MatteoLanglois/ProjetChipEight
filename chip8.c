#include "chip8.h"

void chip8_init(const char* path) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
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
            free(chip8->RAM);
            RAM_destroy(chip8->RAM);
            exit(1);
        }

        chip8->keyboard = malloc(sizeof(struct Keyboard));
        if (Keyboard_init(chip8->keyboard) == 1) {
            printf("Erreur lors de l'initialisation du clavier");
            free(chip8->display);
            RAM_destroy(chip8->RAM);
            exit(1);
        }

        chip8->speaker = malloc(sizeof(struct Speaker));
        if (Speaker_init(chip8->speaker) == 1) {
            printf("Erreur lors de l'initialisation du haut-parleur");
            free(chip8->display);
            free(chip8->keyboard);
            RAM_destroy(chip8->RAM);
            exit(1);
        }

        chip8->processor = Proc_init(chip8->display, chip8->keyboard, chip8->RAM,
                                     chip8->speaker);
        if (chip8->processor == NULL) {
            printf("Erreur lors de l'initialisation du processeur");
            free(chip8->display);
            free(chip8->keyboard);
            free(chip8->speaker);
            Proc_destroy(chip8->processor);
            RAM_destroy(chip8->RAM);
            exit(1);
        }


        chip8_load(chip8, path);
        chip8_cycle(chip8);
    }
    if (errcode != QUIT) {
            fprintf(stderr,"The program has terminated abnormally (errcode=%s)\n",errorstr());
            exit(1);
    }
}

void chip8_destroy(struct chip8* chip8) {
    Proc_destroy(chip8->processor);
    RAM_destroy(chip8->RAM);
    Display_destroy(chip8->display);
    Keyboard_destroy(chip8->keyboard);
    Speaker_destroy(chip8->speaker);
    free(chip8);
    SDL_Quit();
}

void chip8_load(struct chip8* chip8, const char* path) {
    // Ouvrir le fichier
    FILE* rom = fopen(path, "r");
    if (rom == NULL) {
        printf("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    // Lire le fichier
    uint16_t address = 511;
    uint8_t* buffer = malloc(sizeof(uint8_t));
    while (fread(buffer, 1, 1, rom) == 1) {
        char* pass;
        sprintf(pass, "%02X", *buffer);
        RAM_write(chip8->RAM, address, (uint8_t) strtol(pass, NULL, 16));
        address++;
    }
    free(buffer);

    // Fermer le fichier
    fclose(rom);
}

// Viser 500Hz
void chip8_cycle(struct chip8* chip8) {
    uint running = 1;
    int cpt = 0;
    Display_CLS(chip8->display);
    while (running) {
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