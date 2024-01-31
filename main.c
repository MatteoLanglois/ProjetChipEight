#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include <dirent.h>
#include <string.h>

char* getChoosedRom();

int main() {
    // On récupère la ROM choisie par l'utilisateur
    char* rom = getChoosedRom();
    // On initialise la machine
    struct chip8* machine = chip8_init(rom);
    // On enlève la ROM de la mémoire
    free(rom);
    // On lance la machine
    chip8_start(machine);
    // On détruit la machine
    chip8_destroy(machine);
}

char* getChoosedRom() {
    struct dirent *dir;
    char* path = "../rom";
    DIR *d = opendir(path);
    if (!d) {
        path = "rom";
        d = opendir(path);
        if (!d) {
            printf("Le dossier n'est pas valide\n");
        }
    }

    char** roms_name = calloc(100,sizeof(char*));
    int index_boucle = 1;
    int choix;
    printf("Liste des ROMs disponibles (avec le numéro correspondant):\n");
    while ((dir = readdir(d)) != NULL) {
        if(!(strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".") == 0)) {
            printf("%d -> %s\n", index_boucle, dir->d_name);
            roms_name[index_boucle-1] = dir->d_name;
            index_boucle++;
        }
    }
    closedir(d);
    do {
        printf("Choisissez une ROM à lancer (avec le numéro correspondant) : ");
        scanf("%d", &choix);
    } while(choix < 1 || choix > index_boucle);

    char* res = malloc(sizeof(char)*100);
    sprintf(res, "%s/%s", path, roms_name[choix-1]);


    return res;
}