#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "../libprovided/include/display/display.h"
#include "../libprovided/include/display/sprite.h"
#include "../libprovided/include/keyboard/keyboard.h"
#include "../libprovided/include/misc/error.h"
#include "../libprovided/include/speaker/speaker.h"

/**
 * \brief La structure représentant le processeur de la machine.
*/

/**
 * \struct Processor processor.h "processor.h"
 * \brief La structure représentant le processeur de la machine.
 * La structure contient :
 * * Un pointeur vers une structure Register représentant les registres du
 * processeur.
 * * Un pointeur vers une structure Stack représentant la pile du processeur.
 */
struct Processor {
    /**
     * \brief Un tableau de 16 octets représentant les registres V0 à VF.
     */
    uint8_t regV[16];

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

    /**
     * \brief Un tableau de 16x16 octets représentant la pile du processeur.
     */
    uint16_t stack[16];

    /**
     * \brief Un entier représentant le pointeur de pile. (la position du sommet
     * de la pile)
     */
    int SP;

    /**
     * \brief Un pointeur vers une structure RandomAccessMemory représentant la
     * mémoire vive de la machine.
     */
    struct RandomAccessMemory* RAM;

    /**
     * \brief un pointeur vers la structure display représentant l'écran de la
     * machine
     */
    struct Display* display;

    /**
     * \brief un pointeur vers la structure keyboard représentant le clavier de la
     * machine
     */
    struct Keyboard* keyboard;

    /**
     * \brief un pointeur vers la structure speaker représentant le
     * haut-parleur de la machine
     */
    struct Speaker* speaker;
};

// Gestion du processeur

/**
 * \relates Processor
 * \fn struct Processor* Proc_init()
 * \brief Fonction d'initialisation du processeur.
 *
 * @param display L'écran de la machine.
 * @param keyboard Le clavier de la machine.
 * @param RAM La mémoire vive de la machine.
 * @param speaker Le haut-parleur de la machine.
 * @return Un pointeur vers le processeur initialisé.
 */
struct Processor* Proc_init(struct Display* display, struct Keyboard* keyboard,
        struct RandomAccessMemory* RAM, struct Speaker* speaker);

/**
 * \relates Processor
 * \fn void Proc_destroy(struct Processor* processor)
 * \brief Fonction de destruction du processeur.
 *
 * @param processor Le processeur à détruire.
 */
void Proc_destroy(struct Processor* processor);

// FetchDecodeExecute

/**
 * \relates Processor
 * \fn void processor_fetch_decode_execute()
 * \brief Fonction réalisant une itération de fetch decode execute
 *
 * @param processor Le processeur.
 */
int processor_fetch_decode_execute(struct Processor* processor);

// Load sprite in memory
void load_sprite(struct Processor* processor);


// Les 35 instructions de chip 8

/**
 * \relates Processor
 * \fn void processor_0nnn_sys(struct Processor* processor, const uint16_t addr)
 * \brief Saute à l'adresse nnn.
 *
 * @param processor Le processeur
 * @param addr L'adresse vers laquelle sauter.
 */
int processor_0nnn_sys(struct Processor* processor, uint16_t addr);

/**
 * \relates Processor
 * \fn void processor_00e0_cls(struct Processor* processor)
 * \brief Vide l'affichage
 *
 * @param processor Le processeur.
 */
int processor_00e0_cls(struct Processor* processor);

/**
 * \relates Processor
 * \fn void processor_00ee_ret(struct Processor* processor)
 * \brief Fonction de retour de fonction.
 *
 * @param processor Le processeur.
 */
int processor_00ee_ret(struct Processor* processor);

/**
 * \relates Processor
 * \fn void jmp(struct Processor* processor, const uint16_t addr)
 * \brief Fonction de saut inconditionnel.
 *
 * @param processor Le processeur.
 * @param addr L'adresse vers laquelle sauter.
 */
int processor_1nnn_jp(struct Processor* processor, uint16_t addr);

/**
 * \relates Processor
 * \fn void processor_2nnn_call(struct Processor* processor, const uint16_t addr)
 * \brief Fonction d'appel de fonction.
 *
 * L'interpréteur augmente le pointeur de pîle, puis met la valeur du compteur
 * ordinal dans le sommet de la pile. Ensuite, le compteur de pile est
 * décrémenté.
 *
 * @param processor Le processeur.
 * @param addr L'adresse de la fonction à appeler.
 */
int processor_2nnn_call(struct Processor* processor, uint16_t addr);

