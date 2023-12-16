#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include <stdlib.h>

#define REGV_SIZE 16;

/**
 * /brief La structure représentant les registres du processeur.
 */

/**
 * \struct Register register.h "hardware/register.h"
 * \brief La structure représentant les registres du processeur.
 * La structure contient :
 * * Un tableau de 16 octets représentant les registres V0 à VF.
 * * Un entier représentant le registre I.
 * * Un entier représentant le registre DT.
 * * Un entier représentant le registre ST.
 * * Un entier représentant le compteur ordinal.
 */
struct Register {
    /**
     * \brief Un tableau de 16 octets représentant les registres V0 à VF.
     */
    uint8_t* regV;

    /**
     * \brief Un entier représentant le registre I.
     */
    uint16_t I;

    /**
     * \brief Un entier représentant le registre DT.
     */
    uint8_t DT;

    /**
     * \brief Un entier représentant le registre ST.
     */
    uint8_t ST;

    /**
     * \brief Un entier représentant le compteur ordinal.
     */
    uint16_t programCounter;
};

/**
 * \relates Register
 * \fn struct Register* Reg_Init()
 * \brief Fonction d'initialisation des registres.
 *
 * @return Un pointeur vers les registres initialisés.
 */
struct Register* Reg_Init();

/**
 * \relates Register
 * \fn void Reg_destroy(struct Register* reg)
 * \brief Fonction de destruction des registres.
 *
 * @param reg Les registres à détruire.
 */
void Reg_destroy(struct Register* reg);

#endif
