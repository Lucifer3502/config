/*
 * hal_fs.h
 *
 *  Created on: 2019/1/21
 *      Author: heyan
 */

#ifndef INCLUDE_FLASH_FS_H_
#define INCLUDE_FLASH_FS_H_


#include "common.h"
#include "hal_os.h"

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
extern "C"
{
#endif

/*
 * Please define these macro according to the realy environment.
*/
#define FFS_SECTOR_SIZE     4096
#define FFS_READ_MIN_SIZE   256
#define FFS_WRITE_MIN_SIZE  256

#define NEWLINE_TAG  "\r\n"

#define FFS_API_ATTR


typedef struct flash_fs_tag{
    uint32_t addr;
    uint32_t offset;
    uint32_t scale;
}hal_fs_t;

int32_t hal_fs_open(hal_fs_t *fp, uint32_t addr, uint32_t scale);

int32_t hal_fs_close(hal_fs_t *fp);

int32_t hal_fs_erase(hal_fs_t *fp);

int32_t hal_fs_read(hal_fs_t *fp, uint8_t *buf, uint32_t len);

int32_t hal_fs_write(hal_fs_t *fp, uint8_t *buf, uint32_t len);

int32_t hal_fs_readline(hal_fs_t *fp, uint8_t *buf, uint32_t len);

int32_t hal_fs_lseek(hal_fs_t *fp, uint32_t offset);

int32_t hal_fs_init(uint32_t size);

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
}
#endif

#endif /* INCLUDE_FLASH_FS_H_ */

