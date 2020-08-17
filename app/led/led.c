#include "led.h"


/****************************************************************************
* Function Name  : LED_Config
* Description    : 将LED使用的IO口设置为通用推挽输出
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LED_Config(void)
{

	/*打开GPIOC的时钟*/
	RCC->APB2ENR |= 0x00000010;

    /* 设置GPIOC的低8位为通用推挽输出 */
    GPIOC->CRL = 0x33333333;
    
    /* 初始化为Io口 */
    GPIOC->ODR |= 0xFFFF; 
}

/****************************************************************************
* Function Name  : LED_SetState
* Description    : 设置LED的状态，0代表相应的LED点亮，1表示相应的LED熄灭
* Input          : stateValue：LED的状态
* Output         : None
* Return         : None
****************************************************************************/

void LED_SetState(uint8_t stateValue)
{    
    /* 设置LED灯的状态, GPIO一次设置16位，将其值强制转换位16位 */
    GPIOC->BSRR = (uint16_t)stateValue & 0x00FF;
    GPIOC->BRR =  ~((uint16_t)stateValue & 0x00FF);                  
}



