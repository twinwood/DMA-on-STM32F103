#include"sram.h"

/****************************************************************************
* Function Name  : SRAM_Config
* Description    : 初始化FSMC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SRAM_Config(void)
{  	
/*--------------------------------------------------------------------------------*/
/*------------------- GPIO Config ------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
	RCC->APB2ENR |= 1 << 5;      //使能PORTD时钟
	RCC->APB2ENR |= 1 << 6;      //使能PORTE时钟
 	RCC->APB2ENR |= 1 << 7;      //使能PORTF时钟
	RCC->APB2ENR |= 1 << 8;      //使能PORTG时钟	   
	
	/* PORTD复用推挽输出 */	
	GPIOD->CRH &= 0x00000000;
	GPIOD->CRH |= 0xBBBBBBBB; 
	GPIOD->CRL &= 0xFF00FF00;
	GPIOD->CRL |= 0x00BB00BB;
      	 
	/* PORTE复用推挽输出 */	
	GPIOE->CRH &= 0x00000000;
	GPIOE->CRH |= 0xBBBBBBBB; 
	GPIOE->CRL &= 0x0FFFFF00;
	GPIOE->CRL |= 0xB00000BB;
     
	/* PORTF复用推挽输出 */
	GPIOF->CRH &= 0x0000FFFF;
	GPIOF->CRH |= 0xBBBB0000;  	    	 											 
	GPIOF->CRL &= 0xFF000000;
	GPIOF->CRL |= 0x00BBBBBB; 

	/* PORTG复用推挽输出 PG10->NE3 */     	 											 
	GPIOG->CRH &= 0xFFFFF0FF;
	GPIOG->CRH |= 0x00000B00;  
 	GPIOG->CRL &= 0xFF000000;
	GPIOG->CRL |= 0x00BBBBBB;  	

/* ------------------------------------------------------------------------------ */
/* ------------ FSMC Config ----------------------------------------------------- */
/* ------------------------------------------------------------------------------ */
	/* 寄存器清零                                              */
	/* bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。  */
	/* 这里我们使用NE3 ，也就对应BTCR[4],[5]。				   */ 
	FSMC_Bank1->BTCR[4] = 0x00000000;
	FSMC_Bank1->BTCR[5] = 0x00000000;
	FSMC_Bank1E->BWTR[4] = 0x00000000;
	/* 操作BCR寄存器	使用异步模式,模式A(读写共用一个时序寄存器) */
	/* BTCR[偶数]:BCR寄存器;BTCR[奇数]:BTR寄存器 */
	FSMC_Bank1->BTCR[4] |= 1 << 12;//存储器写使能
	FSMC_Bank1->BTCR[4] |= 1 << 4; //存储器数据宽度为16bit 	    
	/* 操作BTR寄存器 */								    
	FSMC_Bank1->BTCR[5] |= 3 << 8; //数据保持时间（DATAST）为3个HCLK 4/72M=55ns(对EM的SRAM芯片)	 	 
	FSMC_Bank1->BTCR[5] |= 0;      //地址保持时间（ADDHLD）未用到	  	 
	FSMC_Bank1->BTCR[5] |= 0;      //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns	 	 
	/* 闪存写时序寄存器 */ 
	FSMC_Bank1E->BWTR[4] = 0x0FFFFFFF;//默认值
	/* 使能BANK1区域3 */
	FSMC_Bank1->BTCR[4] |= 1; 

	/* 打开FSMC的时钟 */
    RCC->AHBENR |= 1 << 8;          //使能FSMC时钟	  

}

/****************************************************************************
* Function Name  : SRAM_WriteBuffer
* Description    : 向SRAM里面写入一定长度的数据
* Input          : writeBuf：写入缓存
*                * writeAddr：写入起始地址
*                * length：写入数据长度
* Output         : None
* Return         : None
****************************************************************************/

void SRAM_WriteBuffer(uint16_t *writeBuf, uint32_t writeAddr, uint32_t length)
{
  while(length--) /*!< while there is data to write */
  {
    /*!< Transfer data to the memory */
    *(uint16_t *) (Bank1_SRAM3_ADDR + writeAddr) = *writeBuf++;
    
    /*  十六位长度的是地址+2 */  
    writeAddr += 2;
  }   
}

/****************************************************************************
* Function Name  : SRAM_ReadBuffer
* Description    : 读取SRAM数据
* Input          : readBuff：读取缓存
*                * readAddr：读取起始地址
*                * length：读取数据长度
* Output         : None
* Return         : None
****************************************************************************/

void SRAM_ReadBuffer(uint16_t* readBuff, uint32_t readAddr, uint32_t length)
{
  while(length--) /*!< while there is data to read */
  {
    /*!< Read a half-word from the memory */
    *readBuff++ = *(__IO uint16_t*) (Bank1_SRAM3_ADDR + readAddr);

    /*  十六位长度的是地址+2 */  
    readAddr += 2;
  }  
}
