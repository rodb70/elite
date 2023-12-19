/*
 * joystick.c
 *
 *  Created on: 18 Dec 2023
 *      Author: rod
 */
#include <libgen.h>

char * get_filename(char *path)
{
    return basename( path );
}

void joy_poll_joystick( void )
{
    /*if (have_joystick)
    {
        poll_joystick();

        if (joy[0].stick[0].axis[1].d1)
        {
            arrow_up();
        }

        if (joy[0].stick[0].axis[1].d2)
        {
            arrow_down();
        }

        if (joy[0].stick[0].axis[0].d1)
        {
            arrow_left();
        }

        if (joy[0].stick[0].axis[0].d2)
        {
            arrow_right();
        }

        if (joy[0].button[0].b)
        {
            kbd_fire_pressed = 1;
        }

        if (joy[0].button[1].b)
        {
            kbd_inc_speed_pressed = 1;
        }

        if (joy[0].button[2].b)
        {
            kbd_dec_speed_pressed = 1;
        }
    }*/
}
