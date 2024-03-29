#include "chip8.h"
#include <pthread.h>

struct chip8* chip8_init(const char* path) {
    struct chip8* chip8 = NULL;
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        errcode = SDL;
    } else {
        chip8 = malloc(sizeof(struct chip8));
        if (chip8 == NULL) {
            printf("Erreur lors de l'initialisation de la machine");
            errcode = MALLOC;
            exit(1);
        }

        chip8->RAM = RAM_init();
        if (chip8->RAM == NULL) {
            printf("Erreur lors de l'initialisation de la RAM");
            free(chip8);
            exit(1);
        }

        chip8->display = malloc(sizeof(struct Display));
        if (Display_init(chip8->display, 20) == 1) {
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


        if (chip8_load(chip8, path) != 0) {
            printf("Erreur lors de la lecture de la ROM");
            free(chip8->display);
            free(chip8->keyboard);
            free(chip8->speaker);
            Proc_destroy(chip8->processor);
            RAM_destroy(chip8->RAM);
            exit(1);
        }
    }

    return chip8;
}

void chip8_start(struct chip8* chip8) {
    pthread_t pthread_cycle, pthread_timer;

    pthread_create(&pthread_cycle, NULL, chip8_cycle, chip8);
    pthread_create(&pthread_timer, NULL, chip8_dec_timers, chip8);

    pthread_exit(&pthread_timer);
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

int chip8_load(struct chip8* chip8, const char* path) {
    // Ouvrir le fichier
    FILE* rom = fopen(path, "r");
    if (rom == NULL) {
        printf("Erreur lors de l'ouverture du fichier");
        return PERM;
    }

    // Lire le fichier
    uint16_t address = 512;
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
    return 0;
}

// Viser 500Hz
void* chip8_cycle(void* arg) {
    struct chip8* chip8 = arg;
    SDL_Event event;
    int cpt = 0;
    // Mainloop
    while (errcode != QUIT) {
        // Appel du fetch decode execute
        processor_fetch_decode_execute(chip8->processor);
        // Rafraichissement de l'écran à un taux d'environ 60hz (500/8)
        if (cpt == 8) {
            chip8_refresh_screen(chip8);
            cpt = 0;
        }
        // S'il y a un evenement de fermeture de fenetre
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // On quitte la boucle
                errcode = QUIT;
            }
        }

        // Gestion du son
        if (chip8->processor->ST > 0) {
            Speaker_on(chip8->speaker);
        } else {
            Speaker_off(chip8->speaker);
        }

        // Gestion des erreurs
        if (errcode != QUIT && errcode != 0) {
            fprintf(stderr,"Erreur : %d\n", errcode);
        }
        // On attend 2ms pour être à 500Hz
        SDL_Delay(2);
        // On incrémente le compteur
        cpt++;
    }
    return NULL;
}

void* chip8_dec_timers(void* arg) {
    // On récupère la machine
    struct chip8* chip8 = arg;
    int cpt = 0;
    // Tant qu'on ne quitte pas
    while (errcode != QUIT) {
        // On décrémente les timers à un taux d'environ 60hz (500/8)
        if (cpt == 8 && chip8 != NULL && chip8->processor != NULL) {
            cpt = 0;
            if (chip8->processor->ST > 0) {
                chip8->processor->ST--;
            }
            if (chip8->processor->DT > 0) {
                chip8->processor->DT--;
            }
        }
        // On incrémente le compteur
        cpt++;
        // On attend 2ms pour être à 500Hz
        SDL_Delay(2);
    }
    return NULL;
}


void chip8_refresh_screen(struct chip8* chip8) {
    // On rafraichit l'écran
    Display_update(chip8->display);
}