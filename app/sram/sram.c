#include"sram.h"

/****************************************************************************
* Function Name  : SRAM_Config
* Description    : ��ʼ��FSMC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SRAM_Config(void)
{  	
/*--------------------------------------------------------------------------------*/
/*------------------- GPIO Config ------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
	RCC->APB2ENR |= 1 << 5;      //ʹ��PORTDʱ��
	RCC->APB2ENR |= 1 << 6;      //ʹ��PORTEʱ��
 	RCC->APB2ENR |= 1 << 7;      //ʹ��PORTFʱ��
	RCC->APB2ENR |= 1 << 8;      //ʹ��PORTGʱ��	   
	
	/* PORTD����������� */	
	GPIOD->CRH &= 0x00000000;
	GPIOD->CRH |= 0xBBBBBBBB; 
	GPIOD->CRL &= 0xFF00FF00;
	GPIOD->CRL |= 0x00BB00BB;
      	 
	/* PORTE����������� */	
	GPIOE->CRH &= 0x00000000;
	GPIOE->CRH |= 0xBBBBBBBB; 
	GPIOE->CRL &= 0x0FFFFF00;
	GPIOE->CRL |= 0xB00000BB;
     
	/* PORTF����������� */
	GPIOF->CRH &= 0x0000FFFF;
	GPIOF->CRH |= 0xBBBB0000;  	    	 											 
	GPIOF->CRL &= 0xFF000000;
	GPIOF->CRL |= 0x00BBBBBB; 

	/* PORTG����������� PG10->NE3 */     	 											 
	GPIOG->CRH &= 0xFFFFF0FF;
	GPIOG->CRH |= 0x00000B00;  
 	GPIOG->CRL &= 0xFF000000;
	GPIOG->CRL |= 0x00BBBBBB;  	

/* ------------------------------------------------------------------------------ */
/* ------------ FSMC Config ----------------------------------------------------- */
/* ------------------------------------------------------------------------------ */
	/* �Ĵ�������                                              */
	/* bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����  */
	/* ��������ʹ��NE3 ��Ҳ�Ͷ�ӦBTCR[4],[5]��				   */ 
	FSMC_Bank1->BTCR[4] = 0x00000000;
	FSMC_Bank1->BTCR[5] = 0x00000000;
	FSMC_Bank1E->BWTR[4] = 0x00000000;
	/* ����BCR�Ĵ���	ʹ���첽ģʽ,ģʽA(��д����һ��ʱ��Ĵ���) */
	/* BTCR[ż��]:BCR�Ĵ���;BTCR[����]:BTR�Ĵ��� */
	FSMC_Bank1->BTCR[4] |= 1 << 12;//�洢��дʹ��
	FSMC_Bank1->BTCR[4] |= 1 << 4; //�洢�����ݿ��Ϊ16bit 	    
	/* ����BTR�Ĵ��� */								    
	FSMC_Bank1->BTCR[5] |= 3 << 8; //���ݱ���ʱ�䣨DATAST��Ϊ3��HCLK 4/72M=55ns(��EM��SRAMоƬ)	 	 
	FSMC_Bank1->BTCR[5] |= 0;      //��ַ����ʱ�䣨ADDHLD��δ�õ�	  	 
	FSMC_Bank1->BTCR[5] |= 0;      //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns	 	 
	/* ����дʱ��Ĵ��� */ 
	FSMC_Bank1E->BWTR[4] = 0x0FFFFFFF;//Ĭ��ֵ
	/* ʹ��BANK1����3 */
	FSMC_Bank1->BTCR[4] |= 1; 

	/* ��FSMC��ʱ�� */
    RCC->AHBENR |= 1 << 8;          //ʹ��FSMCʱ��	  

}

/****************************************************************************
* Function Name  : SRAM_WriteBuffer
* Description    : ��SRAM����д��һ�����ȵ�����
* Input          : writeBuf��д�뻺��
*                * writeAddr��д����ʼ��ַ
*                * length��д�����ݳ���
* Output         : None
* Return         : None
****************************************************************************/

void SRAM_WriteBuffer(uint16_t *writeBuf, uint32_t writeAddr, uint32_t length)
{
  while(length--) /*!< while there is data to write */
  {
    /*!< Transfer data to the memory */
    *(uint16_t *) (Bank1_SRAM3_ADDR + writeAddr) = *writeBuf++;
    
    /*  ʮ��λ���ȵ��ǵ�ַ+2 */  
    writeAddr += 2;
  }   
}

/****************************************************************************
* Function Name  : SRAM_ReadBuffer
* Description    : ��ȡSRAM����
* Input          : readBuff����ȡ����
*                * readAddr����ȡ��ʼ��ַ
*                * length����ȡ���ݳ���
* Output         : None
* Return         : None
****************************************************************************/

void SRAM_ReadBuffer(uint16_t* readBuff, uint32_t readAddr, uint32_t length)
{
  while(length--) /*!< while there is data to read */
  {
    /*!< Read a half-word from the memory */
    *readBuff++ = *(__IO uint16_t*) (Bank1_SRAM3_ADDR + readAddr);

    /*  ʮ��λ���ȵ��ǵ�ַ+2 */  
    readAddr += 2;
  }  
}
