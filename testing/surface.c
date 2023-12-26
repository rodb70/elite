/*
 * To compile:
 * gcc -ggdb surface.c -o surface -lSDL2 -I ../gfx-sdl
 * gcc -ggdb surface.c -o surface -lSDL2 -I ../gfx-sdl -L  ../build/elite/host-debug -lhagl
 * gcc -ggdb surface.c ../gfx-sdl/bmp.c -o surface -lSDL2 -I ../gfx-sdl -L  ../build/elite/host-debug -lhagl -I ../hagl/hagl/include
 * gcc -ggdb surface.c ../qdbmp/qdbmp/qdbmp.c  -o surface -lSDL2 -I ../gfx-sdl -L ../build/elite/host-debug -lhagl -I ../hagl/hagl/include -I ../qdbmp/qdbmp
 *
 */
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "qdbmp.h"
#include "col-256.h"
#include "hagl.h"

static uint8_t *offscreen;
typedef uint8_t  pixel_t;
typedef pixel_t (pixels_t)[600][800];
static pixels_t *vid_mem;

static void put_pixel(void *self, int16_t x0, int16_t y0, hagl_color_t color)
{
    vid_mem[0][y0][x0] = color;
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

int main(void)
{
    int w = 800;
    int h = 600;

    SDL_Init( SDL_INIT_VIDEO );
    SDL_Window *window = SDL_CreateWindow( "Foo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE );
    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, 0 );
    SDL_Surface *surface = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0 );

    SDL_SetPaletteColors( surface->format->palette, (SDL_Color*)rgb_pallette, 0, 256 );

    offscreen = (uint8_t*) surface->pixels;
    vid_mem = (pixels_t*) surface->pixels;
    for( int i = 0; i < h; i++ )
    {
        for( int j = 0; j < w / 256; j++ )
        {
            for( int c = 0; c < 256; c++ )
            {
                offscreen[ j * 256 + c ] = c;
            }
        }
        offscreen += surface->pitch;
    }

    {
        SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, surface );
        SDL_RenderCopy( renderer, texture, NULL, NULL );
        SDL_RenderPresent( renderer );
    }

    static hagl_backend_t backend;
    backend.buffer = offscreen;
    backend.buffer2 = offscreen;
    backend.width = w;
    backend.height = h;
    backend.depth = 8;
    backend.clip.x1 = w - 1;
    backend.clip.y1 = h - 1;
    backend.put_pixel = put_pixel;
    backend.color = color;

    /* read in bmp */
    static hagl_bitmap_t scanner_img;
    BMP *bmp = BMP_ReadFile( "scanner.bmp" );

    hagl_bitmap_init( &scanner_img, BMP_GetWidth(bmp), BMP_GetHeight(bmp), BMP_GetDepth(bmp), BMP_GetPixelData(bmp) );
    hagl_blit_xy( &backend, 0, 300, &scanner_img );
    {
        SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, surface );
        SDL_RenderCopy( renderer, texture, NULL, NULL );
        SDL_RenderPresent( renderer );
    }
//    uint8_t *pallette = BMP_GetPalletteData(bmp);
//    for( int i = 0; i < 1024; i += 4 )
//    {
//        printf( "rgb(%d,%d,%d), \\\n", pallette[i+2],pallette[i+1],pallette[i]);
//    }

    /* main loop */
    while( true )
    {
        SDL_Event e;
        while( SDL_PollEvent( &e ) > 0 )
        {
            switch( e.type )
            {
            case SDL_QUIT:
                return EXIT_SUCCESS;
            }
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, surface );
        SDL_RenderCopy( renderer, texture, NULL, NULL );
        SDL_RenderPresent( renderer );
        SDL_Delay( 100 );
    }
}

