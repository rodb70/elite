#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "config.h"
#include "gfx.h"
#include "col-256.h"
#include "hagl.h"
#include "font6x9.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *surface;
    hagl_backend_t backend;

} monitor_t;

//DATAFILE *datafile;

typedef uint8_t  pixel_t;
typedef pixel_t (pixels_t)[SCREEN_HEIGHT][SCREEN_WIDTH];
pixels_t *vid_mem;

hagl_backend_t* backend;

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

const wchar_t *GetWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    static wchar_t wc[256];
    mbstowcs( wc, c, cSize );

    return wc;
}

static void put_pixel(void *self, int16_t x0, int16_t y0, hagl_color_t color)
{
    (void)self;
    gfx_fast_plot_pixel( x0, y0, color );
}

static hagl_color_t color(void *self, uint8_t r, uint8_t g, uint8_t b)
{
    (void)self;
    uint32_t color = (r << 16) | (g << 8) | (b);
    int i = 0;
    for( ; i < 256; i++ )
    {
        if( color == rgb_pallette[ i ])
        {
            break;
        }
    }

    return i;
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

    m->window = SDL_CreateWindow( "Newlife", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SDL_WINDOW_RESIZABLE );
    assert( m->window );

    m->renderer = SDL_CreateRenderer( m->window, -1, SDL_RENDERER_SOFTWARE );
    assert( m->renderer );

    m->surface = SDL_CreateRGBSurface( SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 8, 0, 0, 0, 0 );
    assert( m->surface );
    SDL_SetPaletteColors( m->surface->format->palette, (SDL_Color*)rgb_pallette, 0, 256 );
    //SDL_SetTextureBlendMode( m->texture, SDL_BLENDMODE_BLEND );

    vid_mem = (pixels_t*) m->surface->pixels;

    backend = &m->backend;

    backend->width = SCREEN_WIDTH;
    backend->height = SCREEN_HEIGHT;
    backend->depth = 8;
    backend->put_pixel = put_pixel;
    backend->color = color;

    return 0;
}

void gfx_graphics_shutdown (void)
{
}


void gfx_update_screen (void)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface( m->renderer, m->surface );
    SDL_RenderCopy( m->renderer, texture, NULL, NULL );
    SDL_RenderPresent( m->renderer );
}


void gfx_acquire_screen (void)
{
}


void gfx_release_screen (void)
{
}


void gfx_plot_pixel (int x, int y, int col)
{
    gfx_fast_plot_pixel( x, y, col );
}


void gfx_fast_plot_pixel (int x, int y, int col)
{
    vid_mem[ 0 ][ y + GFX_Y_OFFSET ][ x + GFX_X_OFFSET ] = col;
}


void gfx_draw_filled_circle (int cx, int cy, int radius, int circle_colour)
{
    hagl_fill_circle( backend, cx, cy, radius, circle_colour);
}


void gfx_draw_circle (int cx, int cy, int radius, int circle_colour)
{
    hagl_draw_circle( backend, cx, cy, radius, circle_colour);
}


void gfx_draw_line (int x1, int y1, int x2, int y2)
{
    hagl_draw_line(backend, x1, y1, x2, y2, WHITE);
}


void gfx_draw_colour_line (int x1, int y1, int x2, int y2, int line_colour)
{
    hagl_draw_line(backend, x1, y1, x2, y2, line_colour);
}


void gfx_draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3, int col)
{
    hagl_draw_triangle(backend, x1, y1, x2, y2, x3, y3, col);
}


void gfx_draw_rectangle (int tx, int ty, int bx, int by, int col)
{
    hagl_draw_rectangle(backend, tx, ty, bx, by, col);
}


void gfx_display_text (int x, int y, char *txt)
{
    hagl_put_text(backend, GetWC( txt ), x, y, WHITE, font6x9);
}


void gfx_display_colour_text (int x, int y, char *txt, int col)
{
    hagl_put_text(backend, GetWC( txt ), x, y, col, font6x9);
}


void gfx_display_centre_text (int y, char *str, int psize, int col)
{
    int x = ( strlen(str) * 6 ) / 2;
    hagl_put_text(backend, GetWC( str ), x, y, col, font6x9);
}


void gfx_clear_display (void)
{
    hagl_fill_rectangle( backend, GFX_VIEW_TX, GFX_VIEW_TY, GFX_VIEW_BX, GFX_VIEW_BY, BLACK );
}


void gfx_clear_text_area (void)
{
    hagl_fill_rectangle( backend, GFX_VIEW_TX, GFX_VIEW_TY, GFX_VIEW_BX, GFX_VIEW_BY, BLACK);
}


void gfx_clear_area (int tx, int ty, int bx, int by)
{
    hagl_fill_rectangle( backend, tx, ty, bx, by, BLACK);
}


void gfx_display_pretty_text (int tx, int ty, int bx, int by, char *txt)
{
    hagl_surface_t surface;
    surface = *(hagl_surface_t*)backend;

    hagl_set_clip( &surface, tx, ty, bx, by );
    hagl_put_text( &surface, GetWC( txt ), tx, ty, WHITE, font6x9 );
}


void gfx_draw_scanner (void)
{
}


void gfx_set_clip_region (int tx, int ty, int bx, int by)
{
    hagl_set_clip( backend, tx, ty, bx, by );
}


void gfx_polygon (int num_points, int *poly_list, int face_colour)
{
    int16_t points[ num_points * 2 ];
    int i;
    int x,y;

    x = 0;
    y = 1;
    for( i = 0; i < num_points; i++ )
    {
        points[ x ] = poly_list[ x ];
        points[ y ] = poly_list[ y ];
        x += 2;
        y += 2;
    }

    hagl_draw_polygon( backend, num_points, points, face_colour );
}

void gfx_draw_sprite (int sprite_no, int x, int y)
{
#if 0
    BITMAP *sprite_bmp;

    switch (sprite_no)
    {
        case IMG_GREEN_DOT:
            sprite_bmp = datafile[GRNDOT].dat;
            break;

        case IMG_RED_DOT:
            sprite_bmp = datafile[REDDOT].dat;
            break;

        case IMG_BIG_S:
            sprite_bmp = datafile[SAFE].dat;
            break;

        case IMG_ELITE_TXT:
            sprite_bmp = datafile[ELITETXT].dat;
            break;

        case IMG_BIG_E:
            sprite_bmp = datafile[ECM].dat;
            break;

        case IMG_BLAKE:
            sprite_bmp = datafile[BLAKE].dat;
            break;

        case IMG_MISSILE_GREEN:
            sprite_bmp = datafile[MISSILE_G].dat;
            break;

        case IMG_MISSILE_YELLOW:
            sprite_bmp = datafile[MISSILE_Y].dat;
            break;

        case IMG_MISSILE_RED:
            sprite_bmp = datafile[MISSILE_R].dat;
            break;

        default:
            return;
    }

    if (x == -1)
    {
        x = ((256 * GFX_SCALE) - sprite_bmp->w) / 2;
    }

    //FIXME: draw_sprite (gfx_screen, sprite_bmp, x + GFX_X_OFFSET, y + GFX_Y_OFFSET);
#endif
}


int gfx_request_file (char *title, char *path, char *ext)
{
    /* show request */
    return 0;
}


