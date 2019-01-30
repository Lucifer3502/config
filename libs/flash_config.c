/*
 * flash_config.c
 *
 *  Created on: 2019/1/21
 *      Author: heyan
 */

#include "common.h"
#include "log.h"
#include "flash_config.h"
#include "hal_os.h"
#include "hal_fs.h"

#define FLASH_CONFIG_HEAD_TAG  "CFG_HEAD_TAG="NEWLINE_TAG

static flash_conf_manager_t g_conf_manager[FLASH_CONFIG_MAX_NUM];
static uint8_t g_conf_write_flag;
static hal_thread_mutex_t g_conf_mutex;
static uint32_t g_config_addr = FLASH_CONFIG_ADDR_DEF;


static uint32_t FLASH_CONFIG_ATTR
get_config_cur_addr(void)
{
    return g_config_addr;
}

static uint32_t FLASH_CONFIG_ATTR
get_config_bak_addr(void)
{
    if(FLASH_CONFIG_ADDR_DEF == get_config_cur_addr()) {
        return FLASH_CONFIG_ADDR_BAK;
    }

    return FLASH_CONFIG_ADDR_DEF;
}

static int32_t FLASH_CONFIG_ATTR
flash_config_write_one(hal_fs_t *fp, flash_conf_manager_t *conf)
{
    int32_t rt = 0;
    uint8_t ch = FLASH_CONFIG_EQUAL_CH;
    uint32_t len = 0;
    uint8_t buf[FLASH_CONFIG_LINE_LEN];
    memset(buf, 0, FLASH_CONFIG_LINE_LEN);
    
    if(hal_fs_write(fp, conf->name, strlen((char *)conf->name))) {
        rt = -1;
        goto end;
    }
    hal_fs_lseek(fp, fp->offset + strlen((char *)conf->name));
    if(hal_fs_write(fp, &ch, 1)) {
        rt = -1;
        goto end;
    }
    hal_fs_lseek(fp, fp->offset + 1);
    CONFIG_PRINTF("<<< %s=", conf->name);
    switch(conf->type){
    case FLASH_CONF_STRING:{
            if(hal_fs_write(fp, conf->value, strlen((char *)conf->value))) {
                rt = -1;
                goto end;
            }
            hal_fs_lseek(fp, fp->offset + strlen((char *)conf->value));
            CONFIG_PRINTF("%s\r\n", (char *)conf->value);
        }
        break;
    case FLASH_CONF_HEX_STR:{
            byte2hex(buf, FLASH_CONFIG_LINE_LEN, conf->value, conf->size);
            if(hal_fs_write(fp, buf, conf->size * 2)) {
                rt = -1;
                goto end;
            }
            hal_fs_lseek(fp, fp->offset + conf->size * 2);
            CONFIG_PRINTF("%s\r\n", buf);
        }
        break;
    case FLASH_CONF_UINT32:{
            uint32_t value = *(uint32_t *)conf->value;
            len = sprintf((char *)buf, "%u", value);
            if(hal_fs_write(fp, buf, len)) {
                rt = -1;
                goto end;
            }
            hal_fs_lseek(fp, fp->offset + len);
            CONFIG_PRINTF("%s\r\n", buf);
        }
        break;
    case FLASH_CONF_UINT16:{
            uint16_t value = *(uint16_t *)conf->value;
            len = sprintf((char *)buf, "%u", value);
            if(hal_fs_write(fp, buf, len)) {
                rt = -1;
                goto end;
            }
            hal_fs_lseek(fp, fp->offset + len);
            CONFIG_PRINTF("%s\r\n", buf);
        }
        break;
    case FLASH_CONF_UINT8:{
            uint8_t value = *(uint8_t *)conf->value;
            len = sprintf((char *)buf, "%u", value);
            if(hal_fs_write(fp, buf, len)) {
                rt = -1;
                goto end;
            }
            hal_fs_lseek(fp, fp->offset + len);
            CONFIG_PRINTF("%s\r\n", buf);
        }
        break;
    default:
        break;
    }
    
    if(hal_fs_write(fp, (uint8_t *)NEWLINE_TAG, strlen(NEWLINE_TAG))){
        log_error("write error\r\n");
        rt = -1;
        goto end;
    }
    hal_fs_lseek(fp, fp->offset + strlen(NEWLINE_TAG));
    rt = 0;
    
end:
    return rt;
}

