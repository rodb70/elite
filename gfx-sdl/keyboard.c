/*
 * keyboard.c
 *
 *  Created on: 17 Dec 2023
 *      Author: rod
 */

#include "keyboard.h"
#include "elite.h"
#include "gfx.h"

#include <SDL2/SDL.h>

static char key_queu[ 9 ] = {'\0'};
uint8_t k_idx = 0;
void debug_key_log( char key )
{
    key_queu[ k_idx ] = key;
    k_idx++;
    k_idx &= 0x7;
}

void debug_keypresses( void )
{
    char str[80];
    int y = 0;
    gfx_set_clip_region( 1, 1, 750, 500 );
    gfx_clear_area( 513, 0, 750, 500 );
    sprintf( str, "F1_pressed %d", kbd_F1_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F2_pressed %d", kbd_F2_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F3_pressed %d", kbd_F3_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F4_pressed %d", kbd_F4_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F5_pressed %d", kbd_F5_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F6_pressed %d", kbd_F6_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F7_pressed %d", kbd_F7_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F8_pressed %d", kbd_F8_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F9_pressed %d", kbd_F9_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F10_pressed %d", kbd_F10_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F11_pressed %d", kbd_F11_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "F12_pressed %d", kbd_F12_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "y_pressed   %d", kbd_y_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "n_pressed   %d", kbd_n_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "fire_pressed %d", kbd_fire_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "ecm_pressed %d", kbd_ecm_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "energy_bomb_prsd %d", kbd_energy_bomb_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "hyperspace_prsd %d", kbd_hyperspace_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "ctrl_pressed %d", kbd_ctrl_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "jump_pressed %d", kbd_jump_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "escape_pressed %d", kbd_escape_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "dock_pressed %d", kbd_dock_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "d_pressed   %d", kbd_d_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "origin_pressed %d", kbd_origin_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "find_pressed %d", kbd_find_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "fire_missile_pressed %d", kbd_fire_missile_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "target_missile_pressed %d", kbd_target_missile_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "unarm_missile_pressed %d", kbd_unarm_missile_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "pause_pressed  %d", kbd_pause_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "resume_pressed %d", kbd_resume_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "inc_speed_pressed %d", kbd_inc_speed_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "dec_speed_pressed %d", kbd_dec_speed_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "up_pressed  %d", kbd_up_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "down_pressed %d", kbd_down_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "left_pressed %d", kbd_left_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "right_pressed %d", kbd_right_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "enter_pressed %d", kbd_enter_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "backspace_pressed %d", kbd_backspace_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "space_pressed  %d", kbd_space_pressed);
    gfx_display_text( 514, y, str);
    y+= 10;
    extern int find_input;
    sprintf( str, "find_input  %d", find_input);
    gfx_display_text( 514, y, str);
    y+= 10;
    sprintf( str, "key_queu %d %s", k_idx, key_queu);
    gfx_display_text( 514, y, str);
}

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
