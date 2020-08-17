#include "lpwr.h"
#include "system.h"
#include "gui.h"
/****************************************************************************
* Function Name  : LPWR_Config
* Description    : 初始化PA0的外部中断，以用来将单片机从待机模式唤醒
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LPWR_Config(void)
{
    /* 开启GPIO时钟 */
	RCC->APB2ENR |= 1 << 2;     //先使能外设IO PORTA时钟    
	RCC->APB2ENR |= 1;          //开启辅助时钟		  

	GPIOA->CRL &= 0xFFFFFFF0;   //PA0设置成输入	  
	GPIOA->CRL |= 0x00000008;  
    GPIOA->ODR &= 0xFFFFFFFE;
/***************************************************************************/
/**************************** 中断设置 *************************************/
/***************************************************************************/
    /* 设置相应的优先级 */
    NVIC_Config(0, 0, EXTI0_IRQn);

    /* 使用PA0做输入 */
    AFIO->EXTICR[0] &= 0xFFFFFFF0;
    AFIO->EXTICR[0] |= 0x00000000;

    /* 设置外部中断的模式 */
    EXTI->IMR |= 0x00000001;  //开启中断输入线0上面的中断
    EXTI->FTSR &= 0xFFFFFFFE; //清除下降沿触发
    EXTI->RTSR |= 0x00000001; //设置上升沿触发

}

/****************************************************************************
* Function Name  : LPWR_EnterStandbyMode
* Description    : 进入待机模式
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LPWR_EnterStandbyMode(void)
{
    RCC->APB1ENR |= 0x10000000;
    PWR->CSR |= 1 << 8;           //设置WKUP用于唤醒
    /* Clear Wake-up flag */
    PWR->CR |= 0x0004;
    /* Select STANDBY mode */
    PWR->CR |= 0x0002;
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= (uint8_t)0x04;
    __WFI();		    
}

/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI0_IRQHandler(void)
{ 	
    /* 清除中断标志，写入1清除 */
	EXTI->PR |= 0x00000001;		  
}