/*
* write from ram to file
*/
static int32_t FLASH_CONFIG_ATTR
flash_config_write(void)
{
    hal_fs_t fs;
    int32_t ret = 0;
    uint32_t i = 0;
    
    if(hal_fs_open(&fs, get_config_bak_addr(), FLASH_CONFIG_SCALE)) {
        ret = -1;
        goto end;
    }
    if(hal_fs_erase(&fs)) {
        ret = -1;
        goto end;
    }
    log_debug("write config to flash addr = 0x%x...\r\n", fs.addr);
    hal_fs_lseek(&fs, CONFIG_STRLEN(FLASH_CONFIG_HEAD_TAG));
    for(i = 0; i < FLASH_CONFIG_MAX_NUM; i++) {
        if(g_conf_manager[i].name) {
            if(flash_config_write_one(&fs, &g_conf_manager[i])) {
                ret = -1;
                goto end;
            }
        }
    }

    /* write head magic. */
    hal_fs_lseek(&fs, 0);
    if(hal_fs_write(&fs, (uint8_t *)FLASH_CONFIG_HEAD_TAG, CONFIG_STRLEN(FLASH_CONFIG_HEAD_TAG))) {
        ret = -1;
        goto end;
    }
    hal_fs_close(&fs);

    /* erase cur config addr and change the addr value */
    if(hal_fs_open(&fs, get_config_cur_addr(), FLASH_CONFIG_SCALE)) {
        ret = -1;
        goto end;
    }
    if(hal_fs_erase(&fs)) {
        ret = -1;
        goto end;
    }

    g_config_addr = get_config_bak_addr();
end:
    hal_fs_close(&fs);
    return ret;
}

void FLASH_CONFIG_ATTR
flash_config_thread(void *arg)
{
    if(g_conf_write_flag) {
        hal_mutex_lock(&g_conf_mutex);
        if (flash_config_write()) {
            log_fatal("write config to flash error.\r\n");
        }
        g_conf_write_flag = 0;
        hal_mutex_unlock(&g_conf_mutex);
    }
}

int32_t FLASH_CONFIG_ATTR
flash_config_commit(uint8_t flag)
{
    int32_t rt = -1;
    hal_mutex_lock(&g_conf_mutex);
    if(flag) {
        if (flash_config_write()) {
            rt = -1;
            log_fatal("write config to flash error.\r\n");
        }
        g_conf_write_flag = 0;
    } else {
        g_conf_write_flag = 1;
        rt = 0;
    }
    hal_mutex_unlock(&g_conf_mutex);
    return rt;
}

int32_t FLASH_CONFIG_ATTR
flash_config_manage(flash_conf_manager_t *conf)
{
    uint32_t i;
    if(!conf || !conf->name || !conf->value
        || conf->size > FLASH_CONFIG_MAX_VALUE_LEN
        || !conf->size || conf->type >= FLASH_CONF_INVALID) {
        log_error("config_manage err.\r\n");
        return -1;
    }
    
    for(i = 0; i < FLASH_CONFIG_MAX_NUM; i++) {
        if(g_conf_manager[i].name) {
            continue;
        }
        g_conf_manager[i].name = conf->name;
        g_conf_manager[i].value = conf->value;
        g_conf_manager[i].size = conf->size;
        g_conf_manager[i].type = conf->type;
        //log_debug("manage conf %s\r\n", conf->name);
        return 0;
    }

    log_error("config_manage num > %d.\r\n", FLASH_CONFIG_MAX_NUM);
    return -1;
}

static int32_t FLASH_CONFIG_ATTR
parse_config_name_value(uint8_t *buf, uint32_t buf_len, uint8_t **name, uint8_t **value)
{
    uint32_t offset = 0;
    uint32_t len = buf_len - CONFIG_STRLEN(NEWLINE_TAG);
    buf[len] = 0; 
    *name = buf;
    *value = NULL;
    
    while(offset <= len) {
        if(FLASH_CONFIG_EQUAL_CH == buf[offset]) {
            buf[offset] = 0;
            *value = buf + offset + 1;
            return 0;
        }
        offset++;
    }
    return -1;
}

