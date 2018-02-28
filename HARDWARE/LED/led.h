#ifndef _LED_H_
#define _LED_H_
#include "sys.h"

#define LED0 PAout(8)
#define LED1 PDout(2)

void LED_Init(void);
#endif
