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
 */

#ifndef SHIPDATA_H
#define SHIPDATA_H

#include "vector.h"

/* Ship Point? */
struct ship_point
{
    int x;
    int y;
    int z;
    int dist;
    int face1;
    int face2;
    int face3;
    int face4;
};

/* Ship line? */
struct ship_line
{
    int dist;
    int face1;
    int face2;
    int start_point;
    int end_point;
};

/* Ship Face Normal? */
struct ship_face_normal
{
    int dist;
    int x;
    int y;
    int z;
};

struct ship_data
{
    char name[32];
    int num_points;
    int num_lines;
    int num_faces;
    int max_loot;
    int scoop_type;
    double size;
    int front_laser;
    int bounty;
    int vanish_point;
    int energy;
    int velocity;
    int missiles;
    int laser_strength;
    struct ship_point *points;
    struct ship_line *lines;
    struct ship_face_normal *normals;
};

enum {
    SHIP_SUN            =   -2,
    SHIP_PLANET         =   -1,	
    SHIP_MISSILE        =   1,
    SHIP_CORIOLIS,
    SHIP_ESCAPE_CAPSULE,
    SHIP_ALLOY,
    SHIP_CARGO,
    SHIP_BOULDER,
    SHIP_ASTEROID,
    SHIP_ROCK,
    SHIP_SHUTTLE,
    SHIP_TRANSPORTER,
    SHIP_COBRA3,
    SHIP_PYTHON,
    SHIP_BOA,
    SHIP_ANACONDA,
    SHIP_HERMIT,
    SHIP_VIPER,
    SHIP_SIDEWINDER,
    SHIP_MAMBA,
    SHIP_KRAIT,
    SHIP_ADDER,
    SHIP_GECKO,
    SHIP_COBRA1,
    SHIP_WORM,
    SHIP_COBRA3_LONE,
    SHIP_ASP2,
    SHIP_PYTHON_LONE,
    SHIP_FER_DE_LANCE,
    SHIP_MORAY,
    SHIP_THARGOID,
    SHIP_THARGLET,
    SHIP_CONSTRICTOR,
    SHIP_COUGAR,
    SHIP_DODEC,
    /* Add new ships/objects above this line */
    NO_OF_SHIPS = SHIP_DODEC
};

extern struct ship_data missile_data;
extern struct ship_data coriolis_data;
extern struct ship_data esccaps_data;
extern struct ship_data alloy_data;
extern struct ship_data cargo_data;
extern struct ship_data boulder_data;
extern struct ship_data asteroid_data;
extern struct ship_data rock_data;
extern struct ship_data orbit_data;
extern struct ship_data transp_data;
extern struct ship_data cobra3a_data;
extern struct ship_data pythona_data;
extern struct ship_data boa_data;
extern struct ship_data anacnda_data;
extern struct ship_data hermit_data;
extern struct ship_data viper_data;
extern struct ship_data sidewnd_data;
extern struct ship_data mamba_data;
extern struct ship_data krait_data;
extern struct ship_data adder_data;
extern struct ship_data gecko_data;
extern struct ship_data cobra1_data;
extern struct ship_data worm_data;
extern struct ship_data cobra3b_data;
extern struct ship_data asp2_data;
extern struct ship_data pythonb_data;
extern struct ship_data ferdlce_data;
extern struct ship_data moray_data;
extern struct ship_data thargoid_data;
extern struct ship_data thargon_data;
extern struct ship_data constrct_data;
extern struct ship_data cougar_data;
extern struct ship_data dodec_data;

#endif
