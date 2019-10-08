#include "debug.h"

#define ENABLE_UNXX_DEBUG_PROMPT   "ENABLE UNXX DEBUG@"
#define DISABLE_UNXX_DEBUG_PROMPT  "DISABLE UNXX DEBUG@"

uint8_t g_unxx_debug = UNXX_DEBUG_ON;

uint8_t unxx_set_debug(uint8_t *buf)
{
	uint8_t ret = 1;
	
	if(strstr((const char *)buf, ENABLE_UNXX_DEBUG_PROMPT)) {
		g_unxx_debug = UNXX_DEBUG_ON;
	} else if(strstr((const char *)buf, DISABLE_UNXX_DEBUG_PROMPT)) {
		g_unxx_debug = UNXX_DEBUG_OFF;
	} else {
		ret = 0;
	}
	
	return ret;
}

uint8_t unxx_get_debug(void)
{
	return g_unxx_debug;
}

void gsm_message(uint8_t *buf, uint16_t bufsize, char *prompt)
{
	uint16_t i, j, idx = 0;
	uint8_t data[16*4] = {0};
	uint8_t txt[16] = {0};
	
	unxx_debug("\r\n");
	if(prompt) {
		unxx_debug("%s\r\n", prompt);
	}
	
	for(i = 0; i < bufsize; i++) {
		if(i % 16 == 0) {
			if(i > 0) {
				for(j = 0; j < 48; j++) {
					if(j < idx) {
						unxx_debug("%c", data[j]);
					} else {
						unxx_debug(" ");
					}
				}
				unxx_debug("  ");
				for(j = 0; j < 16; j ++) {
					if(txt[j] == 0) {
						break;
					} else if(txt[j] == '\r') {
						unxx_debug("\\r");
					} else if(txt[j] == '\n') {
						unxx_debug("\\n");
					} else {
						unxx_debug("%c", txt[j]);
					}
				}
				unxx_debug("\r\n");
			}
			memset(data, 0, sizeof(data));
			memset(txt , 0, sizeof(txt));
			idx = 0;
		}
		
		snprintf((char *)data + idx, sizeof(data) - idx, " %02x", buf[i]);
		idx += 3;
		
		txt[i%16] = ( buf[i] > 0 && buf[i] < 127 ) ? buf[i] : '.' ;
	}
	
	for(j = 0; j < 48; j++) {
		if(j < idx) {
			unxx_debug("%c", data[j]);
		} else {
			unxx_debug(" ");
		}
	}
	unxx_debug("  ");
	for(j = 0; j < 16; j ++) {
		if(txt[j] == 0) {
			break;
		} else if(txt[j] == '\r') {
			unxx_debug("\\r");
		} else if(txt[j] == '\n') {
			unxx_debug("\\n");
		} else {
			unxx_debug("%c", txt[j]);
		}
	}
	unxx_debug("\r\n");	
}

void print_atcmd(uint8_t *buf, uint16_t bufsize)
{
	uint8_t i;
	
	for(i = 0; i < bufsize; i++) {
		if(buf[i] == '\r') {
			unxx_debug("%s", "\\r");
		} else if(buf[i] == '\n') {
			unxx_debug("%s", "\\n");
		} else {
			unxx_debug("%c", buf[i]);
		}
	}
}




