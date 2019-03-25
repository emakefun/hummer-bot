#ifndef _DEBUG_H_
#define _DEUBG_H_

#ifndef  DEBUG_LEVEL
#define  DEBUG_LEVEL  0xFF
#endif

void arduino_printf(char *fmt ,...);

#define  DEBUG_LEVEL_INFO 0x0001
#define  DEBUG_LEVEL_WRN  0x0002
#define  DEBUG_LEVEL_ERR  0x0003
#define  DEBUG_LEVEL_RED  0x0004
#define  DEBUG_LEVEL_ALL  0x00FF

#define DEBUG_LOG(level, fmt, ...) \
    do{\
        if(level >= DEBUG_LEVEL ){\
        arduino_printf(fmt, ##__VA_ARGS__);}\
    }while(0)

#define DEBUG_ERR(fmt, ...) \
	do{\
		arduino_printf("[Error][%s:%s:%d]",__FILE__,__FUNCTION__,__LINE__);\
		arduino_printf(fmt, ##__VA_ARGS__);\
	}while(0)

#define DEBUG_RED(flag, fmt, ...) \
	do{\
		if(DEBUG_LEVEL & DEBUG_LEVEL_RED){\
		arduino_printf("[[30\33m");\
		arduino_printf(fmt,##__VA_ARGS__);}\
		arduino_printf("\033[0m");\
	}while(0)

#endif  /*  _DEBUG_  */
