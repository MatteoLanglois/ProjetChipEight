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

#ifndef DISPLAY_DISPLAY_H
#define DISPLAY_DISPLAY_H

#include <SDL2/SDL.h>
#include "sprite.h"

/**
 * \brief Emulates a 64x32 monochrome display.
 *
 * A struct Display variable shall be initialized and destroyed using the
 * Display_init and Display_destroy functions, respectively.
 *
 * This implementation relies on the SDL2 library (https://www.libsdl.org/).
 */
struct Display {

        /**
         * The size at which pixels will be drawn on the physical screen.
         */
        int pixel;

        /**
         * The internal representation of the surface (a 0-1 64*32 matrix).
         *
         * This array is dynamically allocated and freed by the Display_init
         * and Display_destroy functions, respectively.
         */
        int** contents;

        /**
         * The underlying SDL window.
         */
        SDL_Window* window;

        /**
         * The underlying SDL renderer.
         */
        SDL_Renderer* renderer;

        /**
         * Indicates whether the underlying SDL surface needs to be refreshed.
         */
        int modified;

};

/**
 * \memberof Display
 * \brief    Initializes a display.
 * \param    display The display to be initialized.
 * \param    pixel   The size at which pixels are to be drawn on the physical screen.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * The \p display variable can later be passed to \ref Display_destroy to be destroyed.
 *
 * Errors:
 *   - MALLOC: out of memory,
 *   - SDL:    a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_init (struct Display* display, int pixel);

/**
 * \memberof Display
 * \brief    Destroys a display.
 * \param    display The display to be destroyed.
 *
 * The \p display variable must have been initialized using the \ref Display_init function beforehand.
 */
void Display_destroy (struct Display* display);

/**
 * \memberof Display
 * \brief    Copies the surface of a display to the screen.
 * \param    display The display to be refreshed.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * Errors:
 *   - SDL: a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_update (struct Display* display);

/**
 * \memberof Display
 * \brief    Clears a display.
 * \param    display The display to be cleared.
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * Errors:
 *   - SDL: a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_CLS (struct Display* display);

/**
 * \memberof Display
 * \brief    Draws a sprite.
 * \param    display The display to be modified.
 * \param    sprite  The sprite to be drawn.
 * \param    x       The x-coordinate where the sprite is to be drawn.
 * \param    y       The y-coordinate where the sprite is to be drawn.
 * \param    VF      The location of the collision flag to be set (see the DRW CHIP 8 instruction for more details).
 * \return   0 on success, 1 in case of an error (\ref errcode is set to indicate the error).
 *
 * Errors:
 *   - SDL: a SDL function has failed; call SDL_GetError() for more information.
 */
int Display_DRW (struct Display* display, const struct Sprite* sprite, uint8_t x, uint8_t y, uint8_t* VF);

// endif DISPLAY_DISPLAY_H
#endif
