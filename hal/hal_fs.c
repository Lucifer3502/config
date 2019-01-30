/*
 * hal_fs.c
 *
 *  Created on: 2019/1/21
 *      Author: heyan
 */

#include "common.h"
#include "hal_fs.h"
#include "log.h"

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
extern "C"
{
#endif

#ifdef HAL_CONF_FILE_PATHNAME
static const char g_fs_path[] = HAL_CONF_FILE_PATHNAME;
#else
static const char g_fs_path[] = "~/config.txt";
#endif

static int32_t g_fs_fd = -1;

int32_t FFS_API_ATTR
hal_fs_open(hal_fs_t *fp, uint32_t addr, uint32_t scale)
{
    int32_t fd = -1;
    if(NULL == fp) {
        return -1;
    }
    
    fp->addr = addr;
    fp->offset = 0;
    fp->scale = scale;

    fd = open(g_fs_path, O_CREAT|O_RDWR, S_IRWXU);
    if(fd < 0) {
        return -1;
    }
    g_fs_fd = fd;
    
    return 0;
}

int32_t FFS_API_ATTR
hal_fs_close(hal_fs_t *fp)
{
    close(g_fs_fd);
    g_fs_fd = -1;
    return 0;
}

int32_t FFS_API_ATTR
hal_fs_erase(hal_fs_t *fp)
{
    uint8_t buf[FFS_SECTOR_SIZE];
    uint32_t sec;
    
    if(fp->addr % FFS_SECTOR_SIZE || fp->scale % FFS_SECTOR_SIZE) {
        return -1;
    }
    
    if(fp->addr != lseek(g_fs_fd, fp->addr, SEEK_SET)) {
        return -1;
    }
    sec = fp->scale / FFS_SECTOR_SIZE;
    memset(buf, 0xff, FFS_SECTOR_SIZE);
    while(sec--) {
        if(FFS_SECTOR_SIZE != write(g_fs_fd, buf, FFS_SECTOR_SIZE)) {
            return -1;
        }
    }
    return 0;
}

int32_t FFS_API_ATTR
hal_fs_read(hal_fs_t *fp, uint8_t *buf, uint32_t len)
{
    if(NULL == fp) {
        return -1;
    }
    if(fp->addr + fp->offset != lseek(g_fs_fd, fp->addr + fp->offset, SEEK_SET)) {
        return -1;
    }
    if(len != read(g_fs_fd, buf, len)) {
        return -1;
    }
    return 0;
}

int32_t FFS_API_ATTR
hal_fs_write(hal_fs_t *fp, uint8_t *buf, uint32_t len)
{
    if(NULL == fp) {
        return -1;
    }
    if((fp->addr + fp->offset) != lseek(g_fs_fd, fp->addr + fp->offset, SEEK_SET)) {
        log_error("lseek error\r\n");
        return -1;
    }
    if(len != write(g_fs_fd, buf, len)) {
        log_error("write error\r\n");
        return -1;
    }
    return 0;
}

int32_t FFS_API_ATTR
hal_fs_readline(hal_fs_t *fp, uint8_t *buf, uint32_t len)
{
    uint32_t offset = 0;
    if(hal_fs_read(fp, buf, len)) {
        return -1;
    }
    while(offset + strlen(NEWLINE_TAG) <= len) {
        if(!memcmp(buf + offset, NEWLINE_TAG, strlen(NEWLINE_TAG))) {
            return offset + strlen(NEWLINE_TAG);
        }
        offset++;
    }
    return -1;
}

int32_t FFS_API_ATTR
hal_fs_lseek(hal_fs_t *fp, uint32_t offset)
{
    fp->offset = offset;
    return 0;
}


int32_t FFS_API_ATTR
hal_fs_init(uint32_t size)
{
    uint8_t buf[FFS_SECTOR_SIZE];
    uint32_t sec = size / FFS_SECTOR_SIZE;
    int32_t fd = -1;
    fd = open(g_fs_path, O_CREAT|O_RDWR, S_IRWXU);
    if(fd < 0) {
        log_error("open %s failed\r\n", g_fs_path);
        return -1;
    }

    memset(buf, 0xff, FFS_SECTOR_SIZE);
    while(sec--) {
        if(FFS_SECTOR_SIZE != write(fd, buf, FFS_SECTOR_SIZE)) {
            log_error("write failed\r\n");
            return -1;
        }
    }
    close(fd);
    return 0;
}

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
}
#endif

