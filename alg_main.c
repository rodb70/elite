/*
 * Elite - The New Kind.
 *
 * Reverse engineered from the BBC disk version of Elite.
 * Additional material by C.J.Pinder.
 *
 * The original Elite code is (C) I.Bell & D.Braben 1984.
 * This version re-engineered in C by C.J.Pinder 1999-2001.
 *
 * email: <christian@newkind.co.uk>
 *
 *
 */

/*
 * alg_main.c
 *
 * Allegro version of the main game handler.
 */

#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include "config.h"
#include "gfx.h"
#include "main.h"
#include "vector.h"
#include "alg_data.h"
#include "elite.h"
#include "docked.h"
#include "intro.h"
#include "shipdata.h"
#include "shipface.h"
#include "space.h"
#include "sound.h"
#include "threed.h"
#include "swat.h"
#include "random.h"
#include "options.h"
#include "stars.h"
#include "missions.h"
#include "pilot.h"
#include "file.h"
#include "keyboard.h"

enum
{
    VS_MIN_X = 1,
    VS_MIN_Y = 1,
    VS_MAX_X = 510,
    VS_MAX_Y = 383
};
int old_cross_x, old_cross_y;
int cross_timer;

int draw_lasers;
int mcount;
int message_count;
char message_string[ 80 ];
int rolling;
int climbing;
int game_paused;
int have_joystick;

int find_input;
char find_name[ 20 ];

/*
 * Initialise the game parameters.
 */

void initialise_game( void )
{
    set_rand_seed( time( NULL ) );
    current_screen = SCR_INTRO_ONE;

    restore_saved_commander();

    flight_speed = 1;
    flight_roll = 0;
    flight_climb = 0;
    docked = 1;
    front_shield = 255;
    aft_shield = 255;
    energy = 255;
    draw_lasers = 0;
    mcount = 0;
    hyper_ready = 0;
    detonate_bomb = 0;
    find_input = 0;
    witchspace = 0;
    game_paused = 0;
    auto_pilot = 0;

    create_new_stars();
    clear_universe();

    cross_x = -1;
    cross_y = -1;
    cross_timer = 0;

    myship.max_speed = 40;/* 0.27 Light Mach */
    myship.max_roll = 31;
    myship.max_climb = 8;/* CF 8 */
    myship.max_fuel = 70;/* 7.0 Light Years */
}

void finish_game(void)
{
    finish = 1;
    game_over = 1;
}

/*
 * Move the planet chart cross hairs to specified position.
 */
void move_cross(int dx, int dy)
{
    cross_timer = 5;

    if( current_screen == SCR_SHORT_RANGE )
    {
        cross_x += (dx * 4);
        cross_y += (dy * 4);
        return;
    }

    if( current_screen == SCR_GALACTIC_CHART )
    {
        cross_x += (dx * 2);
        cross_y += (dy * 2);

        if( cross_x < 1 )
        {
            cross_x = 1;
        }

        if( cross_x > 510 )
        {
            cross_x = 510;
        }

        if( cross_y < 37 )
        {
            cross_y = 37;
        }

        if( cross_y > 293 )
        {
            cross_y = 293;
        }
    }
}

/*
 * Draw the cross hairs at the specified position.
 */

void draw_cross(int cx, int cy)
{
    if( current_screen == SCR_SHORT_RANGE )
    {
        gfx_set_clip_region( 1, 37, 510, 339 );
        xor_mode( TRUE );
        gfx_draw_colour_line( cx - 16, cy, cx + 16, cy, GFX_COL_RED );
        gfx_draw_colour_line( cx, cy - 16, cx, cy + 16, GFX_COL_RED );
        xor_mode( FALSE );
        gfx_set_clip_region( VS_MIN_X, VS_MIN_Y, VS_MAX_X, VS_MAX_Y );
        return;
    }

    if( current_screen == SCR_GALACTIC_CHART )
    {
        gfx_set_clip_region( 1, 37, 510, 293 );
        xor_mode (TRUE);
        gfx_draw_colour_line( cx - 8, cy, cx + 8, cy, GFX_COL_RED );
        gfx_draw_colour_line( cx, cy - 8, cx, cy + 8, GFX_COL_RED );
        xor_mode (FALSE);
        gfx_set_clip_region( VS_MIN_X, VS_MIN_Y, VS_MAX_X, VS_MAX_Y );
    }
}

