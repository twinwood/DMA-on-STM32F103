#ifndef __NIXIETUBE_H
#define __NIXIETUBE_H

#include "stm32f10x.h"
#include "system.h"

/* 定义数码管使用的PIN口 */

#define NixieTube_1_CLR (PFout(6) = 0)
#define NixieTube_1_SET (PFout(6) = 1)
#define NixieTube_2_CLR (PFout(7) = 0)
#define NixieTube_2_SET (PFout(7) = 1)
#define NixieTube_3_CLR (PFout(8) = 0)
#define NixieTube_3_SET (PFout(8) = 1)
#define NixieTube_4_CLR (PFout(9) = 0)
#define NixieTube_4_SET (PFout(9) = 1)

/* 定义数码管输入函数 */
#define NixieTube(dat) {GPIOC->BSRR |= (uint32_t)dat & 0x000000FF;\
                        GPIOC->BRR |= ~((uint32_t)dat & 0x000000FF);}


/* 定义全局函数 */
void NIXIETUBE_Config(void);
void NIXIETUBE_Display(uint8_t *num);












#endif
