#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>

#define STACK_SIZE 16;

/**
 * \brief La structure représentant la pile du processeur.
 */

/**
 * \struct Stack stack.h "hardware/stack.h"
 * \brief La structure représentant la pile du processeur.
 * La structure contient :
 * * Un pointeur vers un maillon représentant le sommet de la pile.
 * * Un entier représentant la taille de la pile.
 */
struct Stack {
    /**
     * \brief Un pointeur vers un maillon représentant le sommet de la pile.
     */
    struct Maillon* tail;

    /**
     * \brief Un entier représentant la taille de la pile.
     */
    uint8_t size;
};

/**
 * \struct Maillon stack.h "hardware/stack.h"
 * \brief La structure représentant un maillon de la pile.
 * La structure contient :
 * * Un entier représentant la valeur du maillon.
 * * Un pointeur vers le maillon précédent.
 */
struct Maillon {
    /**
     * \brief Un entier représentant la valeur du maillon.
     * La valeur est sur 16 bits.
     */
    uint16_t value;

    /**
     * \brief Un pointeur vers le maillon précédent.
     */
    struct Maillon* prec;
};

/**
 * \relates Maillon
 * \fn struct Maillon* Maillon_init(uint16_t value)
 * \brief Initialise un Maillon, avec comme valeur value
 * @param value  La valeur du maillon
 * @return Un pointeur vers le maillon initialisé
 */
struct Maillon* Maillon_init(uint16_t value);

/**
 * \relates Maillon
 * \fn void Maillon_destroy(struct Maillon* maillon)
 * \brief Détruit un maillon
 * @param maillon Le maillon à détruire
 */
int Maillon_destroy(struct Maillon* maillon);

/**
 * \relates Maillon
 * \fn struct Stack* Stack_init()
 * \brief Initialise une pile
 *
 * @return Un pointeur vers la pile initialisée
 */
struct Stack* Stack_init();

/**
 * \relates Stack
 * \fn int addStack(struct Stack* stack, uint16_t value)
 * \brief Ajoute 'value' à la pile 'stack'
 *
 * @param stack La pile à laquelle on ajoute une valeur
 * @param value La valeur à ajouter
 * @return 0 si l'ajout s'est bien passé, -1 sinon
 */
int addStack(struct Stack* stack, uint16_t value);

/**
 * \relates Stack
 * \fn int removeStack(struct Stack* stack)
 * \brief Retire le dernier élément de la pile 'stack'
 *
 * @param stack La pile à laquelle on retire le dernier élément
 * @return 0 si la suppression s'est bien passée, -1 sinon
 */
int removeStack(struct Stack* stack);

/**
 * \relates Stack
 * \fn void Stack_destroy(struct Stack* stack)
 * \brief Détruit la pile 'stack'
 *
 * @param stack La pile à détruire
 */
void Stack_destroy(struct Stack* stack);




#endif
