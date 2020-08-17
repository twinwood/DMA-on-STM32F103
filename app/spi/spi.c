#include "spi.h"

/****************************************************************************
* Function Name  : SPI1_Config
* Description    : 初始化SPI2
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SPI1_Config(void)
{
    /* SPI的IO口和SPI外设打开时钟 */
    RCC->APB2ENR |= 0x00000004 | 0x00001000;

    /* SPI的IO口设置 */
    GPIOA->CRL &= 0x000FFFFF;
    GPIOA->CRL |= 0xBBB00000;
    GPIOA->ODR |= 0x07 << 5;

/***************************************************************************/
/************************* 设置SPI的参数 ***********************************/
/***************************************************************************/
	SPI1->CR1 &= ~((uint16_t)0x01 << 10);		//全双工模式	
	SPI1->CR1 |= (uint16_t)0x01 << 9; 		    //软件nss管理
	SPI1->CR1 |= (uint16_t)0x01 << 8;  

	SPI1->CR1 |= (uint16_t)0x01 << 2; 		    //SPI主机
	SPI1->CR1 &= ~((uint16_t)0x01 << 11);		//8bit数据格式	
	SPI1->CR1 |= (uint16_t)0x01 << 1; 		    //空闲模式下SCK为1 时钟空闲时为高电平
	SPI1->CR1 |= (uint16_t)0x01;               //数据采样从第二个时间边沿开始,CPHA=1  
	/* 对SPI2属于APB1的外设.时钟频率最大为36M. */
	SPI1->CR1 |= (uint16_t)0x07 << 3; 		    //Fsck=Fpclk1/256
	SPI1->CR1 &= ~((uint16_t)0x01 << 7); 	    //MSBfirst   
	SPI1->CR1 |= (uint16_t)1 << 6; 		        //SPI设备使能
    
    SPI1_WriteReadData(0xFF);
}      

/****************************************************************************
* Function Name  : SPI2_Config
* Description    : 初始化SPI2
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SPI2_Config(void)
{    
    /* SPI的IO口和SPI外设打开时钟 */
    RCC->APB2ENR |= 0x00000008;
    RCC->APB1ENR |= 0x00004000;

    /* SPI的IO口设置 */
    GPIOB->CRH &= 0x000FFFFF;
    GPIOB->CRH |= 0xBBB00000;
    GPIOB->ODR |= 0x07 << 13;
/***************************************************************************/
/************************* 设置SPI的参数 ***********************************/
/***************************************************************************/
	SPI2->CR1 &= ~((uint16_t)0x01 << 10);		//全双工模式	
	SPI2->CR1 |= (uint16_t)0x01 << 9; 		    //软件nss管理
	SPI2->CR1 |= (uint16_t)0x01 << 8;  

	SPI2->CR1 |= (uint16_t)0x01 << 2; 		    //SPI主机
	SPI2->CR1 &= ~((uint16_t)0x01 << 11);		//8bit数据格式	
	SPI2->CR1 &= ~((uint16_t)0x01 << 1); 		//空闲模式下SCK为0 时钟空闲时为低电平
	SPI2->CR1 &= ~(uint16_t)0x01;               //数据采样从第一个时间边沿开始,CPHA=1  
	/* 对SPI2属于APB1的外设.时钟频率最大为36M. */
	SPI2->CR1 |= (uint16_t)0x07 << 3; 		    //Fsck=Fpclk1/256
	SPI2->CR1 &= ~((uint16_t)0x01 << 7); 	    //MSBfirst   
	SPI2->CR1 |= (uint16_t)1 << 6; 		        //SPI设备使能

    SPI2_WriteReadData(0xFF);
}      

/****************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : 设置SPI1的传输速度。
* Input          : 速度波特率分频
* Output         : None
* Return         : None
****************************************************************************/

void SPI1_SetSpeed(uint8_t speed)
{
	SPI1->CR1 &= 0xFFC7; 
	SPI1->CR1 |= speed;
	SPI1->CR1 |= 0x40;	
}

/****************************************************************************
* Function Name  : SPI2_SetSpeed
* Description    : 设置SPI2的分频数，以改变SPI2的速度.
* Input          : Speed：分频数
* Output         : None
* Return         : None
****************************************************************************/

void SPI2_SetSpeed(uint8_t Speed)
{	
	SPI2->CR1 &= 0xFFC7; 
	SPI2->CR1 |= Speed;
    SPI2->CR1 |= 0x40;	 		
}

/****************************************************************************
* Function Name  : SPI1_WriteReadData
* Description    : 使用SPI1写入一个字节数据同时读取一个字节数据。
* Input          : dat：要写的8位数据
* Output         : None
* Return         : 读取到的8位数据
****************************************************************************/

uint8_t SPI1_WriteReadData(uint8_t dat)
{
	uint16_t i = 0;

    /* 当发送缓冲器空 */	
 	while((SPI1->SR & 0x02) == 0)
	{
		i++;
		if(i > 10000)
		{
			return 0xFF;
		}
	}
	
    /* 发送数据 */
   	SPI1->DR = dat;
	
	/* 等待接收缓冲器为非空 */
	while((SPI1->SR & 0x01) == 0);
	
 	/* 将读取到的数值返回 */
 	return SPI1->DR;		
}

/****************************************************************************
* Function Name  : SPI2_WriteReadData
* Description    : 使用SPI2写入一个字节数据同时读取一个字节数据。
* Input          : dat：写入的数据
* Output         : None
* Return         : 读取到的数据
*                * 读取失败返回0xFF
****************************************************************************/

uint8_t SPI2_WriteReadData(uint8_t dat)
{
	uint16_t i = 0;

    /* 当发送缓冲器空 */	
 	while((SPI2->SR & 0x02) == 0)
	{
		i++;
		if(i > 10000)
		{
			return 0xFF;
		}
	}
	
    /* 发送数据 */
    SPI2->DR = dat;
	
	/* 等待接收缓冲器为非空 */
	while((SPI2->SR & 0x01) == 0);
	
 	/* 将读取到的数值返回 */
 	return SPI2->DR;		
}



















