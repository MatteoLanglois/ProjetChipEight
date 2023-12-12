#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned int* memory = malloc(4096 * sizeof(unsigned int));
    free(memory);
}
