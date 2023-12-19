/*
 * keyboard.c
 *
 *  Created on: 17 Dec 2023
 *      Author: rod
 */

#include "keyboard.h"

#include <SDL2/SDL.h>

int kbd_keyboard_startup (void)
{
    return 0;
}

int kbd_keyboard_shutdown (void)
{
    return 0;
}

void kbd_poll_keyboard (void)
{
    SDL_Event event;
    int keyPressed = 0;

    while( SDL_PollEvent( &event ))
    {
        switch( event.type )
        {
        case SDL_KEYDOWN:
            keyPressed = 1;
            break;

        case SDL_KEYUP:
            keyPressed = 0;
            break;

        default:
            break;
        }

        (void)keyPressed;
        switch( event.key.keysym.sym )
        {
        case SDLK_TAB :
            kbd_energy_bomb_pressed = keyPressed;
            break;

        case SDLK_F1 :
            kbd_F1_pressed = keyPressed;
            break;

        case SDLK_F2 :
            kbd_F2_pressed = keyPressed;
            break;

        case SDLK_F3 :
            kbd_F3_pressed = keyPressed;
            break;

        case SDLK_F4 :
            kbd_F4_pressed = keyPressed;
            break;

        case SDLK_F5 :
            kbd_F5_pressed = keyPressed;
            break;

        case SDLK_F6 :
            kbd_F6_pressed = keyPressed;
            break;

        case SDLK_F7 :
            kbd_F7_pressed = keyPressed;
            break;

        case SDLK_F8 :
            kbd_F8_pressed = keyPressed;
            break;

        case SDLK_F9 :
            kbd_F9_pressed = keyPressed;
            break;

        case SDLK_F10 :
            kbd_F10_pressed = keyPressed;
            break;

        case SDLK_F11 :
            kbd_F11_pressed = keyPressed;
            break;

        case SDLK_F12 :
            kbd_F12_pressed = keyPressed;
            break;

        case SDLK_a :
            kbd_fire_pressed = keyPressed;
            break;

        case SDLK_c :
            kbd_dock_pressed = keyPressed;
            break;

        case SDLK_d :
            kbd_d_pressed = keyPressed;
            break;

        case SDLK_f :
            kbd_find_pressed = keyPressed;
            break;

        case SDLK_h :
            /* ctrl also pressed */
            kbd_hyperspace_pressed = keyPressed;
            break;

        case SDLK_j :
            kbd_jump_pressed = keyPressed;
            break;

        case SDLK_m :
            kbd_fire_missile_pressed = keyPressed;
            break;

        case SDLK_n :
            kbd_n_pressed = keyPressed;
            break;

        case SDLK_o :
            kbd_origin_pressed = keyPressed;
            break;

        case SDLK_p :
            kbd_pause_pressed = keyPressed;
            break;

        case SDLK_q :
            break;

        case SDLK_r :
            kbd_resume_pressed = keyPressed;
            break;

        case SDLK_w :
            break;

        case SDLK_y :
            kbd_y_pressed = keyPressed;
            break;

        case SDLK_SPACE :
            kbd_space_pressed = keyPressed;
            break;

        case SDLK_RETURN2 :
        case SDLK_RETURN :
            kbd_enter_pressed = keyPressed;
            break;

        case SDLK_KP_0 :
        case SDLK_0 :
            break;

        case SDLK_RIGHT:
        case SDLK_KP_PLUS:
            kbd_right_pressed = keyPressed;
            break;

        case SDLK_LEFT:
        case SDLK_KP_MINUS:
            kbd_left_pressed = keyPressed;
            break;

        case SDLK_UP:
            kbd_up_pressed = keyPressed;
            break;

        case SDLK_DOWN:
            kbd_down_pressed = keyPressed;
            break;

        case SDLK_ESCAPE :
            kbd_escape_pressed = keyPressed;
            break;

        case SDLK_LCTRL :
        case SDLK_RCTRL :
            kbd_ctrl_pressed = keyPressed;
            break;

        }
    }

    SDL_Delay( 10 ); /* Sleep for 5 millisecond */
}

int kbd_read_key (void)
{

    return 0;
}

void kbd_clear_key_buffer (void)
{

}
