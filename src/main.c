
#include "common.h"
#include "hal_fs.h"
#include "flash_config.h"
#include "hal_os.h"


#define ASSERT_FUNC_RETURN_VALUE(ret) \
    do {\
        if(ret) {\
            printf("assert failed.\r\n");\
            return ret; \
        }\
    }while(0);

static uint8_t g_conf1[32] = "hello world";
static uint8_t g_conf2[6];
static uint32_t g_conf3;
static uint16_t g_conf4;
static uint8_t g_conf5;


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

int main(int argc, char *argv[])
{
#if 0
    ASSERT_FUNC_RETURN_VALUE(hal_fs_init(1024 * 1024));
#else
    pthread_t config_thread;
    user_config_init();
    ASSERT_FUNC_RETURN_VALUE(flash_config_init());
    flash_config_commit(0);
    
    pthread_create(&config_thread, NULL, user_config_task, NULL);
    pthread_join(config_thread, NULL);
#endif
    return 0;
}

