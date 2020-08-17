#include "temp.h"

/* 定义内部函数 */
static void DS18B20_WriteData(uint8_t dat);
static uint8_t DS18B20_ReadData(void);

/****************************************************************************
* Function Name  : DS18B20_Init
* Description    : 初始化18B20的GPIO口.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void DS18B20_Init(void)
{
    /* 打开时钟使能 */
    RCC->APB2ENR |= 0x00000100;

    /* 设置GPIO参数 */
    GPIOG->CRH &= 0xFFFF0FFF;
    GPIOG->CRH |= 0x00008000;//设置输入下拉模式
    GPIOG->ODR &= ~(1 << 11);//设置下拉
}

/****************************************************************************
* Function Name  : DS18B20_Reset
* Description    : 复位DS18B20并等待应答.
* Input          : None
* Output         : None
* Return         : 0：成功;0xFF：失败
****************************************************************************/

uint8_t DS18B20_Reset(void)
{
	uint16_t i = 0;

	/* 将总线拉低480us~960us */
	DS18B20_Output();
	DS18B20_OUT = 1;
	DS18B20_OUT = 0;
	SYSTICK_Delay1us(750);
	
	/* 然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低 */
	DS18B20_OUT = 1;
	DS18B20_Input();

    /* 等待下降沿 */
	while(DS18B20_IN)
	{
		i++;
		if(i > 5000)	     //等50ms
		{
			return 0xFF;
		}
		SYSTICK_Delay1us(10);//10us
	}
	
    /* 等待总线释放 */
	i = 0;
	while(!DS18B20_IN)
	{
		i++;
		if(i > 5000)	      //等50ms
		{
			return 0xFF;
		}
		SYSTICK_Delay1us(10); //10us
	}

	return 0;     
}

/****************************************************************************
* Function Name  : DS18B20_WriteData
* Description    : 写一个字节数据
* Input          : dat：要写入的数据
* Output         : None
* Return         : None
****************************************************************************/

static void DS18B20_WriteData(uint8_t dat)
{
	uint8_t i;

	DS18B20_Output();

	for(i=0; i<8; i++)
	{
		/* 每写入一位数据之前先把总线拉低1us */
		DS18B20_OUT = 0;
		SYSTICK_Delay1us(1);
		
		/* 然后写入一个数据，从最低位开始 */
		if((dat & 0x01) == 0x01)
		{
			DS18B20_OUT = 1;	
		}
		else
		{
			DS18B20_OUT = 0;
		}
		dat >>= 1;

		/* 延时持续时间最少60us */
		SYSTICK_Delay1us(60);
		
		/* 释放总线，至少1us给总线恢复时间才能接着写入第二个数值 */
		DS18B20_OUT = 1;			
		SYSTICK_Delay1us(1);	
	}
}

/****************************************************************************
* Function Name  : DS18B20_ReadData
* Description    : 读取一个字节数据.
* Input          : None
* Output         : None
* Return         : dat：返回读取到的字节
****************************************************************************/

static uint8_t DS18B20_ReadData(void)
{
	uint8_t i, dat;
    	
	for(i=0; i<8; i++)
	{
		/* 每写入一位数据之前先把总线拉低1us */
		DS18B20_Output();
		DS18B20_OUT = 0;
		SYSTICK_Delay1us(1);

		/* 然后释放总线,延时6us等待数据稳定 */
		DS18B20_OUT = 1;	  
		SYSTICK_Delay1us(4);

		/* 读取数据，从最低位开始读取 */
		DS18B20_Input();
		dat >>= 1;
		if(DS18B20_IN == 1)
		{
			dat |= 0x80;
		}
		
		/* 读取完之后等待48us再接着读取下一个数 */
		SYSTICK_Delay1us(48);			
	}

	return dat;
}

/****************************************************************************
* Function Name  : DS18B20_ReadTemp
* Description    : 读取温度值.
* Input          : None
* Output         : None
* Return         : temp：读取到的温度值，并保留两位小数点（百倍）。
****************************************************************************/

int16_t DS18B20_ReadTemp(void)
{
	int16_t tempValue = 0;
    uint16_t temp;

    DS18B20_Reset();
	DS18B20_WriteData(0xCC);		//跳过ROM操作命令		 
	DS18B20_WriteData(0x44);	    //温度转换命令

    /* 如果不是连续读的话，这里加个延时。DS18B20转换温度需要时间 */
//    SYSTICK_Delay1ms(200);

	DS18B20_Reset();
	DS18B20_WriteData(0xCC);	    //跳过ROM操作命令
	DS18B20_WriteData(0xBE);	    //发送读取温度命令

	/* 读取16位温度值，先读低位 */
	temp = DS18B20_ReadData();
	tempValue = DS18B20_ReadData();
    tempValue <<= 8;
    tempValue |= temp;
    temp = tempValue;

	/* 温度处理，温度保留两位小数点 */
	if(tempValue < 0)
	{
		temp = temp - 1;
		temp = ~temp;
		temp = (float)temp * 6.25 + 0.5;
        return (-temp);   
	}
	else
	{
		temp = (float)temp * 6.25+ 0.5;
        return  temp;
	}

}




