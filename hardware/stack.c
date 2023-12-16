//
// Created by mlanglois et moi on 13/12/23.
//

#include "stack.h"


struct Maillon* Maillon_init(uint16_t value) {
    struct Maillon* maillon = malloc(sizeof(struct Maillon));
    if (maillon){
      maillon->prec = NULL;
      maillon->value = value;
      return maillon;
    }
    else {
      return NULL;
    }
}

struct Stack* Stack_init(){
    struct Stack* stack = malloc(sizeof(struct Stack));
    if (stack){
      stack->tail = NULL;
      stack->size = 0;
      return stack;
    }
    else{
      return NULL;
    }
}

int addStack(struct Stack* stack, uint16_t value){
    struct Maillon* new = Maillon_init(value);
    if (new){
      if (stack->size < STACK_SIZE){
        new->prec = stack->tail;
        stack->tail = new;
        stack->size++;
        return 0;
      }
      else{
        return 2;
      }
    }
    else {
      return 1;
    }
}

int removeStack(struct Stack* stack){
  if (stack->tail == NULL){
    free(stack);
    return -1;
  }
  uint16_t res = stack->tail->value;
  struct Maillon* remove = stack->tail;
  stack->tail = stack->tail->prec;
  stack->size--;
  free(remove);
  return res;
}

void Stack_destroy(struct Stack* stack){
  while(removeStack(stack) != -1){
  }
  return;
}

int Maillon_destroy(struct Maillon *maillon) {
    if (maillon == NULL) {
        return -1;
    }
    free(maillon);
    return 0;
}
