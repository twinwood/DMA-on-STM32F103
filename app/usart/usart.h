#ifndef __USART_H
#define __USART_H


#include <stm32f10x.h>

/* 使用中断接收 */
#define USE_RX_INTERRUPT
/* 如果使用printf输出 */
#define USE_PRINTF

#ifdef USE_PRINTF
#include<stdio.h>
int fputc(int ch, FILE *f);
#endif


/* 声明全局函数 */
void USART1_Config(uint16_t baudRate);
void USART1_SendWord(uint8_t *wd);











































#endif
