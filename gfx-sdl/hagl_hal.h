/*
 * hagl_hal.h
 *
 *  Created on: 17 Dec 2023
 *      Author: rod
 */

#ifndef HAGL_HAL_H
#define HAGL_HAL_H


#include <stdint.h>
#include <hagl/backend.h>
#include <hagl/color.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Initialize the backend
 */
void hagl_hal_init(hagl_backend_t *backend);


#ifdef __cplusplus
}
#endif
#endif /* HAGL_HAL_H */
