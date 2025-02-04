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
#include "elite.h"
#include "alg_data.h"
#include "keyboard.h"
#include "qdbmp.h"


typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *surface;
    hagl_backend_t backend;

} MONITOR;

typedef uint8_t  pixel_t;
typedef pixel_t (pixels_t)[SCREEN_HEIGHT][SCREEN_WIDTH];
static pixels_t *vid_mem;
static int xor = 0;

static hagl_backend_t* backend;
typedef struct
{
    hagl_bitmap_t bitmap;
    BMP *bmp;
} DATA_FILES;

static hagl_bitmap_t scanner_image;
static BMP *scanner_bmp;

static DATA_FILES data_file[ THEME + 1 ];

static MONITOR monitor = { 0 };
static MONITOR *m = &monitor;

static void SdlUpdateScreen( void )
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface( m->renderer, m->surface );
    SDL_RenderCopy( m->renderer, texture, NULL, NULL );
    SDL_RenderPresent( m->renderer );
    SDL_DestroyTexture( texture );
}

int QuitFilter(void *userdata, SDL_Event *event)
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
    const size_t cSize = strlen( c )+1;
    static wchar_t wc[ 1024 ] = {0};
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

    SDL_SetEventFilter( QuitFilter, NULL );

    m->window = SDL_CreateWindow( "Newlife", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4, SDL_WINDOW_RESIZABLE );
    assert( m->window );

    m->renderer = SDL_CreateRenderer( m->window, -1, SDL_RENDERER_SOFTWARE );
    assert( m->renderer );

    m->surface = SDL_CreateRGBSurface( SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 8, 0, 0, 0, 0 );
    assert( m->surface );
    SDL_SetPaletteColors( m->surface->format->palette, (SDL_Color*)rgb_pallette, 0, 256 );

    vid_mem = (pixels_t*) m->surface->pixels;

    scanner_bmp = BMP_ReadFile( scanner_filename );
    hagl_bitmap_init( &scanner_image, BMP_GetWidth(scanner_bmp), BMP_GetHeight(scanner_bmp), BMP_GetDepth(scanner_bmp), BMP_GetPixelData(scanner_bmp));

    backend = &m->backend;

    backend->width = SCREEN_WIDTH;
    backend->height = SCREEN_HEIGHT;
    backend->depth = 8;
    backend->put_pixel = put_pixel;
    backend->color = color;
    backend->clip.x1 = SCREEN_WIDTH - 1;
    backend->clip.y1 = SCREEN_HEIGHT - 1;

    // data_file
    for( int i = 0; i < THEME; i++ )
    {
        char *filename = NULL;
        switch( i )
        {
        case BLAKE:
            filename = "data/blake.bmp";
            break;
        case DANUBE:
            continue;
        case ECM:
            filename = "data/ecm.bmp";
            break;
        case ELITE_1:
        case ELITE_2:
            continue;
        case ELITETXT :
            filename = "data/elitetx3.bmp";
            break;
        case FRONTV :
            continue;
            break;
        case GRNDOT :
            filename = "data/greendot.bmp";
            break;
        case MISSILE_G :
            filename = "data/missgrn.bmp";
            break;
        case MISSILE_R:
            filename = "data/missred.bmp";
            break;
        case MISSILE_Y:
            filename = "data/missyell.bmp";
            break;
        case REDDOT:
            filename = "data/reddot.bmp";
            break;
        case SAFE:
            filename = "data/safe.bmp";
            break;
        }
        data_file[ i ].bmp = BMP_ReadFile( filename );
        hagl_bitmap_init( &data_file[ i ].bitmap, BMP_GetWidth(data_file[ i ].bmp), BMP_GetHeight(data_file[ i ].bmp), BMP_GetDepth(data_file[ i ].bmp), BMP_GetPixelData(data_file[ i ].bmp));
    }

    return 0;
}

void gfx_xor_mode( int on )
{
    xor = on;
}

void gfx_graphics_shutdown (void)
{
    for( int i = 0; i < THEME; i++ )
    {
        BMP_Free( data_file[ i ].bmp );
    }
    /* Make sure to eventually release the surface resource */
    BMP_Free( scanner_bmp );
}


void gfx_update_screen (void)
{
    SdlUpdateScreen();

    for( int dly = 0; dly < speed_cap; dly += 5 )
    {
        kbd_poll_keyboard();
        SDL_Delay( 5 );
    }
}


void gfx_acquire_screen (void)
{
}


void gfx_release_screen (void)
{
}


void gfx_plot_pixel (int x, int y, int col)
{
    hagl_put_pixel( backend, x, y, col );
}


void gfx_fast_plot_pixel( int x, int y, int col )
{
    if( 0 != xor )
    {
        vid_mem[ 0 ][ y + GFX_Y_OFFSET ][ x + GFX_X_OFFSET ] ^= col;
    }
    else
    {
        vid_mem[ 0 ][ y + GFX_Y_OFFSET ][ x + GFX_X_OFFSET ] = col;
    }
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
    hagl_draw_line( backend, x1, y1, x2, y2, WHITE);
}


void gfx_draw_colour_line (int x1, int y1, int x2, int y2, int line_colour)
{
    hagl_draw_line( backend, x1, y1, x2, y2, line_colour);
}


void gfx_draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3, int col)
{
    hagl_draw_triangle( backend, x1, y1, x2, y2, x3, y3, col);
}


