#include "lcd_driver.h"
#include "system.h"

/****************************************************************************
* Function Name  : TFT_GPIO_Config
* Description    : ��ʼ��TFT������IO��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_GPIO_Config(void)
{	
	/* ��ʱ��ʹ�� */
    RCC->APB2ENR |= 0x00000020 | 0x00000040 | 0x00000100;

    /* ����IOΪ����������� */
    GPIOG->CRL &= 0xFFFFFFF0;  //PG0
    GPIOG->CRL |= 0x0000000B;
    GPIOG->CRH &= 0xFFF0FFFF;  //PG12
    GPIOG->CRH |= 0x000B0000;

    GPIOD->CRL &= 0xFF00FF00;
    GPIOD->CRL |= 0x00BB00BB;
    GPIOD->CRH &= 0x00000000;
    GPIOD->CRH |= 0xBBBBBBBB;

    GPIOE->CRL &= 0x0FFFFFFF;
    GPIOE->CRL |= 0xB0000000;
    GPIOE->CRH &= 0x00000000;
    GPIOE->CRH |= 0xBBBBBBBB;

}

/****************************************************************************
* Function Name  : TFT_FSMC_Config
* Description    : ��ʼ��FSMC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_FSMC_Config(void)
{
    /* ʹ��FSMC��ʱ�� */
    RCC->AHBENR |= 1 << 8;     	 	//ʹ��FSMCʱ��
    
	/* bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����*/
	/* ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]�� */				    
	FSMC_Bank1->BTCR[6] = 0x00000000;
	FSMC_Bank1->BTCR[7] = 0x00000000;
	FSMC_Bank1E->BWTR[6] = 0x00000000;
	/* ����BCR�Ĵ���	ʹ���첽ģʽ */
	FSMC_Bank1->BTCR[6] |= 1 << 12;		//�洢��дʹ��
	FSMC_Bank1->BTCR[6] |= 1 << 14;		//��дʹ�ò�ͬ��ʱ��
	FSMC_Bank1->BTCR[6] |= 1 << 4; 		//�洢�����ݿ��Ϊ16bit 	    
	/* ����BTR�Ĵ��� */	
	/* ��ʱ����ƼĴ��� */							    
	FSMC_Bank1->BTCR[7] |= 0 << 28;		//ģʽA 	 							  	 
	FSMC_Bank1->BTCR[7] |= 1 << 0; 		//��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns	 	 
	/* ��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�졣 */
	FSMC_Bank1->BTCR[7] |= 0x0F << 8;  	//���ݱ���ʱ��Ϊ16��HCLK	 	 
	/* дʱ����ƼĴ��� */ 
	FSMC_Bank1E->BWTR[6] |= 0 << 28; 	//ģʽA 	 							    
	FSMC_Bank1E->BWTR[6] |= 0 << 0;		//��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 
 	/* 4��HCLK��HCLK=72M����ΪҺ������IC��д�ź���������Ҳ��50ns��72M/4=24M=55ns */  	 
	FSMC_Bank1E->BWTR[6] |= 3 << 8; 	//���ݱ���ʱ��Ϊ4��HCLK	
	/* ʹ��BANK1,����4 */
	FSMC_Bank1->BTCR[6] |= 1 << 0;		//ʹ��BANK1������4	
}

