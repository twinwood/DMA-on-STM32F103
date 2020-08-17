#include "system.h"

#ifdef USE_UCOSII_OS
#include "os_includes.h"
#endif

/****************************************************************************
* Function Name  : RCC_Reset
* Description    : 复位RCC时钟
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void RCC_Reset(void)
{
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
  RCC->CFGR &= (uint32_t)0xF0FF0000;
  
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;

}
/****************************************************************************
* Function Name  : SYSTEM_SetClock
* Description    : 设置外部时钟作为PLL输入，并设置相应的时钟频率。注意时钟频率
*                * 要是8的倍数。该函数只提供32MHZ到72MHZ的设置。
* Input          : freq：频率参数（该参数只能设置为8的倍数，从4倍到9倍）
* Output         : None
* Return         : 0xFF：表示设置失败，时钟返回初始化状态
*                * 0：表示初始化成功
****************************************************************************/

uint8_t SYSTEM_SetClock(uint8_t freq)
{
	uint32_t rccCrHserdy = 0;
	uint32_t faultTime = 0, RCC_CFGR_PLL;

    /* 选择倍频系数 */
    switch(freq)
    {
        case(32):
            RCC_CFGR_PLL = RCC_CFGR_PLLMULL4;
            break;
        case(40):
            RCC_CFGR_PLL = RCC_CFGR_PLLMULL5;
            break;
        case(48):
            RCC_CFGR_PLL = RCC_CFGR_PLLMULL6;
            break;
        case(56):
            RCC_CFGR_PLL = RCC_CFGR_PLLMULL7;
            break;
        case(64):
            RCC_CFGR_PLL = RCC_CFGR_PLLMULL8;
            break;
        case(72):
            RCC_CFGR_PLL = RCC_CFGR_PLLMULL9;
            break;
        default:
            return 0xFF;
    }

	/*复位RCC_CR寄存器*/
	RCC_Reset();

	/*开启外部时钟*/
	RCC->CR &= (~RCC_CR_HSEON);
	RCC->CR |= RCC_CR_HSEON;//设置第16位

	/*检测外部时钟开启成功*/
	do
	{
		rccCrHserdy = RCC->CR & RCC_CR_HSERDY;//检测第17位是否为1
		faultTime++;//检测时间
	}
	while ((faultTime<0x0FFFFFFF) && (rccCrHserdy==0));
	/* 如果外部时钟开启成功*/
	if ((RCC->CR & RCC_CR_HSERDY) != 0)
	{
		/* Enable Prefetch Buffer */
	    FLASH->ACR |= FLASH_ACR_PRFTBE;

	    /* Flash 2 wait state */
	    FLASH->ACR &= (~(uint32_t)FLASH_ACR_LATENCY);
	    FLASH->ACR |= (uint32_t) FLASH_ACR_LATENCY_2;  

		/*AHB（0000）、APB2（000）不分频（即PLL输出时钟）*/
		/*APB1（100）要2分频（因最大只能36MHZ）*/
		RCC->CFGR &= (~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2));
		RCC->CFGR |= (RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2
		              | RCC_CFGR_PPRE2_DIV1);

		/*设置HSE为输入时钟，同事HSE不分频*/
		RCC->CFGR &= (~(uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE ));
		RCC->CFGR |= (RCC_CFGR_PLLSRC);

		/*设置PLL倍频系数为9倍*/
		RCC->CFGR &= (~RCC_CFGR_PLLMULL);//先清除原来的设置
		RCC->CFGR |= (RCC_CFGR_PLL);//设置倍频系数

		/*打开PLL使能*/
		RCC->CR |= RCC_CR_PLLON;

		/*等待开启PLL开启成功*/
		while ((RCC->CR & RCC_CR_PLLRDY) == 0)
		{
		}

		/*将PLL作为SYSCLK的时候来源*/
		RCC->CFGR &= (~RCC_CFGR_SW);//先清除先前的设置
		RCC->CFGR |= RCC_CFGR_SW_PLL;

		/*等待PLL作为SYSCLK时钟启动成功*/
		while ((RCC->CFGR & RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
		{
		}
	}/*end of if((RCC->CR & RCC_CR_HSERDY) != 0)*/
	/*如果外部时钟开启失败*/
	else
	{
	    RCC_Reset();
        return 0xFF;
	}

    return 0;	
}

/****************************************************************************
* Function Name  : NVIC_PriorityGroupConfig
* Description    : 设置NVIC中断系统抢占优先和从优先的配置（注:现设置为3位抢占
*                * 优先，1位从优先。）
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void NVIC_SetPriorityGroup(uint8_t group)
{
    uint32_t reg;

    /* 组别只有三位长度 */
    reg = (~(uint32_t)group) & 0x00000007;
    reg <<= 8;
    /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
    /* 0x05FA0000是访问钥匙，group是分组设置 */
    SCB->AIRCR = 0x05FA0000 | reg;
    
}

