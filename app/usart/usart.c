#include "usart.h"

#ifdef USE_RX_INTERRUPT   
    #include "system.h"    
#endif


void USART1_NVIC_RxConfig(void);

/****************************************************************************
* Function Name  : USART1_Config
* Description    : Configurates the USART1.
* Input          : baudRate：波特率
* Output         : None
* Return         : None
****************************************************************************/

void USART1_Config(uint16_t baudRate)
{
    uint16_t mantissa, fraction;
    float tmpreg;

    /* 计算波特率的设置参数，默认系统时钟为72MHZ */
    tmpreg=(float)(72000000)/(baudRate * 16);//得到USARTDIV
	mantissa = tmpreg;				          //得到整数部分
	fraction = (tmpreg - mantissa) * 16;     //得到小数部分	 
    mantissa <<= 4;
	mantissa += fraction;

	/* 打开RCC时钟 */
    RCC->APB2ENR |= 0x00000004 | 0x00004000;

    /* 设置PA9复用推挽输出， PA10上拉输入 */
    GPIOA->CRH &= 0xFFFFF00F;
    GPIOA->CRH |= 0x000008B0;
    GPIOA->ODR |= 0x00000400;

    /* 复位 */
    RCC->APB2RSTR |= (uint32_t)0x01<<14;   //复位串口1
	RCC->APB2RSTR &= ~((uint32_t)0x01<<14);//停止复位	   	   

    //波特率设置
 	USART1->BRR = mantissa; // 波特率设置	 
	USART1->CR1 |= 0x200C;  //1位停止,无校验位,并打开发送和接收使能

#ifdef USE_RX_INTERRUPT   
    USART1_NVIC_RxConfig();
#endif 		
}

/****************************************************************************
* Function Name  : USART1_SetWord
* Description    : 通过串口1发送字符串.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void USART1_SendWord(uint8_t *wd)
{	
	while(*wd)                      //检测是否发送数据是否为空
	{
        USART1->DR = *wd;
        while((USART1->SR & (uint16_t)0x0040) == 0);
		wd++;
	}		
}

/* 如果要使用printf函数的话要添加以下代码 */
/* 否则就打开Target Options里面的Target位置选择Use MicroLIB */
#ifdef USE_PRINTF

#pragma import(__use_no_semihosting)             
/* 标准库需要的支持函数 */                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout; 
/* 定义_sys_exit()以避免使用半主机模式 */    
_sys_exit(int x) 
{ 
	x = x; 
}

/****************************************************************************
* Function Name  : fputc
* Description    : 使用printf函数要重定向这个fputc函数.
* Input          : ch , *f
* Output         : None
* Return         : ch
****************************************************************************/
 
//int fputc(int ch, FILE *f)
//{
//	
//	USART_SendData(USART1, (uint8_t) ch);
//	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
//	
//	return ch;	
//}
//* 寄存器版 */
int fputc(int ch, FILE *f)
{      
	while((USART1->SR & 0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}

#endif

#ifdef USE_RX_INTERRUPT

/****************************************************************************
* Function Name  : USART1_NVIC_RxConfig
* Description    : 设置接收中断的中断等级，并打开总中断
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void USART1_NVIC_RxConfig(void)
{
    NVIC_Config(0, 0, USART1_IRQn);//设置中断优先级
    USART1->CR1 |= 0x10;           //开中断
}

/****************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : 串口1的中断函数
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void USART1_IRQHandler (void)
{
    uint8_t dat;
    if(USART1->SR & 0x20)
    {
        dat = USART1->DR;
        USART1->DR = dat;
        while((USART1->SR & 0X40) == 0);
    }
}

#endif















