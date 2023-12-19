#-----------------------------------------------------------------------------
# Purpose: HAGL library
#
#------------------------------------------------------------------------------
# FPATH - is the include path to this makefile fragment
#

#-----------------------------------------------------------------------------
ifeq ($(INC_PART),)
$(if $(MAKECMDGOALS),$(MAKECMDGOALS),all):
	@$(MAKE) -C .. $@
endif

#-----------------------------------------------------------------------------
# Declare the library compiled to
LIB := hagl

CFLAGS += -Wno-error=old-style-declaration
CFLAGS += -Wno-error=unused-parameter
CFLAGS += -Wno-error=unused-value
CFLAGS += -Wno-error=sign-compare
#CFLAGS += -Wno-error=pedantic
#-----------------------------------------------------------------------------
# Declare C files
#CSRC += hagl-drv.c
CSRC += hagl/src/hagl.c
CSRC += hagl/src/hagl_blit.c
CSRC += hagl/src/hagl_char.c
CSRC += hagl/src/hagl_circle.c
CSRC += hagl/src/hagl_clip.c
CSRC += hagl/src/hagl_color.c
CSRC += hagl/src/hagl_ellipse.c
CSRC += hagl/src/hagl_hline.c
CSRC += hagl/src/hagl_line.c
CSRC += hagl/src/hagl_pixel.c
CSRC += hagl/src/hagl_polygon.c
CSRC += hagl/src/hagl_rectangle.c
CSRC += hagl/src/hagl_triangle.c
CSRC += hagl/src/hagl_vline.c
CSRC += hagl/src/hagl_bitmap.c
CSRC += hagl/src/fontx.c
CSRC += hagl/src/hsl.c
CSRC += hagl/src/rgb565.c
CSRC += hagl/src/rgb888.c

#-----------------------------------------------------------------------------
# If set to y then the include path is added to with this directory
# Delete or set to 'n' if not needed in include path 
INC += $(FPATH)/hagl/include
#ADD_TO_INC_PATH := y