/****************************************************************************
* Function Name  : NVIC_Config
* Description    : 设置相应NVIC的中断优先级.
* Input          : preemptionPriority：抢占优先级别
*                * subPriority：从优先级别
*                * Channel：设置的中断通道
* Output         : None
* Return         : None
****************************************************************************/

void NVIC_Config(uint8_t preemptionPriority,uint8_t subPriority,uint8_t Channel)
{
    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    /* Compute the Corresponding IRQ Priority --------------------------------*/
    /* 读取NVIC分组设置 */    
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    /* 设置中断优先级 */
    tmppriority = (uint32_t)preemptionPriority << tmppre;
    tmppriority |=  subPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[Channel] = tmppriority;
    
    /* 使能相应的中断 */
    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[Channel >> 0x05] = (uint32_t)0x01 << (Channel & (uint8_t)0x1F);
}

#ifndef USE_UCOSII_OS
/****************************************************************************
* Function Name  : SYSTICK_Init
* Description    : 开始系统滴答定时器，并设置定时时间。
*                * 它的中断函数在stm32f10x_it.c的139行。
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Config(void)
{
    /* 时钟是72MHZ时，所以计数72次(最大为16777215)，就是1us */
	/* 主要是为了设置时钟 */
	if(SysTick_Config(72) == 0)//开启成功返回0
	{
		/* 调用这个设置函数的时候默认是直接打开计数器的，现在把它关掉。 */
		/* 关闭中断 */	
		 SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);		 	
	}    
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1us
* Description    : 延时函数。
* Input          : us：延时的时间
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1us(uint16_t us)
{
	uint32_t countValue;

	SysTick->LOAD  = (us * 72) - 1;             //设置重装数值, 72MHZ时
	SysTick->VAL   = 0;	                        //清零计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//打开计时器

	do
	{
		countValue = SysTick->CTRL; 	
	}
	while(!(countValue & (1 << 16)));			//等待时间到来

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器	
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1ms
* Description    : 延时函数。
* Input          : ms：延时的时间
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1ms(uint16_t ms)
{
	uint32_t countValue;

	SysTick->LOAD  = 72000 - 1;                 //设置重装数值, 72MHZ时延时1ms
	SysTick->VAL   = 0;	                        //清零计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//打开计时器

	do
	{
		countValue = SysTick->CTRL;
		if(countValue & (1 << 16))              //当到1ms是计数减1
		{
			ms--;
		} 	
	}
	while(ms);			                        //等待时间到来

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器	
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1s
* Description    : 延时函数。
* Input          : s：延时的时间
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1s(uint16_t s)
{
	uint32_t countValue;

	s *= 5; 
	SysTick->LOAD  = 72000 * 200 - 1;           //设置重装数值, 72MHZ时延时200ms
	SysTick->VAL   = 0;	                        //清零计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//打开计时器

	do
	{
		countValue = SysTick->CTRL;
		if(countValue & (1 << 16))              //当到200ms是计数减1
		{
			s--;
		} 	
	}
	while(s);			                        //等待时间到来

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器	
}

#else


/****************************************************************************
* Function Name  : SYSTICK_Delay1us
* Description    : 延时函数。
* Input          : us：延时的时间
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1us(uint16_t us)
{
    uint32_t oldValue, newValue, ticks, reloadValue, countValue = 0;

    /* 读取重装值 */
    reloadValue = SysTick->LOAD;
    
    /* 计算延时需要多少个滴答，现在设置的延时是在时钟为72MHZ的基础上设置的 */
    ticks = us * 72;

    oldValue = SysTick->VAL;  //刚进入时的计数器值
    while(1)
    {
        newValue = SysTick->VAL;
        if(newValue != oldValue)
        {
            if(newValue < oldValue)    //滴答计数器是倒数计数器
            {
                countValue += oldValue - newValue;
            }
            else
            {
                countValue += reloadValue - newValue + oldValue;
            }
            if(countValue >= ticks)
            {
                break;
            }    
        }
    }   	
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1ms
* Description    : 延时函数。
* Input          : ms：延时的时间
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1ms(uint16_t ms)
{
    if(OSRunning != 0)
    {
        OSTimeDlyHMSM(0, 0, 0, ms); 
    }
    else
    {
        while(ms--)
        {
            SYSTICK_Delay1us(1000);    
        }    
    }   	
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1s
* Description    : 延时函数。
* Input          : s：延时的时间
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1s(uint16_t s)
{
    OSTimeDlyHMSM(0, 0, s, 0);    	
}

#endif

