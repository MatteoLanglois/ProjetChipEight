#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>
#include <stdlib.h>

#include "stack.h"
#include "register.h"

/**
 * \brief La structure représentant le processeur de la machine.
*/

/**
 * \struct Processor processor.h "hardware/processor.h"
 * \brief La structure représentant le processeur de la machine.
 * La structure contient :
 * * Un pointeur vers une structure Register représentant les registres du
 * processeur.
 * * Un pointeur vers une structure Stack représentant la pile du processeur.
 */
struct Processor {
    /**
     * \brief Un pointeur vers une structure Register représentant les registres
     */
    struct Register* reg;

    /**
     * \brief Un pointeur vers une structure Stack représentant la pile du
     * processeur.
     */
    struct Stack* stack;
};

/**
 * \relates Processor
 * \fn struct Processor* Proc_init()
 * \brief Fonction d'initialisation du processeur.
 *
 * @return Un pointeur vers le processeur initialisé.
 */
struct Processor* Proc_init();

/**
 * \relates Processor
 * \fn void Proc_delete(struct Processor* processor)
 * \brief Fonction de destruction du processeur.
 *
 * @param processor Le processeur à détruire.
 */
void Proc_delete(struct Processor* processor);

/**
 * \relates Processor
 * \fn void ret(struct Processor* processor)
 * \brief Fonction de retour de fonction.
 *
 * @param processor Le processeur.
 */
void ret(struct Processor* processor);

/**
 * \relates Processor
 * \fn void jmp(struct Processor* processor, const uint16_t addr)
 * \brief Fonction de saut inconditionnel.
 *
 * @param processor Le processeur.
 * @param addr L'adresse vers laquelle sauter.
 */
void jmp(struct Processor* processor, uint16_t addr);

/**
 * \relates Processor
 * \fn void call(struct Processor* processor, const uint16_t addr)
 * \brief Fonction d'appel de fonction.
 *
 * @param processor Le processeur.
 * @param addr L'adresse de la fonction à appeler.
 */
void call(struct Processor* processor, uint16_t addr);

#endif