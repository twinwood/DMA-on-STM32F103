#include "beep.h"
#include "system.h"

/****************************************************************************
* Function Name  : BEEP_Config
* Description    : 初始化蜂鸣器的IO配置
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void BEEP_Config(void)
{
    /*打开GPIOB的时钟*/
    RCC->APB2ENR |= 0x00000008;
    
    /* 设置GPIOC的低8位为通用推挽输出 */
    GPIOB->CRL &= 0xFF0FFFFF;
    GPIOB->CRL |= 0x00300000;	                       
}

/****************************************************************************
* Function Name  : BEEP_Work
* Description    : 蜂鸣器响滴一声
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void BEEP_Work(void)
{
    uint16_t i;
    uint8_t j;

    for(j=0; j<50; j++)           //产生一段时间的PWM波，使蜂鸣器发出滴一声
    {
        PBout(5) = 1;             //通过我们定义的位操作宏操作PB5口输出高电平
        for(i=0; i<4500; i++);    //延时调整PWM的频率
        PBout(5) = 0; 
        for(i=0; i<4500; i++);   
    }
}