/**
 * \relates Processor
 * \fn void processor_3xkk_se(struct Processor* processor, uint8_t reg, uint8_t val)
 * \brief Fonction de saut si égalité.
 *
 * L'interpréteur compare le registre reg à val, et si ils sont égaux, ajoute 2 au
 * compteur ordinal pour passer à l'instruction suivante.
 *
 * @param processor Le processeur.
 * @param reg Le registre à comparer.
 * @param val La valeur à comparer.
 */
int processor_3xkk_se(struct Processor* processor, uint8_t reg, uint8_t val);

/**
 * \relates Processor
 * \fn void processor_3xkk_se(struct Processor* processor, const uint8_t reg, const uint8_t val)
 * \brief Passe la prochaine instruction si reg != val.
 *
 * L'interpréteur compare le registre reg à val, et si ils sont égaux, ajoute 2 au
 * compteur ordinal pour passer à l'instruction suivante.
 *
 * @param processor Le processeur.
 * @param reg Le registre à comparer.
 * @param val La valeur à comparer.
 */
int processor_4xkk_sne(struct Processor* processor, uint8_t reg, uint8_t val);

/**
 * \relates Processor
 * \fn void processor_5xy0_sereg(struct Processor* processor, uint8_t reg1, uint8_t reg2)
 * \brief Passe la prochaine instruction si reg1 = reg2.
 * 
 * L'interpréteur compare reg1 et reg2, s'ils sont égaux, incrémente le compteur
 * ordinal par 2.
 * 
 * @param processor 
 * @param reg1 
 * @param reg2 
 */