void gfx_draw_rectangle (int tx, int ty, int bx, int by, int col)
{
    hagl_draw_rectangle( backend, tx, ty, bx, by, col);
}

void gfx_draw_fill_rectangle (int tx, int ty, int bx, int by, int col)
{
    hagl_fill_rectangle( backend, tx, ty, bx, by, col);
}

void gfx_display_text (int x, int y, char *txt)
{
    hagl_put_text( backend, GetWC( txt ), x, y, WHITE, font6x9);
}


void gfx_display_colour_text( int x, int y, char *txt, int col )
{
    hagl_put_text( backend, GetWC( txt ), x, y, col, font6x9);
}


void gfx_display_centre_text( int y, char *str, int psize, int col )
{
    int x = GFX_X_CENTRE - (( strlen( str ) * 6 ) / 2);
    (void)psize;
    hagl_put_text( backend, GetWC( str ), x, y, col, font6x9 );
}


void gfx_clear_display( void )
{
    hagl_fill_rectangle( backend, GFX_VIEW_TX, GFX_VIEW_TY, GFX_VIEW_BX, GFX_VIEW_BY, BLACK );
}


void gfx_clear_text_area( void )
{
    //hagl_fill_rectangle( backend, GFX_VIEW_TX, GFX_VIEW_TY, GFX_VIEW_BX, GFX_VIEW_BY, BLACK);
}


void gfx_clear_area (int tx, int ty, int bx, int by)
{
    hagl_fill_rectangle( backend, tx, ty, bx, by, BLACK);
}


void gfx_display_pretty_text (int tx, int ty, int bx, int by, char *txt)
{
    char *ttxt = strdup( txt );
    char *tstr = ttxt;
    const int y_chars = ( bx - tx ) / 6;
    hagl_surface_t surface;
    surface = *(hagl_surface_t*)backend;

    hagl_set_clip( &surface, tx, ty, bx, by );
    for( int pos = y_chars; pos < (int)strlen( tstr ) && y_chars < (int)strlen( tstr ); pos = y_chars )
    {
        while( pos > 0 && ' ' != tstr[ pos ])
        {
            pos--;
        }

        tstr[ pos ] = '\0';
        hagl_put_text( &surface, GetWC( tstr ), tx, ty, WHITE, font6x9 );

        tstr = tstr + pos + 1;
        ty += 9;
    }
    hagl_put_text( &surface, GetWC( tstr ), tx, ty, WHITE, font6x9 );

    free( ttxt );
}


void gfx_draw_scanner (void)
{
    hagl_blit_xy( backend, 0, 385, &scanner_image );
}


void gfx_set_clip_region (int tx, int ty, int bx, int by)
{
    hagl_set_clip( backend, tx, ty, bx-1, by-1 );
}


void gfx_polygon( int num_points, int *poly_list, int face_colour )
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

    //hagl_draw_polygon( backend, num_points, points, face_colour );
    hagl_fill_polygon( backend, num_points, points, face_colour );
}

void gfx_draw_sprite (int sprite_no, int x, int y)
{
    if( -1 == x )
    {
        x = (( 256 * GFX_SCALE ) - ( BMP_GetWidth( data_file[ sprite_no ].bmp ))) / 2;
    }
    hagl_blit_xy( backend, x, y, &data_file[ sprite_no ].bitmap );
}


int gfx_request_file (char *title, char *path, char *ext)
{
    (void)ext;
    int keyasc = 0;

    for( char *pth = path; *pth != 0; pth++ )
    {
        if(( *pth >= 'A' ) && ( *pth <= 'Z' ))
        {
            *pth |= 0x20;
        }
    }

    SDL_Delay( 250 );
    while( 0 != kbd_read_key())
    {
        continue;
    }
    kbd_backspace_pressed = 0;
    kbd_enter_pressed = 0;

    /* show request */
    gfx_draw_rectangle( 84, 95, 406,175, GFX_COL_RED_3 );
    gfx_draw_rectangle( 85, 96, 405, 109, GFX_COL_RED_4 );
    gfx_draw_rectangle( 85, 109, 405, 174, GFX_COL_RED_4 );
    gfx_display_pretty_text( 88, 99, 404, 108, title );
    gfx_draw_rectangle( 98, 129, 392, 142, GFX_COL_RED_4 );
    gfx_display_pretty_text( 100, 131, 391, 141, path );
    SdlUpdateScreen();

    while( 1 )
    {
        keyasc = kbd_read_key();
        if( kbd_backspace_pressed )
        {
            gfx_clear_area( 100, 131, 391, 141 );
            if( path[ 0 ] != '\0' )
            {
                path[ strlen( path ) - 1 ] = '\0';
            }
            kbd_backspace_pressed = 0;
            keyasc = 0;
        }

        if((( keyasc >= (int)'0' ) && ( keyasc <= (int)'z' )) || ( '.' == keyasc ))
        {
            if(( keyasc >= 'A' ) && ( keyasc <= 'Z' ))
            {
                keyasc |= 0x20;
            }
            int len = strlen( path );
            if( len < 254 )
            {
                path[ len ] = (char)keyasc;
                path[ len + 1 ] = '\0';

            }
        }

        gfx_display_pretty_text( 100, 131, 391, 141, path );

        SdlUpdateScreen();

        if( kbd_enter_pressed )
        {
            kbd_enter_pressed = 0;
            keyasc = 1;
            gfx_draw_fill_rectangle( 84, 95, 406,175, GFX_COL_BLACK );
            break;
        }
    }

    return keyasc;
}


