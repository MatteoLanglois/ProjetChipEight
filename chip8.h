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

/**
 * \struct chip8 chip8.h "chip8.h"
 * \brief La structure représentant la machine.
 * La structure contient :
 * - Un pointeur vers le processeur.
 * - Un pointeur vers la mémoire vive.
 * - Un pointeur vers l'affichage.
 * - Un pointeur vers le clavier.
 * - Un pointeur vers le haut-parleur.
 */
struct chip8 {
    /**
     * \brief Le processeur du chip8.
     *
     * \relates chip8
     */
    struct Processor* processor;

    /**
     * \brief La mémoire vive du chip8.
     *
     * \relates chip8
     */
    struct RandomAccessMemory* RAM;

    /**
     * \brief L'affichage du chip8.
     *
     * \relates chip8
     */
    struct Display* display;

    /**
     * \brief Le clavier du chip8.
     *
     * \relates chip8
     */
    struct Keyboard* keyboard;

    /**
     * \brief Le haut-parleur du chip8.
     *
     * \relates chip8
     */
    struct Speaker* speaker;
};

/**
 * \relates chip8
 * \fn void chip8_init(const char* path)
 * \brief Fonction d'initialisation de la machine.
 * \param path Le chemin vers la ROM à charger.
 */
void chip8_init(const char* path);

/**
 * \relates chip8
 * \fn void chip8_destroy(struct chip8* chip8)
 * \brief Fonction de destruction de la machine.
 * \param chip8 La machine à détruire.
 */
void chip8_destroy(struct chip8* chip8);

/**
 * \relates chip8
 * \fn int chip8_load(struct chip8* chip8, const char* path)
 * \brief Fonction de chargement de la ROM dans la mémoire vive.
 * \param chip8 La machine.
 * \param path Le chemin vers la ROM à charger.
 * \return 0 si le chargement s'est bien passé, -1 sinon.
 */
int chip8_load(struct chip8* chip8, const char* path);

/**
 * \relates chip8
 * \fn void chip8_cycle(struct chip8* chip8)
 * \brief Fonction de boucle de la machine.
 * \param arg La machine.
 */
void* chip8_cycle(void* arg);

/**
 * \relates chip8
 * \fn void chip8_dec_timers(struct chip8* chip8)
 * \brief Fonction de décrémentation des timers.
 * \param chip8 La machine.
 */
void* chip8_dec_timers(void* arg);

/**
 * \relates chip8
 * \fn void chip8_refresh_screen(struct chip8* chip8)
 * \brief Fonction de rafraîchissement de l'écran.
 * \param chip8 La machine.
 */
void chip8_refresh_screen(struct chip8* chip8);

#endif //PROJETCHIPEIGHT_CHIP8_H
