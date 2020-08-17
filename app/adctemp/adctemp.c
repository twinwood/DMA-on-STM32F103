#include "adctemp.h"

/****************************************************************************
* Function Name  : ADCTEMP_Config
* Description    : 初始化内部ADC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void ADCTEMP_Config(void)
{    
	/* 打开时钟 */
    RCC->APB2ENR |= 0x00000200;

    RCC->CFGR &= ~(3 << 14);
    RCC->CFGR |= 2 << 14;   //6分频 72M/6=12,ADC 最大时钟不能超过 14M

    /* 初始化ADC参数 */
    ADC1->CR1 &= 0xF0FFFF;     //工作模式清零
	ADC1->CR1 |= 0 << 16;      //独立工作模式

    ADC1->CR1 &= ~(1 << 8);    //非扫描模式	  
	ADC1->CR2 &= ~(1 << 1);    //单次转换模式
	ADC1->CR2 &= ~(7 << 17);	   
	ADC1->CR2 |= 7 << 17;	   //软件控制转换  
	ADC1->CR2 |= 1 << 20;      //使用用外部触发(SWSTART)!!!	必须使用一个事件来触发
	ADC1->CR2 &= ~(1 << 11);   //右对齐	 
	ADC1->SQR1 &= ~(0XF << 20);
	ADC1->SQR1 |= 0;           //1个转换在规则序列中 也就是只转换规则序列1 			   
	/* 设置通道1的采样时间 */
	ADC1->SMPR2 &= ~(7 << 3);  //通道1采样时间清空	  
 	ADC1->SMPR2 |= 7 << 3;     //通道1  239.5周期,提高采样时间可以提高精确度	 
	ADC1->CR2 |= 1;	           //开启AD转换器	 
	    
    /* 开启内部温度传感器 */
    ADC1->CR2 |= 1 << 23;
    
    ADC1->CR2 |= 1 << 3;         //使能复位校准  
	while(ADC1->CR2 & (1 << 3)); //等待校准结束 			 
    /* 该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。*/ 		 
	ADC1->CR2 |= 1 << 2;         //开启AD校准	   
	while(ADC1->CR2 & (1 << 2)); //等待校准结束        
}

/****************************************************************************
* Function Name  : ADCTEMP_GetAdc
* Description    : 读取温度传感器的AD值
* Input          : None
* Output         : None
* Return         : 返回读取到的AD值
****************************************************************************/

static uint16_t ADCTEMP_GetAdc(void)
{
    uint16_t value;

    /* 温度传感器是通道16，规则采样顺序值为1，采样时间为 239.5周期。 */
    ADC1->SMPR1 &= ~(7 << 21);
    ADC1->SMPR1 |= 7 << 21;

    /* 规则采样顺序值为1 ,通道16*/
    ADC1->SQR3 &= ~0x1F;
    ADC1->SQR3 |= 16;

    ADC1->CR2 |= 1 << 22;        //启动规则转换通道
    while(!(ADC1->SR & 0x02));   //等待转换结束

	/* 读取ADC1规则组的转换结果 */	
	value = ADC1->DR;; 
        
    return value;
}

/****************************************************************************
* Function Name  : ADCTEMP_GetTemp
* Description    : 读取温度值
* Input          : None
* Output         : None
* Return         : 返回读取到的温度值（放大100倍，保留两位小数点）
****************************************************************************/

int16_t ADCTEMP_GetTemp(void)
{
    uint32_t adValue = 0;
    int16_t tempValue;
    float adVoltage;
    uint8_t time;
    
    /*  读取十次然后求出平均值 */
    for(time = 0; time < 10; time++)
    {
        adValue += ADCTEMP_GetAdc();
    }
    adValue = adValue / 10;

    /* 将AD值转换为电压值 */
    adVoltage = (float)adValue * 3.3 / 4096;

    /* 根据公式求出温度值 */
    adVoltage = ((float)(ADCTEMP_Vsence25 - adVoltage) / ADCTEMP_AvgSlope) + 25; 
    tempValue = adVoltage * 100;   //保留两位小数点

    return tempValue;
}







