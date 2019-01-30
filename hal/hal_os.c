/*
 * hal_os.c
 *
 *  Created on: 2019/1/21
 *      Author: heyan
 */

#include "common.h"
#include "hal_os.h"

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
extern "C"
{
#endif

int32_t hal_mutex_init(hal_thread_mutex_t *mutex)
{
    return pthread_mutex_init(mutex, NULL);
}
 
int32_t hal_mutex_destory(hal_thread_mutex_t *mutex)
{
    return pthread_mutex_destroy(mutex);
}

int32_t hal_mutex_lock(hal_thread_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}


int32_t hal_mutex_unlock(hal_thread_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}

int32_t hex2byte(unsigned char *dest, int dest_len, unsigned char *src, int src_len)
{
    if((src_len % 2) || (dest_len < src_len / 2))
        return -1;

    int i;
    int j;
    for(i = 0, j = 0; i < src_len; i++, j++) {
        if(src[i] >= '0' && src[i] <= '9') {
            dest[j] = src[i] - '0';
        } else if(src[i] >= 'a' && src[i] <= 'f') {
            dest[j] = src[i] - 'a' + 10;
        } else if(src[i] >= 'A' && src[i] <= 'F') {
            dest[j] = src[i] - 'A' + 10;
        } else {
            return -1;
        }
        i++;
        unsigned char tmp = dest[j] << 4;
        if(src[i] >= '0' && src[i] <= '9') {
            dest[j] = tmp+ (src[i] - '0');
        } else if(src[i] >= 'a' &&  src[i] <= 'f') {
            dest[j] = tmp + (src[i] - 'a' + 10);
        } else if(src[i] >= 'A' && src[i] <= 'F') {
            dest[j] = tmp + (src[i] - 'A' + 10);
        } else {
            return -1;
        }
    }
    return 0;
}

int32_t byte2hex(unsigned char *dest, unsigned int dest_len, unsigned char *src, unsigned int src_len)
{
    uint32_t i;
    if(dest_len < src_len * 2 || !dest || !src) {
        return -1;
    }

    for(i = 0; i < src_len; i++) {
        sprintf((char *)&dest[i * 2], "%02X", src[i]);
    }

    return 0;
}


#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
}
#endif


