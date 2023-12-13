#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>

#define STACK_SIZE 16;

struct Stack {
  struct Maillon* tail;
  uint8_t size;
};

struct Maillon {
  uint16_t value;
  struct Maillon* prec;
};

/**
 * Initialise un Maillon, avec comme valeur value
 * initMaillon(5);
 */

struct Maillon* Maillon_init(uint16_t value);

/**
 * Initialise une Pile de taille 0
 * Stack_init();
 */
struct Stack* Stack_init();

/**
 * Ajoute 'value' à la pile 'stack'
 * addStack(stack,5);
 */

int addStack(struct Stack* stack, uint16_t value);

/**
 * Retire le dernier élement ajouté à la pile 'stack'
 * et le renvoi
 * removeStack(stack); 
 */
int removeStack(struct Stack* stack);

/**
 * Vide la pile 'stack'
 * Stack_destroy(stack);
 */
void Stack_destroy(struct Stack* stack);




#endif
