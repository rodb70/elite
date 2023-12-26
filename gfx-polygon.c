/*
 * gfx-polygon.c
 *
 *  Created on: 17 Dec 2023
 *      Author: rod
 */

#include "gfx.h"

#define MAX_POLYS    100

static int start_poly;
static int total_polys;

struct poly_data
{
    int z;
    int no_points;
    int face_colour;
    int point_list[16];
    int next;
};

static struct poly_data poly_chain[MAX_POLYS];

void gfx_start_render (void)
{
    start_poly = 0;
    total_polys = 0;
}


void gfx_render_polygon (int num_points, int *point_list, int face_colour, int zavg)
{
    int i;
    int x;
    int nx;

    if( MAX_POLYS == total_polys )
    {
        return;
    }

    x = total_polys;
    total_polys++;

    poly_chain[ x ].no_points = num_points;
    poly_chain[ x ].face_colour = face_colour;
    poly_chain[ x ].z = zavg;
    poly_chain[ x ].next = -1;

    for (i = 0; i < 16; i++)
    {
        poly_chain[x].point_list[i] = point_list[i];
    }

    if (x == 0)
    {
        return;
    }

    if (zavg > poly_chain[start_poly].z)
    {
        poly_chain[x].next = start_poly;
        start_poly = x;
        return;
    }

    for (i = start_poly; poly_chain[i].next != -1; i = poly_chain[i].next)
    {
        nx = poly_chain[i].next;

        if (zavg > poly_chain[nx].z)
        {
            poly_chain[i].next = x;
            poly_chain[x].next = nx;
            return;
        }
    }

    poly_chain[i].next = x;
}


void gfx_render_line (int x1, int y1, int x2, int y2, int dist, int col)
{
    int point_list[ 4 ];

    point_list[ 0 ] = x1;
    point_list[ 1 ] = y1;
    point_list[ 2 ] = x2;
    point_list[ 3 ] = y2;

    gfx_render_polygon( 2, point_list, col, dist );
}


void gfx_finish_render (void)
{
    int num_points;
    int *pl;
    int i;
    int col;

    if( 0 == total_polys )
    {
        return;
    }

    for( i = start_poly; i != -1; i = poly_chain[ i ].next )
    {
        num_points = poly_chain[ i ].no_points;
        pl = poly_chain[ i ].point_list;
        col = poly_chain[ i ].face_colour;

        if( 2 == num_points )
        {
            gfx_draw_colour_line( pl[ 0 ], pl[ 1 ], pl[ 2 ], pl[ 3 ], col );
            continue;
        }

        gfx_polygon( num_points, pl, col );
    };
}