static int32_t FLASH_CONFIG_ATTR
_flash_config_read_one(uint8_t *value, flash_conf_manager_t *conf)
{
    switch (conf->type) {
    case FLASH_CONF_STRING:{
            if(strncmp((char *)value, conf->value, conf->size)) {
                strncpy(conf->value, (char *)value, conf->size - 1);
                g_conf_write_flag = 1;
            }
        }
        break;

    case FLASH_CONF_HEX_STR:{
            uint8_t tmp[FLASH_CONFIG_MAX_VALUE_LEN + 4] = {0};
            byte2hex(tmp, FLASH_CONFIG_MAX_VALUE_LEN, conf->value, conf->size);
            if(strncmp((char *)value, (char *)tmp, conf->size * 2)) {
                hex2byte(conf->value, conf->size, value, strlen((char *)value));
                g_conf_write_flag = 1;
            }
        }
        break;
        
    case FLASH_CONF_UINT32:{
            uint32_t tmp = atoi((char *)value);
            if(tmp != *(uint32_t *)(conf->value)) {
                *(uint32_t *)(conf->value) = tmp;
                g_conf_write_flag = 1;
            }
        }
        break;
        
    case FLASH_CONF_UINT16:{
            uint16_t tmp = atoi((char *)value);
            if(tmp != *(uint16_t *)(conf->value)) {
                *(uint16_t *)(conf->value) = tmp;
                g_conf_write_flag = 1;
            }
        }
        break;
        
    case FLASH_CONF_UINT8:{
            uint8_t tmp = atoi((char *)value);
            if(tmp != *(uint8_t *)(conf->value)) {
                *(uint8_t *)(conf->value) = tmp;
                g_conf_write_flag = 1;
            }
        }
        break;
    default: 
        break;
    }

    return 0;
}

static int32_t FLASH_CONFIG_ATTR
flash_config_read_one(uint8_t *name, uint8_t *value)
{
    uint32_t i;
    for(i = 0; i < FLASH_CONFIG_MAX_NUM; i++) {
        if(g_conf_manager[i].name && !strcmp((char *)name, (char *)g_conf_manager[i].name)) {
            _flash_config_read_one(value, &g_conf_manager[i]);
            return 0;
        }
    }
    return -1;
}

/*
* read from flash to ram
*/
static int32_t FLASH_CONFIG_ATTR
flash_config_read(void)
{
    hal_fs_t fs;
    int32_t ret = 0;
    uint8_t buf[FLASH_CONFIG_LINE_LEN];
    int32_t len = 0;
    uint8_t *name = NULL;
    uint8_t *value = NULL;

    log_debug("read config from flash start...\r\n");
    if(hal_fs_open(&fs, get_config_cur_addr(), FLASH_CONFIG_SCALE)) {
        ret = -1;
        goto end;
    }

    if(0 > hal_fs_read(&fs, buf, FLASH_CONFIG_LINE_LEN)) {
        ret = -1;
        goto end;
    }
    if(CONFIG_MEMCMP (buf, FLASH_CONFIG_HEAD_TAG, CONFIG_STRLEN(FLASH_CONFIG_HEAD_TAG))) {
        log_info("empty config.\r\n");
        ret = 0;
        goto end;
    }

    hal_fs_lseek(&fs, CONFIG_STRLEN(FLASH_CONFIG_HEAD_TAG));
    while(1) {
        len = hal_fs_readline(&fs, buf, FLASH_CONFIG_LINE_LEN);
        if(len < 0) {
            break;
        }
        if(!parse_config_name_value(buf, len, &name, &value)) {
            CONFIG_PRINTF(">>> %s=%s\r\n", name, value);
            flash_config_read_one(name, value);
        }
        hal_fs_lseek(&fs, fs.offset + len);
    }

end:
    log_debug("read config from flash over.\r\n");
    g_conf_write_flag = 0;
    hal_fs_close(&fs);
    return ret;
    
}

int32_t FLASH_CONFIG_ATTR
flash_config_modify(uint8_t *name, uint8_t *value)
{
    if(NULL == name || NULL == value) {
        return -1;
    }

    return flash_config_read_one(name, value);
}

int32_t FLASH_CONFIG_ATTR
flash_config_init(void)
{
    hal_fs_t fs;
    int32_t ret = 0;
    uint8_t buf[FLASH_CONFIG_LINE_LEN];
    uint32_t buf_len = FLASH_CONFIG_LINE_LEN;
    
    if(hal_fs_open(&fs, FLASH_CONFIG_ADDR_DEF, FLASH_CONFIG_SCALE)) {
        log_error("open flash error\r\n");
        ret = -1;
        goto err;
    }
    if(0 > hal_fs_read(&fs, buf, buf_len)) {
        log_error("read flash error\r\n");
        ret = -1;
        goto err;
    }
    if(!CONFIG_MEMCMP (buf, FLASH_CONFIG_HEAD_TAG, CONFIG_STRLEN(FLASH_CONFIG_HEAD_TAG))) {
        g_config_addr = FLASH_CONFIG_ADDR_DEF;
    } else {
        g_config_addr = FLASH_CONFIG_ADDR_BAK;
    }

    log_debug("The config locate at flash addr = 0x%x\r\n", g_config_addr);
    hal_fs_close(&fs);
    return flash_config_read();
    
err:
    hal_fs_close(&fs);
    return ret;
}

