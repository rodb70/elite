/*
 * hagl_hal_color.h
 *
 *  Created on: 17 Dec 2023
 *      Author: rod
 */

#ifndef HAGL_HAL_COLOR_H
#define HAGL_HAL_COLOR_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HAL must provide typedef for colors. This HAL uses XBGR888. */
typedef uint8_t hagl_color_t;

#ifdef __cplusplus
}
#endif
#endif /* HAGL_HAL_COLOR_H */
