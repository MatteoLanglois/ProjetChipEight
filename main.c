#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include <dirent.h>
#include <string.h>

int main() {
    struct dirent *dir;
    DIR *d = opendir("../rom");
    int i = 1;
    char** roms_name = calloc(100,sizeof(char*));
    if (d)
    {
        printf("Liste des ROMs disponibles (avec le numéro correspondant):\n");
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".") == 0)
                continue;
            else {
                printf("%d -> %s\n", i, dir->d_name);
                roms_name[i-1] = dir->d_name;
                i++;
            }
        }
        closedir(d);
    }
    int choix;
    do {
        printf("Choisissez une ROM à lancer (avec le numéro correspondant) : ");
        scanf("%d", &choix);
    } while(choix < 1 || choix > i);

    char* test = roms_name[choix-1];
    char* res = malloc(sizeof(char)*100);
    sprintf(res, "../rom/%s", test);
    free(roms_name);
    chip8_init(res);
    free(res);
}
