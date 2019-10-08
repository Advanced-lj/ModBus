#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "stm32l0xx.h"
#include "string.h"

#define UNXX_DEBUG

#ifdef UNXX_DEBUG

extern uint8_t g_unxx_debug;

#define unxx_debug(...)    \
	{                        \
		if(g_unxx_debug) {     \
			printf(__VA_ARGS__); \
		}                      \
	}
#else
	
#define unxx_debug(...)

#endif

enum {
	UNXX_DEBUG_OFF = 0,
	UNXX_DEBUG_ON,
};

extern uint8_t unxx_set_debug(uint8_t *buf);	
extern uint8_t unxx_get_debug(void);
extern void gsm_message(uint8_t *buf, uint16_t bufsize, char *prompt);	
extern void print_atcmd(uint8_t *buf, uint16_t bufsize);

#endif  /*__DEBUG_H__*/
