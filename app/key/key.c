#include "key.h"
#include "system.h"
#include "usart.h"

/* 声明函数 */
static void KEY_Delay10ms(void);

/****************************************************************************
* Function Name  : KEY_Config
* Description    : 初始化按键是用的IO口
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void KEY_Config(void)
{
	/*打开GPIOE的时钟*/
	RCC->APB2ENR |= 0x00000040;
    
    /* 设置PE2/PE3/PE4为上拉输入 */
    GPIOE->CRL &= 0xFFF000FF;
    GPIOE->CRL |= 0x00088800;
    GPIOE->ODR |= (0x07 << 2); 
    	          
    /* 打开GPIOA的时钟 */  
    RCC->APB2ENR |= 0x00000004;
    
    /* PA0为下拉输入 */
    GPIOA->CRL &= 0xFFFFFFF0;
    GPIOA->CRL |= 0x00000008;
    GPIOA->ODR &= 0xFFFFFFFE;
}

#ifdef USE_EXTI
/****************************************************************************
* Function Name  : KEY_NVIC_Config
* Description    : 初始化外部中断
* Input          : key：选择使用外部中断的按键
* Output         : None
* Return         : None
****************************************************************************/

void KEY_NVIC_Config(uint8_t key)
{
    /* 打开时钟使能 */
    if(key & KEY_UP)
    {
        /* 打开GPIOA的时钟 */  
        RCC->APB2ENR |= 0x00000004;
    }
    if(key & (KEY_DOWN | KEY_LEFT | KEY_RIGHT))
    {
        /*打开GPIOE的时钟*/
	    RCC->APB2ENR |= 0x00000040;     
    }
    /*打开AFIO的时钟*/
	RCC->APB2ENR |= 0x00000001;

    /* 设置GPIO参数 */    
    if(key & KEY_UP)
    {
        /* PA0为下拉输入 */
        GPIOA->CRL &= 0xFFFFFFF0;
        GPIOA->CRL |= 0x00000008;
        GPIOA->ODR &= 0xFFFFFFFE;
    }
    if(key & (KEY_DOWN | KEY_LEFT | KEY_RIGHT))
    {
        if(key & KEY_LEFT)
        {
            /* 设置PE2/PE3/PE4为上拉输入 */
            GPIOE->CRL &= 0xFFFFF0FF;
            GPIOE->CRL |= 0x00000800;
            GPIOE->ODR |= (0x01 << 2);
        }
        if(key & KEY_DOWN)
        {
            /* 设置PE2/PE3/PE4为上拉输入 */
            GPIOE->CRL &= 0xFFFF0FFF;
            GPIOE->CRL |= 0x00008000;
            GPIOE->ODR |= (0x01 << 3);        
        }
        if(key & KEY_RIGHT)
        {
            /* 设置PE2/PE3/PE4为上拉输入 */
            GPIOE->CRL &= 0xFFF0FFFF;
            GPIOE->CRL |= 0x00080000;
            GPIOE->ODR |= (0x01 << 4);
        }
    }

/***************************************************************************/
/**************************** 中断设置 *************************************/
/***************************************************************************/
    
    if(key & KEY_UP)
    {
        NVIC_Config(0, 0, EXTI0_IRQn);    
    }
    if(key & KEY_LEFT)
    {
        NVIC_Config(0, 0, EXTI2_IRQn); 
    }
    if(key & KEY_DOWN)
    {
        NVIC_Config(0, 0, EXTI3_IRQn); 
    }
    if(key & KEY_RIGHT)
    {
        NVIC_Config(0, 0, EXTI4_IRQn); 
    }

	/* 选择EXTI */
    if(key & KEY_UP)
    {
        /* 使用PA0做输入 */
        AFIO->EXTICR[0] &= 0xFFFFFFF0;
        AFIO->EXTICR[0] |= 0x00000000;
    }
    if(key & KEY_LEFT)
    {
        /* 使用PE2做输入 */
        AFIO->EXTICR[0] &= 0xFFFFF0FF;
        AFIO->EXTICR[0] |= 0x00000400;
    }
    if(key & KEY_DOWN)
    {
        /* 使用PE3做输入 */
        AFIO->EXTICR[0] &= 0xFFFF0FFF;
        AFIO->EXTICR[0] |= 0x00004000;
    }
    if(key & KEY_RIGHT)
    {
        /* 使用PE4做输入 */
        AFIO->EXTICR[1] &= 0xFFFFFFF0;
        AFIO->EXTICR[1] |= 0x00000004;
    }

    if(key & KEY_UP)
    {
        EXTI->IMR |= 0x00000001;  //开启中断输入线0上面的中断
        EXTI->FTSR &= 0xFFFFFFFE; //清除下降沿触发
        EXTI->RTSR |= 0x00000001; //设置上升沿触发
	}
    if(key & (KEY_DOWN | KEY_LEFT | KEY_RIGHT))
    {
        if(key & KEY_LEFT)
        {
            EXTI->IMR |= (uint32_t)0x01 << 2;     //开启中断输入线2上面的中断
            EXTI->RTSR &= ~((uint32_t)0x01 << 2); //清除上升沿触发
            EXTI->FTSR |= (uint32_t)0x01 << 2;    //设置下降沿触发
        }
        if(key & KEY_DOWN)
        {
            EXTI->IMR |= (uint32_t)0x01 << 3;     //开启中断输入线2上面的中断
            EXTI->RTSR &= ~((uint32_t)0x01 << 3); //清除上升沿触发
            EXTI->FTSR |= (uint32_t)0x01 << 3;    //设置下降沿触发
        }
        if(key & KEY_RIGHT)
        {
            EXTI->IMR |= (uint32_t)0x01 << 4;     //开启中断输入线2上面的中断
            EXTI->RTSR &= ~((uint32_t)0x01 << 4); //清除上升沿触发
            EXTI->FTSR |= (uint32_t)0x01 << 4;    //设置下降沿触发
        }
    }
}