void draw_laser_sights(void)
{
    int laser = 0;
    int x1, y1, x2, y2;

    switch( current_screen )
    {
    case SCR_FRONT_VIEW :
        gfx_display_centre_text( 32, "Front View", 120, GFX_COL_WHITE );
        laser = cmdr.front_laser;
        break;

    case SCR_REAR_VIEW :
        gfx_display_centre_text( 32, "Rear View", 120, GFX_COL_WHITE );
        laser = cmdr.rear_laser;
        break;

    case SCR_LEFT_VIEW :
        gfx_display_centre_text( 32, "Left View", 120, GFX_COL_WHITE );
        laser = cmdr.left_laser;
        break;

    case SCR_RIGHT_VIEW :
        gfx_display_centre_text( 32, "Right View", 120, GFX_COL_WHITE );
        laser = cmdr.right_laser;
        break;
    }

    if( laser )
    {
        x1 = 128 * GFX_SCALE;
        y1 = (96 - 8) * GFX_SCALE;
        y2 = (96 - 16) * GFX_SCALE;

        gfx_draw_colour_line( x1 - 1, y1, x1 - 1, y2, GFX_COL_GREY_1 );
        gfx_draw_colour_line( x1, y1, x1, y2, GFX_COL_WHITE );
        gfx_draw_colour_line( x1 + 1, y1, x1 + 1, y2, GFX_COL_GREY_1 );

        y1 = (96 + 8) * GFX_SCALE;
        y2 = (96 + 16) * GFX_SCALE;

        gfx_draw_colour_line( x1 - 1, y1, x1 - 1, y2, GFX_COL_GREY_1 );
        gfx_draw_colour_line( x1, y1, x1, y2, GFX_COL_WHITE );
        gfx_draw_colour_line( x1 + 1, y1, x1 + 1, y2, GFX_COL_GREY_1 );

        x1 = (128 - 8) * GFX_SCALE;
        y1 = 96 * GFX_SCALE;
        x2 = (128 - 16) * GFX_SCALE;

        gfx_draw_colour_line( x1, y1 - 1, x2, y1 - 1, GFX_COL_GREY_1 );
        gfx_draw_colour_line( x1, y1, x2, y1, GFX_COL_WHITE );
        gfx_draw_colour_line( x1, y1 + 1, x2, y1 + 1, GFX_COL_GREY_1 );

        x1 = (128 + 8) * GFX_SCALE;
        x2 = (128 + 16) * GFX_SCALE;

        gfx_draw_colour_line( x1, y1 - 1, x2, y1 - 1, GFX_COL_GREY_1 );
        gfx_draw_colour_line( x1, y1, x2, y1, GFX_COL_WHITE );
        gfx_draw_colour_line( x1, y1 + 1, x2, y1 + 1, GFX_COL_GREY_1 );
    }
}

void arrow_right(void)
{
    switch( current_screen )
    {
    case SCR_MARKET_PRICES:
        buy_stock();
        break;

    case SCR_SETTINGS:
        select_right_setting();
        break;

    case SCR_SHORT_RANGE:
    case SCR_GALACTIC_CHART:
        move_cross( 1, 0 );
        break;

    case SCR_FRONT_VIEW:
    case SCR_REAR_VIEW:
    case SCR_RIGHT_VIEW:
    case SCR_LEFT_VIEW:
        if( flight_roll > 0 )
        {
            flight_roll = 0;
        }
        else
        {
            decrease_flight_roll();
            decrease_flight_roll();
            rolling = 1;
        }
        break;
    }
}

void arrow_left(void)
{
    switch( current_screen )
    {
    case SCR_MARKET_PRICES:
        sell_stock();
        break;

    case SCR_SETTINGS:
        select_left_setting();
        break;

    case SCR_SHORT_RANGE:
    case SCR_GALACTIC_CHART:
        move_cross( -1, 0 );
        break;

    case SCR_FRONT_VIEW:
    case SCR_REAR_VIEW:
    case SCR_RIGHT_VIEW:
    case SCR_LEFT_VIEW:
        if( flight_roll < 0 )
        {
            flight_roll = 0;
        }
        else
        {
            increase_flight_roll();
            increase_flight_roll();
            rolling = 1;
        }
        break;
    }
}

