
#ifndef _INCLUDE_LOG_H_
#define _INCLUDE_LOG_H_


#define NORMAL_COLOR  "\033[0m"
#define RED_COLOR  "\033[1;31m"
#define GREEN_COLOR  "\033[1;32m"
#define YELLO_COLOR  "\033[1;33m"
#define BLUE_COLOR  "\033[1;34m"


#define log(tag, fmt, ...) \
    do {\
        printf(tag ": %s@%s#%d: " fmt, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
    }while(0)
#define log_debug(fmt, ...)  \
    do {\
        log(GREEN_COLOR"[DEBUG]", fmt NORMAL_COLOR, ##__VA_ARGS__); \
    }while(0)
    
#define log_info(fmt, ...)  \
    do {\
        log(BLUE_COLOR"[INFO]", fmt NORMAL_COLOR, ##__VA_ARGS__);  \
    }while(0)

#define log_warn(fmt, ...)  \
    do {\
        log(YELLO_COLOR"[warn]", fmt NORMAL_COLOR, ##__VA_ARGS__);  \
    }while(0)

#define log_error(fmt, ...) \
    do {\
        log(RED_COLOR"[ERROR]", fmt NORMAL_COLOR, ##__VA_ARGS__); \
    }while(0)
    
#define log_fatal(fmt, ...) \
    do {\
        log(RED_COLOR"[FATAL]", fmt NORMAL_COLOR, ##__VA_ARGS__);; \
    }while(0)



#endif  /*_INCLUDE_LOG_H_*/

