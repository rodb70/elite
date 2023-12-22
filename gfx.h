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


/**
 *
 * Elite - The New Kind.
 *
 * The code in this file has not been derived from the original Elite code.
 * Written by C.J.Pinder 1999/2000.
 *
 **/


#ifndef GFX_H
#define GFX_H
#include "alg_data.h"

#ifdef RES_512_512

#define GFX_SCALE		(2)
#define GFX_X_OFFSET            (0)
#define GFX_Y_OFFSET            (0)
#define GFX_X_CENTRE            (256)
#define GFX_Y_CENTRE            (192)

#define GFX_VIEW_TX		1
#define GFX_VIEW_TY		1
#define GFX_VIEW_BX		509
#define GFX_VIEW_BY		381

#endif

#ifdef RES_800_600

#define GFX_SCALE       (2)
#define GFX_X_OFFSET    (144)
#define GFX_Y_OFFSET    (44)
#define GFX_X_CENTRE    (256)
#define GFX_Y_CENTRE    (192)

#define GFX_VIEW_TX     1
#define GFX_VIEW_TY     1
#define GFX_VIEW_BX     509
#define GFX_VIEW_BY     381

#endif

#ifndef GFX_SCALE

#define GFX_SCALE               (1)
#define GFX_X_OFFSET            (0)
#define GFX_Y_OFFSET            (0)
#define GFX_X_CENTRE            (128)
#define GFX_Y_CENTRE            (96)

#define GFX_VIEW_TX     (1)
#define GFX_VIEW_TY     (1)
#define GFX_VIEW_BX     (253)
#define GFX_VIEW_BY     (191)

#endif
 

#ifdef ORIGINAL_PALLETTE
#define GFX_COL_BLACK		0
#define GFX_COL_DARK_RED	28
#define GFX_COL_WHITE		255
#define GFX_COL_GOLD		39
#define GFX_COL_RED		49
#define GFX_COL_CYAN		11

#define GFX_COL_GREY_1		248
#define GFX_COL_GREY_2		235
#define GFX_COL_GREY_3		234
#define GFX_COL_GREY_4		237

#define GFX_COL_BLUE_1		45
#define GFX_COL_BLUE_2		46
#define GFX_COL_BLUE_3		133
#define GFX_COL_BLUE_4		4

#define GFX_COL_RED_3		1
#define GFX_COL_RED_4		71

#define GFX_COL_WHITE_2		242

#define GFX_COL_YELLOW_1	37
#define GFX_COL_YELLOW_2	39
#define GFX_COL_YELLOW_3	89
#define GFX_COL_YELLOW_4	160
#define GFX_COL_YELLOW_5	251

#define GFX_ORANGE_1		76
#define GFX_ORANGE_2		77
#define GFX_ORANGE_3		122

#define GFX_COL_GREEN_1		2
#define GFX_COL_GREEN_2		17
#define GFX_COL_GREEN_3		86

#define GFX_COL_PINK_1		183
/*
#define IMG_GREEN_DOT		1
#define IMG_RED_DOT		2
#define IMG_BIG_S		3
#define IMG_ELITE_TXT		4
#define IMG_BIG_E		5
#define IMG_DICE		6
#define IMG_MISSILE_GREEN	7
#define IMG_MISSILE_YELLOW	8
#define IMG_MISSILE_RED		9
#define IMG_BLAKE		10
*/
#else
#define GFX_COL_BLACK       0
#define GFX_COL_DARK_RED    52
#define GFX_COL_WHITE       15
#define GFX_COL_GOLD        223
#define GFX_COL_RED         8
#define GFX_COL_CYAN        51

#define GFX_COL_GREY_1      231
#define GFX_COL_GREY_2      237
#define GFX_COL_GREY_3      239
#define GFX_COL_GREY_4      242

#define GFX_COL_BLUE_1      21
#define GFX_COL_BLUE_2      19
#define GFX_COL_BLUE_3      20
#define GFX_COL_BLUE_4      4

#define GFX_COL_RED_3       9
#define GFX_COL_RED_4       124

#define GFX_COL_WHITE_2     232

#define GFX_COL_YELLOW_1    11
#define GFX_COL_YELLOW_2    100
#define GFX_COL_YELLOW_3    106
#define GFX_COL_YELLOW_4    148
#define GFX_COL_YELLOW_5    184

#define GFX_ORANGE_1        214
#define GFX_ORANGE_2        58
#define GFX_ORANGE_3        95

#define GFX_COL_GREEN_1     2
#define GFX_COL_GREEN_2     28
#define GFX_COL_GREEN_3     34

#define GFX_COL_PINK_1      175

#endif

enum
{
    IMG_BLAKE = BLAKE,
    IMG_GREEN_DOT = GRNDOT,
    IMG_RED_DOT = REDDOT,
    IMG_BIG_S = SAFE,
    IMG_ELITE_TXT = ELITETXT,
    IMG_BIG_E = ECM,
    IMG_MISSILE_GREEN = MISSILE_G,
    IMG_MISSILE_YELLOW = MISSILE_Y,
    IMG_MISSILE_RED = MISSILE_R,
};

#define xor_mode(_val) gfx_xor_mode( _val )

int gfx_graphics_startup (void);
void gfx_xor_mode( int on );
void gfx_graphics_shutdown (void);
void gfx_update_screen (void);
void gfx_acquire_screen (void);
void gfx_release_screen (void);
void gfx_plot_pixel (int x, int y, int col);
void gfx_fast_plot_pixel (int x, int y, int col);
void gfx_draw_filled_circle (int cx, int cy, int radius, int circle_colour);
void gfx_draw_circle (int cx, int cy, int radius, int circle_colour);
void gfx_draw_line (int x1, int y1, int x2, int y2);
void gfx_draw_colour_line (int x1, int y1, int x2, int y2, int line_colour);
void gfx_draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3, int col);
void gfx_draw_rectangle (int tx, int ty, int bx, int by, int col);
void gfx_display_text (int x, int y, char *txt);
void gfx_display_colour_text (int x, int y, char *txt, int col);
void gfx_display_centre_text (int y, char *str, int psize, int col);
void gfx_clear_display (void);
void gfx_clear_text_area (void);
void gfx_clear_area (int tx, int ty, int bx, int by);
void gfx_display_pretty_text (int tx, int ty, int bx, int by, char *txt);
void gfx_draw_scanner (void);
void gfx_set_clip_region (int tx, int ty, int bx, int by);
void gfx_polygon (int num_points, int *poly_list, int face_colour);
void gfx_draw_sprite (int sprite_no, int x, int y);
void gfx_start_render (void);
void gfx_render_polygon (int num_points, int *point_list, int face_colour, int zavg);
void gfx_render_line (int x1, int y1, int x2, int y2, int dist, int col);
void gfx_finish_render (void);
int gfx_request_file (char *title, char *path, char *ext);

#endif