/****************************************************************************
* Function Name  : TFT_WriteCmd
* Description    : LCDд������
* Input          : cmd��д���16λ����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteCmd(uint16_t cmd)
{
	TFT->TFT_CMD = cmd;
}

/****************************************************************************
* Function Name  : TFT_WriteData
* Description    : LCDд������
* Input          : dat��д���16λ����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteData(u16 dat)
{
	TFT->TFT_DATA = dat;
}

void TFT_WriteData_Color(u16 color)
{
	TFT->TFT_DATA = color;
}

void LCD_WriteCmdData(u16 cmd,u16 data)
{
	TFT_WriteCmd(cmd);
	TFT_WriteData(data);
}

/****************************************************************************
* Function Name  : TFT_Init
* Description    : ��ʼ��LCD��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_Init(void)
{
	uint16_t i;

	TFT_GPIO_Config();
	TFT_FSMC_Config();

	SYSTICK_Delay1ms(50); 

	TFT_WriteCmd(0x0000);TFT_WriteData(0x0000);
	TFT_WriteCmd(0x0000);TFT_WriteData(0x0000);
	TFT_WriteCmd(0x0000);TFT_WriteData(0x0000);
	
	TFT_WriteCmd(0x0400);TFT_WriteData(0x6200); 
	TFT_WriteCmd(0x0008);TFT_WriteData(0x0808); 


	TFT_WriteCmd(0x0001);TFT_WriteData(0x0100);
 	TFT_WriteCmd(0x0002);TFT_WriteData(0x0100);
	TFT_WriteCmd(0x0003);TFT_WriteData(0x1030);
	TFT_WriteCmd(0x0008);TFT_WriteData(0x0808);
	/*gamma set 20090423 BY LYJ*/

	 TFT_WriteCmd(0x0300);TFT_WriteData(0x000c); 
	 TFT_WriteCmd(0x0301);TFT_WriteData(0x5A0B); 
	 TFT_WriteCmd(0x0302);TFT_WriteData(0x0906); 
	 TFT_WriteCmd(0x0303);TFT_WriteData(0x1017); 
	 TFT_WriteCmd(0x0304);TFT_WriteData(0x2300); 
	 TFT_WriteCmd(0x0305);TFT_WriteData(0x1700); 
	 TFT_WriteCmd(0x0306);TFT_WriteData(0x6309); 
	 TFT_WriteCmd(0x0307);TFT_WriteData(0x0c09); 
	 TFT_WriteCmd(0x0308);TFT_WriteData(0x100c); 
	 TFT_WriteCmd(0x0309);TFT_WriteData(0x2232); 
	 /*power set*/
	  TFT_WriteCmd(0x0010);TFT_WriteData(0x0014);
	  TFT_WriteCmd(0x0011);TFT_WriteData(0x0101);  
	  TFT_WriteCmd(0x0100);TFT_WriteData(0x0230);//
		//for(i=500; i>0; i--);
		SYSTICK_Delay1ms(50);
	  TFT_WriteCmd(0x0101);TFT_WriteData(0x0247);//
		//for(i=500; i>0; i--);
		SYSTICK_Delay1ms(50);
	  TFT_WriteCmd(0x0103);TFT_WriteData(0x0a00);//Starts VLOUT3,Sets the VREG1OUT.
		//for(i=500; i>0; i--);
		SYSTICK_Delay1ms(10);
	  TFT_WriteCmd(0x0280);TFT_WriteData(0xf0ff);//VCOMH voltage   //0xcEff
		SYSTICK_Delay1ms(50);
		//for(i=500; i>0; i--);
	  TFT_WriteCmd(0x0102);TFT_WriteData(0xB1b0);//Starts VLOUT3,Sets the VREG1OUT.
		//for(i=500; i>0; i--);
		SYSTICK_Delay1ms(50);
	 /*window set*/
	  TFT_WriteCmd(0x0210);TFT_WriteData(0x0000);//Window Horizontal RAM Address Start
	  TFT_WriteCmd(0x0211);TFT_WriteData(0x00ef);//Window Horizontal RAM Address End
	  TFT_WriteCmd(0x0212);TFT_WriteData(0x0000);//Window Vertical RAM Address Start
	  TFT_WriteCmd(0x0213);TFT_WriteData(0x018f);//Window Vertical RAM Address End
	  TFT_WriteCmd(0x0200);TFT_WriteData(0x0000);//RAM Address Set (Horizontal Address)
	  TFT_WriteCmd(0x0201);TFT_WriteData(0x0000);//RAM Address Set (Vertical Address)
	  TFT_WriteCmd(0x0401);TFT_WriteData(0x0000);//Base Image Display
	  TFT_WriteCmd(0x0007);TFT_WriteData(0x0100);//Display Control 1
		//for(i=500; i>0; i--);
		SYSTICK_Delay1ms(50);
	  TFT_WriteCmd(0x0202);
}

/****************************************************************************
* Function Name  : TFT_SetWindow
* Description    : ����Ҫ�����Ĵ��ڷ�Χ
* Input          : xStart��������ʼX����
*                * yStart��������ʼY����
*                * xEnd�����ڽ���X����
*                * yEnd�����ڽ���Y����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
 	LCD_WriteCmdData(0x0210, xStart);	 //ˮƽ����ʼ
	LCD_WriteCmdData(0x0211, xEnd);	     //ˮƽ�߽���
	LCD_WriteCmdData(0x0212, yStart);	 //��ֱ����ʼ
	LCD_WriteCmdData(0x0213, yEnd);	     //��ֱ�߽���

	LCD_WriteCmdData(0x0200,xStart);
	LCD_WriteCmdData(0x0201,yStart);

	TFT_WriteCmd(0x0202); 
}

/****************************************************************************
* Function Name  : TFT_ClearScreen
* Description    : ��LCD��������Ӧ����ɫ
* Input          : color��������ɫ
* Output         : None
* Return         : None
****************************************************************************/
	  
void TFT_ClearScreen(uint16_t color)
{
 	uint16_t i, j ;

	TFT_SetWindow(0, 0, TFT_XMAX, TFT_YMAX);	 //��������
  	for(i=0; i<TFT_XMAX+1; i++)
	{
		for (j=0; j<TFT_YMAX+1; j++)
		{
			TFT_WriteData_Color(color);
		}
	}
}
