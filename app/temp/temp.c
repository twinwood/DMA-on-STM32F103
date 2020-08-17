#include "temp.h"

/* �����ڲ����� */
static void DS18B20_WriteData(uint8_t dat);
static uint8_t DS18B20_ReadData(void);

/****************************************************************************
* Function Name  : DS18B20_Init
* Description    : ��ʼ��18B20��GPIO��.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void DS18B20_Init(void)
{
    /* ��ʱ��ʹ�� */
    RCC->APB2ENR |= 0x00000100;

    /* ����GPIO���� */
    GPIOG->CRH &= 0xFFFF0FFF;
    GPIOG->CRH |= 0x00008000;//������������ģʽ
    GPIOG->ODR &= ~(1 << 11);//��������
}

/****************************************************************************
* Function Name  : DS18B20_Reset
* Description    : ��λDS18B20���ȴ�Ӧ��.
* Input          : None
* Output         : None
* Return         : 0���ɹ�;0xFF��ʧ��
****************************************************************************/

uint8_t DS18B20_Reset(void)
{
	uint16_t i = 0;

	/* ����������480us~960us */
	DS18B20_Output();
	DS18B20_OUT = 1;
	DS18B20_OUT = 0;
	SYSTICK_Delay1us(750);
	
	/* Ȼ���������ߣ����DS18B20������Ӧ�Ὣ��15us~60us���������� */
	DS18B20_OUT = 1;
	DS18B20_Input();

    /* �ȴ��½��� */
	while(DS18B20_IN)
	{
		i++;
		if(i > 5000)	     //��50ms
		{
			return 0xFF;
		}
		SYSTICK_Delay1us(10);//10us
	}
	
    /* �ȴ������ͷ� */
	i = 0;
	while(!DS18B20_IN)
	{
		i++;
		if(i > 5000)	      //��50ms
		{
			return 0xFF;
		}
		SYSTICK_Delay1us(10); //10us
	}

	return 0;     
}

/****************************************************************************
* Function Name  : DS18B20_WriteData
* Description    : дһ���ֽ�����
* Input          : dat��Ҫд�������
* Output         : None
* Return         : None
****************************************************************************/

static void DS18B20_WriteData(uint8_t dat)
{
	uint8_t i;

	DS18B20_Output();

	for(i=0; i<8; i++)
	{
		/* ÿд��һλ����֮ǰ�Ȱ���������1us */
		DS18B20_OUT = 0;
		SYSTICK_Delay1us(1);
		
		/* Ȼ��д��һ�����ݣ������λ��ʼ */
		if((dat & 0x01) == 0x01)
		{
			DS18B20_OUT = 1;	
		}
		else
		{
			DS18B20_OUT = 0;
		}
		dat >>= 1;

		/* ��ʱ����ʱ������60us */
		SYSTICK_Delay1us(60);
		
		/* �ͷ����ߣ�����1us�����߻ָ�ʱ����ܽ���д��ڶ�����ֵ */
		DS18B20_OUT = 1;			
		SYSTICK_Delay1us(1);	
	}
}

/****************************************************************************
* Function Name  : DS18B20_ReadData
* Description    : ��ȡһ���ֽ�����.
* Input          : None
* Output         : None
* Return         : dat�����ض�ȡ�����ֽ�
****************************************************************************/

static uint8_t DS18B20_ReadData(void)
{
	uint8_t i, dat;
    	
	for(i=0; i<8; i++)
	{
		/* ÿд��һλ����֮ǰ�Ȱ���������1us */
		DS18B20_Output();
		DS18B20_OUT = 0;
		SYSTICK_Delay1us(1);

		/* Ȼ���ͷ�����,��ʱ6us�ȴ������ȶ� */
		DS18B20_OUT = 1;	  
		SYSTICK_Delay1us(4);

		/* ��ȡ���ݣ������λ��ʼ��ȡ */
		DS18B20_Input();
		dat >>= 1;
		if(DS18B20_IN == 1)
		{
			dat |= 0x80;
		}
		
		/* ��ȡ��֮��ȴ�48us�ٽ��Ŷ�ȡ��һ���� */
		SYSTICK_Delay1us(48);			
	}

	return dat;
}

/****************************************************************************
* Function Name  : DS18B20_ReadTemp
* Description    : ��ȡ�¶�ֵ.
* Input          : None
* Output         : None
* Return         : temp����ȡ�����¶�ֵ����������λС���㣨�ٱ�����
****************************************************************************/

int16_t DS18B20_ReadTemp(void)
{
	int16_t tempValue = 0;
    uint16_t temp;

    DS18B20_Reset();
	DS18B20_WriteData(0xCC);		//����ROM��������		 
	DS18B20_WriteData(0x44);	    //�¶�ת������

    /* ��������������Ļ�������Ӹ���ʱ��DS18B20ת���¶���Ҫʱ�� */
//    SYSTICK_Delay1ms(200);

	DS18B20_Reset();
	DS18B20_WriteData(0xCC);	    //����ROM��������
	DS18B20_WriteData(0xBE);	    //���Ͷ�ȡ�¶�����

	/* ��ȡ16λ�¶�ֵ���ȶ���λ */
	temp = DS18B20_ReadData();
	tempValue = DS18B20_ReadData();
    tempValue <<= 8;
    tempValue |= temp;
    temp = tempValue;

	/* �¶ȴ����¶ȱ�����λС���� */
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




