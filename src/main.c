
#include "common.h"
#include "hal_fs.h"
#include "flash_config.h"
#include "hal_os.h"
#include "log.h"


#define ASSERT_FUNC_RETURN_VALUE(ret) \
    do {\
        if(ret) {\
            printf("assert failed.\r\n");\
            return ret; \
        }\
    }while(0);

static uint8_t g_conf1[32] = "hello,world";
static uint8_t g_conf2[6]= {0x00, 0x01, 0x02, 0x03, 0x04, 0x06};
static uint32_t g_conf3 = 0x12345678;
static uint16_t g_conf4 = 0x1234;
static uint8_t g_conf5 = 0x12;


void user_config_init(void)
{
    uint32_t i;
    flash_conf_manager_t conf[] = {
        {(uint8_t *)"USER_CONF1", g_conf1, sizeof(g_conf1), FLASH_CONF_STRING},
        {(uint8_t *)"USER_CONF2", g_conf2, sizeof(g_conf2), FLASH_CONF_HEX_STR},
        {(uint8_t *)"USER_CONF3", &g_conf3, sizeof(g_conf3), FLASH_CONF_UINT32},
        {(uint8_t *)"USER_CONF4", &g_conf4, sizeof(g_conf4), FLASH_CONF_UINT16},
        {(uint8_t *)"USER_CONF5", &g_conf5, sizeof(g_conf5), FLASH_CONF_UINT8},

    };
    for (i = 0; i < sizeof(conf) / sizeof(conf[0]); i++) {
        flash_config_manage(&conf[i]);
    }
}

void *user_config_task(void *arg)
{
    flash_config_thread(arg);
    usleep(1000);
    return NULL;
}

void usage(void)
{
    /*
    * when have name value args, it will change the value of the name. 
    * The name must be in the list which is init in the user_config_init function.
    */
    printf("usage:\r\n");
    printf("    ffs_demo [name value]\r\n");
}

int main(int argc, char *argv[])
{
    
    if(argc < 3) {
        usage();
        return -1;
    }

    ASSERT_FUNC_RETURN_VALUE(hal_fs_init(1024 * 1024));
    user_config_init();
    ASSERT_FUNC_RETURN_VALUE(flash_config_init());
    if(flash_config_modify((uint8_t *)argv[1], (uint8_t *)argv[2])) {
        log_error("modify the config of %s failed.\r\n", argv[1]);
        return -1;
    }

    flash_config_commit(1);
    return 0;
}

