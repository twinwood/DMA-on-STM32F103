#ifndef __TEMP_H
#define __TMEP_H

#include "stm32f10x.h"
#include "system.h"

/* �궨�� */
//��PG11��Ϊ��������
#define DS18B20_Input()  {GPIOG->CRH &= 0xFFFF0FFF; GPIOG->CRH |= 0x00008000;}
//��PG11��Ϊ�������
#define DS18B20_Output() {GPIOG->CRH &= 0xFFFF0FFF; GPIOG->CRH |= 0x00003000;}

#define DS18B20_OUT      PGout(11)   //ʹ��λ��������
#define DS18B20_IN       PGin(11)

/* ����ȫ�ֱ��� */
void DS18B20_Init(void);
uint8_t DS18B20_Reset(void);
int16_t DS18B20_ReadTemp(void);














#endif
