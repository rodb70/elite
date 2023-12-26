# Makefile

COMPILER := gcc

CPU := host
BLD_TARGET := elite
BLD_TYPE := debug

EXTRA_LIBS :=
INC := .

PEDANDIC := n

PROJ_DIRS := 
PROJ_DIRS += hagl

ifeq ($(CPU),host)
#PROJ_DIRS += gfx-alg
PROJ_DIRS += gfx-sdl
PROJ_DIRS += qdbmp
EXTRA_LIBS += -lm
SHORT_ENUMS := n
endif

#CSRC += alg_gfx.c
CSRC += alg_main.c
CSRC += docked.c
CSRC += elite.c
CSRC += file.c
CSRC += intro.c
#CSRC += keyboard.c
CSRC += missions.c
CSRC += options.c
CSRC += pilot.c
CSRC += planet.c
CSRC += random.c
CSRC += shipdata.c
CSRC += shipface.c
#CSRC += sound.c
CSRC += space.c
CSRC += stars.c
CSRC += swat.c
CSRC += threed.c
CSRC += trade.c
CSRC += vector.c
CSRC += keys.c
CSRC += gfx-polygon.c

include makefiles/main.mk

distclean:
	rm -rf build