void arrow_up(void)
{
    switch( current_screen )
    {
    case SCR_MARKET_PRICES:
        select_previous_stock();
        break;

    case SCR_EQUIP_SHIP:
        select_previous_equip();
        break;

    case SCR_OPTIONS:
        select_previous_option();
        break;

    case SCR_SETTINGS:
        select_up_setting();
        break;

    case SCR_SHORT_RANGE:
    case SCR_GALACTIC_CHART:
        move_cross( 0, -1 );
        break;

    case SCR_FRONT_VIEW:
    case SCR_REAR_VIEW:
    case SCR_RIGHT_VIEW:
    case SCR_LEFT_VIEW:
        if( flight_climb > 0 )
        {
            flight_climb = 0;
        }
        else
        {
            decrease_flight_climb();
        }
        climbing = 1;
        break;
    }
}

void arrow_down(void)
{
    switch( current_screen )
    {
    case SCR_MARKET_PRICES:
        select_next_stock();
        break;

    case SCR_EQUIP_SHIP:
        select_next_equip();
        break;

    case SCR_OPTIONS:
        select_next_option();
        break;

    case SCR_SETTINGS:
        select_down_setting();
        break;

    case SCR_SHORT_RANGE:
    case SCR_GALACTIC_CHART:
        move_cross( 0, 1 );
        break;

    case SCR_FRONT_VIEW:
    case SCR_REAR_VIEW:
    case SCR_RIGHT_VIEW:
    case SCR_LEFT_VIEW:
        if( flight_climb < 0 )
        {
            flight_climb = 0;
        }
        else
        {
            increase_flight_climb();
        }
        climbing = 1;
        break;
    }
}

void return_pressed(void)
{
    switch( current_screen )
    {
    case SCR_EQUIP_SHIP:
        buy_equip();
        break;

    case SCR_OPTIONS:
        do_option();
        break;

    case SCR_SETTINGS:
        toggle_setting();
        break;
    }
}

void y_pressed(void)
{
    switch( current_screen )
    {
    case SCR_QUIT:
        finish_game();
        break;
    }
}

void n_pressed(void)
{
    switch( current_screen )
    {
    case SCR_QUIT:
        if( docked )
            display_commander_status();
        else
            current_screen = SCR_FRONT_VIEW;
        break;
    }
}

void d_pressed(void)
{
    switch( current_screen )
    {
    case SCR_GALACTIC_CHART:
    case SCR_SHORT_RANGE:
        show_distance_to_planet();
        break;

    case SCR_FRONT_VIEW:
    case SCR_REAR_VIEW:
    case SCR_RIGHT_VIEW:
    case SCR_LEFT_VIEW:
        if( auto_pilot )
        {
            disengage_auto_pilot();
        }
        break;
    }
}

void f_pressed(void)
{
    if(( current_screen == SCR_GALACTIC_CHART ) || ( current_screen == SCR_SHORT_RANGE ))
    {
        find_input = 1;
        *find_name = '\0';
        gfx_clear_text_area();
        gfx_display_text( 16, 340, "Planet Name? " );
    }
}

void add_find_char(int letter)
{
    char str[40];

    if( strlen( find_name ) == 16 )
    {
        return;
    }

    str[0] = toupper( letter );
    str[1] = '\0';
    strcat( find_name, str );

    sprintf( str, "Planet Name? %s ", find_name );
    gfx_clear_text_area();
    gfx_display_text( 16, 340, str );
}

void delete_find_char(void)
{
    char str[40];
    int len;

    len = strlen( find_name );
    if( 0 == len )
    {
        return;
    }

    find_name[len - 1] = '\0';

    sprintf( str, "Planet Name? %s ", find_name );
    gfx_clear_text_area();
    gfx_display_text( 16, 340, str );
}

void o_pressed(void)
{
    switch( current_screen )
    {
    case SCR_GALACTIC_CHART:
    case SCR_SHORT_RANGE:
        move_cursor_to_origin();
        break;
    }
}

