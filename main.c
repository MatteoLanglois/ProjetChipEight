#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include <dirent.h>
#include <string.h>

int main() {
    struct dirent *dir;
    DIR *d = opendir("../rom");
    char** roms_name = calloc(100,sizeof(char*));
    if (d) {
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

        char* choosed_rom = roms_name[choix-1];
        char* res = malloc(sizeof(char)*100);
        sprintf(res, "../rom/%s", choosed_rom);
        chip8_init(res);
        free(res);
    } else {
        printf("Le dossier n'est pas valide\n");
    }

    free(roms_name);
}
