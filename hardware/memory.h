#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RAM_max 4096

/**
 * \brief La structure représentant la mémoire vive de la machine.
 */

/**
 * \struct RandomAccessMemory memory.h "hardware/memory.h"
 * \brief La structure représentant la mémoire vive de la machine.
 * La structure contient :
 * * Un tableau de 4096 octets représentant la mémoire vive de la machine.
 */
struct RandomAccessMemory {
    /**
     * \brief Un tableau de 4096 octets représentant la mémoire vive de la
     * machine.
     */
    uint8_t* memory;
};

/**
 * \relates RandomAccessMemory
 * \fn struct RandomAccessMemory* RAM_init()
 * \brief Fonction d'initialisation de la mémoire vive.
 *
 * @return Un pointeur vers la mémoire vive initialisée.
 */
struct RandomAccessMemory* RAM_init();

/**
 * \relates RandomAccessMemory
 * \fn unsigned int RAM_read(const unsigned int* memory, uint16_t addr)
 * \brief Fonction de lecture de la mémoire vive.
 *
 * @param memory La mémoire vive.
 * @param addr L'adresse à laquelle on veut lire.
 * @return La valeur contenue à l'adresse addr.
 */
unsigned int RAM_read(const unsigned int* memory, uint16_t addr);

/**
 * \relates RandomAccessMemory
 * \fn int RAM_write(struct RandomAccessMemory* memory, uint16_t addr, const uint8_t newVal)
 * \brief Fonction d'écriture dans la mémoire vive.
 *
 * @param memory La mémoire vive.
 * @param addr L'adresse à laquelle on veut écrire.
 * @param newVal La valeur à écrire.
 * @return 0 si l'écriture s'est bien passée, -1 sinon.
 */
int RAM_write(struct RandomAccessMemory* memory, uint16_t addr, const uint8_t newVal);

/**
 * \relates RandomAccessMemory
 * \fn int RAM_destroy(struct RandomAccessMemory* memory)
 * \brief Fonction de destruction de la mémoire vive.
 *
 * @param memory La mémoire vive.
 * @return 0 si la destruction s'est bien passée, -1 sinon.
 */
int RAM_destroy(struct RandomAccessMemory* memory);

#endif