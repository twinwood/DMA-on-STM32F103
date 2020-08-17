#include "dma.h"

/****************************************************************************
* Function Name  : DMA_Reset
* Description    : ��λDMA
* Input          : DMAy_Channelx��Ҫ��λ��DMAͨ��
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
* Description    : ��ʼ��DMA������
* Input          : DMA_CHx�����õ�ͨ��
*                * pAddr���������ַ
*                * mAddr�����������ڴ����ַ
*                * length��DMA����Ĵ�С
* Output         : None
* Return         : None
****************************************************************************/

void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,uint32_t pAddr,uint32_t mAddr,uint16_t length)
{
    /* ����ʱ�� */
    RCC->AHBENR |= 0x00000001;
    
    DMA_Reset(DMA_CHx);                    //��DMA��ͨ���Ĵ�������Ϊȱʡֵ

    /* ��ʼ������ */
    DMA_CHx->CPAR = pAddr; 	//DMA1 �����ַ 
	DMA_CHx->CMAR = mAddr; 	//DMA1,�洢����ַ
    DMA_CHx->CNDTR = length;//����DMA����������
    DMA_CHx->CCR |= 1 << 4; //�Ӵ洢����
	DMA_CHx->CCR |= 0;  	//��ͨģʽ
	DMA_CHx->CCR |= 0; 		//�����ַ������ģʽ
	DMA_CHx->CCR |= 1 << 7; //�洢������ģʽ
	DMA_CHx->CCR |= 0; 	 	//�������ݿ��Ϊ8λ
	DMA_CHx->CCR |= 0; 		//�洢�����ݿ��8λ
	DMA_CHx->CCR |= 1 << 12;//�е����ȼ�
	DMA_CHx->CCR |= 0; 		//�Ǵ洢�����洢��ģʽ
}

/****************************************************************************
* Function Name  : DMA_Enable
* Description    : ����һ��DMA����
* Input          : DMA_CHx��ʹ�õ�ͨ��
*                * length��DMA�����С
* Output         : None
* Return         : None
****************************************************************************/

void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx, uint32_t length)
{ 
    DMA_CHx->CCR &= ~(0x01);       //�ر�DMA���� 
	DMA_CHx->CNDTR = length;       //DMA1,���������� 
	DMA_CHx->CCR |= 0x01;          //����DMA���� 
}
