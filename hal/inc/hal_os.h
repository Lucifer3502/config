/*
 * hal_os.h
 *
 *  Created on: 2019/1/21
 *      Author: heyan
 */


#ifndef _INCLUDE_HAL_OS_H_
#define _INCLUDE_HAL_OS_H_

#include "common.h"

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
extern "C"
{
#endif


typedef pthread_mutex_t hal_thread_mutex_t;

int32_t hal_mutex_init(hal_thread_mutex_t *mutex);

int32_t hal_mutex_destory(hal_thread_mutex_t *mutex);

int32_t hal_mutex_lock(hal_thread_mutex_t *mutex);

int32_t hal_mutex_unlock(hal_thread_mutex_t *mutex);

int32_t hex2byte(unsigned char *dest, int dest_len, unsigned char *src, int src_len);

int32_t byte2hex(unsigned char *dest, unsigned int dest_len, unsigned char *src, unsigned int src_len);

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
}
#endif


#endif /* _INCLUDE_HAL_OS_H_ */