void auto_dock(void)
{
    struct univ_object ship;

    ship.location.x = 0;
    ship.location.y = 0;
    ship.location.z = 0;

    set_init_matrix( ship.rotmat );
    ship.rotmat[2].z = 1;
    ship.rotmat[0].x = -1;
    ship.type = -96;
    ship.velocity = flight_speed;
    ship.acceleration = 0;
    ship.bravery = 0;
    ship.rot.z = 0;
    ship.rot.x = 0;

    auto_pilot_ship( &ship );

    if( ship.velocity > 22 )
        flight_speed = 22;
    else
        flight_speed = ship.velocity;

    if( ship.acceleration > 0 )
    {
        flight_speed++;
        if( flight_speed > 22 )
        {
            flight_speed = 22;
        }
    }

    if( ship.acceleration < 0 )
    {
        flight_speed--;
        if( flight_speed < 1 )
        {
            flight_speed = 1;
        }
    }

    if( ship.rot.x == 0 )
    {
        flight_climb = 0;
    }

    if( ship.rot.x < 0 )
    {
        increase_flight_climb();

        if( ship.rot.x < -1 )
        {
            increase_flight_climb();
        }
    }

    if( ship.rot.x > 0 )
    {
        decrease_flight_climb();

        if( ship.rot.x > 1 )
        {
            decrease_flight_climb();
        }
    }

    if( ship.rot.z == 127 )
    {
        flight_roll = -14;
    }
    else
    {
        if( ship.rot.z == 0 )
        {
            flight_roll = 0;
        }

        if( ship.rot.z > 0 )
        {
            increase_flight_roll();

            if( ship.rot.z > 1 )
            {
                increase_flight_roll();
            }
        }

        if( ship.rot.z < 0 )
        {
            decrease_flight_roll();

            if( ship.rot.z < -1 )
            {
                decrease_flight_roll();
            }
        }
    }
}

void run_escape_sequence(void)
{
    int i;
    int newship;
    struct point pt = { 0, 0, 200 };
    Matrix rotmat;

    current_screen = SCR_ESCAPE_POD;

    flight_speed = 1;
    flight_roll = 0;
    flight_climb = 0;

    set_init_matrix( rotmat );
    rotmat[2].z = 1.0;

    newship = add_new_ship( SHIP_COBRA3, pt, rotmat, ROT_127 );
    universe[newship].velocity = 7;
    snd_play_sample( SND_LAUNCH );

    for( i = 0; i < 90; i++ )
    {
        if( i == 40 )
        {
            universe[newship].flags |= FLG_DEAD;
            snd_play_sample( SND_EXPLODE );
        }

        gfx_set_clip_region( VS_MIN_X, VS_MIN_Y, VS_MAX_X, VS_MAX_Y );
        gfx_clear_display();
        update_starfield();
        update_universe();

        universe[newship].location.x = 0;
        universe[newship].location.y = 0;
        universe[newship].location.z += 2;

        gfx_display_centre_text( 358, "Escape pod launched - Ship auto-destuct initiated.", 120,
                GFX_COL_WHITE );

        update_console();
        gfx_update_screen();
    }

    while( (ship_count[SHIP_CORIOLIS] == 0) && (ship_count[SHIP_DODEC] == 0) )
    {
        auto_dock();

        if( (abs( flight_roll ) < 3) && (abs( flight_climb ) < 3) )
        {
            for( i = 0; i < MAX_UNIV_OBJECTS; i++ )
            {
                if( universe[i].type != 0 )
                    universe[i].location.z -= 1500;
            }

        }

        warp_stars = 1;
        gfx_set_clip_region( VS_MIN_X, VS_MIN_Y, VS_MAX_X, VS_MAX_Y );
        gfx_clear_display();
        update_starfield();
        update_universe();
        update_console();
        gfx_update_screen();
    }

    abandon_ship();
}

