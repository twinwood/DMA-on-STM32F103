#include "dma.h"

/****************************************************************************
* Function Name  : DMA_Reset
* Description    : 复位DMA
* Input          : DMAy_Channelx：要复位的DMA通道
* Output         : None
* Return         : None
****************************************************************************/

void DMA_Reset(DMA_Channel_TypeDef* DMAy_Channelx)
{    
    /* Disable the selected DMAy Channelx */
    DMAy_Channelx->CCR &= (uint16_t)(~DMA_CCR1_EN);
    
    /* Reset DMAy Channelx control register */
    DMAy_Channelx->CCR  = 0;
    
    /* Reset DMAy Channelx remaining bytes register */
    DMAy_Channelx->CNDTR = 0;
    
    /* Reset DMAy Channelx peripheral address register */
    DMAy_Channelx->CPAR  = 0;
    
    /* Reset DMAy Channelx memory address register */
    DMAy_Channelx->CMAR = 0;
    
    if (DMAy_Channelx == DMA1_Channel1)
    {
        /* Reset interrupt pending bits for DMA1 Channel1 */
        DMA1->IFCR |= 0x0F;
    }
    else if (DMAy_Channelx == DMA1_Channel2)
    {
        /* Reset interrupt pending bits for DMA1 Channel2 */
        DMA1->IFCR |= 0x0F << 4;
    }
    else if (DMAy_Channelx == DMA1_Channel3)
    {
        /* Reset interrupt pending bits for DMA1 Channel3 */
        DMA1->IFCR |= 0x0F << 8;
    }
    else if (DMAy_Channelx == DMA1_Channel4)
    {
        /* Reset interrupt pending bits for DMA1 Channel4 */
        DMA1->IFCR |= 0x0F << 12;
    }
    else if (DMAy_Channelx == DMA1_Channel5)
    {
        /* Reset interrupt pending bits for DMA1 Channel5 */
        DMA1->IFCR |= 0x0F << 16;
    }
    else if (DMAy_Channelx == DMA1_Channel6)
    {
        /* Reset interrupt pending bits for DMA1 Channel6 */
        DMA1->IFCR |= 0x0F << 20;
    }
    else if (DMAy_Channelx == DMA1_Channel7)
    {
        /* Reset interrupt pending bits for DMA1 Channel7 */
        DMA1->IFCR |= 0x0F << 24;
    }
    else if (DMAy_Channelx == DMA2_Channel1)
    {
        /* Reset interrupt pending bits for DMA2 Channel1 */
        DMA2->IFCR |= 0x0F;
    }
    else if (DMAy_Channelx == DMA2_Channel2)
    {
        /* Reset interrupt pending bits for DMA2 Channel2 */
        DMA2->IFCR |= 0x0F << 4;
    }
    else if (DMAy_Channelx == DMA2_Channel3)
    {
        /* Reset interrupt pending bits for DMA2 Channel3 */
        DMA2->IFCR |= 0x0F << 8;
    }
    else if (DMAy_Channelx == DMA2_Channel4)
    {
        /* Reset interrupt pending bits for DMA2 Channel4 */
        DMA2->IFCR |= 0x0F << 12;
    }
    else
    { 
        if (DMAy_Channelx == DMA2_Channel5)
        {
          /* Reset interrupt pending bits for DMA2 Channel5 */
          DMA2->IFCR |= 0x0F << 16;
        }
    }
}

/****************************************************************************
* Function Name  : DMA_Config
* Description    : 初始化DMA的设置
* Input          : DMA_CHx：设置的通道
*                * pAddr：外设基地址
*                * mAddr：发送数据内存基地址
*                * length：DMA缓存的大小
* Output         : None
* Return         : None
****************************************************************************/

void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,uint32_t pAddr,uint32_t mAddr,uint16_t length)
{
    /* 开启时钟 */
    RCC->AHBENR |= 0x00000001;
    
    DMA_Reset(DMA_CHx);                    //将DMA的通道寄存器重设为缺省值

    /* 初始化设置 */
    DMA_CHx->CPAR = pAddr; 	//DMA1 外设地址 
	DMA_CHx->CMAR = mAddr; 	//DMA1,存储器地址
    DMA_CHx->CNDTR = length;//保存DMA传输数据量
    DMA_CHx->CCR |= 1 << 4; //从存储器读
	DMA_CHx->CCR |= 0;  	//普通模式
	DMA_CHx->CCR |= 0; 		//外设地址非增量模式
	DMA_CHx->CCR |= 1 << 7; //存储器增量模式
	DMA_CHx->CCR |= 0; 	 	//外设数据宽度为8位
	DMA_CHx->CCR |= 0; 		//存储器数据宽度8位
	DMA_CHx->CCR |= 1 << 12;//中等优先级
	DMA_CHx->CCR |= 0; 		//非存储器到存储器模式
}

/****************************************************************************
* Function Name  : DMA_Enable
* Description    : 开启一次DMA传送
* Input          : DMA_CHx：使用的通道
*                * length：DMA缓存大小
* Output         : None
* Return         : None
****************************************************************************/

void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx, uint32_t length)
{ 
    DMA_CHx->CCR &= ~(0x01);       //关闭DMA传输 
	DMA_CHx->CNDTR = length;       //DMA1,传输数据量 
	DMA_CHx->CCR |= 0x01;          //开启DMA传输 
}