/****************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : 外部中断0的中断函数
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & 0x00000001)
	{
        printf(" KEY_UP发生中断！\n");
	}
    /* 清除中断标志，写入1清除 */
	EXTI->PR |= 0x00000001;
}



/****************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : 外部中断2的中断函数
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI2_IRQHandler(void)
{
	if (EXTI->PR & (0x00000001 << 2))
	{
        printf(" KEY_LEFT发生中断！\n");
	}
	/* 清除中断标志，写入1清除 */
	EXTI->PR |= (0x00000001 << 2);
}

/****************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : 外部中断3的中断函数
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI3_IRQHandler(void)
{
	if (EXTI->PR & (0x00000001 << 3))
	{
        printf(" KEY_DOWN发生中断！\n");
	}
	/* 清除中断标志，写入1清除 */
	EXTI->PR |= (0x00000001 << 3);
}

/****************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : 外部中断4的中断函数
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI4_IRQHandler(void)
{
	if (EXTI->PR & (0x00000001 << 4))
	{
        printf(" KEY_RIGHT发生中断！\n");
	}
	/* 清除中断标志，写入1清除 */
	EXTI->PR |= (0x00000001 << 4);
}

#endif

/****************************************************************************
* Function Name  : KEY_Delay10ms
* Description    : 按键使用的消抖延时函数。注意：该延时函数具体延时时间是不确
*                * 定的，并非真的就延时10ms，要精确延时请使用定时器。
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void KEY_Delay10ms(void)
{
    uint16_t i;
    for(i=0; i<0x5FFF; i++);
}

/****************************************************************************
* Function Name  : KEY_Scan
* Description    : 按键扫描，注意该按键扫描支持一次按1个键，不支持同时按多个键
* Input          : None
* Output         : None
* Return         : keyValue：按键的键值
****************************************************************************/

uint8_t KEY_Scan(void)
{
    uint8_t keyValue = 0, timeCount = 0;

    if((KEY0 == 1) || (KEY1 == 0) || (KEY2 == 0) || (KEY3 == 0)) //检测是否有按键按下
    {
        KEY_Delay10ms();                                      //延时消抖

        /* 检测是哪个按键按下 */
        if(KEY0 == 1)
        {
            keyValue = KEY_UP;
        }
        else if(KEY1 == 0)
        {
            keyValue = KEY_LEFT;
        }
        else if(KEY2 == 0)
        {
            keyValue = KEY_DOWN;
        }
        else if(KEY3 == 0)
        {
            keyValue = KEY_RIGHT;
        }
        else
        {
            keyValue = 0;
        }
        
        /* 有按键按下时，做松手检测 */
        if(keyValue != 0)
        {
            while(((KEY0 == 1) || (KEY1 == 0) || (KEY2 == 0) || (KEY3 == 0)) && (timeCount < 150))
            {
                KEY_Delay10ms();
                timeCount++;        
            }
//            KEY_Delay10ms();//由于主函数中程序较少，连续扫描的速度太快，加一个松手消抖减少误读   
        }        
    }

    return keyValue;
}












