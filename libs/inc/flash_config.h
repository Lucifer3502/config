/*
 * flash_config.h
 *
 *  Created on: 2019/1/21
 *      Author: heyan
 */

#ifndef INCLUDE_FLASH_CONFIG_H_
#define INCLUDE_FLASH_CONFIG_H_

#include "common.h"

#define FLASH_CONFIG_ATTR

#define FLASH_CONFIG_MAX_NUM 200
#define FLASH_CONFIG_MAX_NAME_LEN 64
#define FLASH_CONFIG_MAX_VALUE_LEN 128

/* 3 = sizeof('=') + strlen("\r\n"), 4 for align*/
#define FLASH_CONFIG_LINE_LEN  (FLASH_CONFIG_MAX_NAME_LEN + FLASH_CONFIG_MAX_VALUE_LEN + 4)

#define FLASH_CONFIG_ADDR_DEF  0x1000
#define FLASH_CONFIG_ADDR_BAK  0x3000
#define FLASH_CONFIG_SCALE  4096

#define FLASH_CONFIG_EQUAL_CH '='

#define CONFIG_PRINTF printf
#define CONFIG_MEMCMP memcmp
#define CONFIG_STRLEN strlen

enum flash_config_value_type_t{
    FLASH_CONF_STRING,
    FLASH_CONF_HEX_STR,
    FLASH_CONF_UINT32,
    FLASH_CONF_UINT16,
    FLASH_CONF_UINT8,
    FLASH_CONF_INVALID,
};

typedef struct {
    uint8_t *name;
    void *value;
    uint32_t size;
    uint8_t type;
    uint8_t res[7];
}flash_conf_manager_t;

/*
* flag = 0, commit later, it will return immediately; 
* other, commit now, it will block until commit done.
*/
int32_t flash_config_commit(uint8_t flag);

/*
* flash config thread, need to call in loop.
*/
void flash_config_thread(void *arg);

/*
* The config manager is used to sync the config in the flash with  or to RAM.
* This function is used to connet to the config manager. You must call this func before flash_config_init;
*/
int32_t flash_config_manage(flash_conf_manager_t *conf);

int32_t flash_config_modify(uint8_t *name, uint8_t *value);

int32_t flash_config_init(void);


#endif /* INCLUDE_FLASH_CONFIG_H_ */