void handle_flight_keys(void)
{
    if( !find_input )
    {
        kbd_poll_keyboard();
    }

    joy_poll_joystick();

    if( game_paused )
    {
        if( kbd_resume_pressed )
        {
            game_paused = 0;
        }

        return;
    }

    if( kbd_F1_pressed )
    {
        find_input = 0;

        if( docked )
        {
            launch_player();
        }
        else
        {
            if( current_screen != SCR_FRONT_VIEW )
            {
                current_screen = SCR_FRONT_VIEW;
                flip_stars();
            }
        }
    }

    if( kbd_F2_pressed )
    {
        find_input = 0;

        if( !docked )
        {
            if( current_screen != SCR_REAR_VIEW )
            {
                current_screen = SCR_REAR_VIEW;
                flip_stars();
            }
        }
    }

    if( kbd_F3_pressed )
    {
        find_input = 0;

        if( !docked )
        {
            if( current_screen != SCR_LEFT_VIEW )
            {
                current_screen = SCR_LEFT_VIEW;
                flip_stars();
            }
        }
    }

    if( kbd_F4_pressed )
    {
        find_input = 0;

        if( docked )
        {
            equip_ship();
        }
        else
        {
            if( current_screen != SCR_RIGHT_VIEW )
            {
                current_screen = SCR_RIGHT_VIEW;
                flip_stars();
            }
        }
    }

    if( kbd_F5_pressed )
    {
        find_input = 0;
        old_cross_x = -1;
        display_galactic_chart();
    }

    if( kbd_F6_pressed )
    {
        find_input = 0;
        old_cross_x = -1;
        display_short_range_chart();
    }

    if( kbd_F7_pressed )
    {
        find_input = 0;
        display_data_on_planet();
    }

    if( kbd_F8_pressed && ( !witchspace ))
    {
        find_input = 0;
        display_market_prices();
    }

    if( kbd_F9_pressed )
    {
        find_input = 0;
        display_commander_status();
    }

    if( kbd_F10_pressed )
    {
        find_input = 0;
        display_inventory();
    }

    if( kbd_F11_pressed )
    {
        find_input = 0;
        display_options();
    }

    if( find_input )
    {
        int keyasc = 0;
        while( 0 == keyasc )
        {
            keyasc = kbd_read_key();
        }

        if( keyasc != 0 )
        {
            debug_key_log( keyasc );
        }

        if( kbd_enter_pressed )
        {
            find_input = 0;
            find_planet_by_name( find_name );
            kbd_keyboard_startup();
            return;
        }
        else if( kbd_backspace_pressed )
        {
            kbd_backspace_pressed = 0;
            delete_find_char();
            return;
        }
        else if( isalpha( keyasc ) )
        {
            add_find_char( keyasc );
        }

        return;
    }

    if( kbd_y_pressed )
    {
        y_pressed();
    }

    if( kbd_n_pressed )
    {
        n_pressed();
    }

    if( kbd_fire_pressed )
    {
        if(( !docked ) && ( draw_lasers == 0 ))
        {
            draw_lasers = fire_laser();
        }
    }

    if( kbd_dock_pressed )
    {
        if( !docked && cmdr.docking_computer )
        {
            if( instant_dock )
            {
                engage_docking_computer();
            }
            else
            {
                engage_auto_pilot();
            }
        }
    }

    if( kbd_d_pressed )
    {
        d_pressed();
    }

    if( kbd_ecm_pressed )
    {
        if( !docked && cmdr.ecm )
        {
            activate_ecm( 1 );
        }
    }

    if( kbd_find_pressed )
    {
        f_pressed();
    }

    if( kbd_hyperspace_pressed && (!docked) )
    {
        if( kbd_ctrl_pressed )
        {
            start_galactic_hyperspace();
        }
        else
        {
            start_hyperspace();
        }
    }

    if( kbd_jump_pressed && (!docked) && (!witchspace) )
    {
        jump_warp();
    }

    if( kbd_fire_missile_pressed )
    {
        if( !docked )
            fire_missile();
    }

    if( kbd_origin_pressed )
    {
        o_pressed();
    }

    if( kbd_pause_pressed )
    {
        game_paused = 1;
    }

    if( kbd_target_missile_pressed )
    {
        if( !docked )
        {
            arm_missile();
        }
    }

    if( kbd_unarm_missile_pressed )
    {
        if( !docked )
        {
            unarm_missile();
        }
    }

    if( kbd_inc_speed_pressed )
    {
        if( !docked )
        {
            if( flight_speed < myship.max_speed )
            {
                flight_speed++;
            }
        }
    }

    if( kbd_dec_speed_pressed )
    {
        if( !docked )
        {
            if( flight_speed > 1 )
            {
                flight_speed--;
            }
        }
    }

    if( kbd_up_pressed )
    {
        arrow_up();
    }

    if( kbd_down_pressed )
    {
        arrow_down();
    }

    if( kbd_left_pressed )
    {
        arrow_left();
    }

    if( kbd_right_pressed )
    {
        arrow_right();
    }

    if( kbd_enter_pressed )
    {
        return_pressed();
    }

    if( kbd_energy_bomb_pressed )
    {
        if( (!docked) && (cmdr.energy_bomb) )
        {
            detonate_bomb = 1;
            cmdr.energy_bomb = 0;
        }
    }

    if( kbd_escape_pressed )
    {
        if( (!docked) && (cmdr.escape_pod) && (!witchspace) )
        {
            run_escape_sequence();
        }
    }
}

