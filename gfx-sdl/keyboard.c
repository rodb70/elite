/*
 * keyboard.c
 *
 *  Created on: 17 Dec 2023
 *      Author: rod
 */

#include "keyboard.h"
#include "elite.h"

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

    if( 0 != SDL_PollEvent( &event ))
    {
        if( 0 == event.key.repeat )
        {
            switch( event.key.keysym.sym )
            {
            case SDLK_BACKSPACE :
                kbd_backspace_pressed = event.key.state;
                break;

            case SDLK_TAB :
                kbd_energy_bomb_pressed = event.key.state;
                break;

            case SDLK_F1 :
                kbd_F1_pressed = event.key.state;
                break;

            case SDLK_F2 :
                kbd_F2_pressed = event.key.state;
                break;

            case SDLK_F3 :
                kbd_F3_pressed = event.key.state;
                break;

            case SDLK_F4 :
                kbd_F4_pressed = event.key.state;
                break;

            case SDLK_F5 :
                kbd_F5_pressed = event.key.state;
                break;

            case SDLK_F6 :
                kbd_F6_pressed = event.key.state;
                break;

            case SDLK_F7 :
                kbd_F7_pressed = event.key.state;
                break;

            case SDLK_F8 :
                kbd_F8_pressed = event.key.state;
                break;

            case SDLK_F9 :
                kbd_F9_pressed = event.key.state;
                break;

            case SDLK_F10 :
                kbd_F10_pressed = event.key.state;
                break;

            case SDLK_F11 :
                kbd_F11_pressed = event.key.state;
                break;

            case SDLK_F12 :
                kbd_F12_pressed = event.key.state;
                break;

            case SDLK_a :
                kbd_fire_pressed = event.key.state;
                break;

            case SDLK_c :
                kbd_dock_pressed = event.key.state;
                break;

            case SDLK_d :
                kbd_d_pressed = event.key.state;
                break;

            case SDLK_e :
                kbd_ecm_pressed = event.key.state;
                break;

            case SDLK_f :
                kbd_find_pressed = event.key.state;
                break;

            case SDLK_h :
                kbd_hyperspace_pressed = event.key.state;
                break;

            case SDLK_j :
                kbd_jump_pressed = event.key.state;
                break;

            case SDLK_m :
                kbd_fire_missile_pressed = event.key.state;
                break;

            case SDLK_n :
                kbd_n_pressed = event.key.state;
                break;

            case SDLK_o :
                kbd_origin_pressed = event.key.state;
                break;

            case SDLK_p :
                kbd_pause_pressed = event.key.state;
                break;

            case SDLK_q :
                break;

            case SDLK_r :
                kbd_resume_pressed = event.key.state;
                break;

            case SDLK_t :
                kbd_target_missile_pressed = event.key.state;
                break;

            case SDLK_w :
                break;

            case SDLK_u :
                kbd_unarm_missile_pressed = event.key.state;
                break;

            case SDLK_y :
                kbd_y_pressed = event.key.state;
                break;

            case SDLK_SLASH :
                kbd_dec_speed_pressed = event.key.state;
                break;

            case SDLK_SPACE :
                kbd_inc_speed_pressed = event.key.state;
                kbd_space_pressed = event.key.state;
                break;

            case SDLK_RETURN2 :
            case SDLK_RETURN :
                kbd_enter_pressed = event.key.state;
                break;

            case SDLK_KP_0 :
            case SDLK_0 :
                break;

            case SDLK_RIGHT :
            case SDLK_KP_PLUS :
                kbd_right_pressed = event.key.state;
                break;

            case SDLK_LEFT :
            case SDLK_KP_MINUS :
                kbd_left_pressed = event.key.state;
                break;

            case SDLK_s :
            case SDLK_UP :
                kbd_up_pressed = event.key.state;
                break;

            case SDLK_x :
            case SDLK_DOWN :
                kbd_down_pressed = event.key.state;
                break;

            case SDLK_ESCAPE :
                kbd_escape_pressed = event.key.state;
                break;

            case SDLK_LCTRL :
            case SDLK_RCTRL :
                kbd_ctrl_pressed = event.key.state;
                break;

            }
        }
    }

//    kbd_clear_key_buffer();
}

int kbd_read_key (void)
{
    SDL_Event event;
    int keyasc = 0;

    if( 0 != SDL_PollEvent( &event ))
    {
        if (event.type == SDL_KEYDOWN)
        {
            if( SDLK_RETURN == event.key.keysym.sym )
            {
                kbd_enter_pressed = 1;
                return 0;
            }
            else if( SDLK_BACKSPACE == event.key.keysym.sym )
            {
                kbd_backspace_pressed = 1;
                return 0;
            }
            else if( 0 != ( event.key.keysym.mod & ( KMOD_SHIFT | KMOD_ALT | KMOD_CTRL )))
            {
                keyasc = 0;
            }
            else
            {
                const char *key = SDL_GetKeyName( event.key.keysym.sym );
                keyasc = (int)(*key);
            }
        }
    }

    return keyasc;
}

void kbd_clear_key_buffer (void)
{
    SDL_PumpEvents();
    SDL_FlushEvents( SDL_FIRSTEVENT, SDL_LASTEVENT );
}
