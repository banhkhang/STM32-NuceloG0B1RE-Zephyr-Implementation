#ifndef THREAD_MNG_H
#define THREAD_MNG_H

#include <stdint.h>

/* extern variables*/
extern volatile uint8_t count;

/* init prototypes*/
void thread_init(void);
void pin_config(void);
void sys_init(void);

#endif