void set_commander_name(char *path)
{
    char *fname, *cname;
    int i;

    fname = get_filename( path );
    cname = cmdr.name;

    for( i = 0; i < 31; i++ )
    {
        if( !isalnum( *fname ) )
        {
            break;
        }

        *cname++ = toupper( *fname++ );
    }

    *cname = '\0';
}

void save_commander_screen(void)
{
    char path[255];
    int okay;
    int rv;

    current_screen = SCR_SAVE_CMDR;

    gfx_clear_display();
    gfx_display_centre_text( 10, "SAVE COMMANDER", 140, GFX_COL_GOLD );
    gfx_draw_line( 0, 36, 511, 36 );
    gfx_update_screen();

    strcpy( path, cmdr.name );
    strcat( path, ".nkc" );

    okay = gfx_request_file( "Save Commander", path, "nkc" );

    if( !okay )
    {
        display_options();
        return;
    }

    rv = save_commander_file( path );

    if( rv )
    {
        gfx_display_centre_text( 175, "Error Saving Commander!", 140, GFX_COL_GOLD );
        return;
    }

    gfx_display_centre_text( 175, "Commander Saved.", 140, GFX_COL_GOLD );

    set_commander_name( path );
    saved_cmdr = cmdr;
    saved_cmdr.ship_x = docked_planet.d;
    saved_cmdr.ship_y = docked_planet.b;
}

void load_commander_screen(void)
{
    char path[255];
    int rv;

    gfx_clear_display();
    gfx_display_centre_text( 10, "LOAD COMMANDER", 140, GFX_COL_GOLD );
    gfx_draw_line( 0, 36, 511, 36 );
    gfx_update_screen();

    strcpy( path, "jameson.nkc" );

    rv = gfx_request_file( "Load Commander", path, "nkc" );

    if( rv == 0 )
    {
        return;
    }

    rv = load_commander_file( path );

    if( rv )
    {
        saved_cmdr = cmdr;
        gfx_display_centre_text( 175, "Error Loading Commander!", 140, GFX_COL_GOLD );
        gfx_display_centre_text( 200, "Press any key to continue.", 140, GFX_COL_GOLD );
        gfx_update_screen();
        while( 0 != kbd_read_key())
        {
            continue;
        }

        return;
    }

    restore_saved_commander();
    set_commander_name( path );
    saved_cmdr = cmdr;
    update_console();
}

void run_first_intro_screen(void)
{
    current_screen = SCR_INTRO_ONE;

    snd_play_midi( SND_ELITE_THEME, TRUE );

    initialise_intro1();

    for( ;; )
    {
        update_intro1();

        gfx_update_screen();

        kbd_poll_keyboard();

        if( kbd_y_pressed )
        {
            snd_stop_midi();
            load_commander_screen();
            break;
        }

        if( kbd_n_pressed )
        {
            snd_stop_midi();
            break;
        }
    }
}

