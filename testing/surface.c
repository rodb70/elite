/*
 * To compile:
 * gcc -ggdb surface.c -o surface -lSDL2 -I ../gfx-sdl
 */
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "col-256.h"

int main(void)
{
    int w = 800;
    int h = 600;

    SDL_Init( SDL_INIT_VIDEO );
    SDL_Window *window = SDL_CreateWindow( "Foo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w*2, h*2, SDL_WINDOW_RESIZABLE );
    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, 0 );
    SDL_Surface *surface = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0 );

    SDL_SetPaletteColors( surface->format->palette, (SDL_Color*)rgb_pallette, 0, 256 );

    uint8_t *offscreen = (uint8_t*) surface->pixels;
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
