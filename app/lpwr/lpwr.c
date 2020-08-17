#include "lpwr.h"
#include "system.h"
#include "gui.h"
/****************************************************************************
* Function Name  : LPWR_Config
* Description    : ��ʼ��PA0���ⲿ�жϣ�����������Ƭ���Ӵ���ģʽ����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LPWR_Config(void)
{
    /* ����GPIOʱ�� */
	RCC->APB2ENR |= 1 << 2;     //��ʹ������IO PORTAʱ��    
	RCC->APB2ENR |= 1;          //��������ʱ��		  

	GPIOA->CRL &= 0xFFFFFFF0;   //PA0���ó�����	  
	GPIOA->CRL |= 0x00000008;  
    GPIOA->ODR &= 0xFFFFFFFE;
/***************************************************************************/
/**************************** �ж����� *************************************/
/***************************************************************************/
    /* ������Ӧ�����ȼ� */
    NVIC_Config(0, 0, EXTI0_IRQn);

    /* ʹ��PA0������ */
    AFIO->EXTICR[0] &= 0xFFFFFFF0;
    AFIO->EXTICR[0] |= 0x00000000;

    /* �����ⲿ�жϵ�ģʽ */
    EXTI->IMR |= 0x00000001;  //�����ж�������0������ж�
    EXTI->FTSR &= 0xFFFFFFFE; //����½��ش���
    EXTI->RTSR |= 0x00000001; //���������ش���

}

/****************************************************************************
* Function Name  : LPWR_EnterStandbyMode
* Description    : �������ģʽ
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LPWR_EnterStandbyMode(void)
{
    RCC->APB1ENR |= 0x10000000;
    PWR->CSR |= 1 << 8;           //����WKUP���ڻ���
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
    /* ����жϱ�־��д��1��� */
	EXTI->PR |= 0x00000001;		  
}