void run_second_intro_screen(void)
{
    current_screen = SCR_INTRO_TWO;

    snd_play_midi( SND_BLUE_DANUBE, TRUE );

    initialise_intro2();

    flight_speed = 3;
    flight_roll = 0;
    flight_climb = 0;

    for( ;; )
    {
        update_intro2();

        gfx_update_screen();

        kbd_poll_keyboard();

        if( kbd_space_pressed )
        {
            break;
        }
    }

    snd_stop_midi();
}

/*
 * Draw the game over sequence. 
 */

void run_game_over_screen(void)
{
    int i;
    int newship;
    struct point pt = { 0, 0, -400 };
    Matrix rotmat;
    int type;

    current_screen = SCR_GAME_OVER;
    gfx_set_clip_region( VS_MIN_X, VS_MIN_Y, VS_MAX_X, VS_MAX_Y );

    flight_speed = 6;
    flight_roll = 0;
    flight_climb = 0;
    clear_universe();

    set_init_matrix( rotmat );

    newship = add_new_ship( SHIP_COBRA3, pt, rotmat, ROT_0 );
    universe[newship].flags |= FLG_DEAD;

    for( i = 0; i < 5; i++ )
    {
        type = (rand255() & 1) ? SHIP_CARGO : SHIP_ALLOY;

        pt.x = (rand255() & 63) - 32;
        pt.y = (rand255() & 63) - 32;
        pt.z = -400;
        newship = add_new_ship( type, pt, rotmat, ROT_0 );

        universe[newship].rot.z = ((rand255() * 2) & 255) - 128;
        universe[newship].rot.x = ((rand255() * 2) & 255) - 128;
        universe[newship].velocity = rand255() & 15;
    }

    for( i = 0; i < 100; i++ )
    {
        gfx_clear_display();
        update_starfield();
        update_universe();
        gfx_display_centre_text( 190, "GAME OVER", 140, GFX_COL_GOLD );
        gfx_update_screen();
    }
}

/*
 * Draw a break pattern (for launching, docking and hyperspacing).
 * Just draw a very simple one for the moment.
 */

void display_break_pattern(void)
{
    int i;

    gfx_set_clip_region( VS_MIN_X, VS_MIN_Y, VS_MAX_X, VS_MAX_Y );
    gfx_clear_display();

    for( i = 0; i < 20; i++ )
    {
        gfx_draw_circle( 256, 192, 30 + i * 15, GFX_COL_WHITE - i );
        gfx_update_screen();
    }

    if( docked )
    {
        check_mission_brief();
        display_commander_status();
        update_console();
    }
    else
    {
        current_screen = SCR_FRONT_VIEW;
    }
}

void info_message(char *message)
{
    strcpy( message_string, message );
    message_count = 37;
    snd_play_sample( SND_BEEP );
}

