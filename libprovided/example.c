/*-------------------------------------------------------------------------*
 | Copyright (C) 2023 Département Informatique de PolyTech Tours.          |
 |                                                                         |
 | This file is part of PolyChip8, yet another CHIP 8 emulator.            |
 |                                                                         |
 | PolyChip8 is free software; you can redistribute it and/or modify       |
 | it under the terms of the GNU General Public License as published by    |
 | the Free Software Foundation; either version 3 of the License,          |
 | or (at your option) any later version.                                  |
 |                                                                         |
 | PolyChip8 is distributed in the hope that it will be useful,            |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
 | GNU General Public License for more details.                            |
 |                                                                         |
 | You should have received a copy of the GNU General Public License       |
 | along with this program. If not, see <http://www.gnu.org/licenses/>.    |
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include "machine/machine.h"

int main (int argc, char** argv) {
        if (argc != 2) {
                fprintf(stderr, "Unexpected number of arguments.\n");
                fprintf(stderr, "Usage: emulator <ROM.ch8>\n");
                return 1;
        }
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO)) {
                errcode = SDL;
        }
        else {
                printf("The SDL library has been initialized successfully.\n");
                struct Machine machine;
                if (!Machine_init(&machine)) {
                        printf("The CHIP 8 emulator is ready.\n");
                        if (!Machine_load(&machine,argv[1])) {
                                printf("The ROM <%s> has been loaded.\n",argv[1]);
                                printf("Run...\n");
                                while (
                                        !Machine_loop(&machine)
                                );
                        }
                        Machine_destroy(&machine);
                }
                SDL_Quit();
        }
        if (errcode != QUIT) {
                fprintf(stderr,"The program has terminated abnormally (errcode=%s)\n",errorstr());
                return 1;
        }
        return 0;
}
