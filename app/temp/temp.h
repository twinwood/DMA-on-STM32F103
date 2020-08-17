#ifndef __TEMP_H
#define __TMEP_H

#include "stm32f10x.h"
#include "system.h"

/* 宏定义 */
//将PG11设为上拉输入
#define DS18B20_Input()  {GPIOG->CRH &= 0xFFFF0FFF; GPIOG->CRH |= 0x00008000;}
//将PG11设为推挽输出
#define DS18B20_Output() {GPIOG->CRH &= 0xFFFF0FFF; GPIOG->CRH |= 0x00003000;}

#define DS18B20_OUT      PGout(11)   //使用位操作定义
#define DS18B20_IN       PGin(11)

/* 定义全局变量 */
void DS18B20_Init(void);
uint8_t DS18B20_Reset(void);
int16_t DS18B20_ReadTemp(void);














#endif