int main(void)
{
    read_config_file();

    if( 1 == gfx_graphics_startup())
    {
        return 1;
    }

    /* Start the sound system... */
    snd_sound_startup();

    /* Do any setup necessary for the keyboard... */
    /* ... Which does nothing but return a 0 */
    kbd_keyboard_startup();

    finish = 0;
    auto_pilot = 0;

    while( !finish )
    {
        game_over = 0;/* elite.c */
        initialise_game();/* alg_main.c */
        /* After testing, this dock_player seems unnecessary */
        // dock_player();          /* space.c */
        update_console();/* space.c */

        current_screen = SCR_FRONT_VIEW;/* elite.h */
        run_first_intro_screen();/* alg_main.c */
        run_second_intro_screen();/* alg_main.c */

        old_cross_x = -1;/* Globals */
        old_cross_y = -1;/* Only referenced in this file */

        dock_player();/* space.c */
        display_commander_status();/* docked.c */

        while( !game_over )
        {
            snd_update_sound();/* sound.c */
            gfx_update_screen();/* alg_main.c */
            debug_keypresses();
            gfx_set_clip_region( VS_MIN_X, VS_MIN_Y, VS_MAX_X, VS_MAX_Y );/* alg_main.c */

            rolling = 0;/* Globals */
            climbing = 0;/* Only referenced in this file */

            handle_flight_keys();/* alg_main.c */

            if( game_paused )/* alg_main.c only */
            {
                continue;
            }

            if( message_count > 0 )/* alg_main.c only */
            {
                message_count--;
            }

            if( !rolling )
            {
                if( flight_roll > 0 )/* elite.c */
                {
                    decrease_flight_roll();/* space.c */
                }

                if( flight_roll < 0 )
                {
                    increase_flight_roll();/* space.c */
                }
            }

            if( !climbing )
            {
                if( flight_climb > 0 )/* elite.c */
                {
                    decrease_flight_climb();/* space.c */
                }

                if( flight_climb < 0 )
                {
                    increase_flight_climb();/* space.c */
                }
            }

            if( !docked )/* elite.c */
            {
                gfx_acquire_screen();/* alg_main.c */

                if(( current_screen == SCR_FRONT_VIEW ) || (current_screen == SCR_REAR_VIEW )  ||
                   ( current_screen == SCR_LEFT_VIEW ) || (current_screen == SCR_RIGHT_VIEW )  ||
                   ( current_screen == SCR_INTRO_ONE ) || (current_screen == SCR_INTRO_TWO )  ||
                   ( current_screen == SCR_GAME_OVER ))
                {
                    gfx_clear_display();/* alg_main.c */
                    update_starfield();/* stars.c */
                }

                if( auto_pilot )/* elite.c */
                {
                    auto_dock();/* pilot.c */
                    if(( mcount & 127 ) == 0 )/* alg_main.c */
                    {
                        /* alg_main */
                        info_message( "Docking Computers On" );
                    }
                }

                update_universe();/* space.c */

                if( docked )
                {
                    update_console();
                    gfx_release_screen();/* alg_main.c */
                    continue;
                }

                if(( current_screen == SCR_FRONT_VIEW ) || ( current_screen == SCR_REAR_VIEW )  ||
                   ( current_screen == SCR_LEFT_VIEW ) || (current_screen == SCR_RIGHT_VIEW ))
                {
                    if( draw_lasers )/* alg_main.c */
                    {
                        draw_laser_lines();/* swat.c */
                        draw_lasers--;
                    }

                    draw_laser_sights();/* alg_main.c */
                }

                if( message_count > 0 )
                {
                    /* alg_gfx.c */
                    gfx_display_centre_text( 358, message_string, 120, GFX_COL_WHITE );
                }

                if( hyper_ready )
                {
                    display_hyper_status(); /* space.c */
                    if(( mcount & 3 ) == 0 )
                    {
                        countdown_hyperspace(); /* space.c */
                    }
                }

                gfx_release_screen();

                mcount--;
                if( mcount < 0 )
                {
                    mcount = 255;
                }

                if( (mcount & 7) == 0 )
                {
                    regenerate_shields();/* space.c */
                }

                if( (mcount & 31) == 10 )
                {
                    if( energy < 50 )
                    {
                        info_message( "ENERGY LOW" );
                        /* sound.c */
                        snd_play_sample( SND_BEEP );
                    }

                    update_altitude();/* space.c */
                }

                if( (mcount & 31) == 20 )
                {
                    update_cabin_temp();/* space.c */
                }
                /* elite.c */
                if( (mcount == 0) && (!witchspace) )
                {
                    random_encounter();/* swat.c */
                }

                cool_laser();/* swat.c */
                time_ecm();/* swat.c */

                update_console();
            }

            if( SCR_BREAK_PATTERN == current_screen )
            {
                display_break_pattern();/* alg_main.c */
            }

            if( cross_timer > 0 )/* alg_main.c */
            {
                cross_timer--;
                if( 0 == cross_timer )
                {
                    show_distance_to_planet();/* docked.c */
                }
            }
            /* docked.c*//* alg_main.c */
            if(( cross_x != old_cross_x ) || ( cross_y != old_cross_y ))
            {
                /* draw_cross twice? */
                if( old_cross_x != -1 )
                {
                    /* alg_main.c */
                    draw_cross( old_cross_x, old_cross_y );
                }

                old_cross_x = cross_x;
                old_cross_y = cross_y;

                draw_cross( old_cross_x, old_cross_y );
            }
        }

        if( !finish )/* elite.c */
        {
            run_game_over_screen();/* alg_main.c */
        }
    }

    snd_sound_shutdown();

    gfx_graphics_shutdown();

    return 0;
}