int processor_5xy0_sereg(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_6xkk_ldval(struct Processor* processor, uint8_t reg, uint8_t val)
 * \brief Met reg = val.
 *
 * L'interpréteur met la valeur reg dans le registre val.
 *
 * @param processor Le processeur.
 * @param reg1 Le premier registre à comparer.
 * @param reg2 Le deuxième registre à comparer.
 */
int processor_6xkk_ldval(struct Processor* processor, uint8_t reg, uint8_t val);

/**
 * \relates Processor
 * \fn void processor_7xkk_add(struct Processor* processor, uint8_t reg, uint8_t val);
 * \brief Met Vx = Vx + kk.
 *
 * L'interpréteur ajoute la valeur val à la valeur du registre reg, puis la met
 * dans ce registre
 *
 * @param processor
 * @param reg
 * @param val
 */
int processor_7xkk_add(struct Processor* processor, uint8_t reg, uint8_t val);

/**
 * \relates Processor
 * \fn void processor_8xy0_ldreg(struct Processor* processor, uint8_t reg1, uint8_t reg2)
 * \brief Met reg1 = reg2.
 *
 * Stocke la valeur du registre reg2 dans le registre reg1.
 *
 * @param processor Le processeur.
 * @param reg1 Le registre dont on remplace la valeur.
 * @param reg2 Le deuxième registre dont on prend la valeur.
 */
int processor_8xy0_ldreg(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_8xy1_or(struct Processor* processor, uint8_t reg1, uint8_t reg2)
 * \brief Met reg1 = reg1 OU reg2.
 *
 * Réalise une action OU bit à bit de reg1 et reg2, puis stocke le résultat dans
 * reg1. Une comparaison bit à bit OU compare les bits correspondants des deux
 * valeurs, et si l'un des bits est 1, le bit correspondant de la valeur est
 * aussi 1, sinon c'est 0.
 *
 * @param processor Le processeur.
 * @param reg1 Le premier registre à comparer.
 * @param reg2 Le deuxième registre à comparer.
 */
int processor_8xy1_or(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_8xy2_and(struct Processor* processor, uint8_t reg1, uint8_t reg2);
 * \brief Set Vx = Vx AND Vy.
 *
 * Réalise une action ET bit à bit de reg1 et reg2, puis stocke le résultat dans
 * reg1. Une comparaison bit à bit ET compare les bits correspondants des deux
 * valeurs, et si les deux sont 1, le bit correspondant de la valeur est aussi
 * 1, sinon c'est 0
 *
 * @param processor Le processeur.
 * @param reg1 Le premier registre à comparer.
 * @param reg2 Le deuxième registre à comparer.
 */
int processor_8xy2_and(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_8xy3_xor(struct Processor* processor, uint8_t reg1, uint8_t reg2)
 * \brief Met Vx = Vx XOR Vy.
 *
 * Réalise une action OU exclusif bit à bit de reg1 et reg2, puis stocke le
 * résultat dans reg1. Une comparaison bit à bit OU exclusif compare les bits
 * correspondants des deux valeurs, et si les deux sont différents, le bit
 * correspondant de la valeur est 1, sinon c'est 0.
 *
 * @param processor Le processeur.
 * @param reg1 Le premier registre à comparer.
 * @param reg2 Le deuxième registre à comparer.
 */
int processor_8xy3_xor(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_8xy4_addc(struct Processor* processor, uint8_t reg1, uint8_t reg2)
 * \brief Set Vx = Vx + Vy, set VF = carry.
 *
 * Ajoute Vx et Vy ensemble. Si le résultat est est supérieur à 255, ou 8 bits,
 * alors VF est mit à 1, sinon à 0. Les 8 premiers bits du résultat sont
 * stockés dans Vx.
 *
 * @param processor
 * @param reg1
 * @param reg2
 */
int processor_8xy4_addc(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_8xy5_sub(struct Processor* processor, uint8_t reg1, uint8_t reg2);
 * \brief Set Vx = Vx - Vy, set VF = NOT borrow.
 *
 * Si Vx > Vy, alors VF est mis à 1, sinon 0. Ensuite, Vy est soustrait de
 * Vx, et le résultat est mis dans Vx
 *
 * @param processor
 * @param reg1
 * @param reg2
 */
int processor_8xy5_sub(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_8xy6_shr(struct Processor* processor, uint8_t reg);
 * \brief Set Vx = Vx SHR 1.
 *
 * Si le bit de poids faible de reg est 1, alors Vf est mis à 1, sinon 0.
 * Ensuite reg est divisé par 2 (shift right).
 *
 * @param processor
 * @param reg1
 * @param reg2
 */
int processor_8xy6_shr(struct Processor* processor, uint8_t reg1, uint8_t reg2);


/**
 * \relates Processor
 * \fn void processor_8xy7_subn(struct Processor* processor, uint8_t reg1, uint8_t reg2);
 * \brief Set Vx = Vy - Vx, set VF = NOT borrow.
 *
 * Si Vy > Vx, alors Vf est mis à 1, sinon 0.
 * Ensuite Vx est soustrait de Vy, et le résultat est mis dans Vx
 * @param processor
 * @param reg1
 * @param reg2
 */
int processor_8xy7_subn(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_8xyE_shl(struct Processor* processor, uint8_t reg)
 * \brief Met reg = reg SHL 1
 *
 * Si le bit de poids fort de reg est 1, alors Vf est mis à 1, sinon 0.
 * Ensuite reg est multiplié par 2 (shift left).
 *
 * @param processor Le processeur
 * @param reg Le registre
 * @param reg2 Le deuxième registre
 */
int processor_8xyE_shl(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_9xy0_sne_reg(struct Processor* processor, uint8_t reg1, uint8_t reg2)
 * \brief Passe l'instruction suivant si reg1 != reg2
 *
 * Les valeurs de reg1 et reg2 sont comparées, et si elles ne sont pas égales,
 * alors le compteur ordinal est augmenté de 2.
 *
 * @param processor Le processeur
 * @param reg1 Le premier registre
 * @param reg2 Le deuxième registre
 */
int processor_9xy0_sne_reg(struct Processor* processor, uint8_t reg1, uint8_t reg2);

/**
 * \relates Processor
 * \fn void processor_Annn_ldi(struct Processor* processor, uint16_t addr)
 * \brief Met I = addr
 *
 * La valeur du registre I est mise à addr.
 *
 * @param processor Le processeur
 * @param addr L'adresse qui va être stockée dans I.
 */
int processor_Annn_ldi(struct Processor* processor, uint16_t addr);

/**
 * \relates Processor
 * \fn void processor_Bnnn_jpv0(struct Processor* processor, uint16_t addr)
 * \brief Va à l'emplacement addr + V0
 *
 * Le compteur ordinal est mis à addr + la valeur de V0.
 *
 * @param processor Le processeur.
 * @param addr L'adresse utilisée pour le jump.
 */
int processor_Bnnn_jpv0(struct Processor* processor, uint16_t addr);

/**
 * \relates Processor
 * \fn void processor_Cxkk_rnd(struct Processor* processor, uint8_t reg, uint8_t val);
 * \brief Set regx = random byte AND val.
 *
 * L'interpréteur génère un nombre aléatoire entre 0 et 255, puis la fonction
 * AND y est appliqué avec la valeur val. Le résultat est stocké dans Vx
 *
 * @param processor
 * @param reg
 * @param val
 */
int processor_Cxkk_rnd(struct Processor* processor, uint8_t reg, uint8_t val);

/**
 * \relates Processor
 * \fn void processor_Dxyn_drw(struct Processor* processor, uint8_t reg1, uint8_t reg2)
 * \brief Affiche un sprite de n-byte à l'emplacement mémoire I à la position
 * (reg1, reg2), met VF = collision.
 *
 * // Traduire paragraphe
 *
 * @param processor Le processeur
 * @param reg1 Le registre qui contient la position x du sprite.
 * @param reg2 Le registre qui contient la position y du sprite.
 * @param nibble Le nombre de byte à afficher.
 */
int processor_Dxyn_drw(struct Processor* processor, uint8_t reg1, uint8_t reg2,
        uint8_t nibble);

/**
 * \relates Processor
 * \fn void processor_Ex9E_skp(struct Processor* processor, uint8_t reg);
 * \brief Saute la prochaine instruction si la touche correspondant à la valeur
 * de reg est appuyé.
 *
 * Regarde le clavier, et si la touche correspondant à la valeur de reg
 * est actuellement appuyé, alors le PC augmente de 2.
 *
 * @param processor
 * @param reg
 */
int processor_Ex9E_skp(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_ExA1_sknp(struct Processor* processor, uint8_t reg)
 * \brief Passe la prochaine instruction si la touche avec la valeur de reg
 * n'est pas appuyée.
 *
 * Vérifie le clavier, et si la touche correspondant à la valeur de reg est en
 * position haute, le compteur ordinal est augmenté de 2.
 *
 * @param processor Le processeur.
 * @param reg Le registre correspondant à la touche.
 */
int processor_ExA1_sknp(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_Fx07_lddt(struct Processor* processor, uint8_t reg);
 * \brief Set Vx = la valeur de DT
 *
 * La valeur de DT est mise dans Vx
 *
 * @param processor
 * @param reg
 */
int processor_Fx07_lddt(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_Fx0A_ldvk(struct Processor* processor, uint8_t reg)
 * \brief Attends qu'une touche soit pressée, ensuite stocke la valeur de la
 * touche pressée dans reg.
 *
 * Toutes les exécutions processor_3xkk_se stoppe jusqu'à ce qu'une touche soit pressée,
 * ensuite la valeur de la touche est stockée dans reg.
 *
 * @param processor Le processeur
 * @param reg Le registre
 */
int processor_Fx0A_ldvk(struct Processor* processor, uint8_t reg);


/**
 * \relates Processor
 * \fn void processor_Fx15_lddt(struct Processor* processor, uint8_t reg);
 * \brief Set DT = Vx
 *
 * La valeur de DT (delay timer) est remplacé par la valeur de reg
 *
 * @param processor
 * @param reg
 */
int processor_Fx15_lddt(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_Fx18_ldst(struct Processor* processor, uint8_t reg);
 * \brief Met reg = la valeur de ST
 *
 * La valeur de ST est mise dans reg
 *
 * @param processor Le processeur.
 * @param reg Le registre
 */
int processor_Fx18_ldst(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_Fx1E_addi(struct Processor* processor, uint8_t reg)
 * \brief Fais I = I + reg
 *
 * La valeur de I et reg sont additionnées et les résultats sont stockées dans
 * I.
 *
 * @param processor Le processeur.
 * @param reg Le registre à additionner.
 */
int processor_Fx1E_addi(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_Fx29_ldf(struct Processor* processor, uint8_t reg);
 * \brief Set I = l'emplacement du sprite pour la valeur Vx.
 *
 * La valeur de I est mis à l'emplacement du code hexadecimal du sprite
 * corresondant à la valeur Vx.
 *
 * @param processor
 * @param reg
 */
int processor_Fx29_ldf(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_Fx33_ldb(struct Processor* processor, uint8_t reg);
 * \brief Stocke la représentation DCB de Vx dans les emplacements mémoires
 * de I, I+1 et I+2.
 *
 * L'interpréteur prend la valeur décimal de Vx, et met le chiffre des centaines
 *dans I, le chiffre des dizaines dans I+1, et le chiffre des unités dans I+2.
 *
 * @param processor
 * @param reg
 */
int processor_Fx33_ldb(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_Fx55_ldw(struct Processor* processor, uint8_t reg);
 * \brief Stocke les registres de V0 à Vx dans la mémoire à partir de
 * la localisation I.
 *
 * L'interpréteur copie les valeurs des registres V0 à Vx dans la mémoire,
 * en commençant à l'emplacement de l'adresse dans I.
 *
 * @param processor
 * @param reg
 */
int processor_Fx55_ldw(struct Processor* processor, uint8_t reg);

/**
 * \relates Processor
 * \fn void processor_Fx65_ldr(struct Processor* processor, uint8_t reg);
 * \brief Lit les registres de V0 à Vx depuis la mémoire à partir de
 * la localisation I.
 *
 * L'interpréteur lit les valeurs dans la mémoire en commençant à I dans
 * les registres V0 à Vx.
 *
 * @param processor
 * @param reg
 */
int processor_Fx65_ldr(struct Processor* processor, uint8_t reg);

#endif