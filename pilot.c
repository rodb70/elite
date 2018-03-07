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
 * pilot.c
 *
 * The auto-pilot code.  Used for docking computers and for
 * flying other ships to and from the space station.
 */

/*
 * In the original Elite this code was mixed in with the tactics routines.
 * I have split it out to make it more understandable and easier to maintain.
 */


#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "gfx.h"
#include "elite.h"
#include "vector.h"
#include "main.h"
#include "space.h"
#include "sound.h"


/*
 * Fly to a given point in space.
 */

void fly_to_vector (struct univ_object *ship, struct vector vec)
{
    struct vector nvec;
    double direction;
    double dir;
    int rat;
    double rat2;
    double cnt2;

    /* What are these? */
    rat = 3;/* Magic Number */
    rat2 = 0.1666;/* Magic Number */
    cnt2 = 0.8055;/* Magic Number */

    nvec = unit_vector(&vec);
    direction = vector_dot_product (&nvec, &ship->rotmat[2]); 

    if (direction < -0.6666)/* Magic Number */
        rat2 = 0;

    dir = vector_dot_product (&nvec, &ship->rotmat[1]);

    if (direction < -0.861)/* Magic Number */
    {
        ship->rot.x = (dir < 0) ? 7 : -7;
        ship->rot.z = 0;
        return; 
    }

    ship->rot.x = 0;

    if ((fabs(dir) * 2) >= rat2)
    {
        ship->rot.x = (dir < 0) ? rat : -rat;
    }

    if (abs(ship->rot.z) < 16)
    {
        dir = vector_dot_product (&nvec, &ship->rotmat[0]);

        ship->rot.z = 0;

        if ((fabs(dir) * 2) >= rat2)
        {
            ship->rot.z = (dir < 0) ? rat : -rat;

            if (ship->rot.x < 0)
                ship->rot.z = -ship->rot.z;
        }               
    }

    if (direction <= -0.167)/* Magic Number */
    {
        ship->acceleration = -1;
        return;
    }

    if (direction >= cnt2)
    {
        ship->acceleration = 3;
        return;
    }
}



/*
 * Fly towards the planet.
 */

void fly_to_planet (struct univ_object *ship)
{
    struct vector vec;

    vec.x = universe[0].location.x - ship->location.x;
    vec.y = universe[0].location.y - ship->location.y;
    vec.z = universe[0].location.z - ship->location.z;

    fly_to_vector (ship, vec);      
}


/*
 * Fly to a point in front of the station docking bay.
 * Done prior to the final stage of docking.
 */


void fly_to_station_front (struct univ_object *ship)
{
    struct vector vec;

    vec.x = universe[1].location.x - ship->location.x;
    vec.y = universe[1].location.y - ship->location.y;
    vec.z = universe[1].location.z - ship->location.z;

    vec.x += universe[1].rotmat[2].x * 768;/* Magic Number */
    vec.y += universe[1].rotmat[2].y * 768;
    vec.z += universe[1].rotmat[2].z * 768;

    fly_to_vector (ship, vec);      
}


/*
 * Fly towards the space station.
 */

void fly_to_station (struct univ_object *ship)
{
    struct vector vec;

    vec.x = universe[1].location.x - ship->location.x;
    vec.y = universe[1].location.y - ship->location.y;
    vec.z = universe[1].location.z - ship->location.z;

    fly_to_vector (ship, vec);      
}


/*
 * Final stage of docking.
 * Fly into the docking bay.
 */

void fly_to_docking_bay (struct univ_object *ship)
{
    struct vector diff;
    struct vector vec;
    double dir;

    diff.x = ship->location.x - universe[1].location.x;
    diff.y = ship->location.y - universe[1].location.y;
    diff.z = ship->location.z - universe[1].location.z;

    vec = unit_vector (&diff);      

    ship->rot.x = 0;

    if (ship->type < 0)
    {
        ship->rot.z = 1;
        if (((vec.x >= 0) && (vec.y >= 0)) ||
                ((vec.x < 0) && (vec.y < 0)))
        {
            ship->rot.z = -ship->rot.z;
        }

        if (fabs(vec.x) >= 0.0625)/* Magic Number */
        {
            ship->acceleration = 0;
            ship->velocity = 1;
            return;
        }

        if (fabs(vec.y) > 0.002436)/* Magic Number */
            ship->rot.x = (vec.y < 0) ? -1 : 1;

        if (fabs(vec.y) >= 0.0625)/* Magic Number */
        {
            ship->acceleration = 0;
            ship->velocity = 1;
            return;
        }
    }

    ship->rot.z = 0;

    dir = vector_dot_product (&ship->rotmat[0], &universe[1].rotmat[1]);

    if (fabs(dir) >= 0.9166)/* Magic Number */
    {
        ship->acceleration++;
        ship->rot.z = 127;
        return;
    }

    ship->acceleration = 0;
    ship->rot.z = 0;
}


/*
 * Fly a ship to the planet or to the space station and dock it.
 */

void auto_pilot_ship (struct univ_object *ship)
{
    struct vector diff;
    struct vector vec;
    double dist;
    double dir;

    if ((ship->flags & FLG_FLY_TO_PLANET) ||
            ((ship_count[SHIP_CORIOLIS] == 0) && (ship_count[SHIP_DODEC] == 0)))
    {
        fly_to_planet (ship);
        return;
    }

    diff.x = ship->location.x - universe[1].location.x;     
    diff.y = ship->location.y - universe[1].location.y;     
    diff.z = ship->location.z - universe[1].location.z;     

    dist = sqrt (pow (diff.x,2) + pow (diff.y, 2) + pow (diff.z, 2));

    if (dist < 160)/* Magic Number */
    {
        ship->flags |= FLG_REMOVE;              // Ship has docked.
        return;
    }       

    vec = unit_vector (&diff);      
    dir = vector_dot_product (&universe[1].rotmat[2], &vec);

    if (dir < 0.9722)   /* Magic Number */
    {
        fly_to_station_front (ship);
        return;
    }

    dir = vector_dot_product (&ship->rotmat[2], &vec);

    if (dir < -0.9444)/* Magic Number */
    {
        fly_to_docking_bay (ship);
        return;
    }

    fly_to_station (ship);
}


void engage_auto_pilot (void)
{
    if (auto_pilot || witchspace || hyper_ready)
        return; 

    auto_pilot = 1;
    snd_play_midi (SND_BLUE_DANUBE, 1);
}


void disengage_auto_pilot (void)
{
    if (auto_pilot)
    {
        auto_pilot = 0;
        snd_stop_midi();
    }
}
