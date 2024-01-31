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
    // On initialise le dossier
    struct dirent *dir;
    // On choisit le dossier à ouvrir
    char* path = "../rom";
    // On ouvre le dossier
    DIR *d = opendir(path);
    // Si le dossier n'existe pas
    if (!d) {
        // On ouvre un autre dossier
        path = "rom";
        // On ouvre le dossier
        d = opendir(path);
        // Si le dossier n'existe toujours pas
        if (!d) {
            // On affiche un message d'erreur
            printf("Le dossier n'est pas valide\n");
        }
    }

    // On initialise le tableau de noms de ROMs
    char** roms_name = calloc(100,sizeof(char*));
    // On initialise l'index de boucle
    int index_boucle = 1;
    // On initialise le choix de l'utilisateur
    int choix;
    // On affiche les ROMs disponibles
    printf("Liste des ROMs disponibles (avec le numéro correspondant):\n");
    while ((dir = readdir(d)) != NULL) {
        if(!(strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".") == 0)) {
            printf("%d -> %s\n", index_boucle, dir->d_name);
            roms_name[index_boucle-1] = dir->d_name;
            index_boucle++;
        }
    }
    // On ferme le dossier
    closedir(d);
    // On demande à l'utilisateur de choisir une ROM
    do {
        printf("Choisissez une ROM à lancer (avec le numéro correspondant) : ");
        scanf("%d", &choix);
    // Tant que le choix n'est pas valide
    } while(choix < 1 || choix > index_boucle);

    // On initialise le chemin de la ROM
    char* res = malloc(sizeof(char)*100);
    sprintf(res, "%s/%s", path, roms_name[choix-1]);

    // On retourne le chemin de la ROM
    return res;
}