#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <assert.h>

#include "config.h"
#include "gfx.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint16_t tft_fb[ SCREEN_WIDTH ][ SCREEN_HEIGHT ];

} monitor_t;

uint16_t *vid_mem;


static monitor_t monitor = { 0 };
monitor_t *m = &monitor;

int quit_filter(void *userdata, SDL_Event *event)
{
    (void) userdata;

    if( SDL_WINDOWEVENT == event->type )
    {
        if( SDL_WINDOWEVENT_CLOSE == event->window.event )
        {
            exit( 0 );
        }
    }
    else if( SDL_QUIT == event->type )
    {
        exit( 0 );
    }

    return 1;
}

int gfx_graphics_startup(void)
{
    /* Initialise the SDL */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialise! SDL_Error: %s\n", SDL_GetError() );
        exit( -1 );
    }

    SDL_SetEventFilter( quit_filter, NULL );

    m->window = SDL_CreateWindow( "2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH*3, SCREEN_HEIGHT*3, 0 );
    assert( m->window );

    m->renderer = SDL_CreateRenderer( m->window, -1, SDL_RENDERER_SOFTWARE );
    assert( m->renderer );

    m->texture = SDL_CreateTexture( m->renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STATIC,
                                    SCREEN_WIDTH, SCREEN_HEIGHT );
    assert( m->texture );

    SDL_SetTextureBlendMode( m->texture, SDL_BLENDMODE_BLEND );

    vid_mem = &m->tft_fb[0][0];

    return 0; //(void*) m->tft_fb;
}

void gfx_graphics_shutdown (void)
{
}


void gfx_update_screen (void)
{
}


void gfx_acquire_screen (void)
{
}


void gfx_release_screen (void)
{
}


void gfx_plot_pixel (int x, int y, int col)
{
    const SDL_Point point = { .x = x, .y = y };

    SDL_RenderDrawPoints( m->renderer, &point, 1 );
}


void gfx_fast_plot_pixel (int x, int y, int col)
{
    m->tft_fb[x][y] = col;
}


void gfx_draw_filled_circle (int cx, int cy, int radius, int circle_colour)
{
}


void gfx_draw_circle (int cx, int cy, int radius, int circle_colour)
{
}


void gfx_draw_line (int x1, int y1, int x2, int y2)
{
}


void gfx_draw_colour_line (int x1, int y1, int x2, int y2, int line_colour)
{
}


void gfx_draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3, int col)
{
}


void gfx_draw_rectangle (int tx, int ty, int bx, int by, int col)
{
}


void gfx_display_text (int x, int y, char *txt)
{
}


void gfx_display_colour_text (int x, int y, char *txt, int col)
{
}


void gfx_display_centre_text (int y, char *str, int psize, int col)
{
}


void gfx_clear_display (void)
{
}


void gfx_clear_text_area (void)
{
}


void gfx_clear_area (int tx, int ty, int bx, int by)
{
}


void gfx_display_pretty_text (int tx, int ty, int bx, int by, char *txt)
{
}


void gfx_draw_scanner (void)
{
}


void gfx_set_clip_region (int tx, int ty, int bx, int by)
{
}


void gfx_polygon (int num_points, int *poly_list, int face_colour)
{
}


void gfx_draw_sprite (int sprite_no, int x, int y)
{
}


void gfx_start_render (void)
{
}


void gfx_render_polygon (int num_points, int *point_list, int face_colour, int zavg)
{
}


void gfx_render_line (int x1, int y1, int x2, int y2, int dist, int col)
{
}


void gfx_finish_render (void)
{
}


int gfx_request_file (char *title, char *path, char *ext)
{
}


